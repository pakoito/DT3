//==============================================================================
///	
///	File: ScriptingCameraTexture.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "ScriptingCameraTexture.hpp"
#include "Factory.hpp"
#include "Archive.hpp"
#include "System.hpp"
#include "DeviceInput.hpp"

#include "System.hpp"
#include "SystemCallbacks.hpp"
#include "World.hpp"
#include "Profiler.hpp"
#include "ConsoleStream.hpp"

#if DT2_OS == DT2_MACOSX
#include "CameraMacOSX.hpp"
#elif DT2_OS == DT2_IOS
#include "CameraiOS.hpp"
#endif

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
//==============================================================================

#if DT2_OS == DT2_MACOSX
typedef CameraMacOSX Camera;
#elif DT2_OS == DT2_IOS
typedef CameraiOS Camera;
#endif

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingCameraTexture,"Camera",NULL)
IMPLEMENT_PLUG_NODE(ScriptingCameraTexture)

IMPLEMENT_PLUG_INFO(_out)
IMPLEMENT_PLUG_INFO(_rectangle)

IMPLEMENT_EVENT_INFO(_start)
IMPLEMENT_EVENT_INFO(_stop)
		
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingCameraTexture)
		
	PLUG_INIT(_out,"Tex_Out")
		.setOutput(true);

	PLUG_INIT(_rectangle,"Rect")
		.setOutput(true);

	EVENT_INIT(_start,"Start")
        .setInput(true)
        .setEvent(EventBindCreator(&ScriptingCameraTexture::start));

	EVENT_INIT(_stop,"Stop")
        .setInput(true)
        .setEvent(EventBindCreator(&ScriptingCameraTexture::stop));

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingCameraTexture::ScriptingCameraTexture (void)
    :	_out                    (PLUG_INFO(_out)),
		_rectangle              (PLUG_INFO(_rectangle), Rectangle(0.0F,1.0F,0.0F,1.0F)),
        _start                  (EVENT_INFO(_start)),
        _stop                   (EVENT_INFO(_stop))
{

}
		
ScriptingCameraTexture::ScriptingCameraTexture (const ScriptingCameraTexture &rhs)
    :   ScriptingBase           (rhs),
		_out                    (rhs._out),
		_rectangle              (rhs._rectangle),
        _start                  (EVENT_INFO(_start)),
        _stop                   (EVENT_INFO(_stop))
{   

}

ScriptingCameraTexture & ScriptingCameraTexture::operator = (const ScriptingCameraTexture &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

        _out = rhs._out;
        _rectangle = rhs._rectangle;
	}
    return (*this);
}
			
ScriptingCameraTexture::~ScriptingCameraTexture (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingCameraTexture::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingCameraTexture::archiveRead (Archive *archive)
{
    ScriptingBase::archiveRead(archive);

	archive->pushDomain (getClassID ());
    	        					
    archive->popDomain ();
}

void ScriptingCameraTexture::archiveWrite (Archive *archive)
{
    ScriptingBase::archiveWrite(archive);

    archive->pushDomain (getClassID ());
    
    archive->popDomain ();
}

//==============================================================================
//==============================================================================

void ScriptingCameraTexture::start (PlugNode *sender)
{   
    Camera::startRecording();
}

void ScriptingCameraTexture::stop (PlugNode *sender)
{
    Camera::stopRecording();
}

//==============================================================================
//==============================================================================

void ScriptingCameraTexture::gotFrame (std::shared_ptr<TextureResource> tex, Rectangle &rect)
{
    _frame = tex;
    _frame_rect = rect;
}

//==============================================================================
//==============================================================================

void ScriptingCameraTexture::tick (const DTfloat dt)
{
    if (_frame.isValid()) {
        _out = _frame;
        _rectangle = _frame_rect;
    }
}

//==============================================================================
//==============================================================================

void ScriptingCameraTexture::addToWorld (World *world)
{
    ScriptingBase::addToWorld(world);
    
    world->registerForTick(this, makeCallback(this, &type::tick));
    
    Camera::getFrameCapturedCB().add(makeCallback(this, &type::gotFrame));
}

void ScriptingCameraTexture::removeFromWorld (void)
{
    Camera::getFrameCapturedCB().remove(makeCallback(this, &type::gotFrame));

    getWorld()->unregisterForTick(this, makeCallback(this, &type::tick));

    ScriptingBase::removeFromWorld();
}

//==============================================================================
//==============================================================================

} // DT2

