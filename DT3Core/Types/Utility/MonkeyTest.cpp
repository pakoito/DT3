//==============================================================================
///	
///	File: MonkeyTest.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/MonkeyTest.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/Devices/DeviceGraphics.hpp"
#include "DT3Core/Entry/GameMainThread.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

MonkeyTest::MonkeyTest (void)
    :   _state          (STATE_START_TAP),
        _time           (0.0F),
        _time_remaining (0.0F),
        _position_1     (0.0F,0.0F),
        _position_2     (0.0F,0.0F)
{
    _event.clear();
    new_event();
}

MonkeyTest::~MonkeyTest (void)
{

}

//==============================================================================
//==============================================================================

void MonkeyTest::new_event (void)
{
    DTuint event = MoreMath::random_int() % 3;
    
    switch (event) {
        case 0: _state = STATE_START_TAP;   break;
        case 1: _state = STATE_START_DRAG;  break;
        case 2: _state = STATE_START_IDLE;  break;        
    };
    
}

void MonkeyTest::run (void)
{
    DTint width = System::renderer()->screen_width();
    DTint height = System::renderer()->screen_height();

    _time_remaining -= _timer.delta_time();
    
    switch (_state) {
        case STATE_START_TAP: {
                _time_remaining = _time = MoreMath::random_float() * 0.05F;
                
                _position_1 = _position_2 = Vector2(MoreMath::random_float() * width, MoreMath::random_float() * height);
                            
                _event.touches[0].state = DT3::TouchEvent::STATE_PRESSED;
                _event.touches[0].pos = _position_1;
                _event.touches[0].previous_pos = _position_1;
                _event.touches[0].first_pos = _position_1;
                _event.touches[0].delta = DT3::Vector2(0.0F,0.0F);
                _event.touches[0].dt = 0.0F;
                _event.touches[0].velocity = DT3::Vector2(0.0F,0.0F);
                _event.touches[0].timer.delta_time();

                GameMainThread::touch_event(_event);

                _state = STATE_TAPPING;
            } break;
            
        case STATE_TAPPING: {
                if (_time_remaining < 0.0F)
                    _state = STATE_STOP_TAP;
            } break;
            
        case STATE_STOP_TAP: {
                _event.touches[0].state = DT3::TouchEvent::STATE_RELEASED;
                _event.touches[0].pos = _position_1;
                _event.touches[0].previous_pos = _position_1;
                _event.touches[0].delta = DT3::Vector2(0.0F,0.0F);
                _event.touches[0].dt = static_cast<DTfloat>(_event.touches[0].timer.delta_time());
                _event.touches[0].velocity = DT3::Vector2(0.0F,0.0F);

                GameMainThread::touch_event(_event);
                
                new_event();
            } break;
            
            

        case STATE_START_DRAG: {
                _time_remaining = _time = MoreMath::random_float() * 0.05F;
                
                _position_1 = Vector2(MoreMath::random_float() * width, MoreMath::random_float() * height);
                _position_2 = Vector2(MoreMath::random_float() * width, MoreMath::random_float() * height);
                            
                _event.touches[0].state = DT3::TouchEvent::STATE_PRESSED;
                _event.touches[0].pos = _position_1;
                _event.touches[0].previous_pos = _position_1;
                _event.touches[0].first_pos = _position_1;
                _event.touches[0].delta = DT3::Vector2(0.0F,0.0F);
                _event.touches[0].dt = 0.0F;
                _event.touches[0].velocity = DT3::Vector2(0.0F,0.0F);
                _event.touches[0].timer.delta_time();

                GameMainThread::touch_event(_event);

                _state = STATE_DRAGGING;
            } break;
            
        case STATE_DRAGGING: {
        
                DTfloat t = 1.0F - (_time_remaining / _time);
        
                _event.touches[0].state = DT3::TouchEvent::STATE_DOWN;
                _event.touches[0].previous_pos = _event.touches[0].pos;
                _event.touches[0].pos = (_position_2 - _position_1) * t + _position_1;
                _event.touches[0].delta = _event.touches[0].pos - _event.touches[0].previous_pos;
                _event.touches[0].dt = static_cast<DTfloat>(_event.touches[0].timer.delta_time());
                _event.touches[0].velocity = _event.touches[0].delta / _event.touches[0].dt;

                GameMainThread::touch_event(_event);

                if (_time_remaining < 0.0F)
                    _state = STATE_STOP_DRAG;

            } break;
            
        case STATE_STOP_DRAG: {
                _event.touches[0].state = DT3::TouchEvent::STATE_RELEASED;
                _event.touches[0].previous_pos = _event.touches[0].pos;
                _event.touches[0].pos = _position_2;
                _event.touches[0].delta = _event.touches[0].pos - _event.touches[0].previous_pos;
                _event.touches[0].dt = static_cast<DTfloat>(_event.touches[0].timer.delta_time());
                _event.touches[0].velocity = _event.touches[0].delta / _event.touches[0].dt;

                GameMainThread::touch_event(_event);

                new_event();
            } break;
            
            
    
        case STATE_START_IDLE: {
                _time_remaining = _time = MoreMath::random_float() * 0.05F;
                _state = STATE_IDLING;
            } break;

        case STATE_IDLING: {
                if (_time_remaining < 0.0F)
                    _state = STATE_STOP_IDLE;
            } break;
            
        case STATE_STOP_IDLE: {
                new_event();
            } break;


    };
    
    
    
}

//==============================================================================
//==============================================================================

} // DT3
