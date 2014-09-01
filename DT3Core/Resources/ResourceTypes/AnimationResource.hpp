#ifndef DT3_ANIMATIONRESOURCE
#define DT3_ANIMATIONRESOURCE
//==============================================================================
///	
///	File: AnimationResource.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/ResourceTypes/Resource.hpp"
#include <string>
#include <map>
#include <mutex>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class AnimationTrack;

//==============================================================================
/// Animation resource type
//==============================================================================

class AnimationResource: public Resource {
    public:
        DEFINE_TYPE(AnimationResource,Resource)
		DEFINE_CREATE

                                                AnimationResource	(void);
	private:
                                                AnimationResource	(const AnimationResource &rhs);
        AnimationResource &                     operator =			(const AnimationResource &rhs);
	public:
		virtual                                 ~AnimationResource	(void);
                
    public:        	
	
		/// Called to initialize the class
		static void                             initialize_static   (void);

		/// Called to uninitialize the class
		static void                             uninitialize_static (void);

		/// Import the resource with an importer into a platform independent format
		/// \param pathname path to resource
		/// \param args importer args
		/// \return error code
		DTerr                                   import				(const FilePath &pathname, std::string args);

		/// Add an animation track
		/// \param track animation track to add
		void                                    add_track			(const std::shared_ptr<AnimationTrack> &track);

		/// Remove the animation track
		/// \param track animation track to remove
		void                                    remove_track        (const std::shared_ptr<AnimationTrack> &track);

		/// Remove all animation tracks from the animation
		void                                    remove_all_tracks   (void);
		
		
		/// Gets the number of animation tracks
		/// \return number of tracks
		DTuint                                  num_tracks          (void) const				{	return static_cast<DTuint>(_tracks.size());	}

		/// Gets the animation track
		/// \param track index of track
		/// \return animation track
		const std::shared_ptr<AnimationTrack>&  track               (DTuint track) const		{	return _tracks[track];      }
		
		
		
		/// Sets the rate at which the animation plays
		/// \param fps frames per second
		void                                    set_fps				(DTuint fps)				{	_fps = fps;                 }

		/// Gets the rate at which the animation plays
		/// \return frames per second
		DTfloat                                 fps                 (void) const				{	return _fps;                }


		/// Sets the animation start time
		/// \param start_time animation start time
		void                                    set_start_time		(DTfloat start_time)		{	_start_time = start_time;	}

		/// Gets the animation start time
		/// \return animation start time
		DTfloat                                 start_time          (void) const				{	return _start_time;			}

		/// Sets the animation end time
		/// \param end_time animation end time
		void                                    set_end_time        (DTfloat end_time)			{	_end_time = end_time;		}

		/// Gets the animation end time
		/// \return animation end time
		DTfloat                                 end_time            (void) const				{	return _end_time;			}
		

		/// Load the resource
		/// \param pathname path to file
		/// \param args arguments to importer
		/// \return resource
		static std::shared_ptr<AnimationResource>  import_resource (const FilePath &pathname, std::string args = "");
					
	private:
        static std::mutex                                                   _animation_map_lock;
		static std::map<std::string, std::shared_ptr<AnimationResource>>    _animation_map;

        static void                             reload_if_changed   (void);

		std::vector<std::shared_ptr<AnimationTrack>>    _tracks;
		
		DTfloat                                         _fps;
		DTfloat                                         _start_time;
		DTfloat                                         _end_time;
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<AnimationResource> &r);
Stream& operator >>(Stream &s, std::shared_ptr<AnimationResource> &r);

//==============================================================================
//==============================================================================

} // DT3

#endif
