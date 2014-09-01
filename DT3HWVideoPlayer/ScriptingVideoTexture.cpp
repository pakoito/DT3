//==============================================================================
///	
///	File: ScriptingVideoTexture.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "ScriptingVideoTexture.hpp"
#include "Factory.hpp"
#include "Archive.hpp"
#include "System.hpp"
#include "DeviceInput.hpp"

#include "System.hpp"
#include "SystemCallbacks.hpp"
#include "World.hpp"
#include "Profiler.hpp"
#include "ConsoleStream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingVideoTexture,"Video",NULL)
IMPLEMENT_PLUG_NODE(ScriptingVideoTexture)

IMPLEMENT_PLUG_INFO(_file_or_url)
IMPLEMENT_PLUG_INFO(_is_playing)
IMPLEMENT_PLUG_INFO(_out)
IMPLEMENT_PLUG_INFO(_rectangle)
IMPLEMENT_PLUG_INFO(_current_time)
IMPLEMENT_PLUG_INFO(_length)

IMPLEMENT_EVENT_INFO(_play)
IMPLEMENT_EVENT_INFO(_pause)
IMPLEMENT_EVENT_INFO(_rewind)
		
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingVideoTexture)
		
	PLUG_INIT(_file_or_url,"File_Or_URL")
		.setInput(true);

	PLUG_INIT(_out,"Tex_Out")
		.setOutput(true);

	PLUG_INIT(_current_time,"Current_time")
		.setOutput(true);

	PLUG_INIT(_length,"Length")
		.setOutput(true);

	PLUG_INIT(_is_playing,"Is_Playing")
		.setOutput(true);

	PLUG_INIT(_rectangle,"Rect")
		.setOutput(true);

	EVENT_INIT(_play,"Start")
        .setInput(true)
        .setEvent(EventBindCreator(&ScriptingVideoTexture::play));

	EVENT_INIT(_pause,"Stop")
        .setInput(true)
        .setEvent(EventBindCreator(&ScriptingVideoTexture::pause));

	EVENT_INIT(_rewind,"Rewind")
        .setInput(true)
        .setEvent(EventBindCreator(&ScriptingVideoTexture::rewind));

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingVideoTexture::ScriptingVideoTexture (void)
    :	_file_or_url            (PLUG_INFO(_file_or_url)),
        _out                    (PLUG_INFO(_out)),
        _current_time           (PLUG_INFO(_current_time), 0.0F),
        _length                 (PLUG_INFO(_length), 0.0F),
        _is_playing             (PLUG_INFO(_is_playing), false),
		_rectangle              (PLUG_INFO(_rectangle), Rectangle(0.0F,1.0F,0.0F,1.0F)),
        _play                   (EVENT_INFO(_play)),
        _pause                  (EVENT_INFO(_pause)),
        _rewind                 (EVENT_INFO(_rewind)),
        _hw                     (NULL)
{

}
		
ScriptingVideoTexture::ScriptingVideoTexture (const ScriptingVideoTexture &rhs)
    :   ScriptingBase           (rhs),
		_file_or_url            (rhs._file_or_url),
		_out                    (rhs._out),
		_current_time           (rhs._current_time),
		_length                 (rhs._length),
		_is_playing             (rhs._is_playing),
		_rectangle              (rhs._rectangle),
        _play                   (EVENT_INFO(_play)),
        _pause                  (EVENT_INFO(_pause)),
        _rewind                 (EVENT_INFO(_rewind)),
        _hw                     (NULL)
{

}

ScriptingVideoTexture & ScriptingVideoTexture::operator = (const ScriptingVideoTexture &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

        _file_or_url = rhs._file_or_url;
        _last_file_or_url.clear();
        
        _out = rhs._out;
        _current_time = rhs._current_time;
        _length = rhs._length;
        
        _is_playing = rhs._is_playing;
        _rectangle = rhs._rectangle;
        
        close();
	}
    return (*this);
}
			
