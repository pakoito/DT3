//==============================================================================
///	
///	File: ImporterAnimationTWA.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/Importers/ImporterAnimationTWA.hpp"
#include "DT3Core/Resources/ResourceTypes/AnimationResource.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/FileBuffer/BinaryFileStream.hpp"
#include "DT3Core/Types/Utility/Error.hpp"
#include "DT3Core/Types/Animation/AnimationKeyframe.hpp"
#include "DT3Core/Types/Animation/AnimationTrack.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_IMPORTER(ImporterAnimationTWA,twa)

//==============================================================================
/// Some constants that are needed
//==============================================================================



//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ImporterAnimationTWA::ImporterAnimationTWA (void)
{    
	
}
			
ImporterAnimationTWA::~ImporterAnimationTWA (void)
{ 

}

//==============================================================================
//==============================================================================

DTerr ImporterAnimationTWA::import(AnimationResource *target, std::string args)
{
	FilePath pathname(target->path());
    
    // open the file
    BinaryFileStream file;
	FileManager::open(file, pathname, true);
     
    // 
	// Read_ in header
	//
	
	DTuint magic;
	file >> magic;
    if (magic != MAGIC) {
        return DT3_ERR_FILE_WRONG_TYPE;
    }
	
	DTuint version;
	file >> version;
	
	//
	// Read_ in data
	//
	
	DTint section,size;
	file >> section >> size;
	switch(section) {
		case FILE:	read_file(file, size);			break;
		default:	WARNINGMSG("Invalid section");	break;
	};

	//
	// Convert to internal data structures
	//
	
	target->set_fps(_animation._fps);
	target->set_start_time(_animation._start);
	target->set_end_time(_animation._end);
	
	FOR_EACH (i,_animation._tracks) {
		std::string track_name = i->first;
		Track &track = i->second;
		
		// copy keyframes
		std::vector<AnimationKeyframe> new_keyframes;
		new_keyframes.resize( track._keyframes.size() );
		
		for (DTuint j = 0; j < track._keyframes.size(); ++j) {
			new_keyframes[j].set_time(track._keyframes[j]._time);
			new_keyframes[j].set_transform(track._keyframes[j]._rotation, track._keyframes[j]._translation);
		}
		
		// add the new track
		std::shared_ptr<AnimationTrack> anim_track(AnimationTrack::create());
		anim_track->set_name(track_name);
		anim_track->set_keyframes(new_keyframes);
		target->add_track(anim_track);

	}

	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

void ImporterAnimationTWA::read_animation_keyframes (BinaryFileStream &file, DTuint remaining_size, std::vector<KeyFrame> &keyframes)
{

	//
	// Track info
	//
	
	DTuint size;
	file >> size;		
	keyframes.resize(size);

	//
	// Individual keyframes
	//
	for (DTuint i = 0; i < keyframes.size(); ++i) {
		file >> keyframes[i]._time;
        
        Quaternion q;
        file >> q.w >> q.x >> q.y >> q.z;
        
        Vector3 t;
        file >> t.x >> t.y >> t.z;
        
        keyframes[i]._rotation = q;
        keyframes[i]._translation = t;
        
		/*file >> keyframes[i].transform._m11 >> keyframes[i].transform._m21 >> keyframes[i].transform._m31;
		file >> keyframes[i].transform._m12 >> keyframes[i].transform._m22 >> keyframes[i].transform._m32;
		file >> keyframes[i].transform._m13 >> keyframes[i].transform._m23 >> keyframes[i].transform._m33;
		file >> keyframes[i].transform._m14 >> keyframes[i].transform._m24 >> keyframes[i].transform._m34;
        
        keyframes[i].transform._m41 = 0.0F;
        keyframes[i].transform._m42 = 0.0F;
        keyframes[i].transform._m43 = 0.0F;
        keyframes[i].transform._m44 = 1.0F;*/
	}

}

//==============================================================================
//==============================================================================

void ImporterAnimationTWA::read_animation_tracks (BinaryFileStream &file, DTuint remaining_size, std::map<std::string,Track> &tracks)
{
	DTsize start_pos = file.g();
	
	//
	// Number of tracks
	//
	
	DTuint size;
	file >> size;		
	//tracks.resize(size);
	
	//
	// read tracks
	//
	
	while (file.g() - start_pos < remaining_size) {
		Track track;
		
		std::string track_name;
		file >> track_name;
	
		DTint section, section_size;
		file >> section >> section_size;
	
		switch(section) {
			case ANIMATION_TRACK:	read_animation_keyframes(file, section_size, track._keyframes);		break;
			default:				WARNINGMSG("Invalid section");										break;
		};
		
		// add the track to the map
		tracks[track_name] = track;
	}

}

//==============================================================================
//==============================================================================

void ImporterAnimationTWA::read_animation (BinaryFileStream &file, DTuint remaining_size, Animation &animation)
{
	DTsize start_pos = file.g();
	
	//
	// Animation information
	//
	
	file >> animation._fps;
	file >> animation._start;
	file >> animation._end;
	
	//
	// read all of the tracks
	//
	
	while (file.g() - start_pos < remaining_size) {
		DTint section, size;
		file >> section >> size;
	
		switch(section) {
			case ANIMATION_TRACKS:	read_animation_tracks(file, size, animation._tracks);		break;
			default:				WARNINGMSG("Invalid section");								break;
		};
	}
	
}

//==============================================================================
//==============================================================================

void ImporterAnimationTWA::read_file (BinaryFileStream &file, DTuint remaining_size)
{
	DTsize start_pos = file.g();

	//
	// Scene header
	//
	
	while (file.g() - start_pos < remaining_size) {
		DTint section, size;
		file >> section >> size;
	
		switch(section) {
			case ANIMATION:		read_animation(file, size, _animation);		break;
			default:			WARNINGMSG("Invalid section");				break;
		};
	}
}

//==============================================================================
//==============================================================================

} // DT3

