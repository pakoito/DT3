//==============================================================================
///	
///	File: ArchivePropertyReaderWriter.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/ArchivePropertyReaderWriter.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/TextBufferStream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ArchivePropertyReaderWriter::ArchivePropertyReaderWriter (void)
	:	_loading	(false)
{    

}
			
ArchivePropertyReaderWriter::~ArchivePropertyReaderWriter (void)
{
	clear();
}

//==============================================================================
//==============================================================================

Archive &	 ArchivePropertyReaderWriter::operator << (const ArchiveData& data)
{
	// Fill out the current domain with ArchiveData objects
	if (data.flags() & DATA_SETTABLE) {
	
		if (!_loading) {
			std::shared_ptr<ArchiveData> copy = checked_static_cast<ArchiveData>(data.clone());
		
			// add the property to the list to be modified by the GUI
			_properties.push_back(copy);
			
		} else {
			TextBufferStream prop_stream;
			TextBufferStream val_stream;

			_properties[_loading_index]->value(prop_stream);
			data.value(val_stream);
			
			// Check if the parameter has changed and change it
			if (prop_stream.buffer() != val_stream.buffer()) {
				data.set_value(prop_stream);
			}
			
			++_loading_index;
		}

	}
	
	return *this;
}

//==============================================================================
//==============================================================================

void ArchivePropertyReaderWriter::push_domain (const std::string &domain)
{
	// ignore domains
}

void ArchivePropertyReaderWriter::pop_domain (void)
{
	// ignore domains
}

//==============================================================================
//==============================================================================

DTboolean ArchivePropertyReaderWriter::is_reading (void) const
{
	return _loading;
}

DTboolean ArchivePropertyReaderWriter::is_writing (void) const
{
	return !_loading;
}

//==============================================================================
//==============================================================================

void ArchivePropertyReaderWriter::begin_reading	(void)
{
	_loading = true;
	_loading_index = 0;
}

void ArchivePropertyReaderWriter::begin_writing (void)
{
	clear();
	_loading = false;
}

//==============================================================================
//==============================================================================
		
void ArchivePropertyReaderWriter::clear (void)
{
	_properties.clear();
}

//==============================================================================
//==============================================================================

std::shared_ptr<ArchiveData> ArchivePropertyReaderWriter::property (const std::string &name)
{
    for (DTuint i = 0; i < _properties.size(); ++i)
        if (_properties[i]->title() == name)
            return _properties[i];

    return NULL;
}

//==============================================================================
//==============================================================================		

} // DT3

