//==============================================================================
///	
///	File: DeviceGraphicsDX11Texture.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DeviceGraphicsDX11Texture.hpp"

#include "DeviceGraphicsDX11Renderer.hpp"
#include "System.hpp"
#include "Factory.hpp"
#include "TextureResource.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(DeviceGraphicsDX11Texture)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DeviceGraphicsDX11Texture::DeviceGraphicsDX11Texture (void)
	:	_type           (TYPE_2D),
        _is_created     (false),
        _texture_2D     (NULL),
        _texture_3D     (NULL),
        _texture_view   (NULL),
        _width          (0),
        _height         (0),
        _depth          (0),
		_size			(0),
		_resident		(false)
{

}
			
DeviceGraphicsDX11Texture::~DeviceGraphicsDX11Texture (void)
{
    SAFE_RELEASE(_texture_2D);
    SAFE_RELEASE(_texture_3D);
    SAFE_RELEASE(_texture_view);
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Texture::buildTexture2D (DTubyte *buffer, DTuint width, DTuint height, DTint format, DTuint compressed_size, DTboolean mipmapped)
{
    _type = TYPE_2D;
    _width = width;
    _height = height;
    _depth = 1;

    // Create DX11 Texture
    DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());
    ID3D11Device1 *device = renderer->getD3D11Device();

    // Texture Desc
    D3D11_TEXTURE2D_DESC tDesc;

    tDesc.Width = width;
    tDesc.Height = height;
    tDesc.MipLevels = 1;
    tDesc.ArraySize = 1;
    tDesc.SampleDesc.Count = 1;
    tDesc.SampleDesc.Quality = 0;
    tDesc.Usage = D3D11_USAGE_DEFAULT;
    tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    tDesc.CPUAccessFlags = 0;
    tDesc.MiscFlags = mipmapped ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

    DTuint bytesPerPixel = 0;

	if (format == TextureResource::FORMAT_RGBA) {
		tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		_size = width * height * 4;
        bytesPerPixel = 4;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_BGRA) {
		tDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		_size = width * height * 4;
        bytesPerPixel = 4;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_RGBA_5551) {
		tDesc.Format = DXGI_FORMAT_B5G5R5A1_UNORM;  // TODO backwards
		_size = width * height * 2;
        bytesPerPixel = 2;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_RGBA_4444) {
		tDesc.Format = DXGI_FORMAT_B4G4R4A4_UNORM;  // TODO backwards
		_size = width * height * 2;
        bytesPerPixel = 2;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_RGB_565) {
		tDesc.Format = DXGI_FORMAT_B5G6R5_UNORM;    // TODO backwards
		_size = width * height * 2;
        bytesPerPixel = 2;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_LUM_8) {
		tDesc.Format = DXGI_FORMAT_R8_UNORM;
		_size = width * height * 1;
        bytesPerPixel = 1;
	} else if (format == TextureResource::FORMAT_PVR2_RGBA) {
		Assert(0);
		_size = compressed_size;
	} else if (format == TextureResource::FORMAT_PVR4_RGBA) {
		Assert(0);
		_size = compressed_size;
	} else if (format == TextureResource::FORMAT_PVR2_RGB) {
		Assert(0);
		_size = compressed_size;
	} else if (format == TextureResource::FORMAT_PVR4_RGB) {
		Assert(0);
		_size = compressed_size;
	} else if (format == TextureResource::FORMAT_DEPTH_16) {
        Assert(0);
    } else if (format == TextureResource::FORMAT_DEPTH_24) {
        Assert(0);
    }

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = buffer;
    data.SysMemPitch = bytesPerPixel * width;
    data.SysMemSlicePitch = _size;

    SAFE_RELEASE(_texture_2D);
    SAFE_RELEASE(_texture_3D);
    HRESULT hr = device->CreateTexture2D(&tDesc, &data, &_texture_2D);
    Assert(SUCCEEDED(hr));

    // Shader Resource View
    D3D11_SHADER_RESOURCE_VIEW_DESC vdesc;
    vdesc.Format = tDesc.Format;
    vdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    vdesc.Texture2D.MostDetailedMip = 0;
    vdesc.Texture2D.MipLevels = -1;    // All levels

    SAFE_RELEASE(_texture_view);
    hr = device->CreateShaderResourceView(_texture_2D,&vdesc,&_texture_view);
    Assert(SUCCEEDED(hr));
    
    _is_created = true;
}

