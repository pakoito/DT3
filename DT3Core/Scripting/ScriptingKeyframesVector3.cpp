//==============================================================================
///	
///	File: ScriptingKeyframesVector3.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingKeyframesVector3.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include <algorithm>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingKeyframesVector3,"",EdManipKeyframesVector3)
IMPLEMENT_PLUG_NODE(ScriptingKeyframesVector3)

IMPLEMENT_PLUG_INFO_INDEX(_t)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingKeyframesVector3)

	PLUG_INIT(_t,"t")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingKeyframesVector3::ScriptingKeyframesVector3 (void)
	:	_interpolated	(true),
        _linear         (false),
		_t				(PLUG_INFO_INDEX(_t), 0.0F),
		_out			(PLUG_INFO_INDEX(_out), Vector3(0.0F,0.0F,0.0F)),
		_id				(0),
		_keyframe_cache	(0)
{  

}
		
ScriptingKeyframesVector3::ScriptingKeyframesVector3 (const ScriptingKeyframesVector3 &rhs)
    :   ScriptingKeyframes	(rhs),
		_interpolated		(rhs._interpolated),
        _linear             (rhs._linear),
		_t					(rhs._t),
		_out				(rhs._out),
		_id					(rhs._id),
		_keyframes			(rhs._keyframes),
		_keyframe_cache		(0)
{   

}

ScriptingKeyframesVector3 & ScriptingKeyframesVector3::operator = (const ScriptingKeyframesVector3 &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingKeyframes::operator = (rhs);	
		
		_interpolated = rhs._interpolated;
        _linear = rhs._linear;
		_t = rhs._t;
		_out = rhs._out;
		_id = rhs._id;
		_keyframes = rhs._keyframes;
		_keyframe_cache = rhs._keyframe_cache;
	}
    return (*this);
}
			
ScriptingKeyframesVector3::~ScriptingKeyframesVector3 (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingKeyframesVector3::initialize (void)
{
	ScriptingKeyframes::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingKeyframesVector3::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingKeyframes::archive(archive);

	archive->push_domain (class_id ());
    
    *archive << ARCHIVE_PLUG(_t, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_interpolated, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_linear, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);

    DTint keyCount = (DTint) _keyframes.size();
    *archive << ARCHIVE_DATA(keyCount,DATA_PERSISTENT);
    _keyframes.resize(keyCount);
    
    for (DTint i = 0; i < keyCount; ++i) {
        *archive << ARCHIVE_DATA_RAW(_keyframes[i]._time, DATA_PERSISTENT);
        *archive << ARCHIVE_DATA_RAW(_keyframes[i]._value, DATA_PERSISTENT);
        *archive << ARCHIVE_DATA_RAW(_keyframes[i]._tangent, DATA_PERSISTENT);

        if (archive->is_reading())
            _keyframes[i]._id = _id++;
    }
    
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTint  ScriptingKeyframesVector3::set_key_time (DTint k, DTfloat time)
{
	PROFILER(SCRIPTING);

	DTint oldid = _keyframes[k]._id;
	
	_keyframes[k]._time = time;
	std::sort(_keyframes.begin(), _keyframes.end());
	
	for (std::size_t i = 0; i < _keyframes.size(); ++i)
		if (oldid == _keyframes[i]._id)
			return static_cast<DTint>(i);
			
	return -1;
}

void  ScriptingKeyframesVector3::clear_key (DTint k)
{
	PROFILER(SCRIPTING);

	_keyframes.erase(_keyframes.begin() + k);
}

//==============================================================================
//==============================================================================

void ScriptingKeyframesVector3::set_key (void)
{
	PROFILER(SCRIPTING);

	const std::vector<PlugBase*> &outgoing = _out.outgoing_connections();
	if (outgoing.size() > 0) {
		// get first connected plug
		Plug<Vector3> *outplug = static_cast<Plug<Vector3>*>(*(outgoing.begin()));
		
		// get the value of the first connected plug
		Vector3 val = outplug->value_without_compute();
				
		// clear any existing key
		clear_key();

		// add the keyframe
		keyframe k;
		k._time = _t;
		k._value = val;
		k._tangent = Vector3(0.0F,0.0F,0.0F);
		k._id = _id++;
		_keyframes.push_back(k);
		
		std::sort(_keyframes.begin(), _keyframes.end());
	}
}

void ScriptingKeyframesVector3::clear_key (void)
{
	PROFILER(SCRIPTING);

	FOR_EACH (i,_keyframes) {
		if (_t <= i->_time + (1.0F/30.0F) && _t > i->_time - (1.0F/30.0F)) {
			_keyframes.erase(i);
			return;
		}
	}
}

//==============================================================================
//==============================================================================

void ScriptingKeyframesVector3::interpolate (DTfloat t, Vector3 &translation) const
{
	PROFILER(SCRIPTING);

	// Special cases
	if (_keyframes.size() == 0)	{
		return;
	}
	
	if (_keyframes.size() == 1)	{
		translation = _keyframes[0]._value;
		return;
	}
	
	// Scan for the best key
	if (_keyframe_cache < 0)									_keyframe_cache = 0;
	else if (_keyframe_cache > (DTint) _keyframes.size() - 2)	_keyframe_cache = (DTint) _keyframes.size() - 2;
			
	while (1) {
		if (t < _keyframes[_keyframe_cache]._time) {
			--_keyframe_cache;
			if (_keyframe_cache < 0) {
				_keyframe_cache = 0;
				translation = _keyframes[_keyframe_cache]._value;
				break;
			}
		} else if (t > _keyframes[_keyframe_cache+1]._time) {
			++_keyframe_cache;
			if (_keyframe_cache > (DTint) _keyframes.size() - 2) {
				_keyframe_cache = (DTint) _keyframes.size() - 2;
				translation = _keyframes[_keyframe_cache+1]._value;
				break;
			}
		} else {
		
			if (_interpolated) {
                if (_linear) {
                    DTfloat t1 = (t - _keyframes[_keyframe_cache]._time) / (_keyframes[_keyframe_cache+1]._time - _keyframes[_keyframe_cache]._time);

                    translation = (_keyframes[_keyframe_cache]._value * (1.0F-t1) ) + (_keyframes[_keyframe_cache+1]._value * t1);
                } else {
                    DTfloat t1 = (t - _keyframes[_keyframe_cache]._time) / (_keyframes[_keyframe_cache+1]._time - _keyframes[_keyframe_cache]._time);
                    DTfloat t2 = t1 * t1;
                    DTfloat t3 = t1 * t2;
                
                    // interpolate via Hermite spline
                    // See Realtime Rendering, 2nd Ed., Page 492
                    Vector3 p1,p2,p3,p4;
                    p1 = _keyframes[_keyframe_cache]._value * (2.0F * t3 - 3.0F * t2 + 1.0F);
                    p2 = _keyframes[_keyframe_cache]._tangent * (t3 - 2.0F * t2 + t1);

                    p3 = _keyframes[_keyframe_cache+1]._tangent * (t3 - t2);
                    p4 = _keyframes[_keyframe_cache+1]._value * (-2.0F * t3 + 3.0F * t2);
                    
                    translation = p1 + p2 + p3 + p4;
                }
			} else {
				translation = _keyframes[_keyframe_cache]._value;
			}
			
			break;
		}
	
	}
	
}

//==============================================================================
//==============================================================================

DTboolean ScriptingKeyframesVector3::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
				
		Vector3 out = _out;
		interpolate (_t, out);
		_out = out;

		_out.set_clean();
				
		return true;
	} 
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3

