#ifndef DT3_SOUNDBUSSOURCEOBJECT
#define DT3_SOUNDBUSSOURCEOBJECT
//==============================================================================
///	
///	File: SoundBusSourceObject.hpp
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
/// Bus sound source object to play a sound.
//==============================================================================

class SoundBusSourceObject: public PlaceableObject {
    public:
        DEFINE_TYPE(SoundBusSourceObject,PlaceableObject)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    SoundBusSourceObject			(void);	
                                    SoundBusSourceObject			(const SoundBusSourceObject &rhs);
        SoundBusSourceObject &      operator =						(const SoundBusSourceObject &rhs);	
        virtual                     ~SoundBusSourceObject			(void);
  
        virtual void				archive                         (const std::shared_ptr<Archive> &archive);
	
		/// Called to initialize the object
		virtual void				initialize						(void);
	
		/// Play the bus
		/// \param sender node (if any) that caused this event
        void                        play                            (PlugNode *sender);

		/// Stop the bus
		/// \param sender node (if any) that caused this event
        void                        stop                            (PlugNode *sender);

		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world                    (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world               (void);

        /// Accessors for the bus name
        DEFINE_ACCESSORS(bus_name, set_bus_name, std::string, _bus_name)
        
	private:
        Plug<SoundPacket>           _sound_packet;
        std::string                 _bus_name;
        
        Event                       _play;
        Event                       _stop;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
