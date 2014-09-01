#ifndef DT3_SCRIPTINGSOUND
#define DT3_SCRIPTINGSOUND
//==============================================================================
///	
///	File: ScriptingSound.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSoundBase.hpp"
#include "DT3Core/Types/Sound/SoundPacket.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class SoundResource;

//==============================================================================
/// Sound Loader.
//==============================================================================

class ScriptingSound: public ScriptingSoundBase {
    public:
        DEFINE_TYPE(ScriptingSound,ScriptingSoundBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingSound          (void);	
									ScriptingSound          (const ScriptingSound &rhs);
        ScriptingSound &			operator =				(const ScriptingSound &rhs);	
        virtual                     ~ScriptingSound         (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void                            initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean                               compute					(const PlugBase *plug);

		/// Reset Event
        void                                    event_reset             (PlugNode *sender);

		/// Sets the sound
		/// \param attr sound attribute
        void                                    set_sound_property      (const std::shared_ptr<SoundResource> &sound);

		/// Gets the sound
		/// \return sound attribute
        const std::shared_ptr<SoundResource>&   sound_property          (void) const				{   return _sound;	}

	private:		
		std::shared_ptr<SoundResource>              _sound;
		Plug<SoundPacket>     _sound_packet;
        
        Plug<DTboolean>                             _looping;
        Event                                       _reset_e;
        
        DTuint                                      _chunk_index;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
