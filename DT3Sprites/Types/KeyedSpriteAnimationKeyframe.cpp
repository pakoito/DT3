//==============================================================================
///	
///	File: KeyedSpriteAnimationKeyframe.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "KeyedSpriteAnimationKeyframe.hpp"
#include "Archive.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

KeyedSpriteAnimationKeyframe::KeyedSpriteAnimationKeyframe (void)
	:   _keyframe_cache             (0),
        _id                         (0),
        _time                       (0.0F),
		_translation                (0.0F,0.0F),
		_rotation					(0.0F),
		_scale						(1.0F),
		_plus_x						(0.1F),
		_minus_x					(0.1F),
		_plus_y						(0.1F),
		_minus_y					(0.1F),
		_plus_u						(1.0F),
		_minus_u					(0.0F),
		_plus_v						(1.0F),
		_minus_v					(0.0F),
		_visible					(true),
        _order                      (0.0F),
        _grid_size_x                (0),
        _grid_size_y                (0)
{
    
}

KeyedSpriteAnimationKeyframe::KeyedSpriteAnimationKeyframe (const KeyedSpriteAnimationKeyframe &rhs)
	:	_keyframe_cache             (rhs._keyframe_cache),
        _id                         (rhs._id),
        _time                       (rhs._time),
		_translation				(rhs._translation),
		_rotation					(rhs._rotation),
		_scale						(rhs._scale),
		_plus_x						(rhs._plus_x),
		_minus_x					(rhs._minus_x),
		_plus_y						(rhs._plus_y),
		_minus_y					(rhs._minus_y),
		_plus_u						(rhs._plus_u),
		_minus_u					(rhs._minus_u),
		_plus_v						(rhs._plus_v),
		_minus_v					(rhs._minus_v),
		_visible					(rhs._visible),
        _order                      (rhs._order),
        _grid_size_x                (rhs._grid_size_x),
        _grid_size_y                (rhs._grid_size_y),
        _grid                       (rhs._grid)
{

}	
            
KeyedSpriteAnimationKeyframe& KeyedSpriteAnimationKeyframe::operator = (const KeyedSpriteAnimationKeyframe &rhs)
{	
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {		
        _keyframe_cache = rhs._keyframe_cache;
        _id = rhs._id;
		_time = rhs._time;
		_translation = rhs._translation;
		_rotation = rhs._rotation;
		_scale = rhs._scale;
		_plus_x = rhs._plus_x;
		_minus_x = rhs._minus_x;
		_plus_y = rhs._plus_y;
		_minus_y = rhs._minus_y;
		_plus_u = rhs._plus_u;
		_minus_u = rhs._minus_u;
		_plus_v = rhs._plus_v;
		_minus_v = rhs._minus_v;
        
		_visible = rhs._visible;
        _order = rhs._order;
        
        _grid_size_x = rhs._grid_size_x;
        _grid_size_y = rhs._grid_size_y;
        _grid = rhs._grid;

    }
    return (*this);
}	

