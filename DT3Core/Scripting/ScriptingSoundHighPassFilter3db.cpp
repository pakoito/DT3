//==============================================================================
///	
///	File: ScriptingSoundHighPassFilter3db.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSoundHighPassFilter3db.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSoundHighPassFilter3db,"Sound",NULL)
IMPLEMENT_PLUG_NODE(ScriptingSoundHighPassFilter3db)

IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_in)
IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_out)
IMPLEMENT_PLUG_INFO_INDEX(_cutoff_frequency)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSoundHighPassFilter3db)

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

ScriptingSoundHighPassFilter3db::ScriptingSoundHighPassFilter3db (void)
    :   _sound_packet_in    (PLUG_INFO_INDEX(_sound_packet_in)),
		_sound_packet_out   (PLUG_INFO_INDEX(_sound_packet_out)),
		_cutoff_frequency   (PLUG_INFO_INDEX(_cutoff_frequency), 8000.0F),
        _last_in_sample_left    (0),
        _last_out_sample_left   (0),
        _last_in_sample_right   (0),
        _last_out_sample_right  (0)
{  

}
		
ScriptingSoundHighPassFilter3db::ScriptingSoundHighPassFilter3db (const ScriptingSoundHighPassFilter3db &rhs)
    :   ScriptingSoundBase  (rhs),
		_sound_packet_in	(rhs._sound_packet_in),
		_sound_packet_out   (rhs._sound_packet_out),
        _cutoff_frequency   (rhs._cutoff_frequency),
        _last_in_sample_left     (rhs._last_in_sample_left),
        _last_out_sample_left    (rhs._last_out_sample_left),
        _last_in_sample_right    (rhs._last_in_sample_right),
        _last_out_sample_right   (rhs._last_out_sample_right)
{   

}	

ScriptingSoundHighPassFilter3db & ScriptingSoundHighPassFilter3db::operator = (const ScriptingSoundHighPassFilter3db &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingSoundBase::operator = (rhs);

		_sound_packet_in = rhs._sound_packet_in;
		_sound_packet_out = rhs._sound_packet_out;
        _cutoff_frequency = rhs._cutoff_frequency;
        _last_in_sample_left = rhs._last_in_sample_left;
        _last_out_sample_left = rhs._last_out_sample_left;
        _last_in_sample_right = rhs._last_in_sample_right;
        _last_out_sample_right = rhs._last_out_sample_right;
	}
    return (*this);
}
			
ScriptingSoundHighPassFilter3db::~ScriptingSoundHighPassFilter3db (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSoundHighPassFilter3db::initialize (void)
{
	ScriptingSoundBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingSoundHighPassFilter3db::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingSoundBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_PLUG(_cutoff_frequency, DATA_PERSISTENT | DATA_SETTABLE);
                                                                			
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingSoundHighPassFilter3db::compute (const PlugBase *plug)
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

        DTfloat period = 1.0F / sound_packet_in.frequency();
        DTfloat RC = 1.0F / (TWO_PI * _cutoff_frequency);
        DTfloat alpha = RC / ( RC + period );
    
        DTshort *data_in = (DTshort *) sound_packet_in.buffer();
        DTshort *data_out = (DTshort *) sound_packet_out.buffer();

        DTint alpha_int = UNIT_TO_10BIT_INT(alpha);

        if (sound_packet_in.format() == SoundResource::FORMAT_MONO16) {

            DTsize num_samples = sound_packet_in.num_samples();
            DTshort *sample_in = &data_in[0];
            DTshort *sample_out = &data_out[0];

            for (DTuint s = 0; s < num_samples; ++s) {
                
                DTshort last_in_sample = (*sample_in);
                
                (*sample_out) = static_cast<DTshort>(REM_10BIT(CLAMP_PAD_10BIT(alpha_int * (_last_out_sample_left + last_in_sample - _last_in_sample_left))));
                _last_in_sample_left = last_in_sample;
                _last_out_sample_left = (*sample_out);
                
                ++sample_in;
                ++sample_out;
            }
            
        } else if (sound_packet_in.format() == SoundResource::FORMAT_STEREO16) {
        
            DTsize num_samples = sound_packet_in.num_samples();
            DTshort *sample_in = &data_in[0];
            DTshort *sample_out = &data_out[0];

            for (DTuint s = 0; s < num_samples; ++s) {
                
                DTshort last_in_sample = (*sample_in);

                (*sample_out) = static_cast<DTshort>(REM_10BIT(CLAMP_PAD_10BIT(alpha_int * (_last_out_sample_left + last_in_sample - _last_in_sample_left))));
                _last_in_sample_left = last_in_sample;
                _last_out_sample_left = (*sample_out);
                
                ++sample_in;
                ++sample_out;

                last_in_sample = (*sample_in);
                
                (*sample_out) = static_cast<DTshort>(REM_10BIT(CLAMP_PAD_10BIT(alpha_int * (_last_out_sample_right + last_in_sample - _last_in_sample_right))));
                _last_in_sample_right = last_in_sample;
                _last_out_sample_right = (*sample_out);
                
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