void DeviceGraphicsDX11Texture::updateTexture2D (DTubyte *buffer, DTuint width, DTuint height, DTint format, DTuint compressed_size, DTboolean mipmapped)
{
    _type = TYPE_2D;
    
    Assert(_width == width);
    Assert(_height == height);

    DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());
    ID3D11DeviceContext1 *context = renderer->getD3D11Context();

    DTuint bytesPerPixel = 0;

	if (format == TextureResource::FORMAT_RGBA) {
        bytesPerPixel = 4;
	} else if (format == TextureResource::FORMAT_BGRA) {
        bytesPerPixel = 4;
	} else if (format == TextureResource::FORMAT_RGBA_5551) {
        bytesPerPixel = 2;
	} else if (format == TextureResource::FORMAT_RGBA_4444) {
        bytesPerPixel = 2;
	} else if (format == TextureResource::FORMAT_RGB_565) {
        bytesPerPixel = 2;
	} else if (format == TextureResource::FORMAT_LUM_8) {
        bytesPerPixel = 1;
	} else if (format == TextureResource::FORMAT_PVR2_RGBA) {
		Assert(0);
	} else if (format == TextureResource::FORMAT_PVR4_RGBA) {
		Assert(0);
	} else if (format == TextureResource::FORMAT_PVR2_RGB) {
		Assert(0);
	} else if (format == TextureResource::FORMAT_PVR4_RGB) {
		Assert(0);
	} else if (format == TextureResource::FORMAT_DEPTH_16) {
        Assert(0);
    } else if (format == TextureResource::FORMAT_DEPTH_24) {
        Assert(0);
    }

    context->UpdateSubresource(_texture_2D, 0, NULL, buffer, bytesPerPixel, bytesPerPixel*width*height);

	if (mipmapped) {
        context->GenerateMips(_texture_view);
    }
}

void DeviceGraphicsDX11Texture::buildTexture2D (DTuint width, DTuint height, DTint format, DTuint compressed_size, DTboolean mipmapped)
{
    _type = TYPE_2D;
    _width = width;
    _height = height;
    _depth = 1;

    // Create DX11 Texture
    DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());
    ID3D11Device1 *device = renderer->getD3D11Device();

    // Texture Desc
    D3D11_TEXTURE2D_DESC tDesc;

    tDesc.Width = width;
    tDesc.Height = height;
    tDesc.MipLevels = 1;
    tDesc.ArraySize = 1;
    tDesc.SampleDesc.Count = 1;
    tDesc.SampleDesc.Quality = 0;
    tDesc.Usage = D3D11_USAGE_DEFAULT;
    tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    tDesc.MiscFlags = mipmapped ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

    DTuint bytesPerPixel = 0;

	if (format == TextureResource::FORMAT_RGBA) {
		tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		_size = width * height * 4;
        bytesPerPixel = 4;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_BGRA) {
		tDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		_size = width * height * 4;
        bytesPerPixel = 4;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_RGBA_5551) {
		tDesc.Format = DXGI_FORMAT_B5G5R5A1_UNORM;  // TODO backwards
		_size = width * height * 2;
        bytesPerPixel = 2;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_RGBA_4444) {
		tDesc.Format = DXGI_FORMAT_B4G4R4A4_UNORM;  // TODO backwards
		_size = width * height * 2;
        bytesPerPixel = 2;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_RGB_565) {
		tDesc.Format = DXGI_FORMAT_B5G6R5_UNORM;    // TODO backwards
		_size = width * height * 2;
        bytesPerPixel = 2;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_LUM_8) {
		tDesc.Format = DXGI_FORMAT_R8_UNORM;
		_size = width * height * 1;
        bytesPerPixel = 1;
	} else if (format == TextureResource::FORMAT_PVR2_RGBA) {
		Assert(0);
		_size = compressed_size;
	} else if (format == TextureResource::FORMAT_PVR4_RGBA) {
		Assert(0);
		_size = compressed_size;
	} else if (format == TextureResource::FORMAT_PVR2_RGB) {
		Assert(0);
		_size = compressed_size;
	} else if (format == TextureResource::FORMAT_PVR4_RGB) {
		Assert(0);
		_size = compressed_size;
	} else if (format == TextureResource::FORMAT_DEPTH_16) {
        tDesc.Format = DXGI_FORMAT_D16_UNORM;
		_size = width * height * 2;
        bytesPerPixel = 2;
    } else if (format == TextureResource::FORMAT_DEPTH_24) {
        tDesc.Format = DXGI_FORMAT_D16_UNORM;
		_size = width * height * 2;
        bytesPerPixel = 2;
    }

    SAFE_RELEASE(_texture_2D);
    SAFE_RELEASE(_texture_3D);
    HRESULT hr = device->CreateTexture2D(&tDesc, NULL, &_texture_2D);
    Assert(SUCCEEDED(hr));

    // Shader Resource View
    D3D11_SHADER_RESOURCE_VIEW_DESC vdesc;
    vdesc.Format = tDesc.Format;
    vdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    vdesc.Texture2D.MostDetailedMip = 0;
    vdesc.Texture2D.MipLevels = -1;    // All levels

    SAFE_RELEASE(_texture_view);
    hr = device->CreateShaderResourceView(_texture_2D,&vdesc,&_texture_view);
    Assert(SUCCEEDED(hr));
    
    _is_created = true;
}

