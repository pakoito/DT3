#ifndef DT3_SOUNDSOURCEOBJECT
#define DT3_SOUNDSOURCEOBJECT
//==============================================================================
///	
///	File: SoundSourceObject.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/PlaceableObject.hpp"
#include "DT3Core/Types/Sound/SoundPacket.hpp"

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class SoundSource;

//==============================================================================
/// Sound source object to play a sound.
//==============================================================================

class SoundSourceObject: public PlaceableObject {
    public:
        DEFINE_TYPE(SoundSourceObject,PlaceableObject)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    SoundSourceObject				(void);	
                                    SoundSourceObject				(const SoundSourceObject &rhs);
        SoundSourceObject &			operator =						(const SoundSourceObject &rhs);	
        virtual                     ~SoundSourceObject				(void);
  
        virtual void				archive                         (const std::shared_ptr<Archive> &archive);
	
		/// Called to initialize the object
		virtual void				initialize						(void);
	
		/// Play the sound
		/// \param sender node (if any) that caused this event
        void                        play                            (PlugNode *sender);

		/// Stop the sound
		/// \param sender node (if any) that caused this event
        void                        stop                            (PlugNode *sender);

		/// Called when the sound is stopped
		/// \param sender node (if any) that caused this event
        void                        stopped                         (PlugNode *sender);

		/// Callback called when the component is getting a tick
		/// \param dt delta time
        void                        tick                            (const DTfloat dt);

		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world                    (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world               (void);

	private:
        Plug<SoundPacket>               _sound_packet;
    
		Plug<DTfloat>                   _gain;
		Plug<DTfloat>                   _pitch;
		Plug<DTfloat>                   _rolloff;
        
        Event                           _play;
        Event                           _stop;
        Event                           _stopped;

		std::shared_ptr<SoundSource>    _source;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
