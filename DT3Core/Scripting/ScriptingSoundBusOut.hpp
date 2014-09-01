#ifndef DT3_SCRIPTINGSOUNDBUSOUT
#define DT3_SCRIPTINGSOUNDBUSOUT
//==============================================================================
///	
///	File: ScriptingSoundBusOut.hpp
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

class SoundSource;

//==============================================================================
/// Audio bus output.
//==============================================================================

class ScriptingSoundBusOut: public ScriptingSoundBase {
    public:
        DEFINE_TYPE(ScriptingSoundBusOut,ScriptingSoundBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingSoundBusOut    (void);	
									ScriptingSoundBusOut    (const ScriptingSoundBusOut &rhs);
        ScriptingSoundBusOut &		operator =				(const ScriptingSoundBusOut &rhs);	
        virtual                     ~ScriptingSoundBusOut   (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world       (void);

	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);
        
        DEFINE_ACCESSORS(gain_left, set_gain_left, DTfloat, _gain_left)
        DEFINE_ACCESSORS(gain_right, set_gain_right, DTfloat, _gain_right)

        DEFINE_ACCESSORS(level_left, set_level_left, DTfloat, _level_left)
        DEFINE_ACCESSORS(level_right, set_level_right, DTfloat, _level_right)

	private:		
		Plug<SoundPacket>     _sound_packet_in;
		Plug<SoundPacket>     _sound_packet_out;
        
        Plug<DTfloat>                               _gain_left;
        Plug<DTfloat>                               _gain_right;

        Plug<DTfloat>                               _level_left;
        Plug<DTfloat>                               _level_right;
        
		std::shared_ptr<SoundSource>                _source;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
