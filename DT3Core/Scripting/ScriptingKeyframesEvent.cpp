//==============================================================================
///	
///	File: ScriptingKeyframesEvent.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingKeyframesEvent.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/World/World.hpp"
#include <algorithm>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(ScriptingKeyframesEvent)
IMPLEMENT_PLUG_NODE(ScriptingKeyframesEvent)

IMPLEMENT_PLUG_INFO_INDEX(_t)
IMPLEMENT_EVENT_INFO_INDEX(_out)
		
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingKeyframesEvent)

	PLUG_INIT(_t,"t")
		.set_input(true);

	EVENT_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingKeyframesEvent::ScriptingKeyframesEvent (void)
	:	_t				(PLUG_INFO_INDEX(_t), 0.0F),
        _last_t         (0.0F),
		_out			(EVENT_INFO_INDEX(_out)),
		_id				(0)
{  

}
		
ScriptingKeyframesEvent::ScriptingKeyframesEvent (const ScriptingKeyframesEvent &rhs)
    :   ScriptingKeyframes	(rhs),
		_t					(rhs._t),
        _last_t             (rhs._last_t),
		_out				(rhs._out),
		_id					(rhs._id),
		_keyframes			(rhs._keyframes)
{   

}

ScriptingKeyframesEvent & ScriptingKeyframesEvent::operator = (const ScriptingKeyframesEvent &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingKeyframes::operator = (rhs);	
		
		_t = rhs._t;
        _last_t = rhs._last_t;
		_id = rhs._id;
		_keyframes = rhs._keyframes;
	}
    return (*this);
}
			
ScriptingKeyframesEvent::~ScriptingKeyframesEvent (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingKeyframesEvent::initialize (void)
{
	ScriptingKeyframes::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingKeyframesEvent::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingKeyframes::archive(archive);

	archive->push_domain (class_id ());

    *archive << ARCHIVE_PLUG(_t, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_EVENT(_out, DATA_SETTABLE);

    if (archive->is_reading()) {

        DTint keyCount;
        *archive << ARCHIVE_DATA(keyCount,DATA_PERSISTENT);
        _keyframes.resize(keyCount);
        
        for (DTint i = 0; i < keyCount; ++i) {
            *archive << ARCHIVE_DATA_RAW(_keyframes[i]._time, DATA_PERSISTENT);

            if (archive->is_reading())
                _keyframes[i]._id = _id++;
        }

        _last_t = _t;
    } else {

        DTint keyCount = (DTint) _keyframes.size();
        *archive << ARCHIVE_DATA(keyCount,DATA_PERSISTENT);

        for (DTint i = 0; i < keyCount; ++i) {
            *archive << ARCHIVE_DATA_RAW(_keyframes[i]._time, DATA_PERSISTENT);
        }
    }
    
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTint  ScriptingKeyframesEvent::set_key_time (DTint k, DTfloat time)
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

void  ScriptingKeyframesEvent::clear_key (DTint k)
{
	PROFILER(SCRIPTING);

	_keyframes.erase(_keyframes.begin() + k);
}

//==============================================================================
//==============================================================================

void ScriptingKeyframesEvent::set_key (void)
{
	PROFILER(SCRIPTING);
		
    // clear any existing key
    clear_key();
    
    // add the keyframe
    keyframe k;
    k._time = _t;
    k._id = _id++;
    _keyframes.push_back(k);
    
    std::sort(_keyframes.begin(), _keyframes.end());
	
}

void ScriptingKeyframesEvent::clear_key (void)
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

void ScriptingKeyframesEvent::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    DTfloat t = _t;
    
    // Special cases
    if (_keyframes.size() == 0)	{
        return;
    }
    
    DTfloat min_t = MoreMath::min(t, _last_t);
    DTfloat max_t = MoreMath::max(t, _last_t);
    
    for (std::size_t i = 0; i < _keyframes.size(); ++i) {
        
        if (_keyframes[i]._time > min_t && _keyframes[i]._time <= max_t) {
            _out.send(this);
        }
    
    }
}

//==============================================================================
//==============================================================================

void ScriptingKeyframesEvent::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingKeyframesEvent::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3

