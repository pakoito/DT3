//==============================================================================
///	
///	File: ScriptingScreen.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingScreen.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Devices/DeviceGraphics.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingScreen,"Engine",NULL)
IMPLEMENT_PLUG_NODE(ScriptingScreen)

IMPLEMENT_PLUG_INFO_INDEX(_width)
IMPLEMENT_PLUG_INFO_INDEX(_height)
IMPLEMENT_PLUG_INFO_INDEX(_aspect)
	
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingScreen)

	PLUG_INIT(_width,"Width")
		.set_output(true);

	PLUG_INIT(_height,"Height")
		.set_output(true);

	PLUG_INIT(_aspect,"Aspect")
		.set_output(true);
                
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingScreen::ScriptingScreen (void)
    :	_width          (PLUG_INFO_INDEX(_width), 1.0F),
        _height         (PLUG_INFO_INDEX(_height), 1.0F),
        _aspect         (PLUG_INFO_INDEX(_aspect), 1.0F)
{  

}
		
ScriptingScreen::ScriptingScreen (const ScriptingScreen &rhs)
    :   ScriptingBase	(rhs),
        _width          (rhs._width),
        _height         (rhs._height),
        _aspect         (rhs._aspect)
{   

}

ScriptingScreen & ScriptingScreen::operator = (const ScriptingScreen &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

        _width = rhs._width;
        _height = rhs._height;
        _aspect = rhs._aspect;
	}
    return (*this);
}
			
ScriptingScreen::~ScriptingScreen (void)
{
    SystemCallbacks::screen_opened_cb().remove(make_callback(this, &ScriptingScreen::screen_opened));
    SystemCallbacks::screen_changed_cb().remove(make_callback(this, &ScriptingScreen::screen_opened));
}

//==============================================================================
//==============================================================================

void ScriptingScreen::initialize (void)
{
	ScriptingBase::initialize();
    
    SystemCallbacks::screen_opened_cb().add(make_callback(this, &ScriptingScreen::screen_opened));
    SystemCallbacks::screen_changed_cb().add(make_callback(this, &ScriptingScreen::screen_opened));
}

//==============================================================================
//==============================================================================

void ScriptingScreen::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_width, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_height, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_aspect, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

void ScriptingScreen::archive_done   (const std::shared_ptr<Archive> &archive)
{		
	ScriptingBase::archive_done (archive);
    
    if (archive->is_writing())
        return;

    screen_changed(0,0);
}

//==============================================================================
//==============================================================================

void ScriptingScreen::screen_opened (DTuint width, DTuint height)
{
    if (System::renderer()) {
        _width = System::renderer()->screen_width();
        _height = System::renderer()->screen_height();
        _aspect = System::renderer()->screen_aspect();
    }
}

void ScriptingScreen::screen_changed (DTuint width, DTuint height)
{
    if (System::renderer()) {
        _width = System::renderer()->screen_width();
        _height = System::renderer()->screen_height();
        _aspect = System::renderer()->screen_aspect();
    }
}

//==============================================================================
//==============================================================================

} // DT3

