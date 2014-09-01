//==============================================================================
///	
///	File: ScriptingTouch.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingTouch.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Utility/TouchEvent.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingTouch,"Input",NULL)
IMPLEMENT_PLUG_NODE(ScriptingTouch)

IMPLEMENT_PLUG_INFO_INDEX(_touch_pos_1)
IMPLEMENT_PLUG_INFO_INDEX(_touch_vel_1)
IMPLEMENT_PLUG_INFO_INDEX(_touch_initial_1)
IMPLEMENT_PLUG_INFO_INDEX(_touch_active_1)
IMPLEMENT_PLUG_INFO_INDEX(_touch_pos_2)
IMPLEMENT_PLUG_INFO_INDEX(_touch_vel_2)
IMPLEMENT_PLUG_INFO_INDEX(_touch_initial_2)
IMPLEMENT_PLUG_INFO_INDEX(_touch_active_2)
IMPLEMENT_PLUG_INFO_INDEX(_touch_pos_3)
IMPLEMENT_PLUG_INFO_INDEX(_touch_vel_3)
IMPLEMENT_PLUG_INFO_INDEX(_touch_initial_3)
IMPLEMENT_PLUG_INFO_INDEX(_touch_active_3)
	
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingTouch)

	PLUG_INIT(_touch_pos_1,"Touch_Pos_1")
		.set_output(true);

	PLUG_INIT(_touch_vel_1,"Touch_Vel_1")
		.set_output(true);

	PLUG_INIT(_touch_initial_1,"Touch_Initial_1")
		.set_output(true);

	PLUG_INIT(_touch_active_1,"Touch_Active_1")
		.set_output(true);
        
	PLUG_INIT(_touch_pos_2,"Touch_Pos_2")
		.set_output(true);

	PLUG_INIT(_touch_vel_2,"Touch_Vel_2")
		.set_output(true);

	PLUG_INIT(_touch_initial_2,"Touch_Initial_2")
		.set_output(true);
    
	PLUG_INIT(_touch_active_2,"Touch_Active_2")
		.set_output(true);
        
	PLUG_INIT(_touch_pos_3,"Touch_Pos_3")
		.set_output(true);

	PLUG_INIT(_touch_vel_3,"Touch_Vel_3")
		.set_output(true);

	PLUG_INIT(_touch_initial_3,"Touch_Initial_3")
		.set_output(true);

	PLUG_INIT(_touch_active_3,"Touch_Active_3")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingTouch::ScriptingTouch (void)
    :	_touch_pos_1	(PLUG_INFO_INDEX(_touch_pos_1), Vector2(0.0F,0.0F)),
        _touch_vel_1	(PLUG_INFO_INDEX(_touch_vel_1), Vector2(0.0F,0.0F)),
        _touch_initial_1(PLUG_INFO_INDEX(_touch_initial_1), Vector2(0.0F,0.0F)),
		_touch_active_1	(PLUG_INFO_INDEX(_touch_active_1), false),
		_touch_pos_2	(PLUG_INFO_INDEX(_touch_pos_2), Vector2(0.0F,0.0F)),
		_touch_vel_2	(PLUG_INFO_INDEX(_touch_vel_2), Vector2(0.0F,0.0F)),
        _touch_initial_2(PLUG_INFO_INDEX(_touch_initial_2), Vector2(0.0F,0.0F)),
		_touch_active_2	(PLUG_INFO_INDEX(_touch_active_2), false),
		_touch_pos_3	(PLUG_INFO_INDEX(_touch_pos_3), Vector2(0.0F,0.0F)),
		_touch_vel_3	(PLUG_INFO_INDEX(_touch_vel_3), Vector2(0.0F,0.0F)),
        _touch_initial_3(PLUG_INFO_INDEX(_touch_initial_3), Vector2(0.0F,0.0F)),
		_touch_active_3	(PLUG_INFO_INDEX(_touch_active_3), false),
        _clear_vel_1    (false),
        _clear_vel_2    (false),
        _clear_vel_3    (false)

{  

}
		
