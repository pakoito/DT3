//==============================================================================
///	
///	File: ScriptingRadioButton.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingRadioButton.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingRadioButton,"Controls",NULL)
IMPLEMENT_PLUG_NODE(ScriptingRadioButton)

IMPLEMENT_EVENT_INFO_INDEX(_in1)
IMPLEMENT_EVENT_INFO_INDEX(_in2)
IMPLEMENT_EVENT_INFO_INDEX(_in3)
IMPLEMENT_EVENT_INFO_INDEX(_in4)
IMPLEMENT_EVENT_INFO_INDEX(_in5)
IMPLEMENT_EVENT_INFO_INDEX(_in6)
IMPLEMENT_EVENT_INFO_INDEX(_in7)
IMPLEMENT_EVENT_INFO_INDEX(_in8)
IMPLEMENT_EVENT_INFO_INDEX(_in9)
IMPLEMENT_EVENT_INFO_INDEX(_in10)
IMPLEMENT_EVENT_INFO_INDEX(_in11)
IMPLEMENT_EVENT_INFO_INDEX(_in12)
IMPLEMENT_EVENT_INFO_INDEX(_in13)
IMPLEMENT_EVENT_INFO_INDEX(_in14)
IMPLEMENT_EVENT_INFO_INDEX(_in15)
IMPLEMENT_EVENT_INFO_INDEX(_in16)
IMPLEMENT_EVENT_INFO_INDEX(_in17)
IMPLEMENT_EVENT_INFO_INDEX(_in18)
IMPLEMENT_EVENT_INFO_INDEX(_in19)
IMPLEMENT_EVENT_INFO_INDEX(_in20)

IMPLEMENT_EVENT_INFO_INDEX(_set1)
IMPLEMENT_EVENT_INFO_INDEX(_reset1)
IMPLEMENT_EVENT_INFO_INDEX(_set2)
IMPLEMENT_EVENT_INFO_INDEX(_reset2)
IMPLEMENT_EVENT_INFO_INDEX(_set3)
IMPLEMENT_EVENT_INFO_INDEX(_reset3)
IMPLEMENT_EVENT_INFO_INDEX(_set4)
IMPLEMENT_EVENT_INFO_INDEX(_reset4)
IMPLEMENT_EVENT_INFO_INDEX(_set5)
IMPLEMENT_EVENT_INFO_INDEX(_reset5)
IMPLEMENT_EVENT_INFO_INDEX(_set6)
IMPLEMENT_EVENT_INFO_INDEX(_reset6)
IMPLEMENT_EVENT_INFO_INDEX(_set7)
IMPLEMENT_EVENT_INFO_INDEX(_reset7)
IMPLEMENT_EVENT_INFO_INDEX(_set8)
IMPLEMENT_EVENT_INFO_INDEX(_reset8)
IMPLEMENT_EVENT_INFO_INDEX(_set9)
IMPLEMENT_EVENT_INFO_INDEX(_reset9)
IMPLEMENT_EVENT_INFO_INDEX(_set10)
IMPLEMENT_EVENT_INFO_INDEX(_reset10)
IMPLEMENT_EVENT_INFO_INDEX(_set11)
IMPLEMENT_EVENT_INFO_INDEX(_reset11)
IMPLEMENT_EVENT_INFO_INDEX(_set12)
IMPLEMENT_EVENT_INFO_INDEX(_reset12)
IMPLEMENT_EVENT_INFO_INDEX(_set13)
IMPLEMENT_EVENT_INFO_INDEX(_reset13)
IMPLEMENT_EVENT_INFO_INDEX(_set14)
IMPLEMENT_EVENT_INFO_INDEX(_reset14)
IMPLEMENT_EVENT_INFO_INDEX(_set15)
IMPLEMENT_EVENT_INFO_INDEX(_reset15)
IMPLEMENT_EVENT_INFO_INDEX(_set16)
IMPLEMENT_EVENT_INFO_INDEX(_reset16)
IMPLEMENT_EVENT_INFO_INDEX(_set17)
IMPLEMENT_EVENT_INFO_INDEX(_reset17)
IMPLEMENT_EVENT_INFO_INDEX(_set18)
IMPLEMENT_EVENT_INFO_INDEX(_reset18)
IMPLEMENT_EVENT_INFO_INDEX(_set19)
IMPLEMENT_EVENT_INFO_INDEX(_reset19)
IMPLEMENT_EVENT_INFO_INDEX(_set20)
IMPLEMENT_EVENT_INFO_INDEX(_reset20)


