//==============================================================================
///	
///	File: ScriptingVector3Filter2Pole.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingVector3Filter2Pole.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingVector3Filter2Pole,"VectorMath",NULL)
IMPLEMENT_PLUG_NODE(ScriptingVector3Filter2Pole)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingVector3Filter2Pole)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingVector3Filter2Pole::ScriptingVector3Filter2Pole (void)
    :   _in				(PLUG_INFO_INDEX(_in), Vector3(0.0F,0.0F,0.0F)),
		_out			(PLUG_INFO_INDEX(_out), Vector3(0.0F,0.0F,0.0F)),
		_type			(Filters::FILTER_BUTTERWORTH_LOW_PASS),
		_num_passes		(1),
		_freq_3db		(1.0F),
		_freq_sampling	(30.0F),
        _initialized    (false)
{  

}
		
ScriptingVector3Filter2Pole::ScriptingVector3Filter2Pole (const ScriptingVector3Filter2Pole &rhs)
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

ScriptingVector3Filter2Pole & ScriptingVector3Filter2Pole::operator = (const ScriptingVector3Filter2Pole &rhs)
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
			
ScriptingVector3Filter2Pole::~ScriptingVector3Filter2Pole (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingVector3Filter2Pole::initialize (void)
{
	ScriptingBase::initialize();

    _passes.resize(_num_passes);
    for (DTuint i = 0; i < _passes.size(); ++i) {
        _passes[i]._fx.initialize(_type, _num_passes, _freq_3db, _freq_sampling, _in->x);
        _passes[i]._fy.initialize(_type, _num_passes, _freq_3db, _freq_sampling, _in->y);
        _passes[i]._fz.initialize(_type, _num_passes, _freq_3db, _freq_sampling, _in->z);
    }
}

//==============================================================================
//==============================================================================

void ScriptingVector3Filter2Pole::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

    archive->push_domain (class_id ());
    
	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
	
	*archive << ARCHIVE_DATA_ACCESSORS("Type", ScriptingVector3Filter2Pole::filter_type, ScriptingVector3Filter2Pole::set_filter_type, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("Butterworth Low Pass")
        .add_enum("Critically Damped Low Pass")
        .add_enum("Bessel Low Pass");

	*archive << ARCHIVE_DATA_ACCESSORS("Num_Passes", ScriptingVector3Filter2Pole::num_passes, ScriptingVector3Filter2Pole::set_num_passes, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA_ACCESSORS("Freq_3db", ScriptingVector3Filter2Pole::freq3db, ScriptingVector3Filter2Pole::set_freq3db, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA_ACCESSORS("Freq_Sampling", ScriptingVector3Filter2Pole::freq_sampling, ScriptingVector3Filter2Pole::set_freq_sampling, DATA_PERSISTENT | DATA_SETTABLE);

	for (DTuint i = 0; i < _passes.size(); ++i) {
        *archive << ARCHIVE_DATA_RAW(_passes[i]._fx, DATA_PERSISTENT | DATA_SETTABLE);
        *archive << ARCHIVE_DATA_RAW(_passes[i]._fy, DATA_PERSISTENT | DATA_SETTABLE);
        *archive << ARCHIVE_DATA_RAW(_passes[i]._fz, DATA_PERSISTENT | DATA_SETTABLE);
	}

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingVector3Filter2Pole::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);
    
    if (!_initialized) {
        _passes.resize(_num_passes);
        for (DTuint i = 0; i < _passes.size(); ++i) {
            _passes[i]._fx.initialize(_type, _num_passes, _freq_3db, _freq_sampling, _in->x);
            _passes[i]._fy.initialize(_type, _num_passes, _freq_3db, _freq_sampling, _in->y);
            _passes[i]._fz.initialize(_type, _num_passes, _freq_3db, _freq_sampling, _in->z);
        }
        
        _initialized = true;
    }

    Vector3 out = _in;
    
    for (DTuint i = 0; i < _passes.size(); ++i) {
        out.x = _passes[i]._fx.filter(out.x);
        out.y = _passes[i]._fy.filter(out.y);
        out.z = _passes[i]._fz.filter(out.z);
    }
    
    _out = out;
    _out.set_dirty();
}

//==============================================================================
//==============================================================================

void ScriptingVector3Filter2Pole::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingVector3Filter2Pole::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3

