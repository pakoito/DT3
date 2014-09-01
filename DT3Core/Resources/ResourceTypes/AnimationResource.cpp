//==============================================================================
///	
///	File: AnimationResource.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/ResourceTypes/AnimationResource.hpp"
#include "DT3Core/Resources/Importers/ImporterAnimation.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/System/Factory.hpp"
#include <algorithm>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_CREATION(AnimationResource)

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,AnimationResource::initialize_static())
GLOBAL_STATIC_DESTRUCTION(0,AnimationResource::uninitialize_static())

//==============================================================================
//==============================================================================

std::mutex                                                  AnimationResource::_animation_map_lock;
std::map<std::string, std::shared_ptr<AnimationResource>>   AnimationResource::_animation_map;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

AnimationResource::AnimationResource (void)
	:	_fps        (0.0F),
		_start_time	(0.0F),
		_end_time	(0.0F)
{    

}
					
AnimationResource::~AnimationResource (void)
{ 
	remove_all_tracks();
}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<AnimationResource> &r)
{
	if (r) {
		s << r->property_path();
	} else {
		s << "";
	}
	return s;
}

Stream& operator >>(Stream &s, std::shared_ptr<AnimationResource> &r)
{
	std::string path;
	s >> path;
	
	r = AnimationResource::import_resource(FilePath(path));
	return s;
}

//==============================================================================
//==============================================================================

void AnimationResource::reload_if_changed (void)
{
    FOR_EACH (i,_animation_map) {
        if (i->second->is_changed()) {
            LOG_MESSAGE << "Reloading : " << i->second->property_path();
            i->second->reload();
        }
    }
}

std::shared_ptr<AnimationResource> AnimationResource::import_resource (const FilePath &pathname, std::string args)
{	
    if (!pathname.exists() && !pathname.in_package()) {
        return NULL;
    }

    std::unique_lock<std::mutex> lock(_animation_map_lock);
    
    auto i = _animation_map.find(pathname.full_path());
    
    // If there is no resource loaded from that path
    if (i == _animation_map.end()) {
                        
        std::shared_ptr<AnimationResource> res = AnimationResource::create();
        DTerr err;
        if ((err = res->import(pathname,args)) != DT3_ERR_NONE) {
            return NULL;
        }
                    
        _animation_map[pathname.full_path()] = res;
        return res;
    }
    
    return i->second;
}

//==============================================================================
//==============================================================================

void AnimationResource::initialize_static (void)
{
    SystemCallbacks::reload_resources_cb().add(make_callback(&type::reload_if_changed));
}

void AnimationResource::uninitialize_static (void)
{
    SystemCallbacks::reload_resources_cb().remove(make_callback(&type::reload_if_changed));
}

//==============================================================================
//==============================================================================

DTerr AnimationResource::import (const FilePath &pathname, std::string args)
{
	DTerr err;
	if ((err = Resource::import (pathname,args)) != DT3_ERR_NONE)
		return err;
			
	FilePath original_path(path());
	std::string extension = original_path.file_ext();
	
	// Build a generic importer for the file type
	std::shared_ptr<BaseClass> generic_importer = Factory::create_importer(extension);
	if (!generic_importer || !generic_importer->isa(ImporterAnimation::kind())) {
		return DT3_ERR_FILE_WRONG_TYPE;
	}

	// Attempt to convert the importer of an image importer
	std::shared_ptr<ImporterAnimation> animation_importer = checked_static_cast<ImporterAnimation>(generic_importer);
    err = animation_importer->import(this, args);
	
	return err;
}

//==============================================================================
//==============================================================================

void AnimationResource::add_track (const std::shared_ptr<AnimationTrack> &track)
{
	_tracks.push_back(track);
}

void AnimationResource::remove_track (const std::shared_ptr<AnimationTrack> &track)
{
    auto i = std::remove(_tracks.begin(), _tracks.end(), track);
    _tracks.erase(i,_tracks.end());
}

void AnimationResource::remove_all_tracks	(void)
{
	_tracks.clear();
}

//==============================================================================
//==============================================================================

} // DT3

