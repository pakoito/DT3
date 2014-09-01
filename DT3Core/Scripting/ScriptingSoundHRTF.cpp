//==============================================================================
///	
///	File: ScriptingSoundHRTF.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================
//
// Portions of this file and the ScriptingSoundHRTFData.hpp file were
// inspired/copied from OpenDT Soft. 
// http://kcat.strangesoft.net/openal.html
//
//==============================================================================

#include "DT3Core/Scripting/ScriptingSoundHRTF.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// HRTF filter coefficients
//==============================================================================

namespace {

#include "DT3Core/Scripting/ScriptingSoundHRTFData.hpp"

const HRTFFilterCoeffs* elevs[] = { &Elev40n, &Elev30n, &Elev20n, &Elev10n, &Elev0, &Elev10, &Elev20, &Elev30, &Elev40, &Elev50, &Elev60, &Elev70, &Elev80, &Elev90 };

const DTfloat ELEV_INCR = 10.0F;

}

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSoundHRTF,"Sound",NULL)
IMPLEMENT_PLUG_NODE(ScriptingSoundHRTF)

IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_in)
IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_out)
IMPLEMENT_PLUG_INFO_INDEX(_source_position)
IMPLEMENT_PLUG_INFO_INDEX(_listener_transform)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSoundHRTF)

	PLUG_INIT(_sound_packet_in,"Sound_Packet_In")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true)
        .set_always_dirty(true);

	PLUG_INIT(_sound_packet_out,"Sound_Packet_Out")
        .set_single_output(true)
		.set_output(true)
        .set_always_dirty(true);

	PLUG_INIT(_source_position,"Source_Position")
		.set_input(true);
    
	PLUG_INIT(_listener_transform,"Listener_Transform")
		.set_input(true);
    
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingSoundHRTF::ScriptingSoundHRTF (void)
    :   _sound_packet_in    (PLUG_INFO_INDEX(_sound_packet_in)),
		_sound_packet_out   (PLUG_INFO_INDEX(_sound_packet_out)),
		_source_position 	(PLUG_INFO_INDEX(_source_position), Vector3(0.0F,0.0F,0.0F) ),
		_listener_transform (PLUG_INFO_INDEX(_listener_transform), Matrix4::identity()),
        _rolloff            (0.0)
{

}
		
ScriptingSoundHRTF::ScriptingSoundHRTF (const ScriptingSoundHRTF &rhs)
    :   ScriptingSoundBase  (rhs),
		_sound_packet_in	(rhs._sound_packet_in),
		_sound_packet_out   (rhs._sound_packet_out),
        _source_position   	(rhs._source_position),
        _listener_transform	(rhs._listener_transform),
        _rolloff            (0.0F)
{

}	

ScriptingSoundHRTF & ScriptingSoundHRTF::operator = (const ScriptingSoundHRTF &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingSoundBase::operator = (rhs);

		_sound_packet_in = rhs._sound_packet_in;
		_sound_packet_out = rhs._sound_packet_out;
        _source_position = rhs._source_position;
        _listener_transform = rhs._listener_transform;
        
        _rolloff = rhs._rolloff;
	}
    return (*this);
}
			
