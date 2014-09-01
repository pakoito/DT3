#ifndef DT3_OCOCULUSHEADTRACKER
#define DT3_OCOCULUSHEADTRACKER
//==============================================================================
///	
///	File: OCOculusHeadTracker.hpp
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

class OCOculusHeadTracker: public ScriptingBase {
    public:
        DEFINE_TYPE(OCOculusHeadTracker,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    OCOculusHeadTracker     (void);	
									OCOculusHeadTracker     (const OCOculusHeadTracker &rhs);
        OCOculusHeadTracker &       operator =				(const OCOculusHeadTracker &rhs);	
        virtual                     ~OCOculusHeadTracker    (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world       (void);

	public:
        
#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS
        /// Registered with world to tick this node
		/// \param dt delta time
		virtual void                tick                    (const DTfloat dt);
#endif

        /// Starts the auto calibration
        void                        begin_auto_calibration  (PlugNode *sender);

	private:		
		Plug<DTboolean>             _gravity_enabled;
		Plug<DTboolean>             _yaw_correction_enabled;

		Plug<DTboolean>             _is_calibrating;
		Plug<DTboolean>             _is_calibrated;

		Plug<Matrix3>               _orientation;
		Plug<Vector3>               _acceleration;
		Plug<Vector3>               _angular_velocity;
    
        Event                       _begin_auto_calibration;
    
#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS
        OVR::Ptr<OVR::SensorDevice> _sensor;
        OVR::SensorFusion           _sensor_fusion;
    
        // Magnetometer calibration
        OVR::Util::MagCalibration   _mag_cal;
#endif
};

//==============================================================================
//==============================================================================

} // DT3

#endif
