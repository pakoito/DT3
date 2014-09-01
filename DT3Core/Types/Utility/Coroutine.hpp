#ifndef DT3_COROUTINE
#define DT3_COROUTINE
//==============================================================================
///	
///	File: Coroutine.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/ContextSwitcher.hpp"
#include "DT3Core/Types/Utility/TimerHires.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include <vector>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

template <class T>
class Coroutine {
    public:
        DEFINE_TYPE_SIMPLE_BASE(Coroutine)
    
        typedef void (T::*CB)(Coroutine<T>*);

									Coroutine           (T *object, CB cb, DTuint stack_size);
	
	private:
									Coroutine           (const Coroutine &rhs);
        Coroutine &                 operator =          (const Coroutine &rhs);
    
    public:
                                    ~Coroutine          (void)  {}

	public:
                
		/// Resume executing coroutine (NEVER CALLED FROM COROUTINE!!)
        void                        resume              (void);
        
		/// Yield currently running coroutine (CALLED FROM COROUTINE ONLY!!)
		/// \param yield_time Time to yield for
        void                        yield               (DTfloat yield_time = 0.0F);
                
    private:
        static void                 coroutine_helper    (void *data);
    
        CB                          _cb;
        T                           *_object;
    
        std::vector<DTubyte>        _stack;
        
        uContext                    _context;
        uContext                    _link_context;
        
        TimerHires                  _yield_timer;
        DTfloat                     _yield_time;
};

//==============================================================================
//==============================================================================

template <class T>
Coroutine<T>::Coroutine (T *object, Coroutine<T>::CB cb, DTuint stack_size)
{
    _object = object;
    _cb = cb;
    
    // Resize the stack
    _stack.resize(stack_size);
    
    // Set bottom memory to known value
    *((DTuint*)(&_stack[0])) = 0xDEADBEEF;
    
    ::memset(&_context,0,sizeof(_context));
    ::memset(&_link_context,0,sizeof(_link_context));
    
    // Set up the context for the coroutine
    ContextSwitcher::context(&_context);

    _context.uc_stack_sp    = (&_stack[0]) + 4;   // Special pattern for detecting overflows
    _context.uc_stack_ss    = stack_size - 4;     // Special pattern for detecting overflows
    
    ContextSwitcher::make_context(&_context, &Coroutine<T>::coroutine_helper, this);
    
    _yield_timer.reset_abs_time();
    _yield_timer.delta_time();
    _yield_time = 0.0F;

}

//==============================================================================
//==============================================================================

template <class T>
void Coroutine<T>::resume (void)
{
    ASSERT( *((DTuint*)(&_stack[0])) == 0xDEADBEEF );

	DTfloat dt = static_cast<DTfloat>(_yield_timer.delta_time());
    if (dt > DT3_MAX_TICK)
        dt = DT3_MAX_TICK;

    _yield_time -= dt;
    //LOG_MESSAGE << "Yield Time: " << _yield_time;

    if (_yield_time <= 0.0F) {
        ContextSwitcher::swap_context(&_link_context, &_context);
    }
    
    ASSERT( *((DTuint*)(&_stack[0])) == 0xDEADBEEF );
}

template <class T>
void Coroutine<T>::yield (DTfloat yield_time)
{
    ASSERT( *((DTuint*)(&_stack[0])) == 0xDEADBEEF );

    //LOG_MESSAGE << "Yield: " << yield_time;

    _yield_time = yield_time;

    ContextSwitcher::swap_context(&_context, &_link_context);

    ASSERT( *((DTuint*)(&_stack[0])) == 0xDEADBEEF );
}

//==============================================================================
//==============================================================================

template <class T>
void Coroutine<T>::coroutine_helper (void *data)
{
    Coroutine<T> *co = (Coroutine<T> *) data;

    ((co->_object)->*(co->_cb))(co);
    
    // If you made it this far, then BAD! Coroutines should never exit.
    ASSERT(0);
}

//==============================================================================
//==============================================================================


} // DT3

#endif
