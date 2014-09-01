//==============================================================================
///	
///	File: ScriptingCompute.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingCompute.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include <cmath>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingCompute,"Math",NULL)
IMPLEMENT_PLUG_NODE(ScriptingCompute)

IMPLEMENT_PLUG_INFO_INDEX(_in_a1)
IMPLEMENT_PLUG_INFO_INDEX(_in_a2)
IMPLEMENT_PLUG_INFO_INDEX(_in_a3)
IMPLEMENT_PLUG_INFO_INDEX(_in_a4)

IMPLEMENT_PLUG_INFO_INDEX(_in_b1)
IMPLEMENT_PLUG_INFO_INDEX(_in_b2)
IMPLEMENT_PLUG_INFO_INDEX(_in_b3)
IMPLEMENT_PLUG_INFO_INDEX(_in_b4)

IMPLEMENT_PLUG_INFO_INDEX(_in_c1)
IMPLEMENT_PLUG_INFO_INDEX(_in_c2)
IMPLEMENT_PLUG_INFO_INDEX(_in_c3)
IMPLEMENT_PLUG_INFO_INDEX(_in_c4)


IMPLEMENT_PLUG_INFO_INDEX(_out_a1)
IMPLEMENT_PLUG_INFO_INDEX(_out_a2)
IMPLEMENT_PLUG_INFO_INDEX(_out_a3)
IMPLEMENT_PLUG_INFO_INDEX(_out_a4)

IMPLEMENT_PLUG_INFO_INDEX(_out_b1)
IMPLEMENT_PLUG_INFO_INDEX(_out_b2)
IMPLEMENT_PLUG_INFO_INDEX(_out_b3)
IMPLEMENT_PLUG_INFO_INDEX(_out_b4)

