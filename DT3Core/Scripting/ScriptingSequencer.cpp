//==============================================================================
///	
///	File: ScriptingSequencer.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSequencer.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSequencer,"Animation",NULL)
IMPLEMENT_PLUG_NODE(ScriptingSequencer)

IMPLEMENT_PLUG_INFO_INDEX(_go)
IMPLEMENT_PLUG_INFO_INDEX(_repeat)
IMPLEMENT_PLUG_INFO_INDEX(_out1)
IMPLEMENT_PLUG_INFO_INDEX(_out2)
IMPLEMENT_PLUG_INFO_INDEX(_out3)
IMPLEMENT_PLUG_INFO_INDEX(_out4)
IMPLEMENT_PLUG_INFO_INDEX(_out5)
IMPLEMENT_PLUG_INFO_INDEX(_out6)
IMPLEMENT_PLUG_INFO_INDEX(_out7)
IMPLEMENT_PLUG_INFO_INDEX(_out8)
IMPLEMENT_PLUG_INFO_INDEX(_out_time1)
IMPLEMENT_PLUG_INFO_INDEX(_out_time2)
IMPLEMENT_PLUG_INFO_INDEX(_out_time3)
IMPLEMENT_PLUG_INFO_INDEX(_out_time4)
IMPLEMENT_PLUG_INFO_INDEX(_out_time5)
IMPLEMENT_PLUG_INFO_INDEX(_out_time6)
IMPLEMENT_PLUG_INFO_INDEX(_out_time7)
IMPLEMENT_PLUG_INFO_INDEX(_out_time8)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSequencer)

	PLUG_INIT(_go,"Go")
		.affects(PLUG_INFO_INDEX(_out1))
		.affects(PLUG_INFO_INDEX(_out2))
		.affects(PLUG_INFO_INDEX(_out3))
		.affects(PLUG_INFO_INDEX(_out4))
		.affects(PLUG_INFO_INDEX(_out5))
		.affects(PLUG_INFO_INDEX(_out6))
		.affects(PLUG_INFO_INDEX(_out7))
		.affects(PLUG_INFO_INDEX(_out8))
		.set_input(true);
		
	PLUG_INIT(_repeat,"Repeat")
		.affects(PLUG_INFO_INDEX(_out1))
		.affects(PLUG_INFO_INDEX(_out2))
		.affects(PLUG_INFO_INDEX(_out3))
		.affects(PLUG_INFO_INDEX(_out4))
		.affects(PLUG_INFO_INDEX(_out5))
		.affects(PLUG_INFO_INDEX(_out6))
		.affects(PLUG_INFO_INDEX(_out7))
		.affects(PLUG_INFO_INDEX(_out8))
		.set_input(true);
		
	PLUG_INIT(_out_time1,"Out_Time_1")
		.affects(PLUG_INFO_INDEX(_out1))
		.affects(PLUG_INFO_INDEX(_out2))
		.affects(PLUG_INFO_INDEX(_out3))
		.affects(PLUG_INFO_INDEX(_out4))
		.affects(PLUG_INFO_INDEX(_out5))
		.affects(PLUG_INFO_INDEX(_out6))
		.affects(PLUG_INFO_INDEX(_out7))
		.affects(PLUG_INFO_INDEX(_out8))
		.set_input(true);

	PLUG_INIT(_out_time2,"Out_Time_2")
		.affects(PLUG_INFO_INDEX(_out1))
		.affects(PLUG_INFO_INDEX(_out2))
		.affects(PLUG_INFO_INDEX(_out3))
		.affects(PLUG_INFO_INDEX(_out4))
		.affects(PLUG_INFO_INDEX(_out5))
		.affects(PLUG_INFO_INDEX(_out6))
		.affects(PLUG_INFO_INDEX(_out7))
		.affects(PLUG_INFO_INDEX(_out8))
		.set_input(true);

	PLUG_INIT(_out_time3,"Out_Time_3")
		.affects(PLUG_INFO_INDEX(_out1))
		.affects(PLUG_INFO_INDEX(_out2))
		.affects(PLUG_INFO_INDEX(_out3))
		.affects(PLUG_INFO_INDEX(_out4))
		.affects(PLUG_INFO_INDEX(_out5))
		.affects(PLUG_INFO_INDEX(_out6))
		.affects(PLUG_INFO_INDEX(_out7))
		.affects(PLUG_INFO_INDEX(_out8))
		.set_input(true);

	PLUG_INIT(_out_time4,"Out_Time_4")
		.affects(PLUG_INFO_INDEX(_out1))
		.affects(PLUG_INFO_INDEX(_out2))
		.affects(PLUG_INFO_INDEX(_out3))
		.affects(PLUG_INFO_INDEX(_out4))
		.affects(PLUG_INFO_INDEX(_out5))
		.affects(PLUG_INFO_INDEX(_out6))
		.affects(PLUG_INFO_INDEX(_out7))
		.affects(PLUG_INFO_INDEX(_out8))
		.set_input(true);

	PLUG_INIT(_out_time5,"Out_Time_5")
		.affects(PLUG_INFO_INDEX(_out1))
		.affects(PLUG_INFO_INDEX(_out2))
		.affects(PLUG_INFO_INDEX(_out3))
		.affects(PLUG_INFO_INDEX(_out4))
		.affects(PLUG_INFO_INDEX(_out5))
		.affects(PLUG_INFO_INDEX(_out6))
		.affects(PLUG_INFO_INDEX(_out7))
		.affects(PLUG_INFO_INDEX(_out8))
		.set_input(true);

	PLUG_INIT(_out_time6,"Out_Time_6")
		.affects(PLUG_INFO_INDEX(_out1))
		.affects(PLUG_INFO_INDEX(_out2))
		.affects(PLUG_INFO_INDEX(_out3))
		.affects(PLUG_INFO_INDEX(_out4))
		.affects(PLUG_INFO_INDEX(_out5))
		.affects(PLUG_INFO_INDEX(_out6))
		.affects(PLUG_INFO_INDEX(_out7))
		.affects(PLUG_INFO_INDEX(_out8))
		.set_input(true);

	PLUG_INIT(_out_time7,"Out_Time_7")
		.affects(PLUG_INFO_INDEX(_out1))
		.affects(PLUG_INFO_INDEX(_out2))
		.affects(PLUG_INFO_INDEX(_out3))
		.affects(PLUG_INFO_INDEX(_out4))
		.affects(PLUG_INFO_INDEX(_out5))
		.affects(PLUG_INFO_INDEX(_out6))
		.affects(PLUG_INFO_INDEX(_out7))
		.affects(PLUG_INFO_INDEX(_out8))
		.set_input(true);

	PLUG_INIT(_out_time8,"Out_Time_8")
		.affects(PLUG_INFO_INDEX(_out1))
		.affects(PLUG_INFO_INDEX(_out2))
		.affects(PLUG_INFO_INDEX(_out3))
		.affects(PLUG_INFO_INDEX(_out4))
		.affects(PLUG_INFO_INDEX(_out5))
		.affects(PLUG_INFO_INDEX(_out6))
		.affects(PLUG_INFO_INDEX(_out7))
		.affects(PLUG_INFO_INDEX(_out8))
		.set_input(true);

	PLUG_INIT(_out1,"Out_1")
		.set_output(true);

	PLUG_INIT(_out2,"Out_2")
		.set_output(true);

	PLUG_INIT(_out3,"Out_3")
		.set_output(true);

	PLUG_INIT(_out4,"Out_4")
		.set_output(true);

	PLUG_INIT(_out5,"Out_5")
		.set_output(true);

	PLUG_INIT(_out6,"Out_6")
		.set_output(true);

	PLUG_INIT(_out7,"Out_7")
		.set_output(true);

	PLUG_INIT(_out8,"Out_8")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingSequencer::ScriptingSequencer (void)
    :   _go				(PLUG_INFO_INDEX(_go), false),
		_repeat			(PLUG_INFO_INDEX(_repeat), false),
		_out1			(PLUG_INFO_INDEX(_out1), false),
		_out2			(PLUG_INFO_INDEX(_out2), false),
		_out3			(PLUG_INFO_INDEX(_out3), false),
		_out4			(PLUG_INFO_INDEX(_out4), false),
		_out5			(PLUG_INFO_INDEX(_out5), false),
		_out6			(PLUG_INFO_INDEX(_out6), false),
		_out7			(PLUG_INFO_INDEX(_out7), false),
		_out8			(PLUG_INFO_INDEX(_out8), false),
		_out_time1		(PLUG_INFO_INDEX(_out_time1), 1.0F),
		_out_time2		(PLUG_INFO_INDEX(_out_time2), 2.0F),
		_out_time3		(PLUG_INFO_INDEX(_out_time3), 3.0F),
		_out_time4		(PLUG_INFO_INDEX(_out_time4), 4.0F),
		_out_time5		(PLUG_INFO_INDEX(_out_time5), 5.0F),
		_out_time6		(PLUG_INFO_INDEX(_out_time6), 6.0F),
		_out_time7		(PLUG_INFO_INDEX(_out_time7), 7.0F),
		_out_time8		(PLUG_INFO_INDEX(_out_time8), 8.0F),
		_time			(0.0F)
{  

}
		
