//==============================================================================
///	
///	File: ScriptingARTracker.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingARTracker.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Math/Vector4.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingARTracker,"Input",NULL)
IMPLEMENT_PLUG_NODE(ScriptingARTracker)
		
IMPLEMENT_PLUG_INFO_INDEX(_orientation)
IMPLEMENT_PLUG_INFO_INDEX(_is_calibrating)
IMPLEMENT_PLUG_INFO_INDEX(_is_calibrated)

IMPLEMENT_EVENT_INFO_INDEX(_begin_auto_calibration)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingARTracker)

	PLUG_INIT(_orientation, "Orientation")
		.set_output(true);

	PLUG_INIT(_is_calibrating, "Is_Calibrating")
		.set_output(true);

	PLUG_INIT(_is_calibrated, "Is_Calibrated")
		.set_output(true);

    EVENT_INIT(_begin_auto_calibration,"Begin_Auto_Calibration")
        .set_input(true)
        .set_event(&ScriptingARTracker::begin_auto_calibration);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingARTracker::ScriptingARTracker (void)
    :   _gyro_orientation               (Matrix3::identity()),
        _accelerometer                  (0.0F,0.0F,0.0F),
        _magnetometer                   (0.0F,0.0F,0.0F),
        _orientation                    (PLUG_INFO_INDEX(_orientation), Matrix3::identity()),
        _correction_mix_factor          (0.02F),
        _correction_mode                (CORRECTION_ACCELEROMETER_AND_MAGNETOMETER),
		_is_calibrating                 (PLUG_INFO_INDEX(_is_calibrating), false),
		_is_calibrated                  (PLUG_INFO_INDEX(_is_calibrated), false),
        _calibration_directions         (0),
        _num_calibration_samples        (0),
        _calibration                    (Vector3(0.0F,0.0F,0.0F)),
        _begin_auto_calibration         (EVENT_INFO_INDEX(_begin_auto_calibration))
{

}
		
ScriptingARTracker::ScriptingARTracker (const ScriptingARTracker &rhs)
    :   ScriptingBase                   (rhs),
        _gyro_orientation               (rhs._gyro_orientation),
        _accelerometer                  (rhs._accelerometer),
        _magnetometer                   (rhs._magnetometer),
        _orientation                    (rhs._orientation),
        _correction_mix_factor          (rhs._correction_mix_factor),
        _correction_mode                (rhs._correction_mode),
		_is_calibrating                 (PLUG_INFO_INDEX(_is_calibrating), false),
		_is_calibrated                  (PLUG_INFO_INDEX(_is_calibrated), false),
        _calibration_directions         (0),
        _num_calibration_samples        (0),
        _calibration                    (Vector3(0.0F,0.0F,0.0F)),
        _begin_auto_calibration         (EVENT_INFO_INDEX(_begin_auto_calibration))
{

}

ScriptingARTracker & ScriptingARTracker::operator = (const ScriptingARTracker &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);
        
        _gyro_orientation = rhs._gyro_orientation;
        _accelerometer = rhs._accelerometer;

        _magnetometer = rhs._magnetometer;
        _calibration = rhs._calibration;

        _orientation = rhs._orientation;
        _correction_mix_factor = rhs._correction_mix_factor;
        
        _correction_mode = rhs._correction_mode;

		_is_calibrating = false;
		_is_calibrated = false;
        _calibration_directions = 0;
        _num_calibration_samples = 0;
        _calibration = Vector3(0.0F,0.0F,0.0F);
    }
    return (*this);
}
			
