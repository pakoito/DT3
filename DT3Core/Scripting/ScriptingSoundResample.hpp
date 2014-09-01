#ifndef DT3_SCRIPTINGSOUNDRESAMPLE
#define DT3_SCRIPTINGSOUNDRESAMPLE
//==============================================================================
///	
///	File: ScriptingSoundResample.hpp
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
/// Resample the sound stream.
//==============================================================================

class ScriptingSoundResample: public ScriptingSoundBase {
    public:
        DEFINE_TYPE(ScriptingSoundResample,ScriptingSoundBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingSoundResample  (void);	
									ScriptingSoundResample  (const ScriptingSoundResample &rhs);
        ScriptingSoundResample &	operator =				(const ScriptingSoundResample &rhs);	
        virtual                     ~ScriptingSoundResample (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<SoundPacket>           _sound_packet_in;
		Plug<SoundPacket>           _sound_packet_out;
        
        Plug<DTuint>                _sampling_rate;
        DTfloat                     _sample_in;
    };

//==============================================================================
//==============================================================================

} // DT3

#endif
