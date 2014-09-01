//==============================================================================
///	
///	File: ScriptingTransitionLevel.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingTransitionLevel.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/System/Application.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Utility/Progress.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingTransitionLevel,"Engine",NULL)
IMPLEMENT_PLUG_NODE(ScriptingTransitionLevel)

IMPLEMENT_PLUG_INFO_INDEX(_pathname)		
IMPLEMENT_PLUG_INFO_INDEX(_transition)		
IMPLEMENT_PLUG_INFO_INDEX(_progress)		
IMPLEMENT_PLUG_INFO_INDEX(_duration)		
IMPLEMENT_PLUG_INFO_INDEX(_same_session)		

IMPLEMENT_EVENT_INFO_INDEX(_transition_e)		

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingTransitionLevel)

	PLUG_INIT(_pathname,"Pathname")
		.set_input(true);

	PLUG_INIT(_transition,"Transition")
		.set_input(true);
        
	PLUG_INIT(_progress,"Progress")
		.set_input(true);
        
	PLUG_INIT(_duration,"Duration")
		.set_input(true);
        
	PLUG_INIT(_same_session,"Use_Same_Session")
		.set_input(true);
        
	EVENT_INIT(_transition_e,"Transition_Event")
        .set_input(true)
        .set_event(&ScriptingTransitionLevel::transition);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingTransitionLevel::ScriptingTransitionLevel (void)
    :   _pathname           (PLUG_INFO_INDEX(_pathname), FilePath("")),
		_transition         (PLUG_INFO_INDEX(_transition), "TransitionFadeOutIn"),
		_progress           (PLUG_INFO_INDEX(_progress), "ScriptProgress"),
		_duration           (PLUG_INFO_INDEX(_duration), 2.0F),
		_same_session       (PLUG_INFO_INDEX(_same_session), false),
        _transition_e       (EVENT_INFO_INDEX(_transition_e))
{  

}
		
ScriptingTransitionLevel::ScriptingTransitionLevel (const ScriptingTransitionLevel &rhs)
    :   ScriptingBase	(rhs),
		_pathname       (rhs._pathname),
        _transition     (rhs._transition),
        _progress       (rhs._progress),
        _duration       (rhs._duration),
        _same_session   (rhs._same_session),
        _transition_e   (rhs._transition_e)
{   

}

ScriptingTransitionLevel & ScriptingTransitionLevel::operator = (const ScriptingTransitionLevel &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_pathname = rhs._pathname;
        _transition = rhs._transition;
        _progress = rhs._progress;
        _duration = rhs._duration;
        _same_session = rhs._same_session;
	}
    return (*this);
}
			
ScriptingTransitionLevel::~ScriptingTransitionLevel (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingTransitionLevel::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingTransitionLevel::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
    
	*archive << ARCHIVE_PLUG(_pathname, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_transition, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_progress, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_duration, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_same_session, DATA_PERSISTENT | DATA_SETTABLE);
		        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingTransitionLevel::transition (PlugNode *sender)
{
	PROFILER(SCRIPTING);

    if (!System::application()->is_transitioning()) {

        std::shared_ptr<Progress> progress = checked_static_cast<Progress>(Factory::create_object(_progress));
        
        if (_same_session)
            System::application()->transition_to_world(_pathname, _transition, _duration, world()->session(), progress);
        else
            System::application()->transition_to_world(_pathname, _transition, _duration, NULL, progress);
    }
}

//==============================================================================
//==============================================================================

} // DT3

