//==============================================================================
///	
///	File: System.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/System.hpp"
#include "DT3Core/System/Application.hpp"

#include "DT3Core/Devices/DeviceGraphics.hpp"
#include "DT3Core/Devices/DeviceAudio.hpp"
#include "DT3Core/Devices/DeviceMusic.hpp"
#include "DT3Core/Devices/DeviceInput.hpp"
#include "DT3Core/Devices/DeviceNetwork.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

// Renderer
std::shared_ptr<DeviceGraphics>         System::_renderer;
std::shared_ptr<DeviceAudio>            System::_audio_device;
std::shared_ptr<DeviceMusic>            System::_music_device;

// Managers
std::shared_ptr<DeviceInput>            System::_input_manager;
std::shared_ptr<DeviceNetwork>          System::_network_manager;

// Application
std::shared_ptr<Application>            System::_application;

//==============================================================================
//==============================================================================



} // DT3
