//==============================================================================
///	
///	File: KeyedSpriteAnimationResource.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "KeyedSpriteAnimationResource.hpp"
#include "ResourceManager.hpp"
#include "Archive.hpp"
#include "System.hpp"
#include "Factory.hpp"
#include "ImporterKeyedSpriteAnimation.hpp"
#include "BinaryStream.hpp"
#include "TextStream.hpp"
#include "FilePath.hpp"
#include "KeyedSpriteAnimationTrack.hpp"
#include "KeyedSpriteAnimationPoseJoint.hpp"
#include "KeyedSpriteAnimation.hpp"
#include "ArchiveProcessPointersRef.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_CREATION(KeyedSpriteAnimationResource)
IMPLEMENT_RESOURCE_CREATION(KeyedSpriteAnimationResource)
IMPLEMENT_STANDARD_RESOURCE_HANDLERS(KeyedSpriteAnimationResource,_keyed_sprite_animation_map,_keyed_sprite_animation_map_lock)

//==============================================================================
//==============================================================================

SpinLock                                                            KeyedSpriteAnimationResource::_keyed_sprite_animation_map_lock;
std::map<String, std::shared_ptr<KeyedSpriteAnimationResource> >	KeyedSpriteAnimationResource::_keyed_sprite_animation_map;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

KeyedSpriteAnimationResource::KeyedSpriteAnimationResource (void)
{    

}

KeyedSpriteAnimationResource::~KeyedSpriteAnimationResource (void)
{ 
	clear_animations();
}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, KeyedSpriteAnimationResource* &r)
{
	if (r) {
		s << r->getPropertyPath();
	} else {
		s << "";
	}
	return s;
}

Stream& operator >>(Stream &s, KeyedSpriteAnimationResource* &r)
{
	std::string path;
	s >> path;
	
	r = KeyedSpriteAnimationResource::keyed_sprite_animation(FilePath(path)).get();
	return s;
}

Stream& operator <<(Stream &s, const std::shared_ptr<KeyedSpriteAnimationResource> &r)
{
	if (r.isValid()) {
		s << r->property_path();
	} else {
		s << "";
	}
	return s;
}

Stream& operator >>(Stream &s, std::shared_ptr<KeyedSpriteAnimationResource> &r)
{
	std::string path;
	s >> path;
	
	r = KeyedSpriteAnimationResource::keyed_sprite_animation(FilePath(path));
	return s;
}

//==============================================================================
//==============================================================================

std::shared_ptr<KeyedSpriteAnimationResource> KeyedSpriteAnimationResource::keyed_sprite_animation (const FilePath &pathname, std::string args)
{	
	return getImpl<KeyedSpriteAnimationResource>::go(_keyed_sprite_animation_map, pathname, args, _keyed_sprite_animation_map_lock);
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationResource::archive_read (Archive *archive)
{
    archive->pushDomain (getClassID ());

	Resource::archive_read(archive);

	DTuint animationCount;
	*archive << ARCHIVE_DATA(animationCount,DATA_PERSISTENT);
	_animations.resize(animationCount, NULL);

	FOR_EACH (i,_animations) {
		archive->add_post_process(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,*i));
	}
	
    archive->pop_domain ();
}

void KeyedSpriteAnimationResource::archive_write (Archive *archive)
{
	Resource::archive_write(archive);

    archive->push_domain (getClassID ());

	DTuint animationCount = (DTuint) _animations.size();
	*archive << ARCHIVE_DATA(animationCount,DATA_PERSISTENT);
	
	FOR_EACH (i,_animations) {
		archive->add_post_process(ARCHIVE_PROCESS_POINTERS_REFERENCED(archive,*i));
	}

    archive->pop_domain ();
}
        
//==============================================================================
//==============================================================================

DTerr KeyedSpriteAnimationResource::import (const FilePath &pathname, String args)
{
	DTerr err;
	if ((err = Resource::import (pathname,args)) != DT3_ERR_NONE)
		return err;
		
	//if ((err = reload ()) != DT3_ERR_NONE)
	//	return err;
	
	return DT3_ERR_NONE;
}

DTerr KeyedSpriteAnimationResource::recache (void)
{
	FilePath original_path(getPath());
	String extension = original_path.file_ext();
	
	// Build a generic importer for the file type
	BaseClass *generic_importer = Factory::create_importer(extension);
	if (!generic_importer || !generic_importer->isA(ImporterKeyedSpriteAnimation::kind())) {
		return DT3_ERR_FILE_WRONG_TYPE;
	}

	// Attempt to convert the importer of an image importer
	ImporterKeyedSpriteAnimation *animation_importer = checked_static_cast<ImporterKeyedSpriteAnimation*>(generic_importer);
	DTerr err = animation_importer->import(this, getArgs());
	
	return err;
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationResource::copy (const KeyedSpriteAnimationResource &rhs)
{
    clearAnimations();
    
	FOR_EACH (i,rhs._animations) {
		_animations.pushBack( (**i).clone() );
	}
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationResource::add_animation (KeyedSpriteAnimation *animation)
{
	_animations.push_back(animation);
}

void KeyedSpriteAnimationResource::delete_animation (KeyedSpriteAnimation *animation)
{
	auto i = _animations.find(animation);
	if (i != _animations.end()) {
		_animations.erase(i);
	}
}

KeyedSpriteAnimation* KeyedSpriteAnimationResource::animation_by_name (const String &animation_name)
{
	FOR_EACH (i,_animations) {
		if ( (**i).name() == animation_name )
			return *i;
	}

	return NULL;
}

void KeyedSpriteAnimationResource::clear_animations (void)
{
	_animations.clear();
}

//==============================================================================
//==============================================================================

} // DT3