void DeviceGraphicsDX11Texture::buildTexture3D (DTubyte *buffer, DTuint width, DTuint height, DTuint depth, DTint format, DTuint compressed_size, DTboolean mipmapped)
{
    _type = TYPE_3D;
    _width = width;
    _height = height;
    _depth = depth;

    // Create DX11 Texture
    DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());
    ID3D11Device1 *device = renderer->getD3D11Device();

    // Texture Desc
    D3D11_TEXTURE3D_DESC tDesc;

    tDesc.Width = width;
    tDesc.Height = height;
    tDesc.Depth = depth;
    tDesc.MipLevels = 1;
    tDesc.Usage = D3D11_USAGE_DEFAULT;
    tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    tDesc.CPUAccessFlags = 0;
    tDesc.MiscFlags = mipmapped ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

    DTuint bytesPerPixel = 0;

	if (format == TextureResource::FORMAT_RGBA) {
		tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		_size = width * height * depth * 4;
        bytesPerPixel = 4;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_BGRA) {
		tDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		_size = width * height * depth * 4;
        bytesPerPixel = 4;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_RGBA_5551) {
		tDesc.Format = DXGI_FORMAT_B5G5R5A1_UNORM;  // TODO backwards
		_size = width * height * depth * 2;
        bytesPerPixel = 2;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_RGBA_4444) {
		tDesc.Format = DXGI_FORMAT_B4G4R4A4_UNORM;  // TODO backwards
		_size = width * height * depth * 2;
        bytesPerPixel = 2;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_RGB_565) {
		tDesc.Format = DXGI_FORMAT_B5G6R5_UNORM;    // TODO backwards
		_size = width * height * depth * 2;
        bytesPerPixel = 2;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_LUM_8) {
		tDesc.Format = DXGI_FORMAT_R8_UNORM;
		_size = width * height * depth * 1;
        bytesPerPixel = 1;
	} else if (format == TextureResource::FORMAT_PVR2_RGBA) {
		Assert(0);
		_size = compressed_size;
	} else if (format == TextureResource::FORMAT_PVR4_RGBA) {
		Assert(0);
		_size = compressed_size;
	} else if (format == TextureResource::FORMAT_PVR2_RGB) {
		Assert(0);
		_size = compressed_size;
	} else if (format == TextureResource::FORMAT_PVR4_RGB) {
		Assert(0);
		_size = compressed_size;
	} else if (format == TextureResource::FORMAT_DEPTH_16) {
        Assert(0);
    } else if (format == TextureResource::FORMAT_DEPTH_24) {
        Assert(0);
    }

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = buffer;
    data.SysMemPitch = bytesPerPixel * width;
    data.SysMemSlicePitch = _size/depth;

    SAFE_RELEASE(_texture_2D);
    SAFE_RELEASE(_texture_3D);
    HRESULT hr = device->CreateTexture3D(&tDesc, &data, &_texture_3D);
    Assert(SUCCEEDED(hr));

    // Shader Resource View
    D3D11_SHADER_RESOURCE_VIEW_DESC vdesc;
    vdesc.Format = tDesc.Format;
    vdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    vdesc.Texture2D.MostDetailedMip = 0;
    vdesc.Texture2D.MipLevels = -1;    // All levels

    SAFE_RELEASE(_texture_view);
    hr = device->CreateShaderResourceView(_texture_3D,&vdesc,&_texture_view);
    Assert(SUCCEEDED(hr));
    
    _is_created = true;
}

