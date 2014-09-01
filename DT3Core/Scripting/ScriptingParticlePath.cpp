//==============================================================================
///	
///	File: ScriptingParticlePath.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticlePath.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Graphics/Particles.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticlePath,"Particles",EdManipScriptingParticlePath)
IMPLEMENT_PLUG_NODE(ScriptingParticlePath)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)
IMPLEMENT_PLUG_INFO_INDEX(_speed)
IMPLEMENT_PLUG_INFO_INDEX(_bias)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticlePath)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_speed,"Speed")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_bias,"Bias")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingParticlePath::ScriptingParticlePath (void)
	:	_in				(PLUG_INFO_INDEX(_in)),
		_out			(PLUG_INFO_INDEX(_out)),
		_speed			(PLUG_INFO_INDEX(_speed),1.0F),
		_bias			(PLUG_INFO_INDEX(_bias),0.0F)
{  

}
		
ScriptingParticlePath::ScriptingParticlePath (const ScriptingParticlePath &rhs)
    :   ScriptingBase       (rhs),
		_in					(rhs._in),
		_out				(rhs._out),
		_speed				(rhs._speed),
		_bias				(rhs._bias),
        _points             (rhs._points)
{   

}

ScriptingParticlePath & ScriptingParticlePath::operator = (const ScriptingParticlePath &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);	
		
		_in = rhs._in;
		_out = rhs._out;
		_speed = rhs._speed;
		_bias = rhs._bias;
        _points = rhs._points;
	}
    return (*this);
}
			
ScriptingParticlePath::~ScriptingParticlePath (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticlePath::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticlePath::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
    
    *archive << ARCHIVE_PLUG(_speed, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_bias, DATA_PERSISTENT | DATA_SETTABLE);
    
    *archive << ARCHIVE_DATA_ACCESSORS("Num_Points", ScriptingParticlePath::num_points, ScriptingParticlePath::set_num_points, DATA_PERSISTENT | DATA_SETTABLE);

    DTint pointCount = (DTint) _points.size();
    *archive << ARCHIVE_DATA(pointCount,DATA_PERSISTENT);
    _points.resize(pointCount);
    
    for (DTint i = 0; i < pointCount; ++i) {
        *archive << ARCHIVE_DATA_RAW(_points[i].value, DATA_PERSISTENT);
        *archive << ARCHIVE_DATA_RAW(_points[i].tangent, DATA_PERSISTENT);
    }
    
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingParticlePath::interpolate (DTfloat t, Vector3 &translation) const
{
	PROFILER(PARTICLES);

	// Special cases
	if (_points.size() == 0)	{
		return;
	}
	
	if (_points.size() == 1)	{
		translation = _points[0].value;
		return;
	}
    
    DTfloat index_float = t;
    DTint index = (DTint)(index_float);
    
    if (index < 0)                                      return;
    if (index > static_cast<DTint>(_points.size()-2))   return;
	
    DTfloat t1 = index_float - index;
    DTfloat t2 = t1 * t1;
    DTfloat t3 = t1 * t2;

    // interpolate via Hermite spline
    // See Realtime Rendering, 2nd Ed., Page 492
    Vector3 p1,p2,p3,p4;
    p1 = _points[index].value * (2.0F * t3 - 3.0F * t2 + 1.0F);
    p2 = _points[index].tangent * (t3 - 2.0F * t2 + t1);

    p3 = _points[index+1].tangent * (t3 - t2);
    p4 = _points[index+1].value * (-2.0F * t3 + 3.0F * t2);
    
    translation = p1 + p2 + p3 + p4;
}

//==============================================================================
//==============================================================================

DTboolean ScriptingParticlePath::compute (const PlugBase *plug)
{
	PROFILER(PARTICLES);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
		
		// Make sure there are input particles
		std::shared_ptr<Particles> particles = _in;
		if (!particles || particles->translations_stream().size() <= 0) {
			_out.set_clean();
            return true;
		}
			
		// Build the velocities stream
		if (particles->velocity_stream().size() <= 0) {
			particles->build_velocity_stream();
		}
		
		// Do processing
        std::vector<Vector3> &translations = particles->translations_stream();
		std::vector<DTfloat> &lifetimes = particles->lifetimes_stream();

		for (DTint i = particles->active_start(); i != particles->active_end(); i = (i + 1) % particles->translations_stream().size()) {
            interpolate (lifetimes[i] * _speed + _bias, translations[i]);
		}

		_out = particles;
		_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3

