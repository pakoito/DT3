//==============================================================================
///	
///	File: ScriptingSoundRandomizer.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSoundRandomizer.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSoundRandomizer,"Sound",NULL)
IMPLEMENT_PLUG_NODE(ScriptingSoundRandomizer)

IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_in_1)
IMPLEMENT_PLUG_INFO_INDEX(_in_weight_1)
IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_in_2)
IMPLEMENT_PLUG_INFO_INDEX(_in_weight_2)
IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_in_3)
IMPLEMENT_PLUG_INFO_INDEX(_in_weight_3)
IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_in_4)
IMPLEMENT_PLUG_INFO_INDEX(_in_weight_4)
IMPLEMENT_PLUG_INFO_INDEX(_sound_packet_out)
IMPLEMENT_EVENT_INFO_INDEX(_randomize_inputs)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSoundRandomizer)

	PLUG_INIT(_sound_packet_in_1,"Sound_Packet_In_1")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true)
        .set_always_dirty(true);

	PLUG_INIT(_in_weight_1,"In_Weight_1")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true);

	PLUG_INIT(_sound_packet_in_2,"Sound_Packet_In_2")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true)
        .set_always_dirty(true);

	PLUG_INIT(_in_weight_2,"In_Weight_2")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true);

	PLUG_INIT(_sound_packet_in_3,"Sound_Packet_In_3")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true)
        .set_always_dirty(true);

	PLUG_INIT(_in_weight_3,"In_Weight_3")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true);

	PLUG_INIT(_sound_packet_in_4,"Sound_Packet_In_4")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true)
        .set_always_dirty(true);

	PLUG_INIT(_in_weight_4,"In_Weight_4")
        .affects(PLUG_INFO_INDEX(_sound_packet_out))
		.set_input(true);


	PLUG_INIT(_sound_packet_out,"Sound_Packet_Out")
        .set_single_output(true)
		.set_output(true)
        .set_always_dirty(true);
            
    EVENT_INIT(_randomize_inputs, "Randomize_Inputs")
        .set_input(true)
        .set_event(&ScriptingSoundRandomizer::randomizeInputs);
    
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingSoundRandomizer::ScriptingSoundRandomizer (void)
    :   _sound_packet_in_1  (PLUG_INFO_INDEX(_sound_packet_in_1)),
        _in_weight_1        (PLUG_INFO_INDEX(_in_weight_1), 1.0F),
        _sound_packet_in_2  (PLUG_INFO_INDEX(_sound_packet_in_2)),
        _in_weight_2        (PLUG_INFO_INDEX(_in_weight_2), 0.0F),
        _sound_packet_in_3  (PLUG_INFO_INDEX(_sound_packet_in_3)),
        _in_weight_3        (PLUG_INFO_INDEX(_in_weight_3), 0.0F),
        _sound_packet_in_4  (PLUG_INFO_INDEX(_sound_packet_in_4)),
        _in_weight_4        (PLUG_INFO_INDEX(_in_weight_4), 0.0F),
		_sound_packet_out   (PLUG_INFO_INDEX(_sound_packet_out)),
        _randomize_inputs   (EVENT_INFO_INDEX(_randomize_inputs)),
        _current_input      (0)
{  

}
		
ScriptingSoundRandomizer::ScriptingSoundRandomizer (const ScriptingSoundRandomizer &rhs)
    :   ScriptingSoundBase  (rhs),
		_sound_packet_in_1	(rhs._sound_packet_in_1),
		_in_weight_1        (rhs._in_weight_1),
		_sound_packet_in_2	(rhs._sound_packet_in_2),
		_in_weight_2        (rhs._in_weight_2),
		_sound_packet_in_3	(rhs._sound_packet_in_3),
		_in_weight_3        (rhs._in_weight_3),
		_sound_packet_in_4	(rhs._sound_packet_in_4),
		_in_weight_4        (rhs._in_weight_4),
		_sound_packet_out   (PLUG_INFO_INDEX(_sound_packet_out)),
        _randomize_inputs   (EVENT_INFO_INDEX(_randomize_inputs)),
        _current_input      (0)
{   

}	

ScriptingSoundRandomizer & ScriptingSoundRandomizer::operator = (const ScriptingSoundRandomizer &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingSoundBase::operator = (rhs);

		_sound_packet_in_1 = rhs._sound_packet_in_1;
		_in_weight_1 = rhs._in_weight_1;
		_sound_packet_in_2 = rhs._sound_packet_in_2;
		_in_weight_2 = rhs._in_weight_2;
		_sound_packet_in_3 = rhs._sound_packet_in_3;
		_in_weight_3 = rhs._in_weight_3;
		_sound_packet_in_4 = rhs._sound_packet_in_4;
		_in_weight_4 = rhs._in_weight_4;
        
        _sound_packet_out = rhs._sound_packet_out;
	}
    return (*this);
}
			
ScriptingSoundRandomizer::~ScriptingSoundRandomizer (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSoundRandomizer::initialize (void)
{
	ScriptingSoundBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingSoundRandomizer::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingSoundBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_PLUG(_in_weight_1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_weight_2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_weight_3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_weight_4, DATA_PERSISTENT | DATA_SETTABLE);
                                                                			
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingSoundRandomizer::randomizeInputs (PlugNode *sender)
{
    DTfloat total_weight = _in_weight_1 + _in_weight_2 + _in_weight_3 + _in_weight_4;
    
    _sound_packet_out.set_dirty();

    if (total_weight <= 0.0F)
        return;
        
    
    DTfloat r = MoreMath::random_float();
    
    // W1
    DTfloat w = _in_weight_1/total_weight;
    if (r <= w) {
        _current_input = 0;
        return;
    } 
    
    r -= w;

    // W2
    w = _in_weight_2/total_weight;
    if (r <= w) {
        _current_input = 1;
        return;
    } 
    
    r -= w;

    // W3
    w = _in_weight_3/total_weight;
    if (r <= w) {
        _current_input = 2;
        return;
    } 
    
    r -= w;

    // W4
    _current_input = 3;
    
}

//==============================================================================
//==============================================================================

DTboolean ScriptingSoundRandomizer::compute (const PlugBase *plug)
{
	PROFILER(SOUND);

    if (super_type::compute(plug))  return true;

	if (plug == &_sound_packet_out) {
    
        switch (_current_input) {
            case 0:     _sound_packet_out = _sound_packet_in_1.as_ref_no_compute();   break;
            case 1:     _sound_packet_out = _sound_packet_in_2.as_ref_no_compute();   break;
            case 2:     _sound_packet_out = _sound_packet_in_3.as_ref_no_compute();   break;
            default:    _sound_packet_out = _sound_packet_in_4.as_ref_no_compute();   break;
        }
    
		_sound_packet_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3

