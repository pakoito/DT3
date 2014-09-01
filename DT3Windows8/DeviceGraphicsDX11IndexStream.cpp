//==============================================================================
///	
///	File: DeviceGraphicsDX11IndexStream.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DeviceGraphicsDX11IndexStream.hpp"
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

IMPLEMENT_FACTORY_CREATION(DeviceGraphicsDX11IndexStream)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DeviceGraphicsDX11IndexStream::DeviceGraphicsDX11IndexStream (void)
	:	_size		(0),
		_data		(NULL),
		_buffer		(NULL),
		_dynamic	(false)
{

}
			
DeviceGraphicsDX11IndexStream::~DeviceGraphicsDX11IndexStream (void)
{
	clearData();
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11IndexStream::archiveRead (Archive *archive)
{
    archive->pushDomain (getClassID ());
    
	BaseClass::archiveRead(archive);
	
	*archive << ARCHIVE_DATA(_size, DATA_PERSISTENT);
	*archive << ARCHIVE_DATA(_dynamic, DATA_PERSISTENT);
	
	DTushort *data_ushort = (DTushort*) allocateData(_size,_dynamic);
	for (DTuint f = 0; f < _size/sizeof(DTushort); ++f) {
		*archive << ARCHIVE_DATA(*data_ushort, DATA_PERSISTENT);
		++data_ushort;
	}
	
	uploadData();
	
    archive->popDomain ();
}

void DeviceGraphicsDX11IndexStream::archiveWrite (Archive *archive)
{
    archive->pushDomain (getClassID ());

	BaseClass::archiveWrite(archive);
	
	*archive << ARCHIVE_DATA(_size, DATA_PERSISTENT);
	*archive << ARCHIVE_DATA(_dynamic, DATA_PERSISTENT);

	DTushort *data_ushort = (DTushort*) _data;
	for (DTuint f = 0; f < _size/sizeof(DTushort); ++f) {
		*archive << ARCHIVE_DATA(*data_ushort, DATA_PERSISTENT);
		++data_ushort;
	}
	
    archive->popDomain ();
}

//==============================================================================
//==============================================================================

void* DeviceGraphicsDX11IndexStream::allocateData		(DTuint size, DTboolean dynamic)
{ 
	clearData();

	Assert(size);

	// Remember size
	_size = size;
	
	// Remember dynamic flag
	_dynamic = dynamic;

	// allocate buffer
	_data = (DTbyte*) new ("OpenGLIndex") DTbyte[_size];
	return _data;
}

void DeviceGraphicsDX11IndexStream::uploadData ()
{
    DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());
    ID3D11Device1 *device = renderer->getD3D11Device();

    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = _size;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    // Set usage flags
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
    _buffer = NULL;
    HRESULT hr = device->CreateBuffer(&desc, &data, &_buffer);
    Assert(SUCCEEDED(hr));

	// Don't need to keep the buffer around if using VBO
	if (_data) delete[] _data;
	_data = NULL;
}

void* DeviceGraphicsDX11IndexStream::getData (void)
{
	return _data;
}

void DeviceGraphicsDX11IndexStream::clearData (void)
{
    SAFE_RELEASE(_buffer);
	delete[] _data;
	
	//_buffer = 0;
	_data = NULL;
	_size = 0;
}

//==============================================================================
//==============================================================================

void* DeviceGraphicsDX11IndexStream::map (DTboolean read, DTboolean write)
{
	Assert(0);
	return NULL;
}

void DeviceGraphicsDX11IndexStream::unmap (void)
{
	Assert(0);
}
		
//==============================================================================
//==============================================================================
		
} // DT2