ScriptingVideoTexture::~ScriptingVideoTexture (void)
{
    close();
}

//==============================================================================
//==============================================================================

void ScriptingVideoTexture::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingVideoTexture::archive_read (Archive *archive)
{
    ScriptingBase::archive_read(archive);

	archive->push_domain (getClassID ());
    
    *archive << ARCHIVE_PLUG(_file_or_url, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_autoplay_when_ready, DATA_PERSISTENT | DATA_SETTABLE);
    
    archive->pop_domain ();
}

void ScriptingVideoTexture::archive_read_done (void)
{

    // Kickstart opening and getting the first frame of video
    _last_file_or_url = _file_or_url;
    
    // Check if we have a URL
    if (URL::isURL(_file_or_url)) {
        open(URL(_file_or_url));
    } else {
        open(FilePath(_file_or_url));
    }

    _out = Video::getFrame(_hw);
}

void ScriptingVideoTexture::archive_write (Archive *archive)
{
    ScriptingBase::archive_write(archive);

    archive->push_domain (getClassID ());
    
    *archive << ARCHIVE_PLUG(_file_or_url, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_autoplay_when_ready, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingVideoTexture::open (const FilePath &path)
{
    close();
    
    _hw = Video::open(path);
}

void ScriptingVideoTexture::open (const URL &url)
{
    close();
    
    _hw = Video::open(url);
}

void ScriptingVideoTexture::close (void)
{
    if (_hw) {
        Video::close(_hw);
    }
}

//==============================================================================
//==============================================================================

void ScriptingVideoTexture::play (PlugNode *sender)
{   
    if (_hw) {
        Video::play(_hw);
    }
}

void ScriptingVideoTexture::pause (PlugNode *sender)
{
    if (_hw) {
        Video::pause(_hw);
    }
}

void ScriptingVideoTexture::rewind (PlugNode *sender)
{
    if (_hw) {
        Video::rewind(_hw);
    }
}

//==============================================================================
//==============================================================================

DTfloat ScriptingVideoTexture::length (void) const
{
    if (_hw) {
        return Video::length(_hw);
    } else {
        return 0.0F;
    }
}

void ScriptingVideoTexture::seek (DTfloat time)
{
    if (_hw) {
        Video::seek(_hw, time);
    }
}

//==============================================================================
//==============================================================================

void ScriptingVideoTexture::tick (const DTfloat dt)
{
    // Check for path change
    if (_last_file_or_url != _file_or_url) {
        _last_file_or_url = _file_or_url;
    
        close();
        
        // Check if we have a URL
        if (URL::isURL(_file_or_url)) {
            open(URL(_file_or_url));
        } else {
            open(FilePath(_file_or_url));
        }
    
    }

    if (!_hw)
        return;
    
    Video::State state = Video::getState(_hw);
    
    if (state == Video::STATE_PAUSED || state == Video::STATE_PLAYING) {
        std::shared_ptr<TextureResource> tex = Video::getFrame(_hw);
        
        // If there was a frame
        if (tex.isValid()) {
            _out = tex;
            _rectangle->set(0.0F, 1.0F, 0.0F, 1.0F);
            
            _rectangle.setDirty();
        }
    }
    
    _current_time = Video::currentTime(_hw);
    _length = Video::length(_hw);
    _is_playing = (state == Video::STATE_PLAYING);
}

//==============================================================================
//==============================================================================

void ScriptingVideoTexture::addToWorld (World *world)
{
    ScriptingBase::addToWorld(world);
    
    world->registerForTick(this, makeCallback(this, &type::tick));
}

void ScriptingVideoTexture::removeFromWorld (void)
{
    getWorld()->unregisterForTick(this, makeCallback(this, &type::tick));

    ScriptingBase::removeFromWorld();
}

//==============================================================================
//==============================================================================

} // DT3