IMPLEMENT_PLUG_INFO_INDEX(_out_c1)
IMPLEMENT_PLUG_INFO_INDEX(_out_c2)
IMPLEMENT_PLUG_INFO_INDEX(_out_c3)
IMPLEMENT_PLUG_INFO_INDEX(_out_c4)
		
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingCompute)

	PLUG_INIT(_in_a1,"A1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_a1))
		.affects(PLUG_INFO_INDEX(_out_a2))
		.affects(PLUG_INFO_INDEX(_out_a3))
		.affects(PLUG_INFO_INDEX(_out_a4))
		.affects(PLUG_INFO_INDEX(_out_b1))
		.affects(PLUG_INFO_INDEX(_out_b2))
		.affects(PLUG_INFO_INDEX(_out_b3))
		.affects(PLUG_INFO_INDEX(_out_b4))
		.affects(PLUG_INFO_INDEX(_out_c1))
		.affects(PLUG_INFO_INDEX(_out_c2))
		.affects(PLUG_INFO_INDEX(_out_c3))
		.affects(PLUG_INFO_INDEX(_out_c4));

	PLUG_INIT(_in_a2,"A2")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_a1))
		.affects(PLUG_INFO_INDEX(_out_a2))
		.affects(PLUG_INFO_INDEX(_out_a3))
		.affects(PLUG_INFO_INDEX(_out_a4))
		.affects(PLUG_INFO_INDEX(_out_b1))
		.affects(PLUG_INFO_INDEX(_out_b2))
		.affects(PLUG_INFO_INDEX(_out_b3))
		.affects(PLUG_INFO_INDEX(_out_b4))
		.affects(PLUG_INFO_INDEX(_out_c1))
		.affects(PLUG_INFO_INDEX(_out_c2))
		.affects(PLUG_INFO_INDEX(_out_c3))
		.affects(PLUG_INFO_INDEX(_out_c4));

	PLUG_INIT(_in_a3,"A3")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_a1))
		.affects(PLUG_INFO_INDEX(_out_a2))
		.affects(PLUG_INFO_INDEX(_out_a3))
		.affects(PLUG_INFO_INDEX(_out_a4))
		.affects(PLUG_INFO_INDEX(_out_b1))
		.affects(PLUG_INFO_INDEX(_out_b2))
		.affects(PLUG_INFO_INDEX(_out_b3))
		.affects(PLUG_INFO_INDEX(_out_b4))
		.affects(PLUG_INFO_INDEX(_out_c1))
		.affects(PLUG_INFO_INDEX(_out_c2))
		.affects(PLUG_INFO_INDEX(_out_c3))
		.affects(PLUG_INFO_INDEX(_out_c4));

	PLUG_INIT(_in_a4,"A4")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_a1))
		.affects(PLUG_INFO_INDEX(_out_a2))
		.affects(PLUG_INFO_INDEX(_out_a3))
		.affects(PLUG_INFO_INDEX(_out_a4))
		.affects(PLUG_INFO_INDEX(_out_b1))
		.affects(PLUG_INFO_INDEX(_out_b2))
		.affects(PLUG_INFO_INDEX(_out_b3))
		.affects(PLUG_INFO_INDEX(_out_b4))
		.affects(PLUG_INFO_INDEX(_out_c1))
		.affects(PLUG_INFO_INDEX(_out_c2))
		.affects(PLUG_INFO_INDEX(_out_c3))
		.affects(PLUG_INFO_INDEX(_out_c4));
        
        
	PLUG_INIT(_in_b1,"B1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_a1))
		.affects(PLUG_INFO_INDEX(_out_a2))
		.affects(PLUG_INFO_INDEX(_out_a3))
		.affects(PLUG_INFO_INDEX(_out_a4))
		.affects(PLUG_INFO_INDEX(_out_b1))
		.affects(PLUG_INFO_INDEX(_out_b2))
		.affects(PLUG_INFO_INDEX(_out_b3))
		.affects(PLUG_INFO_INDEX(_out_b4))
		.affects(PLUG_INFO_INDEX(_out_c1))
		.affects(PLUG_INFO_INDEX(_out_c2))
		.affects(PLUG_INFO_INDEX(_out_c3))
		.affects(PLUG_INFO_INDEX(_out_c4));

	PLUG_INIT(_in_b2,"B2")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_a1))
		.affects(PLUG_INFO_INDEX(_out_a2))
		.affects(PLUG_INFO_INDEX(_out_a3))
		.affects(PLUG_INFO_INDEX(_out_a4))
		.affects(PLUG_INFO_INDEX(_out_b1))
		.affects(PLUG_INFO_INDEX(_out_b2))
		.affects(PLUG_INFO_INDEX(_out_b3))
		.affects(PLUG_INFO_INDEX(_out_b4))
		.affects(PLUG_INFO_INDEX(_out_c1))
		.affects(PLUG_INFO_INDEX(_out_c2))
		.affects(PLUG_INFO_INDEX(_out_c3))
		.affects(PLUG_INFO_INDEX(_out_c4));

	PLUG_INIT(_in_b3,"B3")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_a1))
		.affects(PLUG_INFO_INDEX(_out_a2))
		.affects(PLUG_INFO_INDEX(_out_a3))
		.affects(PLUG_INFO_INDEX(_out_a4))
		.affects(PLUG_INFO_INDEX(_out_b1))
		.affects(PLUG_INFO_INDEX(_out_b2))
		.affects(PLUG_INFO_INDEX(_out_b3))
		.affects(PLUG_INFO_INDEX(_out_b4))
		.affects(PLUG_INFO_INDEX(_out_c1))
		.affects(PLUG_INFO_INDEX(_out_c2))
		.affects(PLUG_INFO_INDEX(_out_c3))
		.affects(PLUG_INFO_INDEX(_out_c4));

	PLUG_INIT(_in_b4,"B4")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_a1))
		.affects(PLUG_INFO_INDEX(_out_a2))
		.affects(PLUG_INFO_INDEX(_out_a3))
		.affects(PLUG_INFO_INDEX(_out_a4))
		.affects(PLUG_INFO_INDEX(_out_b1))
		.affects(PLUG_INFO_INDEX(_out_b2))
		.affects(PLUG_INFO_INDEX(_out_b3))
		.affects(PLUG_INFO_INDEX(_out_b4))
		.affects(PLUG_INFO_INDEX(_out_c1))
		.affects(PLUG_INFO_INDEX(_out_c2))
		.affects(PLUG_INFO_INDEX(_out_c3))
		.affects(PLUG_INFO_INDEX(_out_c4));


	PLUG_INIT(_in_c1,"C1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_a1))
		.affects(PLUG_INFO_INDEX(_out_a2))
		.affects(PLUG_INFO_INDEX(_out_a3))
		.affects(PLUG_INFO_INDEX(_out_a4))
		.affects(PLUG_INFO_INDEX(_out_b1))
		.affects(PLUG_INFO_INDEX(_out_b2))
		.affects(PLUG_INFO_INDEX(_out_b3))
		.affects(PLUG_INFO_INDEX(_out_b4))
		.affects(PLUG_INFO_INDEX(_out_c1))
		.affects(PLUG_INFO_INDEX(_out_c2))
		.affects(PLUG_INFO_INDEX(_out_c3))
		.affects(PLUG_INFO_INDEX(_out_c4));

	PLUG_INIT(_in_c2,"C2")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_a1))
		.affects(PLUG_INFO_INDEX(_out_a2))
		.affects(PLUG_INFO_INDEX(_out_a3))
		.affects(PLUG_INFO_INDEX(_out_a4))
		.affects(PLUG_INFO_INDEX(_out_b1))
		.affects(PLUG_INFO_INDEX(_out_b2))
		.affects(PLUG_INFO_INDEX(_out_b3))
		.affects(PLUG_INFO_INDEX(_out_b4))
		.affects(PLUG_INFO_INDEX(_out_c1))
		.affects(PLUG_INFO_INDEX(_out_c2))
		.affects(PLUG_INFO_INDEX(_out_c3))
		.affects(PLUG_INFO_INDEX(_out_c4));

	PLUG_INIT(_in_c3,"C3")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_a1))
		.affects(PLUG_INFO_INDEX(_out_a2))
		.affects(PLUG_INFO_INDEX(_out_a3))
		.affects(PLUG_INFO_INDEX(_out_a4))
		.affects(PLUG_INFO_INDEX(_out_b1))
		.affects(PLUG_INFO_INDEX(_out_b2))
		.affects(PLUG_INFO_INDEX(_out_b3))
		.affects(PLUG_INFO_INDEX(_out_b4))
		.affects(PLUG_INFO_INDEX(_out_c1))
		.affects(PLUG_INFO_INDEX(_out_c2))
		.affects(PLUG_INFO_INDEX(_out_c3))
		.affects(PLUG_INFO_INDEX(_out_c4));

	PLUG_INIT(_in_c4,"C4")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_a1))
		.affects(PLUG_INFO_INDEX(_out_a2))
		.affects(PLUG_INFO_INDEX(_out_a3))
		.affects(PLUG_INFO_INDEX(_out_a4))
		.affects(PLUG_INFO_INDEX(_out_b1))
		.affects(PLUG_INFO_INDEX(_out_b2))
		.affects(PLUG_INFO_INDEX(_out_b3))
		.affects(PLUG_INFO_INDEX(_out_b4))
		.affects(PLUG_INFO_INDEX(_out_c1))
		.affects(PLUG_INFO_INDEX(_out_c2))
		.affects(PLUG_INFO_INDEX(_out_c3))
		.affects(PLUG_INFO_INDEX(_out_c4));


	PLUG_INIT(_out_a1,"OA1")
		.set_output(true);

	PLUG_INIT(_out_a2,"OA2")
		.set_output(true);

	PLUG_INIT(_out_a3,"OA3")
		.set_output(true);

	PLUG_INIT(_out_a4,"OA4")
		.set_output(true);
        

	PLUG_INIT(_out_b1,"OB1")
		.set_output(true);

	PLUG_INIT(_out_b2,"OB2")
		.set_output(true);

	PLUG_INIT(_out_b3,"OB3")
		.set_output(true);

	PLUG_INIT(_out_b4,"OB4")
		.set_output(true);
        

	PLUG_INIT(_out_c1,"OC1")
		.set_output(true);

	PLUG_INIT(_out_c2,"OC2")
		.set_output(true);

	PLUG_INIT(_out_c3,"OC3")
		.set_output(true);

	PLUG_INIT(_out_c4,"OC4")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingCompute::ScriptingCompute (void)
    :   _in_a1		(PLUG_INFO_INDEX(_in_a1), 0.0F),
        _in_a2		(PLUG_INFO_INDEX(_in_a2), 0.0F),
        _in_a3		(PLUG_INFO_INDEX(_in_a3), 0.0F),
        _in_a4		(PLUG_INFO_INDEX(_in_a4), 0.0F),
        _in_b1		(PLUG_INFO_INDEX(_in_b1), 0.0F),
        _in_b2		(PLUG_INFO_INDEX(_in_b2), 0.0F),
        _in_b3		(PLUG_INFO_INDEX(_in_b3), 0.0F),
        _in_b4		(PLUG_INFO_INDEX(_in_b4), 0.0F),
        _in_c1		(PLUG_INFO_INDEX(_in_c1), 0.0F),
        _in_c2		(PLUG_INFO_INDEX(_in_c2), 0.0F),
        _in_c3		(PLUG_INFO_INDEX(_in_c3), 0.0F),
        _in_c4		(PLUG_INFO_INDEX(_in_c4), 0.0F),
        _out_a1		(PLUG_INFO_INDEX(_out_a1), 0.0F),
        _out_a2		(PLUG_INFO_INDEX(_out_a2), 0.0F),
        _out_a3		(PLUG_INFO_INDEX(_out_a3), 0.0F),
        _out_a4		(PLUG_INFO_INDEX(_out_a4), 0.0F),
        _out_b1		(PLUG_INFO_INDEX(_out_b1), 0.0F),
        _out_b2		(PLUG_INFO_INDEX(_out_b2), 0.0F),
        _out_b3		(PLUG_INFO_INDEX(_out_b3), 0.0F),
        _out_b4		(PLUG_INFO_INDEX(_out_b4), 0.0F),
        _out_c1		(PLUG_INFO_INDEX(_out_c1), 0.0F),
        _out_c2		(PLUG_INFO_INDEX(_out_c2), 0.0F),
        _out_c3		(PLUG_INFO_INDEX(_out_c3), 0.0F),
        _out_c4		(PLUG_INFO_INDEX(_out_c4), 0.0F)
{  

}
		
