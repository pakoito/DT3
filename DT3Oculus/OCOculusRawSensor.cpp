//==============================================================================
///	
///	File: OCOculusRawSensor.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Oculus/OCOculusRawSensor.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(OCOculusRawSensor,"Oculus",NULL)
IMPLEMENT_PLUG_NODE(OCOculusRawSensor)

IMPLEMENT_PLUG_INFO_INDEX(_acceleration)
IMPLEMENT_PLUG_INFO_INDEX(_rotation_rate)
IMPLEMENT_PLUG_INFO_INDEX(_magnetic_field)
IMPLEMENT_PLUG_INFO_INDEX(_temperature)
IMPLEMENT_PLUG_INFO_INDEX(_time_delta)
		
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(OCOculusRawSensor)
		
	PLUG_INIT(_acceleration, "Acceleration")
		.set_input(true);

	PLUG_INIT(_rotation_rate, "Rotation_Rate")
		.set_input(true);

	PLUG_INIT(_magnetic_field, "Magnetic_Field")
		.set_output(true);

	PLUG_INIT(_temperature, "Temperature")
		.set_output(true);

	PLUG_INIT(_time_delta, "Time_Delta")
		.set_output(true);
    
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

OCOculusRawSensor::OCOculusRawSensor (void)
    :	_acceleration       (PLUG_INFO_INDEX(_acceleration), Vector3(0.0F,0.0F,0.0F)),
		_rotation_rate      (PLUG_INFO_INDEX(_rotation_rate), Vector3(0.0F,0.0F,0.0F)),
		_magnetic_field     (PLUG_INFO_INDEX(_magnetic_field), Vector3(0.0F,0.0F,0.0F)),
		_temperature        (PLUG_INFO_INDEX(_temperature), 0.0F),
		_time_delta          (PLUG_INFO_INDEX(_time_delta), 0.0F)
{

}
		
OCOculusRawSensor::OCOculusRawSensor (const OCOculusRawSensor &rhs)
    :   ScriptingBase           (rhs),
        _acceleration           (rhs._acceleration),
		_rotation_rate          (rhs._rotation_rate),
		_magnetic_field         (rhs._magnetic_field),
		_temperature            (rhs._temperature),
        _time_delta              (rhs._time_delta)
{   

}

OCOculusRawSensor & OCOculusRawSensor::operator = (const OCOculusRawSensor &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

        _acceleration = rhs._acceleration;
		_rotation_rate = rhs._rotation_rate;
		_magnetic_field = rhs._magnetic_field;
		_temperature = rhs._temperature;
		_time_delta = rhs._time_delta;
	}
    return (*this);
}
			
OCOculusRawSensor::~OCOculusRawSensor (void)
{

}

//==============================================================================
//==============================================================================

void OCOculusRawSensor::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

    archive->push_domain (class_id());
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS
void OCOculusRawSensor::OnMessage (const OVR::Message &msg)
{
    if (msg.Type != OVR::Message_BodyFrame)
        return;
    
    const OVR::MessageBodyFrame &msg_body_frame = static_cast<const OVR::MessageBodyFrame&>(msg);
    
    _acceleration = Vector3(    msg_body_frame.Acceleration.x,
                                msg_body_frame.Acceleration.y,
                                msg_body_frame.Acceleration.z);
    _rotation_rate = Vector3(   msg_body_frame.RotationRate.x,
                                msg_body_frame.RotationRate.y,
                                msg_body_frame.RotationRate.z);
    _magnetic_field = Vector3(  msg_body_frame.MagneticField.x,
                                msg_body_frame.MagneticField.y,
                                msg_body_frame.MagneticField.z);
    _temperature = msg_body_frame.Temperature;
    _time_delta = msg_body_frame.TimeDelta;
}
#endif

//==============================================================================
//==============================================================================

void OCOculusRawSensor::add_to_world (World *world)
{
    ScriptingBase::add_to_world(world);

#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS
    if (OCOculusBase::has_valid_hmd()) {
        _sensor = OCOculusBase::new_sensor();
        _sensor->SetMessageHandler(this);
    }
#endif

}

void OCOculusRawSensor::remove_from_world (void)
{
#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS
    if (_sensor) {
        _sensor->SetMessageHandler(NULL);
        _sensor.Clear();
    }
#endif

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3

