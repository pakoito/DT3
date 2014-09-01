//==============================================================================
///	
///	File: ScriptingFilter2Pole.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingFilter2Pole.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingFilter2Pole,"Math",NULL)
IMPLEMENT_PLUG_NODE(ScriptingFilter2Pole)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingFilter2Pole)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingFilter2Pole::ScriptingFilter2Pole (void)
    :   _in				(PLUG_INFO_INDEX(_in), 0.0F),
		_out			(PLUG_INFO_INDEX(_out), 0.0F),
		_type			(Filters::FILTER_BUTTERWORTH_LOW_PASS),
		_num_passes		(1),
		_freq_3db		(1.0F),
		_freq_sampling	(30.0F),
        _initialized    (false)
{  

}
		
ScriptingFilter2Pole::ScriptingFilter2Pole (const ScriptingFilter2Pole &rhs)
    :   ScriptingBase	(rhs),
		_in				(rhs._in),
		_out			(rhs._out),
		_type			(rhs._type),
		_num_passes		(rhs._num_passes),
		_freq_3db		(rhs._freq_3db),
		_freq_sampling	(rhs._freq_sampling),
        _initialized    (false)
{   

}

ScriptingFilter2Pole & ScriptingFilter2Pole::operator = (const ScriptingFilter2Pole &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in = rhs._in;
		_out = rhs._out;

		_type = rhs._type;
		_num_passes = rhs._num_passes;
		_freq_3db = rhs._freq_3db;
		_freq_sampling = rhs._freq_sampling;

        _initialized = false;   
	}
    return (*this);
}
			
ScriptingFilter2Pole::~ScriptingFilter2Pole (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingFilter2Pole::initialize (void)
{
	ScriptingBase::initialize();

    _passes.resize(_num_passes);
    for (DTuint i = 0; i < _passes.size(); ++i) {
        _passes[i].initialize(_type, _num_passes, _freq_3db, _freq_sampling, _in);
    }
}

//==============================================================================
//==============================================================================

void ScriptingFilter2Pole::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
    
    *archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
    
    *archive << ARCHIVE_DATA_ACCESSORS("Type", ScriptingFilter2Pole::filter_type, ScriptingFilter2Pole::set_filter_type, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("Butterworth Low Pass")
        .add_enum("Critically Damped Low Pass")
        .add_enum("Bessel Low Pass");

    *archive << ARCHIVE_DATA_ACCESSORS("Num_Passes", ScriptingFilter2Pole::num_passes, ScriptingFilter2Pole::set_num_passes, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Freq_3db", ScriptingFilter2Pole::freq3db, ScriptingFilter2Pole::set_freq3db, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Freq_Sampling", ScriptingFilter2Pole::freq_sampling, ScriptingFilter2Pole::set_freq_sampling, DATA_PERSISTENT | DATA_SETTABLE);

    for (DTuint i = 0; i < _passes.size(); ++i) {
        *archive << ARCHIVE_DATA_RAW(_passes[i], DATA_PERSISTENT);
    }
    
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingFilter2Pole::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    if (!_initialized) {
        _passes.resize(_num_passes);
        for (DTuint i = 0; i < _passes.size(); ++i) {
            _passes[i].initialize(_type, _num_passes, _freq_3db, _freq_sampling, _in);
        }
        
        _initialized = true;
    }

    DTfloat out = _in;
    
    for (DTuint i = 0; i < _passes.size(); ++i) {
        out = _passes[i].filter(out);
    }
    
    _out = out;
    _out.set_dirty();
}

//==============================================================================
//==============================================================================

void ScriptingFilter2Pole::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingFilter2Pole::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3

