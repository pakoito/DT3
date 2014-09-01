//==============================================================================
///	
///	File: OCOculusBase.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Oculus/OCOculusBase.hpp"

#if DT3_OS != DT3_ANDROID && DT3_OS != DT3_IOS

#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Objects/CameraObject.hpp"
#include "DT3Core/Devices/DeviceGraphics.hpp"
#include <string>
#include <cmath>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,OCOculusBase::initialize())
GLOBAL_STATIC_DESTRUCTION(0,OCOculusBase::destroy())

//==============================================================================
//==============================================================================

OVR::Ptr<OVR::DeviceManager> OCOculusBase::_manager;
OVR::Ptr<OVR::HMDDevice>     OCOculusBase::_hmd;
OVR::HMDInfo                 OCOculusBase::_hmd_info;

//==============================================================================
//==============================================================================

class OCOculusBaseLog: public OVR::Log
{
    public:
    
        // Provide our own implementation of the OVR log object
        virtual void LogMessageVarg(OVR::LogMessageType messageType, const char* fmt, va_list argList) {
            
            const DTint BUFFER_SIZE = 1024;
            
            std::string buffer;
            buffer.reserve(BUFFER_SIZE);
            
            vsnprintf (&buffer[0], sizeof(BUFFER_SIZE), fmt, argList);
            
            // Remove newlines
            buffer = MoreStrings::find_and_replace(buffer, "\n", " ");
            
            switch(messageType) {
                case OVR::Log_Text:      LOG_MESSAGE << buffer;  break;
                case OVR::Log_Error:     LOG_ERROR << buffer;  break;
                case OVR::Log_DebugText: 
                case OVR::Log_Debug:
                case OVR::Log_Assert:    LOG_DEBUG << buffer;  break;
            };
            
        }
};

//==============================================================================
//==============================================================================

void OCOculusBase::initialize (void)
{
#ifndef DT3_EDITOR
    OVR::System::Init(new OCOculusBaseLog());
    
    // We use DeviceManager::Create to create a new instance of DeviceManager.
    _manager = *(OVR::DeviceManager::Create());
#endif
    
    // Scan for device
    scan_for_devices();
}

void OCOculusBase::destroy (void)
{
#ifndef DT3_EDITOR
    // Clears the smart pointers and decrements reference count
    _manager.Clear();
    _hmd.Clear();
    
    OVR::System::Destroy();
#endif
}

//==============================================================================
//==============================================================================

DTboolean OCOculusBase::has_valid_hmd(void)
{
    return _hmd != NULL;
}

//==============================================================================
//==============================================================================

DTboolean OCOculusBase::scan_for_devices (void)
{
    // Set default info
    _hmd_info.HResolution = 1280;
    _hmd_info.VResolution = 800;
    _hmd_info.HScreenSize = 0.14976F;
    _hmd_info.VScreenSize = 0.0935F;
    _hmd_info.VScreenCenter = 0.5F * _hmd_info.VScreenSize;
    _hmd_info.EyeToScreenDistance = 0.5F * _hmd_info.VScreenSize;
    _hmd_info.LensSeparationDistance = 0.0635F;
    _hmd_info.InterpupillaryDistance = 0.064F;
    
    _hmd_info.DistortionK[0] = 1.0F;
    _hmd_info.DistortionK[1] = 0.22F;
    _hmd_info.DistortionK[2] = 0.24F;
    _hmd_info.DistortionK[3] = 0.0F;
    
    _hmd_info.ChromaAbCorrection[0] = 0.996F;
    _hmd_info.ChromaAbCorrection[1] = -0.004F;
    _hmd_info.ChromaAbCorrection[2] = 1.014F;
    _hmd_info.ChromaAbCorrection[3] = 0.0F;

#ifndef DT3_EDITOR
    // Get the device
    _hmd = *(_manager->EnumerateDevices<OVR::HMDDevice>().CreateDevice());
    if (_hmd == NULL) {
        LOG_MESSAGE << "Unable to find Oculus device.";
        return false;
    }
    
    // Get the device info
    if (!_hmd->GetDeviceInfo(&_hmd_info)) {
        _hmd.Clear();
        LOG_MESSAGE << "Unable to get info for Oculus device.";
        return false;
    }

    return true;
#else
    return false;
#endif
}

OVR::Ptr<OVR::SensorDevice> OCOculusBase::new_sensor (void)
{
#ifndef DT3_EDITOR
    return *(_hmd->GetSensor());
#else
    return OVR::Ptr<OVR::SensorDevice>();
#endif
}

//==============================================================================
//==============================================================================

