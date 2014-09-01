//==============================================================================
///	
///	File: ComponentGUIVideoPlayer.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "ComponentGUIVideoPlayer.hpp"
#include "ArchiveObjectQueue.hpp"
#include "Factory.hpp"
#include "MaterialResource.hpp"
#include "TextureResource.hpp"
#include "FontResource.hpp"
#include "World.hpp"
#include "GUIObject.hpp"
#include "DrawBatcher.hpp"
#include "DrawBatcherQuads.hpp"
#include "MoreMath.hpp"
#include "TextRenderer.hpp"
#include "System.hpp"
#include "SystemCallbacks.hpp"
#include "DeviceGraphics.hpp"
#include "Globals.hpp"
#include "DrawUtils.hpp"
#include "GlyphCache.hpp"
#include <cmath>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_COMPONENT(ComponentGUIVideoPlayer,"GUI","ComponentAdapter")
IMPLEMENT_PLUG_NODE(ComponentGUIVideoPlayer)

IMPLEMENT_PLUG_INFO(_file_or_url)

IMPLEMENT_PLUG_INFO(_is_playing)
IMPLEMENT_PLUG_INFO(_current_time)
IMPLEMENT_PLUG_INFO(_length)

IMPLEMENT_PLUG_INFO(_video_material)
IMPLEMENT_PLUG_INFO(_play_material)
IMPLEMENT_PLUG_INFO(_pause_material)
IMPLEMENT_PLUG_INFO(_thumb_material)
IMPLEMENT_PLUG_INFO(_track_loaded)
IMPLEMENT_PLUG_INFO(_track_unloaded)
IMPLEMENT_PLUG_INFO(_background)

IMPLEMENT_EVENT_INFO(_play)
IMPLEMENT_EVENT_INFO(_pause)
IMPLEMENT_EVENT_INFO(_rewind)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ComponentGUIVideoPlayer)
    
	PLUG_INIT(_file_or_url,"File_Or_URL")
		.setInput(true);

	PLUG_INIT(_current_time,"Current_time")
		.setOutput(true);

	PLUG_INIT(_length,"Length")
		.setOutput(true);

	PLUG_INIT(_is_playing,"Is_Playing")
		.setOutput(true);

    PLUG_INIT(_video_material, "Video_Material")
		.setInput(true);

    PLUG_INIT(_play_material, "Play_Material")
		.setInput(true);
    
    PLUG_INIT(_pause_material, "Pause_Material")
		.setInput(true);
                
    PLUG_INIT(_thumb_material, "Thumb_Material")
		.setInput(true);
        
    PLUG_INIT(_track_loaded, "Track_Loaded")
		.setInput(true);

    PLUG_INIT(_track_unloaded, "Track_Unloaded")
		.setInput(true);

    PLUG_INIT(_background, "Background")
		.setInput(true);
    
	EVENT_INIT(_play,"Start")
        .setInput(true)
        .setEvent(EventBindCreator(&ComponentGUIVideoPlayer::play));

	EVENT_INIT(_pause,"Stop")
        .setInput(true)
        .setEvent(EventBindCreator(&ComponentGUIVideoPlayer::pause));

	EVENT_INIT(_rewind,"Rewind")
        .setInput(true)
        .setEvent(EventBindCreator(&ComponentGUIVideoPlayer::rewind));

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ComponentGUIVideoPlayer::ComponentGUIVideoPlayer (void)
    :   _file_or_url    (PLUG_INFO(_file_or_url)),
        _current_time   (PLUG_INFO(_current_time), 0.0F),
        _length         (PLUG_INFO(_length), 0.0F),
        _is_playing     (PLUG_INFO(_is_playing), false),
        _video_material (PLUG_INFO(_video_material)),
        _play_material  (PLUG_INFO(_play_material)),
        _pause_material (PLUG_INFO(_pause_material)),
        _thumb_material (PLUG_INFO(_thumb_material)),
        _track_loaded   (PLUG_INFO(_track_loaded)),
        _track_unloaded (PLUG_INFO(_track_unloaded)),
        _background     (PLUG_INFO(_background)),
        _play           (EVENT_INFO(_play)),
        _pause          (EVENT_INFO(_pause)),
        _rewind         (EVENT_INFO(_rewind)),
        _controls_size  (0.02F),
        _track_size     (0.005F),
        _thumb_size     (0.01F),
        _hw             (NULL)
{

}
		