ScriptingSoundHRTF::~ScriptingSoundHRTF (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSoundHRTF::initialize (void)
{
	ScriptingSoundBase::initialize();
    
    for (DTuint i = 0; i < ARRAY_SIZE(_buffer); ++i) {
        _buffer[i] = 0;
    }
    
    _buffer_index = 0;

}

//==============================================================================
//==============================================================================

void ScriptingSoundHRTF::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingSoundBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_PLUG(_source_position, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_listener_transform, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_rolloff, DATA_PERSISTENT | DATA_SETTABLE);
    
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingSoundHRTF::calc_elev (DTfloat elev, DTfloat &mix, const HRTFFilterCoeffs **e0, const HRTFFilterCoeffs **e1)
{
    DTfloat d = elev / ELEV_INCR;
        
    DTfloat d_floor = std::floor(d);
    DTfloat d_ceil = std::ceil(d);
    
    DTint index0 = (DTint) d_floor;
    DTint index1 = (DTint) d_ceil;
    
    if (index0 < -4)        index0 = -4;
    else if (index0 > 9)    index0 = 9;

    if (index1 < -4)        index1 = -4;
    else if (index1 > 9)    index1 = 9;
    
    *e0 = elevs[index0 + 4];
    *e1 = elevs[index1 + 4];
    mix = d - d_floor;
    
    ASSERT( ((index0 + 4) >= 0) && ((index0 + 4) < ARRAY_SIZE(elevs)) );
    ASSERT( ((index1 + 4) >= 0) && ((index1 + 4) < ARRAY_SIZE(elevs)) );

    //LOG_MESSAGE << "ELEV: " << (index0 + 4) << "  " << (index1 + 4) << "  " << mix;
}

void ScriptingSoundHRTF::calc_azimuth (DTfloat azimuth, const HRTFFilterCoeffs *e, DTfloat &mix, const DTshort **l0, const DTshort **r0, const DTshort **l1, const DTshort **r1)
{
    // Special case 90 degrees
    if (e->num_angles <= 1) {
    
        *l0 = e->coeffs[0][0];
        *r0 = e->coeffs[0][1];
        *l1 = e->coeffs[0][0];
        *r1 = e->coeffs[0][1];
        
        mix = 0.5F;
        
    } else {

        DTfloat azimuth_incr = static_cast<DTfloat>(e->max_angle) / static_cast<DTfloat>(e->num_angles - 1);
        DTfloat d = azimuth / azimuth_incr;
        DTfloat d_abs = std::abs(d);

        DTfloat d_floor = std::floor(d_abs);
        DTfloat d_ceil = std::ceil(d_abs);
        
        DTint index0 = (DTint) d_floor;
        DTint index1 = (DTint) d_ceil;
            
        mix = d_abs - d_floor;

        *l0 = e->coeffs[index0][0];
        *r0 = e->coeffs[index0][1];
        
        // Wrap index1 (becomes index0 with channels reversed)
        if (index1 < e->num_angles) {
            *l1 = e->coeffs[index1][0];
            *r1 = e->coeffs[index1][1];
        } else {
            *l1 = e->coeffs[index0][1];
            *r1 = e->coeffs[index0][0];
        }
        
        // Handle negative angles by swapping left and right
        if (d < 0.0) {
            std::swap(*l0, *r0);
            std::swap(*l1, *r1);
        }
        
        //LOG_MESSAGE << "AZ: " << index0 << "  " << index1 << "  " << mix;
    
    }
    
    
}

//==============================================================================
//==============================================================================

#define INTERP(a,b,m) ( (a * m) + (b * (1.0F-m)) )

DTboolean ScriptingSoundHRTF::compute (const PlugBase *plug)
{
	PROFILER(SOUND);

    if (super_type::compute(plug))  return true;

	if (plug == &_sound_packet_out) {
    
        SoundPacket &sound_packet_in = _sound_packet_in.as_ref_no_compute();
        SoundPacket &sound_packet_out = _sound_packet_out.as_ref_no_compute();
    
        //
        // Filter coefficients calculateion
        //
    
        // Get Listener Relative position
        Vector3 relative_pos = _listener_transform->inversed() * _source_position;
        DTfloat distance = relative_pos.abs();
        Vector3 direction = relative_pos / distance;
        
        DTfloat rolloff = 1.0F / (1.0F + _rolloff * distance * distance);
            
        // Elevation
        DTfloat elev = std::asin(direction.y) * RAD_TO_DEG;
        DTfloat azimuth = -std::atan2(-direction.x, -direction.z) * RAD_TO_DEG;
        
        //LOG_MESSAGE << "Elev: " << elev << "  Azimuth: " << azimuth;
        
        DTfloat emix,amix0, amix1;
        const HRTFFilterCoeffs *e0, *e1;
        
        const DTshort *l00, *r00, *l10, *r10;
        const DTshort *l01, *r01, *l11, *r11;
        
        calc_elev (elev, emix, &e0, &e1);
        calc_azimuth (azimuth, e0, amix0, &l00, &r00, &l10, &r10);
        calc_azimuth (azimuth, e1, amix1, &l01, &r01, &l11, &r11);
        
        //
        // Interpolate coefficients
        //
            
        DTshort left[HRTF_LENGTH];
        DTshort right[HRTF_LENGTH];
        
        for (DTuint i = 0; i < HRTF_LENGTH; ++i) {
            left[i] = (DTshort) (INTERP( INTERP(l00[i],l10[i],amix0), INTERP(l01[i],l11[i],amix1), emix));
        }

        for (DTuint i = 0; i < HRTF_LENGTH; ++i) {
            right[i] = (DTshort) (INTERP( INTERP(r00[i],r10[i],amix0), INTERP(r01[i],r11[i],amix1), emix));
        }
        
        //
        // Process sounds
        //
    
        
        if (!_sound_packet_in.has_incoming_connection()) {
            sound_packet_out.set_format(SoundResource::FORMAT_STEREO16);
            sound_packet_out.set_num_bytes(0);
            sound_packet_out.set_frequency(44100);
            return true;
        }

            
        if (sound_packet_in.format() == SoundResource::FORMAT_MONO16 &&
        	sound_packet_in.frequency() == 44100) {
                
            DTsize num_samples = sound_packet_in.num_samples();
                        
            sound_packet_out.set_num_samples(num_samples);
            sound_packet_out.set_format(SoundResource::FORMAT_STEREO16);
            sound_packet_out.set_frequency(sound_packet_in.frequency());

            DTshort *data_in = (DTshort *) sound_packet_in.buffer();
            DTshort *data_out = (DTshort *) sound_packet_out.buffer();

            for (DTuint i = 0; i < num_samples; ++i) {
                
                DTshort &sample = data_in[i];
                DTshort &out_left = data_out[i*2];
                DTshort &out_right = data_out[i*2+1];
            
                // FIR filter
                _buffer[_buffer_index] = sample;
                
                // Note: y = (coeff / DTSHORT_MAX * sample / DTSHORT_MAX) * DTSHORT_MAX yields 16 bit short
                // so:   y = (coeff * sample) / DTSHORT_MAX
                
                DTint y;
                DTint out;
                
                y = 0;
                for (DTuint j = 0; j < HRTF_LENGTH; ++j) {
                    y += left[j] * _buffer[(_buffer_index + j) % HRTF_LENGTH];
                }
                
                out = static_cast<DTint>((y / DTSHORT_MAX) * rolloff);
                if (out > DTSHORT_MAX)          out = DTSHORT_MAX;
                else if (out < DTSHORT_MIN)     out = DTSHORT_MIN;
                out_left = static_cast<DTshort>(out);

                y = 0;
                for (DTuint j = 0; j < HRTF_LENGTH; ++j) {
                    y += right[j] * _buffer[(_buffer_index + j) % HRTF_LENGTH];
                }
                
                out = static_cast<DTint>((y / DTSHORT_MAX) * rolloff);
                if (out > DTSHORT_MAX)          out = DTSHORT_MAX;
                else if (out < DTSHORT_MIN)     out = DTSHORT_MIN;
                out_right = static_cast<DTshort>(out);
                
                // Increment buffer index
                _buffer_index = (_buffer_index + 1) % HRTF_LENGTH;
            
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