ScriptingSequencer::ScriptingSequencer (const ScriptingSequencer &rhs)
    :   ScriptingBase	(rhs),
		_go				(rhs._go),
		_repeat			(rhs._repeat),
		_out1			(rhs._out1),
		_out2			(rhs._out2),
		_out3			(rhs._out3),
		_out4			(rhs._out4),
		_out5			(rhs._out5),
		_out6			(rhs._out6),
		_out7			(rhs._out7),
		_out8			(rhs._out8),
		_out_time1		(rhs._out_time1),
		_out_time2		(rhs._out_time2),
		_out_time3		(rhs._out_time3),
		_out_time4		(rhs._out_time4),
		_out_time5		(rhs._out_time5),
		_out_time6		(rhs._out_time6),
		_out_time7		(rhs._out_time7),
		_out_time8		(rhs._out_time8),
		_time			(rhs._time)
{   

}

ScriptingSequencer & ScriptingSequencer::operator = (const ScriptingSequencer &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_go = rhs._go;
		_repeat = rhs._repeat;
		
		_out1 = rhs._out1;
		_out2 = rhs._out2;
		_out3 = rhs._out3;
		_out4 = rhs._out4;
		_out5 = rhs._out5;
		_out6 = rhs._out6;
		_out7 = rhs._out7;
		_out8 = rhs._out8;
		
		_out_time1 = rhs._out_time1;
		_out_time2 = rhs._out_time2;
		_out_time3 = rhs._out_time3;
		_out_time4 = rhs._out_time4;
		_out_time5 = rhs._out_time5;
		_out_time6 = rhs._out_time6;
		_out_time7 = rhs._out_time7;
		_out_time8 = rhs._out_time8;
		
		_time = rhs._time;
	}
    return (*this);
}
			