void OCOculusBase::calculate_stereo_camera(const CameraObject *center_camera, CameraObject *stereo_camera, Camera c)
{
    //
    // This code is modified from the Oculus SDK
    //

    // Compute Aspect Ratio. Stereo mode cuts width in half.
    DTfloat aspect_ratio = static_cast<DTfloat>(_hmd_info.HResolution) * 0.5F / static_cast<DTfloat>(_hmd_info.VResolution);
    
    // Compute Vertical FOV based on distance.
    DTfloat half_screen_distance = static_cast<DTfloat>(_hmd_info.VScreenSize / 2);
    
    DTfloat yfov = 2.0f * std::atan(calculate_scale_factor() * half_screen_distance / _hmd_info.EyeToScreenDistance);
    
    // Post-projection viewport coordinates range from (-1.0, 1.0), with the
    // center of the left viewport falling at (1/4) of horizontal screen size.
    // We need to shift this projection center to match with the lens center.
    // We compute this shift in physical units (meters) to correct
    // for different screen sizes and then rescale to viewport coordinates.
    DTfloat view_center             = _hmd_info.HScreenSize * 0.25F;
    DTfloat eye_projection_shift     = view_center - _hmd_info.LensSeparationDistance * 0.5F;
    DTfloat projection_center_offset = 4.0F * eye_projection_shift / _hmd_info.HScreenSize;
    
    // Projection matrix for the "center eye", which the left/right matrices are based on.
    Matrix4 post_projection_matrix(Matrix4::identity());
    if (c == CAMERA_LEFT) {
        post_projection_matrix.set_translation(Vector3(projection_center_offset, 0.0F, 0.0F));
    } else {
        post_projection_matrix.set_translation(Vector3(-projection_center_offset, 0.0F, 0.0F));
    }
    
    // Apply camera
    *stereo_camera = *center_camera;
    stereo_camera->set_post_projection_matrix(post_projection_matrix);
    stereo_camera->set_aspect_ratio_mul(1.0F / System::renderer()->screen_aspect() * aspect_ratio);
    stereo_camera->set_angle(yfov * RAD_TO_DEG);
    stereo_camera->set_perspective_fill_width(false);
    
    // View transformation translation in world units.
    DTfloat half_ipd  = _hmd_info.InterpupillaryDistance * 0.5f;
    Matrix4 offset(Matrix4::identity());
    
    if (c == CAMERA_LEFT) {
        offset.set_translation(Vector3(half_ipd, 0, 0));
    } else {
        offset.set_translation(Vector3(-half_ipd, 0, 0));
    }
    
    stereo_camera->set_transform(offset * center_camera->transform());
}

//==============================================================================
//==============================================================================

void OCOculusBase::native_screen_resolution   (DTint &horz_screen_resolution, DTint &vert_screen_resolution)
{
    horz_screen_resolution = _hmd_info.HResolution;
    vert_screen_resolution = _hmd_info.VResolution;
}

void OCOculusBase::distortion_coefficients (DTfloat &k0, DTfloat &k1, DTfloat &k2, DTfloat &k3)
{
    k0 = _hmd_info.DistortionK[0];
    k1 = _hmd_info.DistortionK[1];
    k2 = _hmd_info.DistortionK[2];
    k3 = _hmd_info.DistortionK[3];
}

void OCOculusBase::chromatic_aberration_coefficients (DTfloat &k0, DTfloat &k1, DTfloat &k2, DTfloat &k3)
{
    k0 = _hmd_info.ChromaAbCorrection[0];
    k1 = _hmd_info.ChromaAbCorrection[1];
    k2 = _hmd_info.ChromaAbCorrection[2];
    k3 = _hmd_info.DistortionK[3];
}

//==============================================================================
//==============================================================================

DTfloat OCOculusBase::calculate_lens_center (void)
{
    return 1.0F - 2.0F * static_cast<DTfloat>(_hmd_info.LensSeparationDistance) / static_cast<DTfloat>(_hmd_info.HScreenSize);
}

DTfloat OCOculusBase::calculate_lens_distortion (DTfloat r)
{
    DTfloat r2 = r*r;
    DTfloat r4 = r2*r2;
    DTfloat r6 = r2*r4;

    return  _hmd_info.DistortionK[0] +
            _hmd_info.DistortionK[1] * r2 +
            _hmd_info.DistortionK[2] * r4 +
            _hmd_info.DistortionK[3] * r6;
}

DTfloat OCOculusBase::calculate_scale_factor (void)
{
    DTfloat lens_center = calculate_lens_center();
    return calculate_lens_distortion(-1.0F - lens_center);
}

//==============================================================================
//==============================================================================

} // DT3

#endif

