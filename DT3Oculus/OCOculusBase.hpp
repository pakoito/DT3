#ifndef DT3_OCOCULUSBASE
#define DT3_OCOCULUSBASE
//==============================================================================
///	
///	File: OCOculusBase.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"

#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS

#include "OVR.h"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Forward Declarations
//==============================================================================

class CameraObject;

//==============================================================================
//==============================================================================

class OCOculusBase {
    public:
        DEFINE_TYPE_SIMPLE_BASE(OCOculusBase)

	private:
                                            OCOculusBase                (void);
                                            OCOculusBase                (const OCOculusBase&);
        OCOculusBase&                       operator =                  (const OCOculusBase&);
        virtual                             ~OCOculusBase               (void);
        
	public:
        /// Initialize the OCOculusBase
        static void                         initialize                  (void);

		/// Destroy the OCOculusBase
        static void                         destroy                     (void);

        /// Check for availability of Oculus Device
        static DTboolean                    has_valid_hmd               (void);
    
		/// Destroy the OCOculusBase
        /// \return success
        static DTboolean                    scan_for_devices            (void);
    
        /// Returns a OVR smart pointer to the raw sensor
        static OVR::Ptr<OVR::SensorDevice>  new_sensor                  (void);

        enum Camera {
            CAMERA_LEFT,
            CAMERA_RIGHT
        };
    
        /// Update a stereo camera given a center camera.
        /// \param center_camera source camera
        /// \param stereo_camera destination camera
        /// \param c which camera to generate
        static void                         calculate_stereo_camera     (const CameraObject *center_camera, CameraObject *stereo_camera, Camera c);
    
    
        /// Return the actual resolution of the screen
        static void                         native_screen_resolution    (DTint &horz_screen_resolution, DTint &vert_screen_resolution);

        /// Return the distortion coefficients
        static void                         distortion_coefficients     (DTfloat &k0, DTfloat &k1, DTfloat &k2, DTfloat &k3);

        /// Return the chromatic aberration coefficients
        static void                         chromatic_aberration_coefficients  (DTfloat &k0, DTfloat &k1, DTfloat &k2, DTfloat &k3);
    
        /// Calculate Lens Center.
        static DTfloat                      calculate_lens_center       (void);
    
        /// Calculate Lens Distortion.
        static DTfloat                      calculate_lens_distortion   (DTfloat r);

        /// Calculate Scale factor.
        static DTfloat                      calculate_scale_factor      (void);

    private:

        static OVR::Ptr<OVR::DeviceManager> _manager;
    
        static OVR::Ptr<OVR::HMDDevice>     _hmd;
        static OVR::HMDInfo                 _hmd_info;

};

//==============================================================================
//==============================================================================

} // DT3

#endif
#endif
