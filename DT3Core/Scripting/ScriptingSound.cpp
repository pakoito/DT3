//==============================================================================
///	
///	File: ScriptingSound.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSound.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSound,"Resources",NULL)
IMPLEMENT_PLUG_NODE(ScriptingSound)

IMPLEMENT_PLUG_INFO_INDEX(_sound_packet)
IMPLEMENT_PLUG_INFO_INDEX(_looping)

IMPLEMENT_EVENT_INFO_INDEX(_reset_e)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSound)

	PLUG_INIT(_sound_packet,"Sound_Packet")
        .set_single_output(true)
		.set_output(true)
        .set_always_dirty(true);

	PLUG_INIT(_looping,"Looping")
		.set_input(true);
        
	EVENT_INIT(_reset_e,"Reset_Event")
        .set_input(true)
        .set_event(&ScriptingSound::event_reset);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingSound::ScriptingSound (void)
    :   _sound_packet	(PLUG_INFO_INDEX(_sound_packet)),
		_looping        (PLUG_INFO_INDEX(_looping), false),
        _reset_e        (EVENT_INFO_INDEX(_reset_e)),
        _chunk_index    (0)
{  

}
		
ScriptingSound::ScriptingSound (const ScriptingSound &rhs)
    :   ScriptingSoundBase	(rhs),
		_sound          (rhs._sound),
		_sound_packet	(rhs._sound_packet),
		_looping        (rhs._looping),
        _reset_e        (rhs._reset_e),
        _chunk_index    (rhs._chunk_index)
{   

}

ScriptingSound & ScriptingSound::operator = (const ScriptingSound &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingSoundBase::operator = (rhs);

		_sound = rhs._sound;
		_sound_packet = rhs._sound_packet;
        _looping = rhs._looping;
        _chunk_index = rhs._chunk_index;
	}
    return (*this);
}
			
ScriptingSound::~ScriptingSound (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSound::initialize (void)
{
	ScriptingSoundBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingSound::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingSoundBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_DATA_ACCESSORS("Sound", ScriptingSound::sound_property, ScriptingSound::set_sound_property, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_looping, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_EVENT(_reset_e, DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingSound::set_sound_property (const std::shared_ptr<SoundResource> &sound)
{
	PROFILER(SOUND);

	_sound = sound;
	_sound_packet.set_dirty();
}

//==============================================================================
//==============================================================================

void ScriptingSound::event_reset (PlugNode *sender)
{
	PROFILER(SOUND);
    _chunk_index = 0;
}

//==============================================================================
//==============================================================================

DTboolean ScriptingSound::compute (const PlugBase *plug)
{
	PROFILER(SOUND);

    if (super_type::compute(plug))  return true;

	if (plug == &_sound_packet) {
        
        // Stream chunk in
        if (_sound && _sound->count_chunks() > 0) {
            DTuint chunk_index = _chunk_index;
            
            if (_looping) {
                _chunk_index = (_chunk_index + 1) % _sound->count_chunks();
            } else {
                _chunk_index = _chunk_index + 1;
            }

            _sound->streamed_chunk(_sound_packet.as_ref_no_compute(), chunk_index);
            
        } else {
            _sound_packet.as_ref_no_compute().set_num_bytes(0);
        }
    
		_sound_packet.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3

