//==============================================================================
///	
///	File: ScriptingMusicPlayer.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingMusicPlayer.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Devices/DeviceMusic.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingMusicPlayer,"Media",NULL)
IMPLEMENT_PLUG_NODE(ScriptingMusicPlayer)

IMPLEMENT_PLUG_INFO_INDEX(_path)		
IMPLEMENT_PLUG_INFO_INDEX(_is_playing)		
IMPLEMENT_PLUG_INFO_INDEX(_gain)		
IMPLEMENT_PLUG_INFO_INDEX(_stop_on_destroy)		
IMPLEMENT_PLUG_INFO_INDEX(_loop)		
IMPLEMENT_EVENT_INFO_INDEX(_play)		
IMPLEMENT_EVENT_INFO_INDEX(_stop)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingMusicPlayer)

	PLUG_INIT(_is_playing,"Is_Playing")
		.set_output(true);
        
	PLUG_INIT(_gain,"Gain")
		.set_input(true);

	PLUG_INIT(_stop_on_destroy,"Stop_On_Destroy")
		.set_input(true);

	PLUG_INIT(_path,"Path")
		.set_input(true);
        
	PLUG_INIT(_loop,"Loop")
		.set_input(true);
        
	EVENT_INIT(_play,"Play")
        .set_input(true)
        .set_event(&ScriptingMusicPlayer::play);

	EVENT_INIT(_stop,"Stop")
        .set_input(true)
        .set_event(&ScriptingMusicPlayer::stop);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingMusicPlayer::ScriptingMusicPlayer (void)
    :   _is_playing     (PLUG_INFO_INDEX(_is_playing), 1.0F),
		_gain           (PLUG_INFO_INDEX(_gain), 1.0F),
		_stop_on_destroy(PLUG_INFO_INDEX(_stop_on_destroy), false),
		_path           (PLUG_INFO_INDEX(_path), FilePath()),
		_loop           (PLUG_INFO_INDEX(_loop), true),
        _play           (EVENT_INFO_INDEX(_play)),
        _stop           (EVENT_INFO_INDEX(_stop))
{  

}
		
ScriptingMusicPlayer::ScriptingMusicPlayer (const ScriptingMusicPlayer &rhs)
    :   ScriptingBase	(rhs),
		_is_playing     (rhs._is_playing),
		_gain           (rhs._gain),
		_stop_on_destroy(rhs._stop_on_destroy),
        _path           (rhs._path),
        _loop           (rhs._loop),
        _play           (rhs._play),
        _stop           (rhs._stop)
{   

}

ScriptingMusicPlayer & ScriptingMusicPlayer::operator = (const ScriptingMusicPlayer &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_is_playing = rhs._is_playing;
		_gain = rhs._gain;
		_stop_on_destroy = rhs._stop_on_destroy;
        _path = rhs._path;
        _loop = rhs._loop;
	}
    return (*this);
}
			
ScriptingMusicPlayer::~ScriptingMusicPlayer (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingMusicPlayer::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingMusicPlayer::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());

	*archive << ARCHIVE_PLUG(_path, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_gain, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_stop_on_destroy, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_loop, DATA_PERSISTENT | DATA_SETTABLE);
		        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingMusicPlayer::play (PlugNode *sender)
{
	PROFILER(SCRIPTING);

    if (System::music_renderer()) {
        System::music_renderer()->play_music(_path, 1.0F, _loop);
        tick (0.0F);    // Update gain and status
    }
}

void ScriptingMusicPlayer::stop (PlugNode *sender)
{
	PROFILER(SCRIPTING);

    if (System::music_renderer())
        System::music_renderer()->stop_music();
}

//==============================================================================
//==============================================================================

void ScriptingMusicPlayer::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    if (System::music_renderer() && System::music_renderer()->current_music() == _path) {
        _is_playing = System::music_renderer()->playing();
        System::music_renderer()->set_volume(_gain);
    }
}

//==============================================================================
//==============================================================================

void ScriptingMusicPlayer::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingMusicPlayer::remove_from_world (void)
{
    if (_stop_on_destroy) 
        stop(NULL);

    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3

