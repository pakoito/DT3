#ifndef DT3_SCRIPTINGSOUNDBIQUADFILTER
#define DT3_SCRIPTINGSOUNDBIQUADFILTER
//==============================================================================
///	
///	File: ScriptingSoundBiquadFilter.hpp
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
/// Audio filter.
//==============================================================================

class ScriptingSoundBiquadFilter: public ScriptingSoundBase {
    public:
        DEFINE_TYPE(ScriptingSoundBiquadFilter,ScriptingSoundBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingSoundBiquadFilter      (void);
                                    ScriptingSoundBiquadFilter      (const ScriptingSoundBiquadFilter &rhs);
        ScriptingSoundBiquadFilter& operator =                      (const ScriptingSoundBiquadFilter &rhs);
        virtual                     ~ScriptingSoundBiquadFilter     (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
    
        enum {
            LOW_PASS,
            HIGH_PASS,
            BAND_PASS1,
            BAND_PASS2,
            NOTCH,
            APF,
            PEAKING_EQ,
            LOW_SHELF,
            HIGH_SHELF
        };
    
    
		Plug<SoundPacket>         _sound_packet_in;
		Plug<SoundPacket>         _sound_packet_out;
        
        Plug<DTuint>                                _filter_type;
        Plug<DTfloat>                               _cutoff_frequency;
        Plug<DTfloat>                               _db_gain;
        Plug<DTfloat>                               _Q;
        
        DTshort                                     _last_in_minus_1;
        DTshort                                     _last_in_minus_2;
        DTshort                                     _last_out_minus_1;
        DTshort                                     _last_out_minus_2;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