ScriptingCompute::ScriptingCompute (const ScriptingCompute &rhs)
    :   ScriptingBase	(rhs),
		_in_a1			(rhs._in_a1),
		_in_a2			(rhs._in_a2),
		_in_a3			(rhs._in_a3),
		_in_a4			(rhs._in_a4),
		_in_b1			(rhs._in_b1),
		_in_b2			(rhs._in_b2),
		_in_b3			(rhs._in_b3),
		_in_b4			(rhs._in_b4),
		_in_c1			(rhs._in_c1),
		_in_c2			(rhs._in_c2),
		_in_c3			(rhs._in_c3),
		_in_c4			(rhs._in_c4),
		_out_a1			(rhs._out_a1),
		_out_a2			(rhs._out_a2),
		_out_a3			(rhs._out_a3),
		_out_a4			(rhs._out_a4),
		_out_b1			(rhs._out_b1),
		_out_b2			(rhs._out_b2),
		_out_b3			(rhs._out_b3),
		_out_b4			(rhs._out_b4),
		_out_c1			(rhs._out_c1),
		_out_c2			(rhs._out_c2),
		_out_c3			(rhs._out_c3),
		_out_c4			(rhs._out_c4),
        _eq_a1          (rhs._eq_a1),
        _eq_a2          (rhs._eq_a2),
        _eq_a3          (rhs._eq_a3),
        _eq_a4          (rhs._eq_a4),
        _eq_b1          (rhs._eq_b1),
        _eq_b2          (rhs._eq_b2),
        _eq_b3          (rhs._eq_b3),
        _eq_b4          (rhs._eq_b4),
        _eq_c1          (rhs._eq_c1),
        _eq_c2          (rhs._eq_c2),
        _eq_c3          (rhs._eq_c3),
        _eq_c4          (rhs._eq_c4)
{   

}

