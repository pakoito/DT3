//==============================================================================
///	
///	File: SoundResource.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/ResourceTypes/SoundResource.hpp"
#include "DT3Core/Resources/Importers/ImporterSound.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Sound/SoundPacket.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/System/Factory.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_CREATION(SoundResource)

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,SoundResource::initialize_static())
GLOBAL_STATIC_DESTRUCTION(0,SoundResource::uninitialize_static())

//==============================================================================
//==============================================================================

std::mutex                                              SoundResource::_sound_map_lock;
std::map<std::string, std::shared_ptr<SoundResource>>   SoundResource::_sound_map;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

SoundResource::SoundResource (void)
    :   _format							(FORMAT_STEREO16),
        _frequency						(0)
{

}

SoundResource::~SoundResource (void)
{

}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<SoundResource> &r)
{
	if (r) {
		s << r->property_path();
	} else {
		s << "";
	}
	return s;
}

Stream& operator >>(Stream &s, std::shared_ptr<SoundResource> &r)
{
	std::string path;
	s >> path;
	
	r = SoundResource::import_resource(FilePath(path));
	return s;
}

//==============================================================================
//==============================================================================

void SoundResource::reload_if_changed (void)
{
    FOR_EACH (i,_sound_map) {
        if (i->second->is_changed()) {
            LOG_MESSAGE << "Reloading : " << i->second->property_path();
            i->second->reload();
        }
    }
}

std::shared_ptr<SoundResource> SoundResource::import_resource (const FilePath &pathname, std::string args)
{	
    if (!pathname.exists() && !pathname.in_package()) {
        return NULL;
    }

    std::unique_lock<std::mutex> lock(_sound_map_lock);
    
    auto i = _sound_map.find(pathname.full_path());
    
    // If there is no resource loaded from that path
    if (i == _sound_map.end()) {
                        
        std::shared_ptr<SoundResource> res = SoundResource::create();
        DTerr err;
        if ((err = res->import(pathname,args)) != DT3_ERR_NONE) {
            return NULL;
        }
                    
        _sound_map[pathname.full_path()] = res;
        return res;
    }
    
    return i->second;
}

//==============================================================================
//==============================================================================

void SoundResource::initialize_static (void)
{
    SystemCallbacks::reload_resources_cb().add(make_callback(&type::reload_if_changed));
}

void SoundResource::uninitialize_static (void)
{
    SystemCallbacks::reload_resources_cb().remove(make_callback(&type::reload_if_changed));
}

//==============================================================================
//==============================================================================

void SoundResource::streamed_chunk (SoundPacket &packet, DTsize chunk_index) const
{
	// First chunk is preloaded one
	if (chunk_index >= count_chunks()) {
    
        packet.set_num_bytes(0);
        packet.set_format(format());
        packet.set_frequency(frequency());
   
	} else {
        // Start with maximum size
        packet.set_num_bytes(DT3_AUDIO_STREAMING_BUFFER_SIZE);
        
        DTsize num_bytes = _importer->stream(packet.buffer(), DT3_AUDIO_STREAMING_BUFFER_SIZE * chunk_index, DT3_AUDIO_STREAMING_BUFFER_SIZE);
        
        // Set to actual size
        packet.set_num_bytes(num_bytes);
        packet.set_format(format());
        packet.set_frequency(frequency());
                
	}
}

DTsize SoundResource::count_chunks (void) const
{
    DTsize c = _importer->length() / DT3_AUDIO_STREAMING_BUFFER_SIZE;
    
    if (_importer->length() % DT3_AUDIO_STREAMING_BUFFER_SIZE != 0)
        ++c;

    return c;
}

DTsize SoundResource::length (void) const
{
    return _importer->length();
}

//==============================================================================
//==============================================================================

DTsize SoundResource::num_samples (void) const
{
    DTsize l = length();
    
    switch (_format) {
        case FORMAT_MONO16:     return l/2;
        case FORMAT_STEREO16:   return l/4;
        default:
            return 0;
    };
    
    
}

DTfloat SoundResource::sample (DTsize s) const
{
    switch (_format) {
        case FORMAT_MONO16: {
            DTshort data;
            _importer->stream((DTubyte*) &data, s*2, sizeof(data));
            return (DTint) data / static_cast<DTfloat>(DTSHORT_MAX);
        }
            
        case FORMAT_STEREO16: {
            DTshort data[2];
            _importer->stream((DTubyte*) data, s*4, sizeof(data));
            return ((DTint) (data[0]+data[1])/2) / static_cast<DTfloat>(DTSHORT_MAX);
        }

        default:
            return 0.0F;
    };
 
}

void SoundResource::sample_window (DTsize s, DTsize num, DTfloat &min_sample, DTfloat &max_sample) const
{
    switch (_format) {
        case FORMAT_MONO16: {
            DTshort *data = new DTshort[num];
            _importer->stream((DTubyte*) &data, s*2, sizeof(num*2));

            DTshort min_s = data[0];
            DTshort max_s = data[0];
            
            DTshort *ip = data;
            for (DTuint i = 0; i < num; ++i, ++ip) {
                min_s = MoreMath::min(min_s, *ip);
                max_s = MoreMath::min(max_s, *ip);
            }
            
            min_sample = (DTint) min_s / static_cast<DTfloat>(DTSHORT_MAX);
            max_sample = (DTint) max_s / static_cast<DTfloat>(DTSHORT_MAX);
            
			delete[] data;

            return;
        }
                
        case FORMAT_STEREO16: {
            DTshort *data = new DTshort[num*2];
            _importer->stream((DTubyte*) data, s*4, sizeof(num*4));

            DTshort min_s = data[0];
            DTshort max_s = data[0];
            
            DTshort *ip = data;
            for (DTuint i = 0; i < num*2; ++i, ++ip) {
                min_s = MoreMath::min(min_s, *ip);
                max_s = MoreMath::min(max_s, *ip);
            }
            
            min_sample = (DTint) min_s / static_cast<DTfloat>(DTSHORT_MAX);
            max_sample = (DTint) max_s / static_cast<DTfloat>(DTSHORT_MAX);
            
			delete[] data;

            return;
        }

        default:
            min_sample = 0.0F;
            max_sample = 0.0F;
            return;
    };
}

//==============================================================================
//==============================================================================

DTerr SoundResource::import (const FilePath &pathname, std::string args)
{
	DTerr err;
	if ((err = Resource::import (pathname,args)) != DT3_ERR_NONE)
		return err;
	
	FilePath original_path(path());
	std::string extension = original_path.file_ext();
	
	// Build a generic importer for the file type
	std::shared_ptr<BaseClass> generic_importer = Factory::create_importer(extension);
	if (!generic_importer || !generic_importer->isa(ImporterSound::kind())) {
		return DT3_ERR_FILE_WRONG_TYPE;
	}

	// Attempt to convert the importer of an image importer
	std::shared_ptr<ImporterSound> sound_importer = checked_static_cast<ImporterSound>(generic_importer);
    err = sound_importer->import(this, args);

	_importer = sound_importer;
	
	return err;	
}

//==============================================================================
//==============================================================================




} // DT3