//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingRadioButton)

	EVENT_INIT(_in1,"In1")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in1);
    
	EVENT_INIT(_in2,"In2")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in2);

	EVENT_INIT(_in3,"In3")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in3);

	EVENT_INIT(_in4,"In4")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in4);

	EVENT_INIT(_in5,"In5")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in5);

	EVENT_INIT(_in6,"In6")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in6);

	EVENT_INIT(_in7,"In7")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in7);

	EVENT_INIT(_in8,"In8")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in8);

	EVENT_INIT(_in9,"In9")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in9);

	EVENT_INIT(_in10,"In10")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in10);

	EVENT_INIT(_in11,"In11")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in11);

	EVENT_INIT(_in12,"In12")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in12);

	EVENT_INIT(_in13,"In13")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in13);

	EVENT_INIT(_in14,"In14")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in14);

	EVENT_INIT(_in15,"In15")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in15);

	EVENT_INIT(_in16,"In16")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in16);

	EVENT_INIT(_in17,"In17")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in17);

	EVENT_INIT(_in18,"In18")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in18);

	EVENT_INIT(_in19,"In19")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in19);

	EVENT_INIT(_in20,"In20")
        .set_input(true)
        .set_event(&ScriptingRadioButton::in20);
    
    
	EVENT_INIT(_set1,"Set1")
        .set_output(true);
    
	EVENT_INIT(_reset1,"Reset1")
        .set_output(true);

	EVENT_INIT(_set2,"Set2")
        .set_output(true);
    
	EVENT_INIT(_reset2,"Reset2")
        .set_output(true);

	EVENT_INIT(_set3,"Set3")
        .set_output(true);
    
	EVENT_INIT(_reset3,"Reset3")
        .set_output(true);

	EVENT_INIT(_set4,"Set4")
        .set_output(true);
    
	EVENT_INIT(_reset4,"Reset4")
        .set_output(true);

	EVENT_INIT(_set5,"Set5")
        .set_output(true);
    
	EVENT_INIT(_reset5,"Reset5")
        .set_output(true);

	EVENT_INIT(_set6,"Set6")
        .set_output(true);
    
	EVENT_INIT(_reset6,"Reset6")
        .set_output(true);

	EVENT_INIT(_set7,"Set7")
        .set_output(true);
    
	EVENT_INIT(_reset7,"Reset7")
        .set_output(true);

	EVENT_INIT(_set8,"Set8")
        .set_output(true);
    
	EVENT_INIT(_reset8,"Reset8")
        .set_output(true);

	EVENT_INIT(_set9,"Set9")
        .set_output(true);
    
	EVENT_INIT(_reset9,"Reset9")
        .set_output(true);

	EVENT_INIT(_set10,"Set10")
        .set_output(true);
    
	EVENT_INIT(_reset10,"Reset10")
        .set_output(true);

	EVENT_INIT(_set11,"Set11")
        .set_output(true);
    
	EVENT_INIT(_reset11,"Reset11")
        .set_output(true);

	EVENT_INIT(_set12,"Set12")
        .set_output(true);
    
	EVENT_INIT(_reset12,"Reset12")
        .set_output(true);

	EVENT_INIT(_set13,"Set13")
        .set_output(true);
    
	EVENT_INIT(_reset13,"Reset13")
        .set_output(true);

	EVENT_INIT(_set14,"Set14")
        .set_output(true);
    
	EVENT_INIT(_reset14,"Reset14")
        .set_output(true);

	EVENT_INIT(_set15,"Set15")
        .set_output(true);
    
	EVENT_INIT(_reset15,"Reset15")
        .set_output(true);

	EVENT_INIT(_set16,"Set16")
        .set_output(true);
    
	EVENT_INIT(_reset16,"Reset16")
        .set_output(true);

	EVENT_INIT(_set17,"Set17")
        .set_output(true);
    
	EVENT_INIT(_reset17,"Reset17")
        .set_output(true);

	EVENT_INIT(_set18,"Set18")
        .set_output(true);
    
	EVENT_INIT(_reset18,"Reset18")
        .set_output(true);

	EVENT_INIT(_set19,"Set19")
        .set_output(true);
    
	EVENT_INIT(_reset19,"Reset19")
        .set_output(true);

	EVENT_INIT(_set20,"Set20")
        .set_output(true);
    
	EVENT_INIT(_reset20,"Reset20")
        .set_output(true);

    
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingRadioButton::ScriptingRadioButton (void)
    :   _in1    (EVENT_INFO_INDEX(_in1)),
        _in2    (EVENT_INFO_INDEX(_in2)),
        _in3    (EVENT_INFO_INDEX(_in3)),
        _in4    (EVENT_INFO_INDEX(_in4)),
        _in5    (EVENT_INFO_INDEX(_in5)),
        _in6    (EVENT_INFO_INDEX(_in6)),
        _in7    (EVENT_INFO_INDEX(_in7)),
        _in8    (EVENT_INFO_INDEX(_in8)),
        _in9    (EVENT_INFO_INDEX(_in9)),
        _in10   (EVENT_INFO_INDEX(_in10)),
        _in11   (EVENT_INFO_INDEX(_in11)),
        _in12   (EVENT_INFO_INDEX(_in12)),
        _in13   (EVENT_INFO_INDEX(_in13)),
        _in14   (EVENT_INFO_INDEX(_in14)),
        _in15   (EVENT_INFO_INDEX(_in15)),
        _in16   (EVENT_INFO_INDEX(_in16)),
        _in17   (EVENT_INFO_INDEX(_in17)),
        _in18   (EVENT_INFO_INDEX(_in18)),
        _in19   (EVENT_INFO_INDEX(_in19)),
        _in20   (EVENT_INFO_INDEX(_in20)),
        _set1   (EVENT_INFO_INDEX(_set1)),
        _reset1 (EVENT_INFO_INDEX(_reset1)),
        _set2   (EVENT_INFO_INDEX(_set2)),
        _reset2 (EVENT_INFO_INDEX(_reset2)),
        _set3   (EVENT_INFO_INDEX(_set3)),
        _reset3 (EVENT_INFO_INDEX(_reset3)),
        _set4   (EVENT_INFO_INDEX(_set4)),
        _reset4 (EVENT_INFO_INDEX(_reset4)),
        _set5   (EVENT_INFO_INDEX(_set5)),
        _reset5 (EVENT_INFO_INDEX(_reset5)),
        _set6   (EVENT_INFO_INDEX(_set6)),
        _reset6 (EVENT_INFO_INDEX(_reset6)),
        _set7   (EVENT_INFO_INDEX(_set7)),
        _reset7 (EVENT_INFO_INDEX(_reset7)),
        _set8   (EVENT_INFO_INDEX(_set8)),
        _reset8 (EVENT_INFO_INDEX(_reset8)),
        _set9   (EVENT_INFO_INDEX(_set9)),
        _reset9 (EVENT_INFO_INDEX(_reset9)),
        _set10   (EVENT_INFO_INDEX(_set10)),
        _reset10 (EVENT_INFO_INDEX(_reset10)),
        _set11   (EVENT_INFO_INDEX(_set11)),
        _reset11 (EVENT_INFO_INDEX(_reset11)),
        _set12   (EVENT_INFO_INDEX(_set12)),
        _reset12 (EVENT_INFO_INDEX(_reset12)),
        _set13   (EVENT_INFO_INDEX(_set13)),
        _reset13 (EVENT_INFO_INDEX(_reset13)),
        _set14   (EVENT_INFO_INDEX(_set14)),
        _reset14 (EVENT_INFO_INDEX(_reset14)),
        _set15   (EVENT_INFO_INDEX(_set15)),
        _reset15 (EVENT_INFO_INDEX(_reset15)),
        _set16   (EVENT_INFO_INDEX(_set16)),
        _reset16 (EVENT_INFO_INDEX(_reset16)),
        _set17   (EVENT_INFO_INDEX(_set17)),
        _reset17 (EVENT_INFO_INDEX(_reset17)),
        _set18   (EVENT_INFO_INDEX(_set18)),
        _reset18 (EVENT_INFO_INDEX(_reset18)),
        _set19   (EVENT_INFO_INDEX(_set19)),
        _reset19 (EVENT_INFO_INDEX(_reset19)),
        _set20   (EVENT_INFO_INDEX(_set20)),
        _reset20 (EVENT_INFO_INDEX(_reset20))
{

}
		
