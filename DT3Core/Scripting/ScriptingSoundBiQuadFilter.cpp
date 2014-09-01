//==============================================================================
///	
///	File: ScriptingSoundBiquadFilter.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSoundBiQuadFilter.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSoundBiquadFilter,"Sound",NULL)
IMPLEMENT_PLUG_NODE(ScriptingSoundBiquadFilter)

IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_in)
IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_out)
IMPLEMENT_PLUG_INFO_INDEX(_filter_type)
IMPLEMENT_PLUG_INFO_INDEX(_cutoff_frequency)
IMPLEMENT_PLUG_INFO_INDEX(_db_gain)
IMPLEMENT_PLUG_INFO_INDEX(_Q)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSoundBiquadFilter)

	PLUG_INIT(_sound_packet_in,"Sound_Packet_In")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true)
        .set_always_dirty(true);

	PLUG_INIT(_sound_packet_out,"Sound_Packet_Out")
        .set_single_output(true)
		.set_output(true)
        .set_always_dirty(true);

	PLUG_INIT(_filter_type,"Filter_Type")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true);

	PLUG_INIT(_cutoff_frequency,"Cutoff_Frequency")
		.set_input(true);

	PLUG_INIT(_db_gain,"Db_Gain")
		.set_input(true);

	PLUG_INIT(_Q,"Q")
		.set_input(true);

    
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingSoundBiquadFilter::ScriptingSoundBiquadFilter (void)
    :   _sound_packet_in    (PLUG_INFO_INDEX(_sound_packet_in)),
		_sound_packet_out   (PLUG_INFO_INDEX(_sound_packet_out)),
		_filter_type        (PLUG_INFO_INDEX(_filter_type), 0),
		_cutoff_frequency   (PLUG_INFO_INDEX(_cutoff_frequency), 8000.0F),
		_db_gain            (PLUG_INFO_INDEX(_db_gain), 1.0F),
		_Q                  (PLUG_INFO_INDEX(_Q), 1.0F),
        _last_in_minus_1    (0.0F),
        _last_in_minus_2    (0.0F),
        _last_out_minus_1   (0.0F),
        _last_out_minus_2   (0.0F)
{  

}
		
ScriptingSoundBiquadFilter::ScriptingSoundBiquadFilter (const ScriptingSoundBiquadFilter &rhs)
    :   ScriptingSoundBase  (rhs),
		_sound_packet_in	(rhs._sound_packet_in),
		_sound_packet_out   (rhs._sound_packet_out),
        _filter_type        (rhs._filter_type),
        _cutoff_frequency   (rhs._cutoff_frequency),
		_db_gain            (rhs._db_gain),
		_Q                  (rhs._Q),
        _last_in_minus_1    (rhs._last_in_minus_1),
        _last_in_minus_2    (rhs._last_in_minus_2),
        _last_out_minus_1   (rhs._last_out_minus_1),
        _last_out_minus_2   (rhs._last_out_minus_2)
{   

}	

ScriptingSoundBiquadFilter & ScriptingSoundBiquadFilter::operator = (const ScriptingSoundBiquadFilter &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingSoundBase::operator = (rhs);

		_sound_packet_in = rhs._sound_packet_in;
		_sound_packet_out = rhs._sound_packet_out;
        _filter_type = rhs._filter_type;
        _cutoff_frequency = rhs._cutoff_frequency;
		_db_gain = rhs._db_gain;
		_Q = rhs._Q;
        
        _last_in_minus_1 = rhs._last_in_minus_1;
        _last_in_minus_2 = rhs._last_in_minus_2;
        _last_out_minus_1 = rhs._last_out_minus_1;
        _last_out_minus_2 = rhs._last_out_minus_2;
	}
    return (*this);
}
			
