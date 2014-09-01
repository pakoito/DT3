//==============================================================================
///	
///	File: ScriptingSoundGain.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSoundGain.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSoundGain,"Sound",NULL)
IMPLEMENT_PLUG_NODE(ScriptingSoundGain)

IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_in)
IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_out)
IMPLEMENT_PLUG_INFO_INDEX(_output_gain)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSoundGain)

	PLUG_INIT(_sound_packet_in,"Sound_Packet_In")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true)
        .set_always_dirty(true);

	PLUG_INIT(_sound_packet_out,"Sound_Packet_Out")
        .set_single_output(true)
		.set_output(true)
        .set_always_dirty(true);
    
	PLUG_INIT(_output_gain,"Output_Gain")
        .set_input(true);
    
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingSoundGain::ScriptingSoundGain (void)
    :   _sound_packet_in    (PLUG_INFO_INDEX(_sound_packet_in)),
		_sound_packet_out   (PLUG_INFO_INDEX(_sound_packet_out)),
		_output_gain        (PLUG_INFO_INDEX(_output_gain), 1.0F)
{  

}
		
ScriptingSoundGain::ScriptingSoundGain (const ScriptingSoundGain &rhs)
    :   ScriptingSoundBase  (rhs),
		_sound_packet_in	(rhs._sound_packet_in),
		_sound_packet_out   (rhs._sound_packet_out),
        _output_gain        (rhs._output_gain)
{   

}	

ScriptingSoundGain & ScriptingSoundGain::operator = (const ScriptingSoundGain &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingSoundBase::operator = (rhs);

		_sound_packet_in = rhs._sound_packet_in;
		_sound_packet_out = rhs._sound_packet_out;
        _output_gain = rhs._output_gain;
	}
    return (*this);
}
			
ScriptingSoundGain::~ScriptingSoundGain (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSoundGain::initialize (void)
{
	ScriptingSoundBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingSoundGain::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingSoundBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_PLUG(_output_gain, DATA_PERSISTENT | DATA_SETTABLE);
                                                                			
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingSoundGain::compute (const PlugBase *plug)
{
	PROFILER(SOUND);

    if (super_type::compute(plug))  return true;

	if (plug == &_sound_packet_out) {
    
        SoundPacket &sound_packet_in = _sound_packet_in.as_ref_no_compute();
        SoundPacket &sound_packet_out = _sound_packet_out.as_ref_no_compute();
        
        if (!_sound_packet_in.has_incoming_connection()) {
            sound_packet_out.set_format(SoundResource::FORMAT_STEREO16);
            sound_packet_out.set_num_bytes(0);
            sound_packet_out.set_frequency(44100);
            return true;
        }
        
        // This copies the buffer so no extra memory is needed
        sound_packet_out = sound_packet_in;
                    
        if (sound_packet_in.format() == SoundResource::FORMAT_MONO16 ||
            sound_packet_in.format() == SoundResource::FORMAT_STEREO16) {
                
            DTshort *data_in = (DTshort *) sound_packet_in.buffer();
            DTshort *data_out = (DTshort *) sound_packet_out.buffer();

            DTsize num_samples = sound_packet_in.num_samples();
            DTint gain_int = UNIT_TO_10BIT_INT(_output_gain);
            
            for (DTsize s = 0; s < num_samples; ++s) {
                (*data_out) = (DTshort) REM_10BIT(CLAMP_PAD_10BIT( (*data_in) * gain_int));
                
                ++data_in;
                ++data_out;
            }
            
        }
    
		_sound_packet_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3

