//==============================================================================
///	
///	File: ScriptingKeyframesMaterialResource.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingKeyframesMaterialResource.hpp"
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

IMPLEMENT_FACTORY_CREATION(ScriptingKeyframesMaterialResource)
IMPLEMENT_PLUG_NODE(ScriptingKeyframesMaterialResource)

IMPLEMENT_PLUG_INFO_INDEX(_t)
IMPLEMENT_PLUG_INFO_INDEX(_out)
		
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingKeyframesMaterialResource)

	PLUG_INIT(_t,"t")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingKeyframesMaterialResource::ScriptingKeyframesMaterialResource (void)
	:	_t				(PLUG_INFO_INDEX(_t), 0.0F),
		_out			(PLUG_INFO_INDEX(_out)),
		_id				(0),
		_keyframe_cache	(0)
{  

}
		
ScriptingKeyframesMaterialResource::ScriptingKeyframesMaterialResource (const ScriptingKeyframesMaterialResource &rhs)
    :   ScriptingKeyframes	(rhs),
		_t					(rhs._t),
		_out				(PLUG_INFO_INDEX(_out)),
		_id					(rhs._id),
		_keyframe_cache		(0)
{   

}

ScriptingKeyframesMaterialResource & ScriptingKeyframesMaterialResource::operator = (const ScriptingKeyframesMaterialResource &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingKeyframes::operator = (rhs);	
		
		_t = rhs._t;
		_out = rhs._out;
		_id = rhs._id;
		_keyframe_cache = rhs._keyframe_cache;
	}
    return (*this);
}
			
ScriptingKeyframesMaterialResource::~ScriptingKeyframesMaterialResource (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingKeyframesMaterialResource::initialize (void)
{
	ScriptingKeyframes::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingKeyframesMaterialResource::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingKeyframes::archive(archive);

	archive->push_domain (class_id ());
    
    *archive << ARCHIVE_PLUG(_t, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);

    DTint keyCount = (DTint) _keyframes.size();
    *archive << ARCHIVE_DATA(keyCount,DATA_PERSISTENT);
    _keyframes.resize(keyCount);
    
    for (DTint i = 0; i < (DTint)keyCount; ++i) {
        *archive << ARCHIVE_DATA_RAW(_keyframes[i]._time, DATA_PERSISTENT);
        *archive << ARCHIVE_DATA_RAW(_keyframes[i]._value, DATA_PERSISTENT);

        if (archive->is_reading())
            _keyframes[i]._id = _id++;
    }
    
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTint  ScriptingKeyframesMaterialResource::set_key_time (DTint k, DTfloat time)
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

void  ScriptingKeyframesMaterialResource::clear_key (DTint k)
{
	PROFILER(SCRIPTING);

	auto i = _keyframes.begin() + k;
	_keyframes.erase(i);
}

//==============================================================================
//==============================================================================

void ScriptingKeyframesMaterialResource::set_key (void)
{
	PROFILER(SCRIPTING);

	const std::vector<PlugBase*> &outgoing = _out.outgoing_connections();
	if (outgoing.size() > 0) {
		// get first connected plug
		Plug<std::shared_ptr<MaterialResource>> *outplug = static_cast<Plug<std::shared_ptr<MaterialResource>>*>(*(outgoing.begin()));
		
		// get the value of the first connected plug
		std::shared_ptr<MaterialResource> val = outplug->value_without_compute();
		
		// clear any existing key
		clear_key();
		
		// add the keyframe
		keyframe k;
		k._time = _t;
		k._value = val;
		k._id = _id++;
		_keyframes.push_back(k);
		
		std::sort(_keyframes.begin(), _keyframes.end());
	}
}

void ScriptingKeyframesMaterialResource::clear_key (void)
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

DTboolean ScriptingKeyframesMaterialResource::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
		
		DTfloat t = _t;
		
		// Special cases
		if (_keyframes.size() == 0)	{
			_out.set_clean();
			return true;
		}
		
		if (_keyframes.size() == 1)	{
			_out = _keyframes[0]._value;
			return true;
		}
		
		// Scan for the best key
		if (_keyframe_cache < 0)									_keyframe_cache = 0;
		else if (_keyframe_cache > (DTint) _keyframes.size() - 2)	_keyframe_cache = (DTint) _keyframes.size() - 2;
				
		while (1) {
			if (t < _keyframes[_keyframe_cache]._time) {
				--_keyframe_cache;
				if (_keyframe_cache < 0) {
					_keyframe_cache = 0;
					_out = _keyframes[_keyframe_cache]._value;
					break;
				}
			} else if (t > _keyframes[_keyframe_cache+1]._time) {
				++_keyframe_cache;
				if (_keyframe_cache > (DTint) _keyframes.size() - 2) {
					_keyframe_cache = (DTint) _keyframes.size() - 2;
					_out = _keyframes[_keyframe_cache+1]._value;
					break;
				}
			} else {			
				_out = _keyframes[_keyframe_cache]._value;
				break;
			}
		
		}
		
		_out.set_clean();
		
		return true;
	} 
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3