ScriptingSequencer::~ScriptingSequencer (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSequencer::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingSequencer::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());

	*archive << ARCHIVE_PLUG(_out_time1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out_time2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out_time3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out_time4, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out_time5, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out_time6, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out_time7, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out_time8, DATA_PERSISTENT | DATA_SETTABLE);
	
	*archive << ARCHIVE_PLUG(_out1, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out2, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out3, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out4, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out5, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out6, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out7, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out8, DATA_PERSISTENT);

	*archive << ARCHIVE_PLUG(_go, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingSequencer::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    if (_go) {
        _time += dt;
        
        if ((_repeat) && _time >= (_out_time8)) {
            _time -= (_out_time8);
        }
                
        if (_time >= 0.0F && _time < (_out_time1))				_out1 = true;
        else													_out1 = false;

        if (_time >= (_out_time1) && _time < (_out_time2))	_out2 = true;
        else													_out2 = false;

        if (_time >= (_out_time2) && _time < (_out_time3))	_out3 = true;
        else													_out3 = false;

        if (_time >= (_out_time3) && _time < (_out_time4))	_out4 = true;
        else													_out4 = false;

        if (_time >= (_out_time4) && _time < (_out_time5))	_out5 = true;
        else													_out5 = false;

        if (_time >= (_out_time5) && _time < (_out_time6))	_out6 = true;
        else													_out6 = false;

        if (_time >= (_out_time6) && _time < (_out_time7))	_out7 = true;
        else													_out7 = false;

        if (_time >= (_out_time7) && _time < (_out_time8))	_out8 = true;
        else													_out8 = false;
        

    } else {
        _time = 0.0F;
    }
}

//==============================================================================
//==============================================================================

void ScriptingSequencer::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingSequencer::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3