ScriptingARTracker::~ScriptingARTracker (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingARTracker::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingARTracker::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

    archive->push_domain (class_id ());

	*archive << ARCHIVE_DATA(_correction_mix_factor, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_correction_mode, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("None")
        .add_enum("Accelerometer")
        .add_enum("Accelerometer and Magnetometer");

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingARTracker::begin_auto_calibration (PlugNode *sender)
{
    _is_calibrating = true;
    _is_calibrated = false;
    
    // Reset the samples
    _num_calibration_samples = 0;
    
    // Reset the heuristic for calibration being complete
    _calibration_directions = 0;
}

//==============================================================================
//==============================================================================

void ScriptingARTracker::gyro (const Vector3 &w)
{
	PROFILER(SCRIPTING);
   
    DTfloat dt = static_cast<DTfloat>(_gyro_sample_timer.delta_time());
    
    Vector3 gyro = w * dt;

    DTfloat angle = std::sqrt(gyro.x*gyro.x + gyro.y*gyro.y + gyro.z*gyro.z);
    if (angle <= 0.0F)
        return;
    
    Vector3 axis = gyro/angle;
    
    Matrix3 rot = Matrix3::set_rotation_around(axis, angle);
    
    _gyro_orientation = _gyro_orientation * rot;
    _gyro_orientation.ortho();
}

void ScriptingARTracker::accelerometer (const Vector3 &a)
{
	PROFILER(SCRIPTING);

    _accelerometer = a;
}

void ScriptingARTracker::magnetometer (const Vector3 &m)
{
	PROFILER(SCRIPTING);

    _magnetometer = m - _calibration;
    
    if (_is_calibrating) {
    
        // Add calibration samples
        if (append_calibration_sample(m)) {
        
            // Restart face check if we found a better sample
            _calibration_directions = 0;

            if (!process_calibration_samples())
                return;
            
        }
        
        //
        // Testing for completeness
        //
        
        // See if the device has been moved enough. Keep track of all of the directions
        // (i.e. six sides of cube) that the magnetometer is oriented towards. This
        // is like making sure the player looks in every direction.
        
        Vector3 diff = m - _calibration;
        
        DTfloat abs_x = std::abs(diff.x);
        DTfloat abs_y = std::abs(diff.y);
        DTfloat abs_z = std::abs(diff.z);
        
        DTboolean x_major = (abs_x >= abs_y) && (abs_x >= abs_z);
        DTboolean y_major = (abs_y >= abs_x) && (abs_y >= abs_z);
        DTboolean z_major = (!x_major) && (!y_major);
        
        // First 6 bits for faces
        if ( (_calibration_directions & 0x0000003F) != 0x0000003F) {
            if (x_major)        _calibration_directions |= (diff.x > 0.0F) ? (0x00000001 << 0) : (0x00000001 << 1);
            else if (y_major)   _calibration_directions |= (diff.y > 0.0F) ? (0x00000001 << 2) : (0x00000001 << 3);
            else if (z_major)   _calibration_directions |= (diff.z > 0.0F) ? (0x00000001 << 4) : (0x00000001 << 5);
            
        // Done!
        } else {
            _calibration_directions = 0;

            _is_calibrating = false;
            _is_calibrated = true;
            
            LOG_MESSAGE << "Calibration done!";
            
        }
                
    }
}

//==============================================================================
//==============================================================================

DTboolean ScriptingARTracker::append_calibration_sample (const Vector3 &m)
{
	PROFILER(SCRIPTING);

    // Fill up the samples first
    if (_num_calibration_samples < 6) {
        _calibration_samples[_num_calibration_samples] = m;
        ++_num_calibration_samples;
        
    // Add samples that are more "distinct" than the existing samples
    } else {
    
        DTint closest_sum_dist_index = -1;
        DTfloat closest_sum_dist_distance = std::numeric_limits<DTfloat>::infinity();
    
        // Rank all of the existing samples
        for (DTuint i = 0; i < ARRAY_SIZE(_calibration_samples); ++i) {
        
            DTfloat dist = 0.0F;
        
            for (DTuint j = 0; j < ARRAY_SIZE(_calibration_samples); ++j) {
                if (i == j)
                    continue;
                
                dist += (_calibration_samples[i] - _calibration_samples[j]).abs();                
            }
            
            // pick closest distance
            if (dist < closest_sum_dist_distance) {
                closest_sum_dist_distance = dist;
                closest_sum_dist_index = i;
            }

        }
        
        // Rank new sample
        DTfloat new_sample_sum_dist_distance = 0.0F;
        for (DTint j = 0; j < ARRAY_SIZE(_calibration_samples); ++j) {
            if (j == closest_sum_dist_index)    // Skip the sample we're replacing
                continue;
            
            new_sample_sum_dist_distance += (m - _calibration_samples[j]).abs();
        }

        // Replace if better
        if (new_sample_sum_dist_distance > closest_sum_dist_distance) {
            _calibration_samples[closest_sum_dist_index] = m;
            LOG_MESSAGE << "Replaced sample " << closest_sum_dist_index << " at distance " << new_sample_sum_dist_distance;
            return true;
        }
    
    }
    
    return false;
}

DTboolean ScriptingARTracker::process_calibration_samples (void)
{
	PROFILER(SCRIPTING);

    // Note: Looks like there's significant error by using only floats
    // instead of doubles. TODO: Change me when I redo Matrix4 for doubles

    // This process is described in Freescale App Note AN4246
    // http://www.freescale.com/files/sensors/doc/app_note/AN4246.pdf
    
    // Sample data from freescale docs
    //    _calibration_samples[0] = Vector3(167.4F, -242.4F, 91.7F);
    //    _calibration_samples[1] = Vector3(140.3F, -221.9F, 86.8F);
    //    _calibration_samples[2] = Vector3(152.4F, -230.4F, -0.6F);
    //    _calibration_samples[3] = Vector3(180.3F, -270.6F, 71.0F);
    //    _calibration_samples[4] = Vector3(190.9F, -212.4F, 62.7F);
    //    _calibration_samples[5] = Vector3(192.9F, -242.4F, 17.1F);
    
    
    // Calculate Y
    DTfloat Y[ARRAY_SIZE(_calibration_samples)];
    
    for (DTuint i = 0; i < ARRAY_SIZE(_calibration_samples); ++i) {
        Y[i] = _calibration_samples[i].abs2();
    }
    
    // Build X
    DTfloat X[ARRAY_SIZE(_calibration_samples)][4];
    DTfloat XT[4][ARRAY_SIZE(_calibration_samples)];
    
    for (DTuint i = 0; i < ARRAY_SIZE(_calibration_samples); ++i) {
        X[i][0] = _calibration_samples[i].x;
        X[i][1] = _calibration_samples[i].y;
        X[i][2] = _calibration_samples[i].z;
        X[i][3] = 1.0F;

        XT[0][i] = _calibration_samples[i].x;
        XT[1][i] = _calibration_samples[i].y;
        XT[2][i] = _calibration_samples[i].z;
        XT[3][i] = 1.0F;
    }
    
    //
    // B = (XT * X)^-1 (XT * Y)
    //
    
    // XT * X   (Note: 4x6 matrix times 6x4 matrix = 4x4 matrix so then we can use matrix4)
    Matrix4 temp1;

    for (DTint i = 0; i < 4; i++) {
        for (DTint j = 0; j < 4; j++) {
        
            DTfloat sum = 0.0F;
            
            for (DTint k = 0; k < ARRAY_SIZE(_calibration_samples); k++)
                sum += (XT[i][k] * X[k][j]);
            
            temp1._m[i][j] = sum;
        }
    }
    
    //
    // XT * Y
    //
    
    Vector4 temp2;
    
    for (DTint i = 0; i < 4; ++i) {
        DTfloat sum = 0.0F;
        
        for (DTuint j = 0; j < ARRAY_SIZE(_calibration_samples); ++j) {
            sum += XT[i][j] * Y[j];
        }
        
        temp2.v[i] = sum;
    }
    
    //
    // Calculate B
    //
    Vector4 B = temp1.inversed() * temp2;
    _calibration = B * 0.5F;
    
    // Check valid result
    if (B.v[3] < 0.0F)
        return false;
    
    //
    // Display results
    //
        
    DTfloat field_strength = std::sqrt( B.v[3] +
                                        _calibration.x*_calibration.x +
                                        _calibration.y*_calibration.y +
                                        _calibration.z*_calibration.z);

    LOG_MESSAGE << "V: " << _calibration.x << " " << _calibration.y << " " << _calibration.z << " uT";
    LOG_MESSAGE << "Field Strength: " << field_strength << " uT";
    
    return true;
}

//==============================================================================
//==============================================================================

void ScriptingARTracker::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    // If the magnetometer is calibrated, we can use it to correct our orientation
    if (_is_calibrated && (_correction_mode == CORRECTION_ACCELEROMETER_AND_MAGNETOMETER) ) {
    
        if (_accelerometer.abs2() == 0.0F || _magnetometer.abs2() == 0.0F)
            return;
        
        // So, this is what we are trying to do here...
        // The Accelerometer and Magnetometer are combined to generate an orientation matrix.
        // This matrix is the world reference coordinate system in device (i.e. Camera)
        // space. So what we do is assume that if the world reference coordinate system is
        // in world space, it is the identity matrix.
        //
        // So basically, Wi = D * Wd where
        //      Wi = Identity,
        //      D = Device or camera orientation
        //      Wd = Device relative world reference coordinate
        //
        // Solving for D, D = Wd^-1
        //
        // But, this is pretty noisy... so it gets mixed into the gyro orientation a little
        // bit every frame.
        
        //
        // Build world reference coordinate
        //

        // Combine accelerometer and magnetometer
        Vector3 accelerometer_norm = -_accelerometer.normalized();
        Vector3 magnetometer_norm = -_magnetometer.normalized();
        
        Vector3 perp = Vector3::cross(accelerometer_norm, magnetometer_norm).normalized();
        magnetometer_norm = Vector3::cross(perp, accelerometer_norm).normalized();
        
        Matrix3 world_reference_coord_system = Matrix3( perp.x, accelerometer_norm.x, magnetometer_norm.x,
                                                        perp.y, accelerometer_norm.y, magnetometer_norm.y,
                                                        perp.z, accelerometer_norm.z, magnetometer_norm.z).inversed();
        
        //
        // Mix in with gyro orientation
        //
                
        _gyro_orientation = (_correction_mix_factor * world_reference_coord_system) + ((1.0F - _correction_mix_factor) * _gyro_orientation);
        _gyro_orientation.ortho();
        
    } else if (_correction_mode == CORRECTION_ACCELEROMETER || _correction_mode == CORRECTION_ACCELEROMETER_AND_MAGNETOMETER) {
    
        if (_accelerometer.abs2() == 0.0F)
            return;

        // Same as above but the magnetometer is ignored for heading direction
    
        Vector3 accelerometer_norm = -_accelerometer.normalized();
        Vector3 backwards = _gyro_orientation.inversed().z_axis();
    
        Vector3 perp = Vector3::cross(accelerometer_norm, backwards);
        backwards = Vector3::cross(perp, accelerometer_norm);
        
        Matrix3 world_reference_coord_system = Matrix3( perp.x, accelerometer_norm.x, backwards.x,
                                                        perp.y, accelerometer_norm.y, backwards.y,
                                                        perp.z, accelerometer_norm.z, backwards.z).inversed();
    
        //
        // Mix in with gyro orientation
        //
                
        _gyro_orientation = (_correction_mix_factor * world_reference_coord_system) + ((1.0F - _correction_mix_factor) * _gyro_orientation);
        _gyro_orientation.ortho();

    }
    
    
    //
    // Output orientation
    //
    
    _orientation = _gyro_orientation;
}

//==============================================================================
//==============================================================================

void ScriptingARTracker::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    SystemCallbacks::gyro_cb().add(make_callback(this, &type::gyro));
    SystemCallbacks::acceleration_cb().add(make_callback(this, &type::accelerometer));
    SystemCallbacks::magnetometer_cb().add(make_callback(this, &type::magnetometer));

    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingARTracker::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    SystemCallbacks::gyro_cb().remove(make_callback(this, &type::gyro));
    SystemCallbacks::acceleration_cb().remove(make_callback(this, &type::accelerometer));
    SystemCallbacks::magnetometer_cb().remove(make_callback(this, &type::magnetometer));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3

