//==============================================================================
///	
///	File: ScriptingSoundLowPassFilter3db.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSoundLowPassFilter3db.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSoundLowPassFilter3db,"Sound",NULL)
IMPLEMENT_PLUG_NODE(ScriptingSoundLowPassFilter3db)

IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_in)
IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_out)
IMPLEMENT_PLUG_INFO_INDEX(_cutoff_frequency)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSoundLowPassFilter3db)

	PLUG_INIT(_sound_packet_in,"Sound_Packet_In")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true)
        .set_always_dirty(true);

	PLUG_INIT(_sound_packet_out,"Sound_Packet_Out")
        .set_single_output(true)
		.set_output(true)
        .set_always_dirty(true);

	PLUG_INIT(_cutoff_frequency,"Cutoff_Frequency")
		.set_input(true);

    
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingSoundLowPassFilter3db::ScriptingSoundLowPassFilter3db (void)
    :   _sound_packet_in    (PLUG_INFO_INDEX(_sound_packet_in)),
		_sound_packet_out   (PLUG_INFO_INDEX(_sound_packet_out)),
		_cutoff_frequency   (PLUG_INFO_INDEX(_cutoff_frequency), 8000.0F),
        _last_sample_left   (0),
        _last_sample_right  (0)
{  

}
		
ScriptingSoundLowPassFilter3db::ScriptingSoundLowPassFilter3db (const ScriptingSoundLowPassFilter3db &rhs)
    :   ScriptingSoundBase  (rhs),
		_sound_packet_in	(rhs._sound_packet_in),
		_sound_packet_out   (rhs._sound_packet_out),
        _cutoff_frequency   (rhs._cutoff_frequency),
        _last_sample_left   (rhs._last_sample_left),
        _last_sample_right  (rhs._last_sample_right)
{   

}	

ScriptingSoundLowPassFilter3db & ScriptingSoundLowPassFilter3db::operator = (const ScriptingSoundLowPassFilter3db &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingSoundBase::operator = (rhs);

		_sound_packet_in = rhs._sound_packet_in;
		_sound_packet_out = rhs._sound_packet_out;
        _cutoff_frequency = rhs._cutoff_frequency;
        _last_sample_left = rhs._last_sample_left;
        _last_sample_right = rhs._last_sample_right;
	}
    return (*this);
}
			
ScriptingSoundLowPassFilter3db::~ScriptingSoundLowPassFilter3db (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSoundLowPassFilter3db::initialize (void)
{
	ScriptingSoundBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingSoundLowPassFilter3db::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingSoundBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_PLUG(_cutoff_frequency, DATA_PERSISTENT | DATA_SETTABLE);
                                                                			
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingSoundLowPassFilter3db::compute (const PlugBase *plug)
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
            
        // See http://en.wikipedia.org/wiki/Low-pass_filter
    
        DTfloat period = 1.0F / sound_packet_in.frequency();
        DTfloat alpha = period / ( (1.0F / (TWO_PI * _cutoff_frequency)) + period );
    
        DTshort *data_in = (DTshort *) sound_packet_in.buffer();
        DTshort *data_out = (DTshort *) sound_packet_out.buffer();
        
        DTint one_minus_alpha_int = UNIT_TO_10BIT_INT(1.0F-alpha);
        DTint alpha_int = UNIT_TO_10BIT_INT(alpha);


        if (sound_packet_in.format() == SoundResource::FORMAT_MONO16) {
        
            DTsize num_samples = sound_packet_in.num_samples();
            DTshort *sample_in = &data_in[0];
            DTshort *sample_out = &data_out[0];

            for (DTuint s = 0; s < num_samples; ++s) {
                (*sample_out) = static_cast<DTshort>(REM_10BIT(CLAMP_PAD_10BIT(alpha_int * (*sample_in) + one_minus_alpha_int * _last_sample_left)));
                _last_sample_left = (*sample_out);
                
                ++sample_in;
                ++sample_out;
            }
            
        } else if (sound_packet_in.format() == SoundResource::FORMAT_STEREO16) {

            DTsize num_samples = sound_packet_in.num_samples();
            DTshort *sample_in = &data_in[0];
            DTshort *sample_out = &data_out[0];

            for (DTuint s = 0; s < num_samples; ++s) {
                (*sample_out) = static_cast<DTshort>(REM_10BIT(CLAMP_PAD_10BIT(alpha_int * (*sample_in) + one_minus_alpha_int * _last_sample_left)));
                _last_sample_left = (*sample_out);
            
                ++sample_in;
                ++sample_out;
                
                (*sample_out) = static_cast<DTshort>(REM_10BIT(CLAMP_PAD_10BIT(alpha_int * (*sample_in) + one_minus_alpha_int * _last_sample_right)));
                _last_sample_right = (*sample_out);
            
                ++sample_in;
                ++sample_out;
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

