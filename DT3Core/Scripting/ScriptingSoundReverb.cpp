//==============================================================================
///	
///	File: ScriptingSoundReverb.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSoundReverb.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSoundReverb,"Sound",NULL)
IMPLEMENT_PLUG_NODE(ScriptingSoundReverb)

IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_in)
IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_out)
IMPLEMENT_PLUG_INFO_INDEX(_delay)
IMPLEMENT_PLUG_INFO_INDEX(_reverb_gain)
IMPLEMENT_PLUG_INFO_INDEX(_output_gain)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSoundReverb)

	PLUG_INIT(_sound_packet_in,"Sound_Packet_In")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true)
        .set_always_dirty(true);

	PLUG_INIT(_sound_packet_out,"Sound_Packet_Out")
        .set_single_output(true)
		.set_output(true)
        .set_always_dirty(true);

	PLUG_INIT(_delay,"Delay")
		.set_input(true);

	PLUG_INIT(_reverb_gain,"Reverb_Gain")
        .set_input(true);
    
	PLUG_INIT(_output_gain,"Output_Gain")
        .set_input(true);
    
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingSoundReverb::ScriptingSoundReverb (void)
    :   _sound_packet_in    (PLUG_INFO_INDEX(_sound_packet_in)),
		_sound_packet_out   (PLUG_INFO_INDEX(_sound_packet_out)),
		_delay              (PLUG_INFO_INDEX(_delay), 1000),
		_reverb_gain        (PLUG_INFO_INDEX(_reverb_gain), 0.5F),
		_output_gain        (PLUG_INFO_INDEX(_output_gain), 0.5F),
        _sample_index       (0)
{  

}
		
ScriptingSoundReverb::ScriptingSoundReverb (const ScriptingSoundReverb &rhs)
    :   ScriptingSoundBase  (rhs),
		_sound_packet_in	(rhs._sound_packet_in),
		_sound_packet_out   (rhs._sound_packet_out),
        _delay              (rhs._delay),
        _reverb_gain        (rhs._reverb_gain),
        _output_gain        (rhs._output_gain),
        _sample_index       (0)
{   

}	

ScriptingSoundReverb & ScriptingSoundReverb::operator = (const ScriptingSoundReverb &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingSoundBase::operator = (rhs);

		_sound_packet_in = rhs._sound_packet_in;
		_sound_packet_out = rhs._sound_packet_out;
        _delay = rhs._delay;
        _reverb_gain = rhs._reverb_gain;
        _output_gain = rhs._output_gain;
        _sample_index = rhs._sample_index;
	}
    return (*this);
}
			
ScriptingSoundReverb::~ScriptingSoundReverb (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSoundReverb::initialize (void)
{
	ScriptingSoundBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingSoundReverb::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingSoundBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_PLUG(_delay, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_reverb_gain, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_output_gain, DATA_PERSISTENT | DATA_SETTABLE);
                                                                			
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingSoundReverb::compute (const PlugBase *plug)
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
            
        if (sound_packet_in.format() == SoundResource::FORMAT_MONO16) {
       
            sound_packet_out.set_format(sound_packet_in.format());
            sound_packet_out.set_frequency(sound_packet_in.frequency());
            sound_packet_out.set_num_bytes(sound_packet_in.num_bytes());

            // Check if samples buffer needs resizing
            if (_samples_left.size() != _delay) {
                _samples_left.resize(_delay);
                _sample_index = 0;            
            }
        
            DTshort *data_in = (DTshort *) sound_packet_in.buffer();
            DTshort *data_out = (DTshort *) sound_packet_out.buffer();

            DTsize num_samples = sound_packet_in.num_samples();
            DTint reverb_gain_int = UNIT_TO_10BIT_INT(_reverb_gain);
            DTint output_gain_int = UNIT_TO_10BIT_INT(_output_gain);

            for (DTuint s = 0; s < num_samples; ++s) {
                
                DTint val = _samples_left[_sample_index];
                DTshort fb = static_cast<DTshort>(REM_10BIT(CLAMP_PAD_10BIT(reverb_gain_int * val)));
                
                _samples_left[_sample_index] = fb + (s >= num_samples ? 0 : data_in[s]);
                
                _sample_index += 1;
                if (_sample_index >= _samples_left.size())
                    _sample_index = 0;
                
                data_out[s] = static_cast<DTshort>(REM_10BIT(CLAMP_PAD_10BIT(output_gain_int * val)));
            }
            
            
        } else if (sound_packet_in.format() == SoundResource::FORMAT_STEREO16) {
       
            sound_packet_out.set_format(sound_packet_in.format());
            sound_packet_out.set_frequency(sound_packet_in.frequency());
            sound_packet_out.set_num_bytes(sound_packet_in.num_bytes());

            // Check if samples buffer needs resizing
            if (_samples_left.size() != _delay) {
                _samples_left.resize(_delay);
                _samples_right.resize(_delay);
                _sample_index = 0;            
            }
        
            DTshort *data_in = (DTshort *) sound_packet_in.buffer();
            DTshort *data_out = (DTshort *) sound_packet_out.buffer();

            DTsize num_samples = sound_packet_in.num_samples();
            DTint reverb_gain_int = UNIT_TO_10BIT_INT(_reverb_gain);
            DTint output_gain_int = UNIT_TO_10BIT_INT(_output_gain);

            for (DTuint s = 0; s < num_samples; ++s) {
                
                DTshort val = _samples_left[_sample_index];
                DTshort fb = static_cast<DTshort>(REM_10BIT(CLAMP_PAD_10BIT(reverb_gain_int * val)));
                
                _samples_left[_sample_index] = fb + (s >= num_samples ? 0 : (*data_in));
                (*data_out) = static_cast<DTshort>(REM_10BIT(CLAMP_PAD_10BIT(output_gain_int * val)));
                ++data_out;
                ++data_in;

                val = _samples_right[_sample_index];
                fb = static_cast<DTshort>(REM_10BIT(CLAMP_PAD_10BIT(reverb_gain_int * val)));

                _samples_right[_sample_index] = fb + (s >= num_samples ? 0 : (*data_in));
                (*data_out) = static_cast<DTshort>(REM_10BIT(CLAMP_PAD_10BIT(output_gain_int * val)));
                ++data_out;
                ++data_in;
                
                _sample_index += 1;
                if (_sample_index >= _samples_left.size())
                    _sample_index = 0;

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

