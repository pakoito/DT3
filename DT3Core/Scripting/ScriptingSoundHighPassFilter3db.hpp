#ifndef DT3_SCRIPTINGSOUNDHIGHPASSFILTER3DB
#define DT3_SCRIPTINGSOUNDHIGHPASSFILTER3DB
//==============================================================================
///	
///	File: ScriptingSoundHighPassFilter3db.hpp
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
/// Cheap Audio high pass filter.
//==============================================================================

class ScriptingSoundHighPassFilter3db: public ScriptingSoundBase {
    public:
        DEFINE_TYPE(ScriptingSoundHighPassFilter3db,ScriptingSoundBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingSoundHighPassFilter3db     (void);	
                                    ScriptingSoundHighPassFilter3db     (const ScriptingSoundHighPassFilter3db &rhs);
        ScriptingSoundHighPassFilter3db& operator =                     (const ScriptingSoundHighPassFilter3db &rhs);	
        virtual                     ~ScriptingSoundHighPassFilter3db    (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<SoundPacket>         _sound_packet_in;
		Plug<SoundPacket>         _sound_packet_out;
        
        Plug<DTfloat>                               _cutoff_frequency;
        
        DTshort                                     _last_out_sample_left;
        DTshort                                     _last_in_sample_left;
        DTshort                                     _last_out_sample_right;
        DTshort                                     _last_in_sample_right;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
