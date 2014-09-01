//==============================================================================
///	
///	File: OCOculusHeadTracker.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Oculus/OCOculusHeadTracker.hpp"
#include "DT3Oculus/OCOculusBase.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(OCOculusHeadTracker,"Oculus",NULL)
IMPLEMENT_PLUG_NODE(OCOculusHeadTracker)

IMPLEMENT_PLUG_INFO_INDEX(_gravity_enabled)
IMPLEMENT_PLUG_INFO_INDEX(_yaw_correction_enabled)
IMPLEMENT_PLUG_INFO_INDEX(_is_calibrating)
IMPLEMENT_PLUG_INFO_INDEX(_is_calibrated)
IMPLEMENT_PLUG_INFO_INDEX(_orientation)
IMPLEMENT_PLUG_INFO_INDEX(_acceleration)
IMPLEMENT_PLUG_INFO_INDEX(_angular_velocity)

IMPLEMENT_EVENT_INFO_INDEX(_begin_auto_calibration)
		
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(OCOculusHeadTracker)
		
	PLUG_INIT(_gravity_enabled, "Gravity_Enabled")
		.set_input(true);

	PLUG_INIT(_yaw_correction_enabled, "Yaw_Correction_Enabled")
		.set_input(true);

	PLUG_INIT(_is_calibrating, "Is_Calibrating")
		.set_output(true);

	PLUG_INIT(_is_calibrated, "Is_Calibrated")
		.set_output(true);

	PLUG_INIT(_orientation, "Orientation")
		.set_output(true);

	PLUG_INIT(_acceleration, "Acceleration")
		.set_output(true);

	PLUG_INIT(_angular_velocity, "Angular_Velocity")
		.set_output(true);
    
    EVENT_INIT(_begin_auto_calibration,"Begin_Auto_Calibration")
        .set_input(true)
        .set_event(&OCOculusHeadTracker::begin_auto_calibration);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

OCOculusHeadTracker::OCOculusHeadTracker (void)
    :	_gravity_enabled        (PLUG_INFO_INDEX(_gravity_enabled), true),
		_yaw_correction_enabled (PLUG_INFO_INDEX(_yaw_correction_enabled), false),
		_is_calibrating         (PLUG_INFO_INDEX(_is_calibrating), false),
		_is_calibrated          (PLUG_INFO_INDEX(_is_calibrated), false),
		_orientation            (PLUG_INFO_INDEX(_orientation), Matrix3::identity()),
		_acceleration           (PLUG_INFO_INDEX(_acceleration), Vector3(0.0F,0.0F,0.0F)),
		_angular_velocity       (PLUG_INFO_INDEX(_angular_velocity), Vector3(0.0F,0.0F,0.0F)),
        _begin_auto_calibration (EVENT_INFO_INDEX(_begin_auto_calibration))
{

}
		
OCOculusHeadTracker::OCOculusHeadTracker (const OCOculusHeadTracker &rhs)
    :   ScriptingBase           (rhs),
        _gravity_enabled        (rhs._gravity_enabled),
		_yaw_correction_enabled (rhs._yaw_correction_enabled),
		_is_calibrating         (rhs._is_calibrating),
		_is_calibrated          (rhs._is_calibrated),
		_orientation            (rhs._orientation),
		_acceleration           (rhs._acceleration),
		_angular_velocity       (rhs._angular_velocity),
        _begin_auto_calibration (rhs._begin_auto_calibration)
{   

}

OCOculusHeadTracker & OCOculusHeadTracker::operator = (const OCOculusHeadTracker &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

        _gravity_enabled = rhs._gravity_enabled;
		_yaw_correction_enabled = rhs._yaw_correction_enabled;

		_is_calibrating = rhs._is_calibrating;
		_is_calibrated = rhs._is_calibrated;

		_orientation = rhs._orientation;
		_acceleration = rhs._acceleration;
		_angular_velocity = rhs._angular_velocity;
	}
    return (*this);
}
			
OCOculusHeadTracker::~OCOculusHeadTracker (void)
{

}

//==============================================================================
//==============================================================================

void OCOculusHeadTracker::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

    archive->push_domain (class_id ());
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS
void OCOculusHeadTracker::tick (const DTfloat dt)
{
    // Looking at the SDK, this is a simple assignment so we just sync it contantly
    _sensor_fusion.SetGravityEnabled(_gravity_enabled);
    _sensor_fusion.SetYawCorrectionEnabled(_yaw_correction_enabled);
    
    OVR::Matrix4f ovr_orientation = (OVR::Matrix4f) _sensor_fusion.GetOrientation();
    OVR::Vector3f ovr_acceleration = _sensor_fusion.GetAcceleration();
    OVR::Vector3f ovr_angular_velocity = _sensor_fusion.GetAngularVelocity();
    
    Matrix3 orientation (   ovr_orientation.M[0][0], ovr_orientation.M[0][1], ovr_orientation.M[0][2],
                            ovr_orientation.M[1][0], ovr_orientation.M[1][1], ovr_orientation.M[1][2],
                            ovr_orientation.M[2][0], ovr_orientation.M[2][1], ovr_orientation.M[2][2]);
    
    _orientation = orientation;
    _acceleration = Vector3(ovr_acceleration.x,ovr_acceleration.y,ovr_acceleration.z);
    _angular_velocity = Vector3(ovr_angular_velocity.x,ovr_angular_velocity.y,ovr_angular_velocity.z);
    
    _is_calibrating = _mag_cal.IsAutoCalibrating();
    if (_is_calibrating) {
        _mag_cal.UpdateAutoCalibration(_sensor_fusion);
        
        if (_mag_cal.IsCalibrated()) {
            _is_calibrated = true;
        }
    }
    
}
#endif

//==============================================================================
//==============================================================================

void OCOculusHeadTracker::begin_auto_calibration (PlugNode *sender)
{
    _is_calibrating = false;
    _is_calibrated = false;
    
#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS
    _mag_cal.BeginAutoCalibration(_sensor_fusion);
#endif

}

//==============================================================================
//==============================================================================

void OCOculusHeadTracker::add_to_world (World *world)
{
    ScriptingBase::add_to_world(world);

#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS    
    if (OCOculusBase::has_valid_hmd()) {
        _sensor = OCOculusBase::new_sensor();
        _sensor_fusion.AttachToSensor(_sensor);
    }
    
    world->register_for_tick(this, make_callback(this, &type::tick));
#endif
}

void OCOculusHeadTracker::remove_from_world (void)
{
#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS
    _sensor.Clear();
    
    world()->unregister_for_tick(this, make_callback(this, &type::tick));
#endif

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3

