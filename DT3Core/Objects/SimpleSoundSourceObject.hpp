#ifndef DT3_SIMPLESIMPLESOUNDSOURCEOBJECT
#define DT3_SIMPLESIMPLESOUNDSOURCEOBJECT
//==============================================================================
///	
///	File: SimpleSoundSourceObject.hpp
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

class ScriptingSound;
class SoundSource;

//==============================================================================
/// Simple sound source object to play a sound.
//==============================================================================

class SimpleSoundSourceObject: public PlaceableObject {
    public:
        DEFINE_TYPE(SimpleSoundSourceObject,PlaceableObject)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                                SimpleSoundSourceObject (void);
                                                SimpleSoundSourceObject (const SimpleSoundSourceObject &rhs);
        SimpleSoundSourceObject &               operator =              (const SimpleSoundSourceObject &rhs);
        virtual                                 ~SimpleSoundSourceObject(void);
  
        virtual void                            archive                 (const std::shared_ptr<Archive> &archive);
	
		/// Called to initialize the object
		virtual void                            initialize              (void);

		/// Sets the souund associated with this source.
		/// \param attr sound
        void                                    set_sound_property      (const std::shared_ptr<SoundResource> &attr);

		/// Gets the souund associated with this source.
		/// \return attr sound
        const std::shared_ptr<SoundResource>&   sound_property          (void) const				{   return _sound;	}


		/// Play the sound
		/// \param sender node (if any) that caused this event
        void                                    play                    (PlugNode *sender);

		/// Stop the sound
		/// \param sender node (if any) that caused this event
        void                                    stop                    (PlugNode *sender);

		/// Called when the sound is stopped
		/// \param sender node (if any) that caused this event
        void                                    stopped                 (PlugNode *sender);

		/// Callback called when the component is getting a tick
		/// \param dt delta time
        void                                    tick                    (const DTfloat dt);
        
		/// Object was added to a world
		/// world world that object was added to
        virtual void                            add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                            remove_from_world       (void);


	private:
    
		Plug<DTfloat>                           _gain;
		Plug<DTfloat>                           _pitch;
		Plug<DTfloat>                           _rolloff;
        
        Event                                   _play;
        Event                                   _stop;
        Event                                   _stopped;

		std::shared_ptr<SoundResource>          _sound;
        std::shared_ptr<ScriptingSound>         _sound_source;
		std::shared_ptr<SoundSource>            _source;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