void DeviceGraphicsDX11Texture::buildTextureCube (DTubyte *buffer, DTuint width, DTuint height, DTint format, DTuint compressed_size, DTboolean mipmapped)
{
    _type = TYPE_CUBE;
    _width = width;
    _height = height;
    _depth = 1;

    // Create DX11 Texture
    DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());
    ID3D11Device1 *device = renderer->getD3D11Device();

    // Texture Desc
    D3D11_TEXTURE2D_DESC tDesc;

    tDesc.Width = width;
    tDesc.Height = height;
    tDesc.MipLevels = 1;
    tDesc.ArraySize = 6;
    tDesc.SampleDesc.Count = 1;
    tDesc.SampleDesc.Quality = 0;
    tDesc.Usage = D3D11_USAGE_DEFAULT;
    tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    tDesc.CPUAccessFlags = 0;
    tDesc.MiscFlags = (mipmapped ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0) | D3D11_RESOURCE_MISC_TEXTURECUBE;

    DTuint bytesPerPixel = 0;

	if (format == TextureResource::FORMAT_RGBA) {
		tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		_size = width * height * 4;
        bytesPerPixel = 4;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_BGRA) {
		tDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		_size = width * height * 4;
        bytesPerPixel = 4;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_RGBA_5551) {
		tDesc.Format = DXGI_FORMAT_B5G5R5A1_UNORM;  // TODO backwards
		_size = width * height * 2;
        bytesPerPixel = 2;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_RGBA_4444) {
		tDesc.Format = DXGI_FORMAT_B4G4R4A4_UNORM;  // TODO backwards
		_size = width * height * 2;
        bytesPerPixel = 2;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_RGB_565) {
		tDesc.Format = DXGI_FORMAT_B5G6R5_UNORM;    // TODO backwards
		_size = width * height * 2;
        bytesPerPixel = 2;
        tDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	} else if (format == TextureResource::FORMAT_LUM_8) {
		tDesc.Format = DXGI_FORMAT_R8_UNORM;
		_size = width * height * 1;
        bytesPerPixel = 1;
	} else if (format == TextureResource::FORMAT_PVR2_RGBA) {
		Assert(0);
		_size = compressed_size;
	} else if (format == TextureResource::FORMAT_PVR4_RGBA) {
		Assert(0);
		_size = compressed_size;
	} else if (format == TextureResource::FORMAT_PVR2_RGB) {
		Assert(0);
		_size = compressed_size;
	} else if (format == TextureResource::FORMAT_PVR4_RGB) {
		Assert(0);
		_size = compressed_size;
	} else if (format == TextureResource::FORMAT_DEPTH_16) {
        Assert(0);
    } else if (format == TextureResource::FORMAT_DEPTH_24) {
        Assert(0);
    }

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = buffer;
    data.SysMemPitch = bytesPerPixel * width;
    data.SysMemSlicePitch = _size;

    SAFE_RELEASE(_texture_2D);
    HRESULT hr = device->CreateTexture2D(&tDesc, &data, &_texture_2D);
    Assert(SUCCEEDED(hr));

    // Shader Resource View
    D3D11_SHADER_RESOURCE_VIEW_DESC vdesc;
    vdesc.Format = tDesc.Format;
    vdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    vdesc.TextureCube.MostDetailedMip = 0;
    vdesc.TextureCube.MipLevels = -1;    // All levels

    SAFE_RELEASE(_texture_view);
    hr = device->CreateShaderResourceView(_texture_2D,&vdesc,&_texture_view);
    Assert(SUCCEEDED(hr));
    
    _is_created = true;
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Texture::syncToResource (TextureResource *texture)
{
	if (texture->getRecacheData()) {
		
		// Check if data was purged
		if (texture->getDataWasPurged())
			texture->reload();
		
        if (_is_created && texture->getWidth() == _width && texture->getHeight() == _height && texture->getDepth() == _depth) {
            // Simple RGBA Texture
            if (texture->is2D())
                updateTexture2D(texture->getBuffer(), texture->getWidth(), texture->getHeight(), texture->getFormat(), texture->getSize(), texture->isMipmapped());
            else
                Assert(0);
                
        } else {
            // Simple RGBA Texture
            if (texture->is2D())
                buildTexture2D(texture->getBuffer(), texture->getWidth(), texture->getHeight(), texture->getFormat(), texture->getSize(), texture->isMipmapped());
            else if (texture->is3D())
                buildTexture3D(texture->getBuffer(), texture->getWidth(), texture->getHeight(), texture->getDepth(), texture->getFormat(), texture->getSize(), texture->isMipmapped());
            else if (texture->isCube())
                buildTextureCube(texture->getBuffer(), texture->getWidth(), texture->getHeight(), texture->getFormat(), texture->getSize(), texture->isMipmapped());
            else
                Assert(0);
        }
        
		texture->setRecacheData(false);

        if (texture->getDataCanPurge())
            texture->purge();
	} 
	
	if (texture->getRecacheParameters()) {
		texture->setRecacheParameters(false);
	}
}

//==============================================================================
//==============================================================================

} // DT2