ScriptingSoundBiquadFilter::~ScriptingSoundBiquadFilter (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSoundBiquadFilter::initialize (void)
{
	ScriptingSoundBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingSoundBiquadFilter::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingSoundBase::archive(archive);

    archive->push_domain (class_id ());

	*archive << ARCHIVE_PLUG(_filter_type, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("Low Pass")
        .add_enum("High Pass")
        .add_enum("Band Pass (constant skirt gain)")
        .add_enum("Band Pass (constant 0 db peak gain)")
        .add_enum("Notch")
        .add_enum("APF")
        .add_enum("Peaking EQ")
        .add_enum("Low Shelf")
        .add_enum("High Shelf");
	*archive << ARCHIVE_PLUG(_cutoff_frequency, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_db_gain, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_Q, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingSoundBiquadFilter::compute (const PlugBase *plug)
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


        // Check for empty buffer
        if (sound_packet_in.num_bytes() == 0)
            return true;
        
        // See
        // http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
        
        //
        // Compute coefficients
        //
        
        DTfloat b0=0.0F,b1=0.0F,b2=0.0F,a0=0.0F,a1=0.0F,a2=0.0F;
        
        DTfloat w0 = TWO_PI * _cutoff_frequency / sound_packet_in.frequency();
        DTfloat cos_w0 = std::cos(w0);
        DTfloat sin_w0 = std::sin(w0);
        DTfloat alpha = std::sin(w0)/(2.0F*_Q);
        
        switch(_filter_type) {
            case LOW_PASS:
                b0 =  (1.0F - cos_w0)/2.0F;
                b1 =   1.0F - cos_w0;
                b2 =  (1.0F - cos_w0)/2.0F;
                a0 =   1.0F + alpha;
                a1 =  -2.0F*cos_w0;
                a2 =   1.0F - alpha;
                break;
                
            case HIGH_PASS:
                b0 =  (1.0F + cos_w0)/2.0F;
                b1 = -(1.0F + cos_w0);
                b2 =  (1.0F + cos_w0)/2.0F;
                a0 =   1.0F + alpha;
                a1 =  -2.0F*cos_w0;
                a2 =   1.0F - alpha;
                break;
                
            case BAND_PASS1:
                b0 =   sin_w0/2.0F;
                b1 =   0.0F;
                b2 =  -sin_w0/2.0F;
                a0 =   1.0F + alpha;
                a1 =  -2.0F*cos_w0;
                a2 =   1.0F - alpha;
                break;
                
            case BAND_PASS2:
                b0 =   alpha;
                b1 =   0.0F;
                b2 =  -alpha;
                a0 =   1.0F + alpha;
                a1 =  -2.0F*cos_w0;
                a2 =   1.0F - alpha;
                break;
                
            case NOTCH:
                b0 =   1.0F;
                b1 =  -2.0F*cos_w0;
                b2 =   1.0F;
                a0 =   1.0F + alpha;
                a1 =  -2.0F*cos_w0;
                a2 =   1.0F - alpha;
                break;
                
            case APF:
                b0 =   1.0F - alpha;
                b1 =  -2.0F*cos_w0;
                b2 =   1.0F + alpha;
                a0 =   1.0F + alpha;
                a1 =  -2.0F*cos_w0;
                a2 =   1.0F - alpha;
                break;
                
            case PEAKING_EQ: {
                DTfloat A = std::pow(10.0F,_db_gain/40.0F);
                
                b0 =   1.0F + alpha*A;
                b1 =  -2.0F*cos_w0;
                b2 =   1.0F - alpha*A;
                a0 =   1.0F + alpha/A;
                a1 =  -2.0F*cos_w0;
                a2 =   1.0F - alpha/A;

                } break;
                
            case LOW_SHELF: {
                DTfloat A = std::pow(10.0F,_db_gain/40.0F);

                b0 =    A*( (A+1.0F) - (A-1.0F)*cos_w0 + 2.0F*std::sqrt(A)*alpha );
                b1 =    2.0F*A*( (A-1.0F) - (A+1.0F)*cos_w0                   );
                b2 =    A*( (A+1.0F) - (A-1.0F)*cos_w0 - 2.0F*std::sqrt(A)*alpha );
                a0 =        (A+1.0F) + (A-1.0F)*cos_w0 + 2.0F*std::sqrt(A)*alpha;
                a1 =   -2.0F*( (A-1.0F) + (A+1.0F)*cos_w0                   );
                a2 =        (A+1.0F) + (A-1.0F)*cos_w0 - 2.0F*std::sqrt(A)*alpha;
                } break;
                
            case HIGH_SHELF: {
                DTfloat A = std::pow(10.0F,_db_gain/40.0F);
                
                b0 =    A*( (A+1.0F) + (A-1.0F)*cos_w0 + 2.0F*std::sqrt(A)*alpha );
                b1 =    -2.0F*A*( (A-1.0F) + (A+1.0F)*cos_w0                   );
                b2 =    A*( (A+1.0F) + (A-1.0F)*cos_w0 - 2.0F*std::sqrt(A)*alpha );
                a0 =    (A+1.0F) - (A-1.0F)*cos_w0 + 2.0F*std::sqrt(A)*alpha;;
                a1 =    2.0F*( (A-1.0F) - (A+1.0F)*cos_w0                   );
                a2 =    (A+1.0F) - (A-1.0F)*cos_w0 - 2.0F*std::sqrt(A)*alpha;
                } break;
        };

        
            
        if (sound_packet_in.format() == SoundResource::FORMAT_MONO16) {
 
            sound_packet_out.set_format(sound_packet_in.format());
            sound_packet_out.set_frequency(sound_packet_in.frequency());
            sound_packet_out.set_num_bytes(sound_packet_in.num_bytes());
       
            DTshort *data_in = (DTshort *) sound_packet_in.buffer();
            DTshort *data_out = (DTshort *) sound_packet_out.buffer();

            DTsize num_samples = sound_packet_in.num_samples();
            
            DTint A_int = UNIT_TO_10BIT_INT(b0/a0);
            DTint B_int = UNIT_TO_10BIT_INT(b1/a0);
            DTint C_int = UNIT_TO_10BIT_INT(b2/a0);
            DTint D_int = UNIT_TO_10BIT_INT(a1/a0);
            DTint E_int = UNIT_TO_10BIT_INT(a2/a0);
                                    
            for (DTuint s = 0; s < num_samples; ++s) {
                data_out[s] = static_cast<DTshort>(REM_10BIT(CLAMP_PAD_10BIT(A_int*data_in[s] + B_int*_last_in_minus_1 + C_int*_last_in_minus_2 - D_int*_last_out_minus_1 - E_int*_last_out_minus_2)));
                
                _last_in_minus_2 = _last_in_minus_1;
                _last_in_minus_1 = data_in[s];
                _last_out_minus_2 = _last_out_minus_1;
                _last_out_minus_1 = data_out[s];
            }
            
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

