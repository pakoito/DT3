//==============================================================================
///	
///	File: DeviceGraphicsDX11DataStream.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DeviceGraphicsDX11DataStream.hpp"
#include "DeviceGraphicsDX11Renderer.hpp"
#include "Assert.hpp"
#include "System.hpp"
#include "Archive.hpp"

#include "Factory.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(DeviceGraphicsDX11DataStream)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DeviceGraphicsDX11DataStream::DeviceGraphicsDX11DataStream (void)
	:	_size		(0),
		_data		(NULL),
		_buffer		(NULL),
		_dynamic	(false)
{

}
			
DeviceGraphicsDX11DataStream::~DeviceGraphicsDX11DataStream (void)
{
	clearData();
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11DataStream::archiveRead (Archive *archive)
{
    archive->pushDomain (getClassID ());
    
	BaseClass::archiveRead(archive);
	
	*archive << ARCHIVE_DATA(_size, DATA_PERSISTENT);
	*archive << ARCHIVE_DATA(_format, DATA_PERSISTENT);
	*archive << ARCHIVE_DATA(_dynamic, DATA_PERSISTENT);
	
	DTfloat *data_floats = (DTfloat*) allocateData(_size,_format,_dynamic);
	for (DTuint f = 0; f < _size/sizeof(DTfloat); ++f) {
		*archive << ARCHIVE_DATA(*data_floats, DATA_PERSISTENT);
		++data_floats;
	}
			
	uploadData();	

    archive->popDomain ();
}

void DeviceGraphicsDX11DataStream::archiveWrite (Archive *archive)
{
    archive->pushDomain (getClassID ());

	BaseClass::archiveWrite(archive);
	
	*archive << ARCHIVE_DATA(_size, DATA_PERSISTENT);
	*archive << ARCHIVE_DATA(_format, DATA_PERSISTENT);
	*archive << ARCHIVE_DATA(_dynamic, DATA_PERSISTENT);
	
	DTfloat *data_floats = (DTfloat*) _data;
	for (DTuint f = 0; f < _size/sizeof(DTfloat); ++f) {
		*archive << ARCHIVE_DATA(*data_floats, DATA_PERSISTENT);
		++data_floats;
	}

    archive->popDomain ();
}

//==============================================================================
//==============================================================================

void* DeviceGraphicsDX11DataStream::allocateData		(DTuint size, DTuint format, DTboolean dynamic)
{ 
	clearData();

	Assert(size);

	// Remember size
	_size = size;

	// Remember format
	_format = format;
	
	// Remember dynamic flag
	_dynamic = dynamic;

	// allocate buffer
	_data = (DTbyte*) new ("DX11Data") DTbyte[_size];
	return _data;
}

void DeviceGraphicsDX11DataStream::uploadData ()
{
    DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());
    ID3D11Device1 *device = renderer->getD3D11Device();

    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = _size;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    // Set access flags
    if (_dynamic) {
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    } else {
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.CPUAccessFlags = 0;
    }

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = _data;
    data.SysMemPitch = _size;
    data.SysMemSlicePitch = _size;

    // Create the buffer
    HRESULT hr = device->CreateBuffer(&desc, &data, &_buffer);
    Assert(SUCCEEDED(hr));
	
	// Don't need to keep the buffer around if using VBO
	if (!_dynamic && _data) {
		delete[] _data;
		_data = NULL;
	}
	
}

void* DeviceGraphicsDX11DataStream::getData (void)
{
	return _data;
}

void DeviceGraphicsDX11DataStream::clearData (void)
{
    SAFE_RELEASE(_buffer);
	if (_data) delete[] _data;
	
	_data = NULL;
	_size = 0;
}
		
//==============================================================================
//==============================================================================

void* DeviceGraphicsDX11DataStream::map (DTboolean read, DTboolean write)
{
	Assert(_dynamic && _data);

    DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());
    ID3D11DeviceContext1 *context = renderer->getD3D11Context();

    D3D11_MAPPED_SUBRESOURCE res;
    HRESULT hr = context->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res); 

	return res.pData;
}

void DeviceGraphicsDX11DataStream::unmap (void)
{
	Assert(_dynamic && _data);
	
    DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());
    ID3D11DeviceContext1 *context = renderer->getD3D11Context();

    context->Unmap(_buffer, 0);
}
		
//==============================================================================
//==============================================================================
		
} // DT2
