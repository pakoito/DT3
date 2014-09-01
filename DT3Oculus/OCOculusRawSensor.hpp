#ifndef DT3_OCOCULUSRAWSENSOR
#define DT3_OCOCULUSRAWSENSOR
//==============================================================================
///	
///	File: OCOculusRawSensor.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/Math/Matrix3.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"

#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS
#include "OVR.h"
#endif

namespace DT3 {

//==============================================================================
/// Accelerometer input.
//==============================================================================

#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS
class OCOculusRawSensor: public ScriptingBase, public OVR::MessageHandler {
#else
class OCOculusRawSensor: public ScriptingBase {
#endif
    public:
        DEFINE_TYPE(OCOculusRawSensor,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    OCOculusRawSensor       (void);	
									OCOculusRawSensor       (const OCOculusRawSensor &rhs);
        OCOculusRawSensor &         operator =				(const OCOculusRawSensor &rhs);	
        virtual                     ~OCOculusRawSensor      (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world       (void);

    public:
    
#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS
        // Inherited from OVR::MessageHandler
        virtual void                OnMessage               (const OVR::Message &msg);
#endif
 
	private:		
		Plug<Vector3>               _acceleration;
		Plug<Vector3>               _rotation_rate;
		Plug<Vector3>               _magnetic_field;
		Plug<DTfloat>               _temperature;
		Plug<DTfloat>               _time_delta;
    
#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS
        OVR::Ptr<OVR::SensorDevice> _sensor;
#endif
};

//==============================================================================
//==============================================================================

} // DT3

#endif