ScriptingRadioButton::ScriptingRadioButton (const ScriptingRadioButton &rhs)
    :   ScriptingBase	(rhs),
        _in1    (EVENT_INFO_INDEX(_in1)),
        _in2    (EVENT_INFO_INDEX(_in2)),
        _in3    (EVENT_INFO_INDEX(_in3)),
        _in4    (EVENT_INFO_INDEX(_in4)),
        _in5    (EVENT_INFO_INDEX(_in5)),
        _in6    (EVENT_INFO_INDEX(_in6)),
        _in7    (EVENT_INFO_INDEX(_in7)),
        _in8    (EVENT_INFO_INDEX(_in8)),
        _in9    (EVENT_INFO_INDEX(_in9)),
        _in10   (EVENT_INFO_INDEX(_in10)),
        _in11   (EVENT_INFO_INDEX(_in11)),
        _in12   (EVENT_INFO_INDEX(_in12)),
        _in13   (EVENT_INFO_INDEX(_in13)),
        _in14   (EVENT_INFO_INDEX(_in14)),
        _in15   (EVENT_INFO_INDEX(_in15)),
        _in16   (EVENT_INFO_INDEX(_in16)),
        _in17   (EVENT_INFO_INDEX(_in17)),
        _in18   (EVENT_INFO_INDEX(_in18)),
        _in19   (EVENT_INFO_INDEX(_in19)),
        _in20   (EVENT_INFO_INDEX(_in20)),
        _set1   (EVENT_INFO_INDEX(_set1)),
        _reset1 (EVENT_INFO_INDEX(_reset1)),
        _set2   (EVENT_INFO_INDEX(_set2)),
        _reset2 (EVENT_INFO_INDEX(_reset2)),
        _set3   (EVENT_INFO_INDEX(_set3)),
        _reset3 (EVENT_INFO_INDEX(_reset3)),
        _set4   (EVENT_INFO_INDEX(_set4)),
        _reset4 (EVENT_INFO_INDEX(_reset4)),
        _set5   (EVENT_INFO_INDEX(_set5)),
        _reset5 (EVENT_INFO_INDEX(_reset5)),
        _set6   (EVENT_INFO_INDEX(_set6)),
        _reset6 (EVENT_INFO_INDEX(_reset6)),
        _set7   (EVENT_INFO_INDEX(_set7)),
        _reset7 (EVENT_INFO_INDEX(_reset7)),
        _set8   (EVENT_INFO_INDEX(_set8)),
        _reset8 (EVENT_INFO_INDEX(_reset8)),
        _set9   (EVENT_INFO_INDEX(_set9)),
        _reset9 (EVENT_INFO_INDEX(_reset9)),
        _set10   (EVENT_INFO_INDEX(_set10)),
        _reset10 (EVENT_INFO_INDEX(_reset10)),
        _set11   (EVENT_INFO_INDEX(_set11)),
        _reset11 (EVENT_INFO_INDEX(_reset11)),
        _set12   (EVENT_INFO_INDEX(_set12)),
        _reset12 (EVENT_INFO_INDEX(_reset12)),
        _set13   (EVENT_INFO_INDEX(_set13)),
        _reset13 (EVENT_INFO_INDEX(_reset13)),
        _set14   (EVENT_INFO_INDEX(_set14)),
        _reset14 (EVENT_INFO_INDEX(_reset14)),
        _set15   (EVENT_INFO_INDEX(_set15)),
        _reset15 (EVENT_INFO_INDEX(_reset15)),
        _set16   (EVENT_INFO_INDEX(_set16)),
        _reset16 (EVENT_INFO_INDEX(_reset16)),
        _set17   (EVENT_INFO_INDEX(_set17)),
        _reset17 (EVENT_INFO_INDEX(_reset17)),
        _set18   (EVENT_INFO_INDEX(_set18)),
        _reset18 (EVENT_INFO_INDEX(_reset18)),
        _set19   (EVENT_INFO_INDEX(_set19)),
        _reset19 (EVENT_INFO_INDEX(_reset19)),
        _set20   (EVENT_INFO_INDEX(_set20)),
        _reset20 (EVENT_INFO_INDEX(_reset20))
{

}