ComponentGUIVideoPlayer::ComponentGUIVideoPlayer (const ComponentGUIVideoPlayer &rhs)
    :   ComponentBase       (rhs),
		_file_or_url        (rhs._file_or_url),
		_current_time       (rhs._current_time),
		_length             (rhs._length),
		_is_playing         (rhs._is_playing),
        _video_material     (rhs._video_material),
        _play_material      (rhs._play_material),
        _pause_material     (rhs._pause_material),
        _thumb_material     (rhs._thumb_material),
        _track_loaded       (rhs._track_loaded),
        _track_unloaded     (rhs._track_unloaded),
        _background         (rhs._background),
        _play               (EVENT_INFO(_play)),
        _pause              (EVENT_INFO(_pause)),
        _rewind             (EVENT_INFO(_rewind)),
        _controls_size      (rhs._controls_size),
        _track_size         (rhs._track_size),
        _thumb_size         (rhs._thumb_size),
        _hw                 (NULL)
{

}

ComponentGUIVideoPlayer & ComponentGUIVideoPlayer::operator = (const ComponentGUIVideoPlayer &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ComponentBase::operator = (rhs);
        
		_file_or_url = rhs._file_or_url;

		_current_time = rhs._current_time;
		_length = rhs._length;
		_is_playing = rhs._is_playing;

        _video_material = rhs._video_material;
        _play_material = rhs._play_material;
        _pause_material = rhs._pause_material;
        _thumb_material = rhs._thumb_material;
        _track_loaded = rhs._track_loaded;
        _track_unloaded = rhs._track_unloaded;
        _background = rhs._background;
        
        _controls_size = rhs._controls_size;
        _track_size = rhs._track_size;
        _thumb_size = rhs._thumb_size;

    }
    return (*this);
}
			
ComponentGUIVideoPlayer::~ComponentGUIVideoPlayer (void)
{
    close();
}

//==============================================================================
//==============================================================================

void ComponentGUIVideoPlayer::initialize (void)
{
	ComponentBase::initialize();
}

//==============================================================================
//==============================================================================

