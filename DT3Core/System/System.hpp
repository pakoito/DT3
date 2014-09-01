#ifndef DT3_SYSTEM
#define DT3_SYSTEM
//==============================================================================
///	
///	File: System.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class DeviceGraphics;
class DeviceAudio;
class DeviceMusic;

class DeviceInput;
class DeviceNetwork;

class Application;

//==============================================================================
//==============================================================================

class System {
	
    private:
										System						(void);
										System						(const System &rhs);
        System &						operator =					(const System &rhs);	
        virtual							~System						(void);

	public:
    
        DEFINE_ACCESSORS_STATIC(renderer, set_renderer, std::shared_ptr<DeviceGraphics>, _renderer)
        DEFINE_ACCESSORS_STATIC(audio_renderer, set_audio_renderer, std::shared_ptr<DeviceAudio>, _audio_device)
        DEFINE_ACCESSORS_STATIC(music_renderer, set_music_renderer, std::shared_ptr<DeviceMusic>, _music_device)
    
        DEFINE_ACCESSORS_STATIC(input_manager, set_input_manager, std::shared_ptr<DeviceInput>, _input_manager)
        DEFINE_ACCESSORS_STATIC(network_manager, set_network_manager, std::shared_ptr<DeviceNetwork>, _network_manager)

        DEFINE_ACCESSORS_STATIC(application, set_application, std::shared_ptr<Application>, _application)
		
	private:
		// Renderer
		static std::shared_ptr<DeviceGraphics>      _renderer;
		static std::shared_ptr<DeviceAudio>         _audio_device;
		static std::shared_ptr<DeviceMusic>         _music_device;
        
		// Application
		static std::shared_ptr<Application>         _application;

		// Managers
		static std::shared_ptr<DeviceInput>         _input_manager;
		static std::shared_ptr<DeviceNetwork>       _network_manager;

};

//==============================================================================
//==============================================================================


} // DT3

#endif