ScriptingRadioButton & ScriptingRadioButton::operator = (const ScriptingRadioButton &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);
	}
    return (*this);
}
			
ScriptingRadioButton::~ScriptingRadioButton (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingRadioButton::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingRadioButton::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingRadioButton::flip (PlugNode *sender, Event *in)
{
	PROFILER(SCRIPTING);

    if (in == &_in1)    _set1.send(sender);
    else                _reset1.send(sender);

    if (in == &_in2)    _set2.send(sender);
    else                _reset2.send(sender);

    if (in == &_in3)    _set3.send(sender);
    else                _reset3.send(sender);

    if (in == &_in4)    _set4.send(sender);
    else                _reset4.send(sender);

    if (in == &_in5)    _set5.send(sender);
    else                _reset5.send(sender);

    if (in == &_in6)    _set6.send(sender);
    else                _reset6.send(sender);

    if (in == &_in7)    _set7.send(sender);
    else                _reset7.send(sender);

    if (in == &_in8)    _set8.send(sender);
    else                _reset8.send(sender);

    if (in == &_in9)    _set9.send(sender);
    else                _reset9.send(sender);

    if (in == &_in10)   _set10.send(sender);
    else                _reset10.send(sender);

    if (in == &_in11)   _set11.send(sender);
    else                _reset11.send(sender);

    if (in == &_in12)   _set12.send(sender);
    else                _reset12.send(sender);

    if (in == &_in13)   _set13.send(sender);
    else                _reset13.send(sender);

    if (in == &_in14)   _set14.send(sender);
    else                _reset14.send(sender);

    if (in == &_in15)   _set15.send(sender);
    else                _reset15.send(sender);

    if (in == &_in16)   _set16.send(sender);
    else                _reset16.send(sender);

    if (in == &_in17)   _set17.send(sender);
    else                _reset17.send(sender);

    if (in == &_in18)   _set18.send(sender);
    else                _reset18.send(sender);

    if (in == &_in19)   _set19.send(sender);
    else                _reset19.send(sender);

    if (in == &_in20)   _set20.send(sender);
    else                _reset20.send(sender);
}

//==============================================================================
//==============================================================================

void ScriptingRadioButton::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
}

void ScriptingRadioButton::remove_from_world (void)
{
    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3

