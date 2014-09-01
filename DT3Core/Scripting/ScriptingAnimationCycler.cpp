//==============================================================================
///	
///	File: ScriptingAnimationCycler.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingAnimationCycler.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingAnimationCycler,"Animation",EdManipScriptingAnimationCycler)
IMPLEMENT_PLUG_NODE(ScriptingAnimationCycler)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)
IMPLEMENT_PLUG_INFO_INDEX(_min_time)
IMPLEMENT_PLUG_INFO_INDEX(_max_time)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingAnimationCycler)
      
	PLUG_INIT(_in,"In_Time")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out_Time")
		.set_output(true);

	PLUG_INIT(_min_time,"Cycle_Start_Time")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_max_time,"Cycle_End_Time")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingAnimationCycler::ScriptingAnimationCycler (void)
	:	_in				(PLUG_INFO_INDEX(_in), 0.0F),
        _out            (PLUG_INFO_INDEX(_out), 0.0F),
        _min_time       (PLUG_INFO_INDEX(_min_time), 0.0F),
        _max_time       (PLUG_INFO_INDEX(_max_time), 0.0F)
{  

}
		
ScriptingAnimationCycler::ScriptingAnimationCycler (const ScriptingAnimationCycler &rhs)
    :   ScriptingBase   (rhs),
        _in				(PLUG_INFO_INDEX(_in), 0.0F),
        _out            (PLUG_INFO_INDEX(_out), 0.0F),
        _min_time       (PLUG_INFO_INDEX(_min_time), 0.0F),
        _max_time       (PLUG_INFO_INDEX(_max_time), 0.0F)
{   

}

ScriptingAnimationCycler & ScriptingAnimationCycler::operator = (const ScriptingAnimationCycler &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);	

        _in = rhs._in;
        _out = rhs._out;
        _min_time = rhs._min_time;
        _max_time = rhs._max_time;
    }
    return (*this);
}
			
ScriptingAnimationCycler::~ScriptingAnimationCycler (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingAnimationCycler::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingAnimationCycler::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());

	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_min_time, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_max_time, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingAnimationCycler::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);
    
    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
        
        if (_in <= _min_time) {
            _out = _in;
        } else {
            DTfloat t = _in - _min_time;
            DTfloat diff = _max_time - _min_time;
            
            t = t / diff;
            t = t - std::floor(t);
            t = t * diff;
            
            _out = t + _min_time;
        }
        
		_out.set_clean();
		return true;
	}
	
	return false;

}
//==============================================================================
//==============================================================================

} // DT3