KeyedSpriteAnimationKeyframe::~KeyedSpriteAnimationKeyframe (void)
{

}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationKeyframe::archiveRead (Archive *archive)
{    
    archive->pushDomain (getClassID ());

	*archive << ARCHIVE_DATA(_time,DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_DATA(_translation,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_rotation,DATA_PERSISTENT);
    *archive << ARCHIVE_DATA(_visible,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_scale,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_plus_x,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_minus_x,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_plus_y,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_minus_y,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_plus_u,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_minus_u,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_plus_v,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_minus_v,DATA_PERSISTENT | DATA_SETTABLE);
    
    if (archive->getVersion() >= 1364) {
        *archive << ARCHIVE_DATA(_order,DATA_PERSISTENT);
    }

    if (archive->getVersion() >= 1358) {
        *archive << ARCHIVE_DATA_ACCESSORS("Grid_Size_X", KeyedSpriteAnimationKeyframe::getGridSizeX, KeyedSpriteAnimationKeyframe::setGridSizeX, DATA_PERSISTENT | DATA_SETTABLE);
        *archive << ARCHIVE_DATA_ACCESSORS("Grid_Size_Y", KeyedSpriteAnimationKeyframe::getGridSizeY, KeyedSpriteAnimationKeyframe::setGridSizeY, DATA_PERSISTENT | DATA_SETTABLE);
    
        for (DTint i = 0; i < _grid.size(); ++i) {
            *archive << ARCHIVE_DATA_RAW(_grid[i], DATA_PERSISTENT);
        }
    
    }

	archive->popDomain ();
}

void KeyedSpriteAnimationKeyframe::archiveWrite (Archive *archive)
{    
    archive->pushDomain (getClassID ());

	*archive << ARCHIVE_DATA(_time,DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_DATA(_translation,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_rotation,DATA_PERSISTENT);
	*archive << ARCHIVE_DATA(_visible,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_scale,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_plus_x,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_minus_x,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_plus_y,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_minus_y,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_plus_u,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_minus_u,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_plus_v,DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_minus_v,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_DATA(_order,DATA_PERSISTENT);

    *archive << ARCHIVE_DATA_ACCESSORS("Grid_Size_X", KeyedSpriteAnimationKeyframe::getGridSizeX, KeyedSpriteAnimationKeyframe::setGridSizeX, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Grid_Size_Y", KeyedSpriteAnimationKeyframe::getGridSizeY, KeyedSpriteAnimationKeyframe::setGridSizeY, DATA_PERSISTENT | DATA_SETTABLE);

    for (DTint i = 0; i < _grid.size(); ++i) {
        *archive << ARCHIVE_DATA_RAW(_grid[i], DATA_PERSISTENT);
    }

	archive->popDomain ();
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationKeyframe::setGridSizeX (DTint size_x)
{
    _grid_size_x = (size_x < 2) ? 0 : size_x;
    allocateGrid();
}
        
DTint KeyedSpriteAnimationKeyframe::getGridSizeX (void) const
{
    return _grid_size_x;
}

void KeyedSpriteAnimationKeyframe::setGridSizeY (DTint size_y)
{
    _grid_size_y = (size_y < 2) ? 0 : size_y;
    allocateGrid();
}
        
DTint KeyedSpriteAnimationKeyframe::getGridSizeY (void) const
{
    return _grid_size_y;
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationKeyframe::allocateGrid (void)
{
    DTint desired_size = _grid_size_x * _grid_size_y;
    
    if (desired_size != _grid.size()) {
        _grid.resize(desired_size);
        
        for (DTint x = 0; x < _grid_size_x; ++x) {
            DTfloat tx = (DTfloat) x / (_grid_size_x - 1);
        
            for (DTint y = 0; y < _grid_size_y; ++y) {
                DTfloat ty = (DTfloat) y / (_grid_size_y - 1);
                
                DTint i = y * _grid_size_x + x;

                _grid[i] = Vector2(tx,ty);
            }
        }
    
    }
    
}

//==============================================================================
//==============================================================================

void KeyedSpriteAnimationKeyframe::flipHorz (void)
{
    _translation.x = -_translation.x;
    _rotation = -_rotation;
    
    std::swap(_plus_x, _minus_x);
    
    for (DTint x = 0; x < _grid_size_x/2; ++x) {
        for (DTint y = 0; y < _grid_size_y; ++y) {
            
            DTint i_source = y * _grid_size_x + x;
            DTint i_dst = y * _grid_size_x + (_grid_size_x - x - 1);
            
            std::swap(_grid[i_source], _grid[i_dst]);
            
            _grid[i_source].x = 1.0F-_grid[i_source].x;
            _grid[i_dst].x = 1.0F-_grid[i_dst].x;
        }
    }

}

void KeyedSpriteAnimationKeyframe::flipVert (void)
{
    _translation.y = -_translation.y;
    _rotation = -_rotation;
    
    std::swap(_plus_y, _minus_y);
    
    for (DTint x = 0; x < _grid_size_x; ++x) {        
        for (DTint y = 0; y < _grid_size_y/2; ++y) {
            
            DTint i_source = y * _grid_size_x + x;
            DTint i_dst = (_grid_size_y - y - 1) * _grid_size_x + x;
            
            std::swap(_grid[i_source], _grid[i_dst]);
            
            _grid[i_source].y = 1.0F-_grid[i_source].y;
            _grid[i_dst].y = 1.0F-_grid[i_dst].y;
        }
    }

}

//==============================================================================
//==============================================================================

} // DT3
