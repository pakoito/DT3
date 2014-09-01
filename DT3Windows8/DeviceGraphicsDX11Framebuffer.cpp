//==============================================================================
///	
///	File: DeviceGraphicsDX11Framebuffer.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DeviceGraphicsDX11Framebuffer.hpp"

#include "DeviceGraphicsDX11Renderer.hpp"
#include "System.hpp"
#include "Factory.hpp"
#include "Framebuffer.hpp"
#include "DeviceGraphicsDX11Texture.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(DeviceGraphicsDX11Framebuffer)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DeviceGraphicsDX11Framebuffer::DeviceGraphicsDX11Framebuffer (void)
    :   _color_texture          (NULL),
        _depth_texture          (NULL),
        _color_view_rb          (NULL),
        _depth_view_rb          (NULL),
        _color_texture_rb       (NULL),
        _depth_texture_rb       (NULL)
{

}
			
DeviceGraphicsDX11Framebuffer::~DeviceGraphicsDX11Framebuffer (void)
{
    RELEASE(_color_texture);
    RELEASE(_depth_texture);

    SAFE_RELEASE(_color_view_rb);
    SAFE_RELEASE(_depth_view_rb);
    SAFE_RELEASE(_color_texture_rb);
    SAFE_RELEASE(_depth_texture_rb);
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Framebuffer::buildFramebuffer (Framebuffer *fb)
{
    DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());

    ID3D11Device1 *device = renderer->getD3D11Device();


    DTint width = fb->getWidth();
    DTint height = fb->getHeight();
  
    //
    // Build textures and renderbuffers
    //

    // If Color buffer is a texture
    if (fb->getColorTexture()) {
    
        TextureResource *color_texture = fb->getColorTexture();
        _color_texture = RETAIN(checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer())->getTextureCached( color_texture ));
        
        _color_texture_rb = _color_texture->get2DTexture();
        _color_texture_rb->AddRef();

    // If Color buffer is a render buffer
    } else if (fb->getColorRenderbufferFormat() != Framebuffer::RB_NONE) {
    
        Assert(fb->getColorRenderbufferFormat() == Framebuffer::RB_RGBA8);
        
        // Texture Desc

        D3D11_TEXTURE2D_DESC tDesc;



        tDesc.Width = width;

        tDesc.Height = height;

        tDesc.MipLevels = 1;

        tDesc.ArraySize = 1;

        tDesc.SampleDesc.Count = 1;

        tDesc.SampleDesc.Quality = 0;

        tDesc.Usage = D3D11_USAGE_DEFAULT;

        tDesc.BindFlags = D3D11_BIND_RENDER_TARGET;

        tDesc.CPUAccessFlags = 0;

        tDesc.MiscFlags = 0; 
		tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;



        SAFE_RELEASE(_color_texture_rb);

        HRESULT hr = device->CreateTexture2D(&tDesc, NULL, &_color_texture_rb);

        Assert(SUCCEEDED(hr));

    }

    // Get the texture description
    D3D11_TEXTURE2D_DESC tDesc;
    _color_texture_rb->GetDesc(&tDesc);


    // Create a render target view for the texture

    D3D11_RENDER_TARGET_VIEW_DESC vDesc;

    vDesc.Format = tDesc.Format;

    vDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    vDesc.Texture2D.MipSlice = 0;



    SAFE_RELEASE(_color_view_rb);

    HRESULT hr = device->CreateRenderTargetView(_color_texture_rb, &vDesc,&_color_view_rb);

    Assert(SUCCEEDED(hr));





    // If Depth buffer is a texture
    if (fb->getDepthTexture()) {
    
        TextureResource *depth_texture = fb->getDepthTexture();
        _depth_texture = RETAIN(checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer())->getTextureCached( depth_texture ));
            
        _depth_texture_rb = _depth_texture->get2DTexture();
        _depth_texture_rb->AddRef();

    // If Depth buffer is a render buffer
    } else if (fb->getDepthRenderbufferFormat() != Framebuffer::RB_NONE) {

        // Texture Desc

        D3D11_TEXTURE2D_DESC tDesc;



        tDesc.Width = width;

        tDesc.Height = height;

        tDesc.MipLevels = 1;

        tDesc.ArraySize = 1;

        tDesc.SampleDesc.Count = 1;

        tDesc.SampleDesc.Quality = 0;

        tDesc.Usage = D3D11_USAGE_DEFAULT;

        tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        tDesc.CPUAccessFlags = 0;

        tDesc.MiscFlags = 0; 
    
        if (fb->getDepthRenderbufferFormat() == Framebuffer::RB_DEPTH_16)                tDesc.Format = DXGI_FORMAT_D16_UNORM;
        else if (fb->getDepthRenderbufferFormat() == Framebuffer::RB_DEPTH_24)           tDesc.Format = DXGI_FORMAT_D16_UNORM;
        else if (fb->getDepthRenderbufferFormat() == Framebuffer::RB_DEPTH_24_STENCIL_8) tDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        
        SAFE_RELEASE(_depth_texture_rb);

        HRESULT hr = device->CreateTexture2D(&tDesc, NULL, &_depth_texture_rb);

        Assert(SUCCEEDED(hr));  
    }

    // Get the texture description
    _depth_texture_rb->GetDesc(&tDesc);


    // Create a render target view for the texture

    D3D11_DEPTH_STENCIL_VIEW_DESC dvDesc;

    dvDesc.Format = tDesc.Format;

    dvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    dvDesc.Flags = 0;

    dvDesc.Texture2D.MipSlice = 0;



    SAFE_RELEASE(_depth_view_rb);

    hr = device->CreateDepthStencilView(_depth_texture_rb, &dvDesc,&_depth_view_rb);

    Assert(SUCCEEDED(hr));


}

//==============================================================================
//==============================================================================

} // DT2