void ComponentGUIVideoPlayer::archive_read (Archive *archive)
{
    ComponentBase::archive_read(archive);

	archive->push_domain (getClassID ());
    
	*archive << ARCHIVE_PLUG(_file_or_url, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_video_material, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_play_material, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_pause_material, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_thumb_material, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_track_loaded, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_track_unloaded, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_background, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_DATA(_controls_size, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_track_size, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_thumb_size, DATA_PERSISTENT | DATA_SETTABLE);
    
    archive->pop_domain ();
}

void ComponentGUIVideoPlayer::archive_read_done (void)
{

    // Kickstart opening and getting the first frame of video
    _last_file_or_url = _file_or_url;
    
    // Check if we have a URL
    if (URL::isURL(_file_or_url)) {
        open(URL(_file_or_url));
    } else {
        open(FilePath(_file_or_url));
    }
    
    
    // Tie video texture to material
    if (_hw && _video_material->isValid()) {
    
        std::shared_ptr<TextureResource> tex = Video::getFrame(_hw);
        (**_video_material).setCurrentUnit(0);
        (**_video_material).setTexture(tex);
    }

}

void ComponentGUIVideoPlayer::archive_write (Archive *archive)
{
    ComponentBase::archive_write(archive);

    archive->push_domain (getClassID ());
	
	*archive << ARCHIVE_PLUG(_file_or_url, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_video_material, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_play_material, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_pause_material, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_thumb_material, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_track_loaded, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_track_unloaded, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_background, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_DATA(_controls_size, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_track_size, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_thumb_size, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

Rectangle ComponentGUIVideoPlayer::calcPlayPauseRect (const Rectangle &rect)
{
    DTfloat aspect = System::getRenderer()->getScreenAspect();
    
    return Rectangle (  rect.getMinusX(), rect.getMinusX() + _controls_size / aspect,
                        rect.getMinusY(), rect.getMinusY() + _controls_size);
}

Rectangle ComponentGUIVideoPlayer::calcSeekRect (const Rectangle &rect)
{
    DTfloat aspect = System::getRenderer()->getScreenAspect();
    
    return Rectangle (  rect.getMinusX() + _controls_size / aspect, rect.getPlusX(),
                        rect.getMinusY(), rect.getMinusY() + _controls_size);
}

Rectangle ComponentGUIVideoPlayer::calcTrackRect (const Rectangle &rect)
{
    DTfloat aspect = System::getRenderer()->getScreenAspect();
    DTfloat pad = (_controls_size - _track_size) * 0.5F;
    
    return Rectangle (  rect.getMinusX() + (_controls_size + pad) / aspect, rect.getPlusX() - pad / aspect,
                        rect.getMinusY() + pad, rect.getMinusY() + _controls_size - pad);

}

Rectangle ComponentGUIVideoPlayer::calcThumbRect (const Rectangle &rect, DTfloat current, DTfloat length)
{
    DTfloat aspect = System::getRenderer()->getScreenAspect();
    DTfloat pad = (_controls_size - _track_size) * 0.5F;
    
    DTfloat min_x = rect.getMinusX() + (_controls_size + pad) / aspect;
    DTfloat max_x = rect.getPlusX() - pad / aspect;
    
    DTfloat t = MoreMath::clampZeroOne(current / length);
    
    DTfloat x = (max_x - min_x) * t + min_x;
    DTfloat y = rect.getMinusY() + _controls_size * 0.5F;
    
    return Rectangle (x - _thumb_size*0.5F/aspect, x + _thumb_size*0.5F/aspect, y - _thumb_size * 0.5F, y + _thumb_size*0.5F);
    
}

Rectangle ComponentGUIVideoPlayer::calcBackgroundRect (const Rectangle &rect)
{
    return Rectangle (  rect.getMinusX(), rect.getPlusX(),
                        rect.getMinusY(), rect.getMinusY() + _controls_size);
}

//==============================================================================
//==============================================================================

void ComponentGUIVideoPlayer::open (const FilePath &path)
{
    close();
    
    _hw = Video::open(path);
}

void ComponentGUIVideoPlayer::open (const URL &url)
{
    close();
    
    _hw = Video::open(url);
}

void ComponentGUIVideoPlayer::close (void)
{
    if (_hw) {
        Video::close(_hw);
    }
}

//==============================================================================
//==============================================================================

void ComponentGUIVideoPlayer::play (PlugNode *sender)
{   
    if (_hw) {
        Video::play(_hw);
    }
}

void ComponentGUIVideoPlayer::pause (PlugNode *sender)
{
    if (_hw) {
        Video::pause(_hw);
    }
}

void ComponentGUIVideoPlayer::rewind (PlugNode *sender)
{
    if (_hw) {
        Video::rewind(_hw);
    }
}

//==============================================================================
//==============================================================================

void ComponentGUIVideoPlayer::tick (const DTfloat dt)
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
    
    _current_time = Video::currentTime(_hw);
    _length = Video::length(_hw);
    _is_playing = (Video::getState(_hw) == Video::STATE_PLAYING);
}

//==============================================================================
//==============================================================================

void ComponentGUIVideoPlayer::draw (CameraObject* camera, const Color &parent_color)
{
    GUIObject *gui = checkedCast<GUIObject*>(getOwner());
    if (!gui)
        return;
    
    if (gui->getColor().getA() * parent_color.getA() <= 0.0F)
        return;
       
    Rectangle local_rect = gui->getRectangle();
    Matrix4 transform = gui->getDrawTransform();
    Color c = gui->getColor() * parent_color;
    
    
    // Video
    if ( _video_material->isValid() ) {
        DrawBatcherQuads b;
        
        b.batchBegin(_video_material->get(), transform, DrawBatcherQuads::FMT_V | DrawBatcherQuads::FMT_T1 | DrawBatcherQuads::FMT_C);
        
        b.vertex(Vector3(local_rect.getMinusX(), local_rect.getMinusY(), 0.0F), Texcoord2(0.0F,0.0F), c);
        b.vertex(Vector3(local_rect.getPlusX(), local_rect.getMinusY(), 0.0F), Texcoord2(1.0F,0.0F), c);
        b.vertex(Vector3(local_rect.getPlusX(), local_rect.getPlusY(), 0.0F), Texcoord2(1.0F,1.0F), c);
        b.vertex(Vector3(local_rect.getMinusX(), local_rect.getPlusY(), 0.0F), Texcoord2(0.0F,1.0F), c);
        
        b.batchEnd();
    }

    // Background
    if ( _background->isValid() ) {
        DrawBatcherQuads b;
        Rectangle r = calcBackgroundRect(local_rect);
        
        b.batchBegin(_background->get(), transform, DrawBatcherQuads::FMT_V | DrawBatcherQuads::FMT_T1 | DrawBatcherQuads::FMT_C);
        
        b.vertex(Vector3(r.getMinusX(), r.getMinusY(), 0.0F), Texcoord2(0.0F,0.0F), c);
        b.vertex(Vector3(r.getPlusX(), r.getMinusY(), 0.0F), Texcoord2(1.0F,0.0F), c);
        b.vertex(Vector3(r.getPlusX(), r.getPlusY(), 0.0F), Texcoord2(1.0F,1.0F), c);
        b.vertex(Vector3(r.getMinusX(), r.getPlusY(), 0.0F), Texcoord2(0.0F,1.0F), c);
        
        b.batchEnd();
    }

    // Play/Pause
    if ( _play_material->isValid() && _pause_material->isValid()) {
        DrawBatcherQuads b;
        Rectangle r = calcPlayPauseRect(local_rect);
        
        if (Video::getState(_hw) == Video::STATE_PLAYING)
            b.batchBegin(_pause_material->get(), transform, DrawBatcherQuads::FMT_V | DrawBatcherQuads::FMT_T1 | DrawBatcherQuads::FMT_C);
        else
            b.batchBegin(_play_material->get(), transform, DrawBatcherQuads::FMT_V | DrawBatcherQuads::FMT_T1 | DrawBatcherQuads::FMT_C);
        
        b.vertex(Vector3(r.getMinusX(), r.getMinusY(), 0.0F), Texcoord2(0.0F,0.0F), c);
        b.vertex(Vector3(r.getPlusX(), r.getMinusY(), 0.0F), Texcoord2(1.0F,0.0F), c);
        b.vertex(Vector3(r.getPlusX(), r.getPlusY(), 0.0F), Texcoord2(1.0F,1.0F), c);
        b.vertex(Vector3(r.getMinusX(), r.getPlusY(), 0.0F), Texcoord2(0.0F,1.0F), c);
        
        b.batchEnd();
    }

    // Track
    if ( _track_unloaded->isValid() ) {
        DrawBatcherQuads b;
        Rectangle r = calcTrackRect(local_rect);
        
        b.batchBegin(_track_unloaded->get(), transform, DrawBatcherQuads::FMT_V | DrawBatcherQuads::FMT_T1 | DrawBatcherQuads::FMT_C);
        
        b.vertex(Vector3(r.getMinusX(), r.getMinusY(), 0.0F), Texcoord2(0.0F,0.0F), c);
        b.vertex(Vector3(r.getPlusX(), r.getMinusY(), 0.0F), Texcoord2(1.0F,0.0F), c);
        b.vertex(Vector3(r.getPlusX(), r.getPlusY(), 0.0F), Texcoord2(1.0F,1.0F), c);
        b.vertex(Vector3(r.getMinusX(), r.getPlusY(), 0.0F), Texcoord2(0.0F,1.0F), c);
        
        b.batchEnd();
    }

    // Thumb
    if ( _thumb_material->isValid() ) {
        DrawBatcherQuads b;
        Rectangle r = calcThumbRect(local_rect, _current_time, _length);
        
        b.batchBegin(_thumb_material->get(), transform, DrawBatcherQuads::FMT_V | DrawBatcherQuads::FMT_T1 | DrawBatcherQuads::FMT_C);
        
        b.vertex(Vector3(r.getMinusX(), r.getMinusY(), 0.0F), Texcoord2(0.0F,0.0F), c);
        b.vertex(Vector3(r.getPlusX(), r.getMinusY(), 0.0F), Texcoord2(1.0F,0.0F), c);
        b.vertex(Vector3(r.getPlusX(), r.getPlusY(), 0.0F), Texcoord2(1.0F,1.0F), c);
        b.vertex(Vector3(r.getMinusX(), r.getPlusY(), 0.0F), Texcoord2(0.0F,1.0F), c);
        
        b.batchEnd();
    }
    
    
    
}

//==============================================================================
//==============================================================================

void ComponentGUIVideoPlayer::touchesBegan (GUITouchEvent *event)
{
    GUIObject *gui = checkedCast<GUIObject*>(getOwner());
    if (!gui)
        return;
    
}

void ComponentGUIVideoPlayer::touchesMoved (GUITouchEvent *event)
{
    GUIObject *gui = checkedCast<GUIObject*>(getOwner());
    if (!gui)
        return;
    
}

void ComponentGUIVideoPlayer::touchesEnded (GUITouchEvent *event)
{
    GUIObject *gui = checkedCast<GUIObject*>(getOwner());
    if (!gui)
        return;
    
    if (!_hw)
        return;
    
    // Transform touches into widget coordinates
    Vector2 pos = gui->positionToObjectCoord(event->getPosition());
    Rectangle local_rect = gui->getRectangle();

    //
    // Play/Pause
    //
    
    Rectangle play_pause = calcPlayPauseRect(local_rect);
    if (play_pause.isTouching(pos)) {
        
        if (Video::getState(_hw) == Video::STATE_PLAYING) {
            Video::pause(_hw);
        } else {
            Video::play(_hw);
        }
        
        return;
    }
    
    //
    // Timeline
    //
    
    Rectangle seek_rect = calcSeekRect(local_rect);
    if (seek_rect.isTouching(pos)) {
        Rectangle track_rect = calcTrackRect(local_rect);

        DTfloat t = MoreMath::clampZeroOne( (pos.x - track_rect.getMinusX()) / (track_rect.getPlusX() - track_rect.getMinusX()) );
        Video::seek(_hw, t * Video::length(_hw));
        
        return;
    }
    
}

void ComponentGUIVideoPlayer::touchesCancelled (GUITouchEvent *event)
{
    GUIObject *gui = checkedCast<GUIObject*>(getOwner());
    if (!gui)
        return;

}

//==============================================================================
//==============================================================================

void ComponentGUIVideoPlayer::addToOwner (ObjectBase *owner)
{
    ComponentBase::addToOwner(owner);
    
    getOwner()->getWorld()->registerForTick(owner, makeCallback(this, &type::tick));

    GUIObject *gui = checkedCast<GUIObject*>(owner);
    if (gui) {
        gui->getTouchesBeganCallbacks().add(makeCallback(this, &type::touchesBegan));
        gui->getTouchesMovedCallbacks().add(makeCallback(this, &type::touchesMoved));
        gui->getTouchesEndedCallbacks().add(makeCallback(this, &type::touchesEnded));
        gui->getTouchesCancelledCallbacks().add(makeCallback(this, &type::touchesCancelled));

        gui->getDrawGUICallbacks().add(makeCallback(this, &ComponentGUIVideoPlayer::draw));
    }
}

void ComponentGUIVideoPlayer::removeFromOwner (void)
{
    GUIObject *gui = checkedCast<GUIObject*>(getOwner());
    if (gui) {
        gui->getTouchesBeganCallbacks().remove(makeCallback(this, &type::touchesBegan));
        gui->getTouchesMovedCallbacks().remove(makeCallback(this, &type::touchesMoved));
        gui->getTouchesEndedCallbacks().remove(makeCallback(this, &type::touchesEnded));
        gui->getTouchesCancelledCallbacks().remove(makeCallback(this, &type::touchesCancelled));

        gui->getDrawGUICallbacks().remove(makeCallback(this, &ComponentGUIVideoPlayer::draw));
    }

    getOwner()->getWorld()->unregisterForTick(getOwner(), makeCallback(this, &type::tick));

    ComponentBase::removeFromOwner();
}

//==============================================================================
//==============================================================================

} // DT3