ScriptingCompute & ScriptingCompute::operator = (const ScriptingCompute &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in_a1 = rhs._in_a1;
		_in_a2 = rhs._in_a2;
		_in_a3 = rhs._in_a3;
		_in_a4 = rhs._in_a4;
		_in_b1 = rhs._in_b1;
		_in_b2 = rhs._in_b2;
		_in_b3 = rhs._in_b3;
		_in_b4 = rhs._in_b4;
		_in_c1 = rhs._in_c1;
		_in_c2 = rhs._in_c2;
		_in_c3 = rhs._in_c3;
		_in_c4 = rhs._in_c4;
		_out_a1 = rhs._out_a1;
		_out_a2 = rhs._out_a2;
		_out_a3 = rhs._out_a3;
		_out_a4 = rhs._out_a4;
		_out_b1 = rhs._out_b1;
		_out_b2 = rhs._out_b2;
		_out_b3 = rhs._out_b3;
		_out_b4 = rhs._out_b4;
		_out_c1 = rhs._out_c1;
		_out_c2 = rhs._out_c2;
		_out_c3 = rhs._out_c3;
		_out_c4 = rhs._out_c4;
        _eq_a1 = rhs._eq_a1;
        _eq_a2 = rhs._eq_a2;
        _eq_a3 = rhs._eq_a3;
        _eq_a4 = rhs._eq_a4;
        _eq_b1 = rhs._eq_b1;
        _eq_b2 = rhs._eq_b2;
        _eq_b3 = rhs._eq_b3;
        _eq_b4 = rhs._eq_b4;
        _eq_c1 = rhs._eq_c1;
        _eq_c2 = rhs._eq_c2;
        _eq_c3 = rhs._eq_c3;
        _eq_c4 = rhs._eq_c4;
	}
    return (*this);
}
			
