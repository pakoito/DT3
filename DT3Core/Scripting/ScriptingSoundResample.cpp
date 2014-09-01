//==============================================================================
///	
///	File: ScriptingSoundResample.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSoundResample.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSoundResample,"Sound",NULL)
IMPLEMENT_PLUG_NODE(ScriptingSoundResample)

IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_in)
IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_out)
IMPLEMENT_PLUG_INFO_INDEX(_sampling_rate)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSoundResample)

	PLUG_INIT(_sound_packet_in,"Sound_Packet_In")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true)
        .set_always_dirty(true);

	PLUG_INIT(_sound_packet_out,"Sound_Packet_Out")
        .set_single_output(true)
		.set_output(true)
        .set_always_dirty(true);
    
	PLUG_INIT(_sampling_rate,"Sampling_Rate")
        .set_input(true);
    
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingSoundResample::ScriptingSoundResample (void)
    :   _sound_packet_in    (PLUG_INFO_INDEX(_sound_packet_in)),
		_sound_packet_out   (PLUG_INFO_INDEX(_sound_packet_out)),
		_sampling_rate      (PLUG_INFO_INDEX(_sampling_rate), 44100),
        _sample_in         (0.0F)
{  

}
		
ScriptingSoundResample::ScriptingSoundResample (const ScriptingSoundResample &rhs)
    :   ScriptingSoundBase  (rhs),
		_sound_packet_in	(rhs._sound_packet_in),
		_sound_packet_out   (rhs._sound_packet_out),
        _sampling_rate      (rhs._sampling_rate),
        _sample_in          (rhs._sample_in)
{   

}	

ScriptingSoundResample & ScriptingSoundResample::operator = (const ScriptingSoundResample &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingSoundBase::operator = (rhs);

		_sound_packet_in = rhs._sound_packet_in;
		_sound_packet_out = rhs._sound_packet_out;
        _sampling_rate = rhs._sampling_rate;
        _sample_in = rhs._sample_in;
	}
    return (*this);
}
			
ScriptingSoundResample::~ScriptingSoundResample (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSoundResample::initialize (void)
{
	ScriptingSoundBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingSoundResample::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingSoundBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_PLUG(_sampling_rate, DATA_PERSISTENT | DATA_SETTABLE);
                                                                			
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingSoundResample::compute (const PlugBase *plug)
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
                    
        if (sound_packet_in.format() == SoundResource::FORMAT_MONO16) {
        
            sound_packet_out.set_format(sound_packet_in.format());
            sound_packet_out.set_frequency(_sampling_rate);
                
            DTshort *data_in = (DTshort *) sound_packet_in.buffer();
            DTshort *data_out = (DTshort *) sound_packet_out.buffer();
            
            DTfloat ratio = static_cast<DTfloat>(sound_packet_in.frequency()) / static_cast<DTfloat>(sound_packet_out.frequency());

            DTsize num_samples_in = sound_packet_in.num_samples();
            
            //
            // Run resampling once to get final number of samples
            //
            
            DTfloat old_sample_in = _sample_in;
            DTuint s = static_cast<DTuint>(_sample_in);
            DTuint sample_out = 0;

            while (s < (num_samples_in-1)) {                
                ++sample_out;
                _sample_in += ratio;
                s = static_cast<DTuint>(_sample_in);
            }
            
            sound_packet_out.set_num_bytes(sample_out*2);
            
            //
            // Run actual resampling
            //
            
            _sample_in = old_sample_in;
            s = static_cast<DTuint>(_sample_in);
            sample_out = 0;

            while (s < (num_samples_in-1)) {
                DTfloat t = _sample_in - std::floor(_sample_in);
                data_out[sample_out] = (DTshort) (data_in[s] * (1.0F-t) + data_in[s+1] * t);
                
                ++sample_out;
                _sample_in += ratio;
                
                s = static_cast<DTuint>(_sample_in);
            }
            
            _sample_in -= s;

        } else {
            sound_packet_out = sound_packet_in;
        }
    
		_sound_packet_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3

