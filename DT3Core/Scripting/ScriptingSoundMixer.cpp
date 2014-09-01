//==============================================================================
///	
///	File: ScriptingSoundMixer.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSoundMixer.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSoundMixer,"Sound",NULL)
IMPLEMENT_PLUG_NODE(ScriptingSoundMixer)

IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_in_1)
IMPLEMENT_PLUG_INFO_INDEX(_in_gain_1)
IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_in_2)
IMPLEMENT_PLUG_INFO_INDEX(_in_gain_2)
IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_out)
IMPLEMENT_PLUG_INFO_INDEX(_output_gain)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSoundMixer)

	PLUG_INIT(_sound_packet_in_1,"Sound_Packet_In_1")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true)
        .set_always_dirty(true);

	PLUG_INIT(_in_gain_1,"In_Gain_1")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true);

	PLUG_INIT(_sound_packet_in_2,"Sound_Packet_In_2")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true)
        .set_always_dirty(true);

	PLUG_INIT(_in_gain_2,"In_Gain_2")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true);


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

ScriptingSoundMixer::ScriptingSoundMixer (void)
    :   _sound_packet_in_1  (PLUG_INFO_INDEX(_sound_packet_in_1)),
        _in_gain_1          (PLUG_INFO_INDEX(_in_gain_1), 1.0F),
        _sound_packet_in_2  (PLUG_INFO_INDEX(_sound_packet_in_2)),
        _in_gain_2          (PLUG_INFO_INDEX(_in_gain_2), 1.0F),
		_sound_packet_out   (PLUG_INFO_INDEX(_sound_packet_out)),
		_output_gain        (PLUG_INFO_INDEX(_output_gain), 0.5F)
{  

}
		
ScriptingSoundMixer::ScriptingSoundMixer (const ScriptingSoundMixer &rhs)
    :   ScriptingSoundBase  (rhs),
		_sound_packet_in_1	(rhs._sound_packet_in_1),
		_in_gain_1          (rhs._in_gain_1),
		_sound_packet_in_2	(rhs._sound_packet_in_2),
		_in_gain_2          (rhs._in_gain_2),
		_sound_packet_out   (PLUG_INFO_INDEX(_sound_packet_out)),
        _output_gain        (rhs._output_gain)
{   

}	

ScriptingSoundMixer & ScriptingSoundMixer::operator = (const ScriptingSoundMixer &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingSoundBase::operator = (rhs);

		_sound_packet_in_1 = rhs._sound_packet_in_1;
		_in_gain_1 = rhs._in_gain_1;
		_sound_packet_in_2 = rhs._sound_packet_in_2;
		_in_gain_2 = rhs._in_gain_2;
        
        _sound_packet_out = rhs._sound_packet_out;
        _output_gain = rhs._output_gain;
	}
    return (*this);
}
			
ScriptingSoundMixer::~ScriptingSoundMixer (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSoundMixer::initialize (void)
{
	ScriptingSoundBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingSoundMixer::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingSoundBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_PLUG(_in_gain_1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_gain_2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_output_gain, DATA_PERSISTENT | DATA_SETTABLE);
                                                                			
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingSoundMixer::compute (const PlugBase *plug)
{
	PROFILER(SOUND);

    if (super_type::compute(plug))  return true;

	if (plug == &_sound_packet_out) {
    
        SoundPacket &sound_packet_in_1 = _sound_packet_in_1.as_ref_no_compute();
        SoundPacket &sound_packet_in_2 = _sound_packet_in_2.as_ref_no_compute();
        SoundPacket &sound_packet_out = _sound_packet_out.as_ref_no_compute();
        
        if (!_sound_packet_in_1.has_incoming_connection() || !_sound_packet_in_2.has_incoming_connection()) {
            sound_packet_out.set_format(SoundResource::FORMAT_STEREO16);
            sound_packet_out.set_num_bytes(0);
            sound_packet_out.set_frequency(44100);
            return true;
        }

        // Since stream packets can be different sizes, we have to use a buffered approach
        DTsize buffer_size = MoreMath::max(sound_packet_in_1.num_bytes(), sound_packet_in_2.num_bytes());
        if (buffer_size == 0) {
            sound_packet_out = _sound_packet_in_1;
            return true;
        }
        
        if (    _sound_packet_buffer_1.format() == _sound_packet_buffer_2.format() &&
                _sound_packet_buffer_1.frequency() == _sound_packet_buffer_2.frequency()) {
            
            _sound_packet_buffer_1.append_bytes(_sound_packet_in_1, buffer_size);
            _sound_packet_buffer_2.append_bytes(_sound_packet_in_2, buffer_size);

            if (_sound_packet_buffer_1.format() == SoundResource::FORMAT_MONO16 ||
                _sound_packet_buffer_1.format() == SoundResource::FORMAT_STEREO16) {
                    
                sound_packet_out.set_format(_sound_packet_buffer_1.format());
                sound_packet_out.set_frequency(_sound_packet_buffer_1.frequency());
                sound_packet_out.set_num_bytes(buffer_size);

                DTshort *data_in_1 = (DTshort *) _sound_packet_buffer_1.buffer();
                DTshort *data_in_2 = (DTshort *) _sound_packet_buffer_2.buffer();
                DTshort *data_out = (DTshort *) sound_packet_out.buffer();

                DTsize num_samples = buffer_size / 2;
                
                if (_in_gain_1 == 0.0F && _in_gain_2 != 0.0F) {
                    for (DTuint s = 0; s < num_samples; ++s) {
                        data_out[s] = (DTshort) (data_in_2[s] * _in_gain_2);
                    }
                } else if (_in_gain_1 != 0.0F && _in_gain_2 == 0.0F) {
                    for (DTuint s = 0; s < num_samples; ++s) {
                        data_out[s] = (DTshort) (data_in_1[s] * _in_gain_1);
                    }
                } else if (_in_gain_1 == 0.0F && _in_gain_2 == 0.0F) {
                    for (DTuint s = 0; s < num_samples; ++s) {
                        data_out[s] = 0;
                    }
                } else {
                    for (DTuint s = 0; s < num_samples; ++s) {
                        data_out[s] = (DTshort) (( (DTshort) (data_in_1[s] * _in_gain_1) + (DTshort) (data_in_2[s] * _in_gain_2) ) * _output_gain);
                    }
                }
                
            }
                
            // Clean up processed samples from the buffer
            _sound_packet_buffer_1.remove_bytes(buffer_size);
            _sound_packet_buffer_2.remove_bytes(buffer_size);
    
        } else {
            sound_packet_out = _sound_packet_in_1;
        }
        
		_sound_packet_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3