ScriptingTouch::ScriptingTouch (const ScriptingTouch &rhs)
    :   ScriptingBase	(rhs),
        _touch_pos_1    (rhs._touch_pos_1),
        _touch_vel_1    (rhs._touch_vel_1),
        _touch_initial_1(rhs._touch_initial_1),
        _touch_active_1 (rhs._touch_active_1),
        _touch_pos_2    (rhs._touch_pos_2),
        _touch_vel_2    (rhs._touch_vel_2),
        _touch_initial_2(rhs._touch_initial_2),
        _touch_active_2 (rhs._touch_active_2),
        _touch_pos_3    (rhs._touch_pos_3),
        _touch_vel_3    (rhs._touch_vel_3),
        _touch_initial_3(rhs._touch_initial_3),
        _touch_active_3 (rhs._touch_active_3),
        _clear_vel_1    (rhs._clear_vel_1),
        _clear_vel_2    (rhs._clear_vel_2),
        _clear_vel_3    (rhs._clear_vel_3)
{

}

ScriptingTouch & ScriptingTouch::operator = (const ScriptingTouch &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

        _touch_pos_1 = rhs._touch_pos_1;
        _touch_vel_1 = rhs._touch_vel_1;
        _touch_initial_1 = rhs._touch_initial_1;
        _touch_active_1 = rhs._touch_active_1;
        _touch_pos_2 = rhs._touch_pos_2;
        _touch_vel_2 = rhs._touch_vel_2;
        _touch_initial_2 = rhs._touch_initial_2;
        _touch_active_2 = rhs._touch_active_2;
        _touch_pos_3 = rhs._touch_pos_3;
        _touch_vel_3 = rhs._touch_vel_3;
        _touch_initial_3 = rhs._touch_initial_3;
        _touch_active_3 = rhs._touch_active_3;
        
        _clear_vel_1 = rhs._clear_vel_1;
        _clear_vel_2 = rhs._clear_vel_2;
        _clear_vel_3 = rhs._clear_vel_3;
	}
    return (*this);
}
			
ScriptingTouch::~ScriptingTouch (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingTouch::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingTouch::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_PLUG(_touch_pos_1, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_touch_vel_1, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_touch_initial_1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_touch_active_1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_touch_pos_2, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_touch_vel_2, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_touch_initial_2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_touch_active_2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_touch_pos_3, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_touch_vel_3, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_touch_initial_3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_touch_active_3, DATA_PERSISTENT | DATA_SETTABLE);
	   
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingTouch::touch (const TouchEvent*event, const std::shared_ptr<CameraObject> &camera, WorldNode* &handled)
{
	PROFILER(SCRIPTING);

    _touch_pos_1 = TouchEvent::normalize_input(event->touches[0].pos);
    _touch_vel_1 = TouchEvent::normalize_input_vel(event->touches[0].velocity);
    _touch_initial_1 = TouchEvent::normalize_input(event->touches[0].first_pos);
    _touch_active_1 = (event->touches[0].state == TouchEvent::STATE_DOWN || event->touches[0].state == TouchEvent::STATE_PRESSED);

    _touch_pos_2 = TouchEvent::normalize_input(event->touches[1].pos);
    _touch_vel_2 = TouchEvent::normalize_input_vel(event->touches[1].velocity);
    _touch_initial_2 = TouchEvent::normalize_input(event->touches[1].first_pos);
    _touch_active_2 = (event->touches[1].state == TouchEvent::STATE_DOWN || event->touches[1].state == TouchEvent::STATE_PRESSED);

    _touch_pos_3 = TouchEvent::normalize_input(event->touches[2].pos);
    _touch_vel_3 = TouchEvent::normalize_input_vel(event->touches[2].velocity);
    _touch_initial_3 = TouchEvent::normalize_input(event->touches[2].first_pos);
    _touch_active_3 = (event->touches[2].state == TouchEvent::STATE_DOWN || event->touches[2].state == TouchEvent::STATE_PRESSED);
}

//==============================================================================
//==============================================================================

void ScriptingTouch::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);
}

//==============================================================================
//==============================================================================

void ScriptingTouch::draw (const std::shared_ptr<CameraObject> &camera, const DTfloat lag)
{
	PROFILER(SCRIPTING);

    _touch_vel_1 = Vector2(0.0F,0.0F);
    _touch_vel_2 = Vector2(0.0F,0.0F);
    _touch_vel_3 = Vector2(0.0F,0.0F);
}

//==============================================================================
//==============================================================================

void ScriptingTouch::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    world->register_for_touch(this, make_callback(this, &type::touch));
    world->register_for_draw(this, make_callback(this, &type::draw));
}

void ScriptingTouch::remove_from_world (void)
{
    world()->unregister_for_touch(this, make_callback(this, &type::touch));
    world()->unregister_for_draw(this, make_callback(this, &type::draw));
    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3