ScriptingCompute::~ScriptingCompute (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingCompute::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingCompute::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_PLUG(_in_a1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_a2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_a3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_a4, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_in_b1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_b2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_b3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_b4, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_in_c1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_c2, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_c3, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_c4, DATA_PERSISTENT | DATA_SETTABLE);
													

	*archive << ARCHIVE_PLUG(_out_a1, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out_a2, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out_a3, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out_a4, DATA_PERSISTENT);

	*archive << ARCHIVE_PLUG(_out_b1, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out_b2, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out_b3, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out_b4, DATA_PERSISTENT);

	*archive << ARCHIVE_PLUG(_out_c1, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out_c2, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out_c3, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out_c4, DATA_PERSISTENT);
													
  	*archive << ARCHIVE_DATA_ACCESSORS("Eq_A1", ScriptingCompute::eq_a1, ScriptingCompute::set_eq_a1, DATA_PERSISTENT | DATA_SETTABLE);
  	*archive << ARCHIVE_DATA_ACCESSORS("Eq_A2", ScriptingCompute::eq_a2, ScriptingCompute::set_eq_a2, DATA_PERSISTENT | DATA_SETTABLE);
  	*archive << ARCHIVE_DATA_ACCESSORS("Eq_A3", ScriptingCompute::eq_a3, ScriptingCompute::set_eq_a3, DATA_PERSISTENT | DATA_SETTABLE);
  	*archive << ARCHIVE_DATA_ACCESSORS("Eq_A4", ScriptingCompute::eq_a4, ScriptingCompute::set_eq_a4, DATA_PERSISTENT | DATA_SETTABLE);
                                                                                                                                              
  	*archive << ARCHIVE_DATA_ACCESSORS("Eq_B1", ScriptingCompute::eq_b1, ScriptingCompute::set_eq_b1, DATA_PERSISTENT | DATA_SETTABLE);
  	*archive << ARCHIVE_DATA_ACCESSORS("Eq_B2", ScriptingCompute::eq_b2, ScriptingCompute::set_eq_b2, DATA_PERSISTENT | DATA_SETTABLE);
  	*archive << ARCHIVE_DATA_ACCESSORS("Eq_B3", ScriptingCompute::eq_b3, ScriptingCompute::set_eq_b3, DATA_PERSISTENT | DATA_SETTABLE);
  	*archive << ARCHIVE_DATA_ACCESSORS("Eq_B4", ScriptingCompute::eq_b4, ScriptingCompute::set_eq_b4, DATA_PERSISTENT | DATA_SETTABLE);

  	*archive << ARCHIVE_DATA_ACCESSORS("Eq_C1", ScriptingCompute::eq_c1, ScriptingCompute::set_eq_c1, DATA_PERSISTENT | DATA_SETTABLE);
  	*archive << ARCHIVE_DATA_ACCESSORS("Eq_C2", ScriptingCompute::eq_c2, ScriptingCompute::set_eq_c2, DATA_PERSISTENT | DATA_SETTABLE);
  	*archive << ARCHIVE_DATA_ACCESSORS("Eq_C3", ScriptingCompute::eq_c3, ScriptingCompute::set_eq_c3, DATA_PERSISTENT | DATA_SETTABLE);
  	*archive << ARCHIVE_DATA_ACCESSORS("Eq_C4", ScriptingCompute::eq_c4, ScriptingCompute::set_eq_c4, DATA_PERSISTENT | DATA_SETTABLE);
													
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingCompute::set_eq_a1 (const std::string &eq)
{
    _eq_a1 = eq;
    _eq_a1_parser.parse(_eq_a1);
    
    dirty_outs();
}

std::string ScriptingCompute::eq_a1 (void) const
{
    return _eq_a1;
}

void ScriptingCompute::set_eq_a2 (const std::string &eq)
{
    _eq_a2 = eq;
    _eq_a2_parser.parse(_eq_a2);

    dirty_outs();
}

std::string ScriptingCompute::eq_a2 (void) const
{
    return _eq_a2;
}

void ScriptingCompute::set_eq_a3 (const std::string &eq)
{
    _eq_a3 = eq;
    _eq_a3_parser.parse(_eq_a3);

    dirty_outs();
}

std::string ScriptingCompute::eq_a3 (void) const
{
    return _eq_a3;
}

void ScriptingCompute::set_eq_a4 (const std::string &eq)
{
    _eq_a4 = eq;
    _eq_a4_parser.parse(_eq_a4);

    dirty_outs();
}

std::string ScriptingCompute::eq_a4 (void) const
{
    return _eq_a4;
}


void ScriptingCompute::set_eq_b1 (const std::string &eq)
{
    _eq_b1 = eq;
    _eq_b1_parser.parse(_eq_b1);

    dirty_outs();
}

std::string ScriptingCompute::eq_b1 (void) const
{
    return _eq_b1;
}

void ScriptingCompute::set_eq_b2 (const std::string &eq)
{
    _eq_b2 = eq;
    _eq_b2_parser.parse(_eq_b2);

    dirty_outs();
}

std::string ScriptingCompute::eq_b2 (void) const
{
    return _eq_b2;
}

void ScriptingCompute::set_eq_b3 (const std::string &eq)
{
    _eq_b3 = eq;
    _eq_b3_parser.parse(_eq_b3);

    dirty_outs();
}

std::string ScriptingCompute::eq_b3 (void) const
{
    return _eq_b3;
}

void ScriptingCompute::set_eq_b4 (const std::string &eq)
{
    _eq_b4 = eq;
    _eq_b4_parser.parse(_eq_b4);

    dirty_outs();
}

std::string ScriptingCompute::eq_b4 (void) const
{
    return _eq_b4;
}


void ScriptingCompute::set_eq_c1 (const std::string &eq)
{
    _eq_c1 = eq;
    _eq_c1_parser.parse(_eq_c1);

    dirty_outs();
}

std::string ScriptingCompute::eq_c1 (void) const
{
    return _eq_c1;
}

void ScriptingCompute::set_eq_c2 (const std::string &eq)
{
    _eq_c2 = eq;
    _eq_c2_parser.parse(_eq_c2);

    dirty_outs();
}

std::string ScriptingCompute::eq_c2 (void) const
{
    return _eq_c2;
}

void ScriptingCompute::set_eq_c3 (const std::string &eq)
{
    _eq_c3 = eq;
    _eq_c3_parser.parse(_eq_c3);

    dirty_outs();
}

std::string ScriptingCompute::eq_c3 (void) const
{
    return _eq_c3;
}

void ScriptingCompute::set_eq_c4 (const std::string &eq)
{
    _eq_c4 = eq;
    _eq_c4_parser.parse(_eq_c4);

    dirty_outs();
}

std::string ScriptingCompute::eq_c4 (void) const
{
    return _eq_c4;
}

//==============================================================================
//==============================================================================

void ScriptingCompute::dirty_outs (void)
{
    _out_a1.set_dirty();
    _out_a2.set_dirty();
    _out_a3.set_dirty();
    _out_a4.set_dirty();
    _out_b1.set_dirty();
    _out_b2.set_dirty();
    _out_b3.set_dirty();
    _out_b4.set_dirty();
    _out_c1.set_dirty();
    _out_c2.set_dirty();
    _out_c3.set_dirty();
    _out_c4.set_dirty();
}

void ScriptingCompute::set_variables(ExpressionParser &p)
{
    p.set_variable("A1", *_in_a1);
    p.set_variable("A2", *_in_a2);
    p.set_variable("A3", *_in_a3);
    p.set_variable("A4", *_in_a4);

    p.set_variable("B1", *_in_b1);
    p.set_variable("B2", *_in_b2);
    p.set_variable("B3", *_in_b3);
    p.set_variable("B4", *_in_b4);

    p.set_variable("C1", *_in_c1);
    p.set_variable("C2", *_in_c2);
    p.set_variable("C3", *_in_c3);
    p.set_variable("C4", *_in_c4);
}

//==============================================================================
//==============================================================================

DTboolean ScriptingCompute::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (    plug == &_out_a1 || plug == &_out_a2 || plug == &_out_a3 || plug == &_out_a4 ||
            plug == &_out_b1 || plug == &_out_b2 || plug == &_out_b3 || plug == &_out_b4 ||
            plug == &_out_c1 || plug == &_out_c2 || plug == &_out_c3 || plug == &_out_c4    ) {

        set_variables(_eq_a1_parser);
        set_variables(_eq_a2_parser);
        set_variables(_eq_a3_parser);
        set_variables(_eq_a4_parser);

        set_variables(_eq_b1_parser);
        set_variables(_eq_b2_parser);
        set_variables(_eq_b3_parser);
        set_variables(_eq_b4_parser);

        set_variables(_eq_c1_parser);
        set_variables(_eq_c2_parser);
        set_variables(_eq_c3_parser);
        set_variables(_eq_c4_parser);
        
        DTfloat result;
        
        if (_eq_a1_parser.eval(result)) _out_a1 = result;
        if (_eq_a2_parser.eval(result)) _out_a2 = result;
        if (_eq_a3_parser.eval(result)) _out_a3 = result;
        if (_eq_a4_parser.eval(result)) _out_a4 = result;

        if (_eq_b1_parser.eval(result)) _out_b1 = result;
        if (_eq_b2_parser.eval(result)) _out_b2 = result;
        if (_eq_b3_parser.eval(result)) _out_b3 = result;
        if (_eq_b4_parser.eval(result)) _out_b4 = result;

        if (_eq_c1_parser.eval(result)) _out_c1 = result;
        if (_eq_c2_parser.eval(result)) _out_c2 = result;
        if (_eq_c3_parser.eval(result)) _out_c3 = result;
        if (_eq_c4_parser.eval(result)) _out_c4 = result;
		
        _out_a1.set_clean();
        _out_a2.set_clean();
        _out_a3.set_clean();
        _out_a4.set_clean();

        _out_b1.set_clean();
        _out_b2.set_clean();
        _out_b3.set_clean();
        _out_b4.set_clean();

        _out_c1.set_clean();
        _out_c2.set_clean();
        _out_c3.set_clean();
        _out_c4.set_clean();

		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3

