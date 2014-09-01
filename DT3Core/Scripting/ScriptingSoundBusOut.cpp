//==============================================================================
///	
///	File: ScriptingSoundBusOut.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSoundBusOut.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Sound/SoundSource.hpp"
#include "DT3Core/Devices/DeviceAudio.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSoundBusOut,"Sound",NULL)
IMPLEMENT_PLUG_NODE(ScriptingSoundBusOut)

IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_in)
IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_out)
IMPLEMENT_PLUG_INFO_INDEX(_gain_left)
IMPLEMENT_PLUG_INFO_INDEX(_gain_right)
IMPLEMENT_PLUG_INFO_INDEX(_level_left)
IMPLEMENT_PLUG_INFO_INDEX(_level_right)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSoundBusOut)

	PLUG_INIT(_sound_packet_in,"Sound_Packet_In")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true)
        .set_always_dirty(true);

	PLUG_INIT(_sound_packet_out,"Sound_Packet_Out")
        .set_no_draw(true)
        .set_single_output(true)
		.set_output(true)
        .set_always_dirty(true);
    
	PLUG_INIT(_gain_left,"Gain_Left")
        .set_input(true);
    
	PLUG_INIT(_gain_right,"Gain_Right")
        .set_input(true);
    
	PLUG_INIT(_level_left,"Level_Left")
        .set_output(true);

	PLUG_INIT(_level_right,"Level_Right")
        .set_output(true);
    
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingSoundBusOut::ScriptingSoundBusOut (void)
    :   _sound_packet_in    (PLUG_INFO_INDEX(_sound_packet_in)),
		_sound_packet_out   (PLUG_INFO_INDEX(_sound_packet_out)),
		_gain_left          (PLUG_INFO_INDEX(_gain_left), 1.0F),
		_gain_right         (PLUG_INFO_INDEX(_gain_right), 1.0F),
		_level_left         (PLUG_INFO_INDEX(_level_left), 0.0F),
		_level_right        (PLUG_INFO_INDEX(_level_right), 0.0F),
        _source             (NULL)
{  

}
		
ScriptingSoundBusOut::ScriptingSoundBusOut (const ScriptingSoundBusOut &rhs)
    :   ScriptingSoundBase  (rhs),
		_sound_packet_in	(rhs._sound_packet_in),
		_sound_packet_out   (rhs._sound_packet_out),
        _gain_left          (rhs._gain_left),
        _gain_right         (rhs._gain_right),
        _level_left         (rhs._level_left),
        _level_right        (rhs._level_right),
        _source             (rhs._source)
{   

}	

ScriptingSoundBusOut & ScriptingSoundBusOut::operator = (const ScriptingSoundBusOut &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingSoundBase::operator = (rhs);

		_sound_packet_in = rhs._sound_packet_in;
		_sound_packet_out = rhs._sound_packet_out;
        _gain_left = rhs._gain_left;
        _gain_right = rhs._gain_right;
        _level_left = rhs._level_left;
        _level_right = rhs._level_right;
        
        _source = rhs._source;
	}
    return (*this);
}
			
ScriptingSoundBusOut::~ScriptingSoundBusOut (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSoundBusOut::initialize (void)
{
	ScriptingSoundBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingSoundBusOut::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingSoundBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_PLUG(_gain_left, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_gain_right, DATA_PERSISTENT | DATA_SETTABLE);
                                                                			
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingSoundBusOut::compute (const PlugBase *plug)
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
            _level_left = 0.0F;
            _level_right = 0.0F;
            return true;
        }
        
        DTsize num_samples = sound_packet_in.num_samples();
        if (num_samples == 0) {
            sound_packet_out.set_format(SoundResource::FORMAT_STEREO16);
            sound_packet_out.set_num_bytes(0);
            sound_packet_out.set_frequency(44100);
            _level_left = 0.0F;
            _level_right = 0.0F;
            return true;
        }

        if (sound_packet_in.format() == SoundResource::FORMAT_MONO16) {

            sound_packet_out.set_format(SoundResource::FORMAT_STEREO16);
            sound_packet_out.set_frequency(sound_packet_in.frequency());
            sound_packet_out.set_num_bytes(sound_packet_in.num_bytes()*2);
                
            DTshort *data_in = (DTshort *) sound_packet_in.buffer();
            DTshort *data_out = (DTshort *) sound_packet_out.buffer();

            DTint gain_left_int = UNIT_TO_10BIT_INT(_gain_left);
            DTint gain_right_int = UNIT_TO_10BIT_INT(_gain_right);
            
            DTuint64 sum_left = 0;
            DTuint64 sum_right = 0;

            for (DTuint s = 0; s < num_samples; ++s) {
                (*data_out) = (DTshort) REM_10BIT(CLAMP_PAD_10BIT( (*data_in) * gain_left_int));
                
                sum_left += (*data_out) * (*data_out);

                ++data_out;
                
                (*data_out) = (DTshort) REM_10BIT(CLAMP_PAD_10BIT( (*data_in) * gain_right_int));

                sum_right += (*data_out) * (*data_out);

                ++data_out;
                ++data_in;
            }
            
            sum_left /= num_samples;
            _level_left = std::sqrt(static_cast<DTfloat>(sum_left)) / DTSHORT_MAX;

            sum_right /= num_samples;
            _level_right = std::sqrt(static_cast<DTfloat>(sum_right)) / DTSHORT_MAX;

        } else if (sound_packet_in.format() == SoundResource::FORMAT_STEREO16) {

            sound_packet_out.set_format(sound_packet_in.format());
            sound_packet_out.set_frequency(sound_packet_in.frequency());
            sound_packet_out.set_num_bytes(sound_packet_in.num_bytes());
                
            DTshort *data_in = (DTshort *) sound_packet_in.buffer();
            DTshort *data_out = (DTshort *) sound_packet_out.buffer();

            DTint gain_left_int = UNIT_TO_10BIT_INT(_gain_left);
            DTint gain_right_int = UNIT_TO_10BIT_INT(_gain_right);
            
            DTuint64 sum_left = 0;
            DTuint64 sum_right = 0;

            for (DTuint s = 0; s < num_samples; ++s) {
                (*data_out) = (DTshort) REM_10BIT(CLAMP_PAD_10BIT( (*data_in) * gain_left_int));
                
                sum_left += (*data_out) * (*data_out);

                ++data_out;
                ++data_in;
                
                (*data_out) = (DTshort) REM_10BIT(CLAMP_PAD_10BIT( (*data_in) * gain_right_int));

                sum_right += (*data_out) * (*data_out);

                ++data_out;
                ++data_in;
            }
            
            sum_left /= num_samples;
            _level_left = std::sqrt(static_cast<DTfloat>(sum_left)) / DTSHORT_MAX;

            sum_right /= num_samples;
            _level_right = std::sqrt(static_cast<DTfloat>(sum_right)) / DTSHORT_MAX;

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

void ScriptingSoundBusOut::add_to_world(World *world)
{
    ScriptingSoundBase::add_to_world(world);
    
    _source = SoundSource::create();
    
    PlugBase *s1 = _source->plug_by_name("Sound_Packet");
    PlugBase *s2 = plug_by_name("Sound_Packet_Out");
    s1->set_incoming_connection(s2);
                
    System::audio_renderer()->add_bus(_source);
}

void ScriptingSoundBusOut::remove_from_world (void)
{
    System::audio_renderer()->remove_bus(_source);

    ScriptingSoundBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3

