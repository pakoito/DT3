#ifndef DT3_ASSERT
#define DT3_ASSERT
//==============================================================================
///	
///	File: Assert.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <memory>

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

template <typename... T> class Callback;

//==============================================================================
/// Class
//==============================================================================

class AssertImpl {
 	private:
                                AssertImpl              (void);
                                AssertImpl              (const AssertImpl &rhs);
        AssertImpl&             operator =              (const AssertImpl &rhs);
		virtual                 ~AssertImpl             (void);

	public:
    
        //
        // Callbacks
        //
        
        static void             set_callback_assert_msg (std::shared_ptr<Callback<const DTcharacter*, const DTcharacter*, DTint>> cb);
        static void             call_assert_msg         (const DTcharacter* file, const DTcharacter* func, DTint line);
        static void             call_assert_break       (const DTcharacter* file, const DTcharacter* func, DTint line)  __attribute__ ((noreturn));

    private:
        static std::shared_ptr<Callback<const DTcharacter*, const DTcharacter*, DTint>> _callback_assert_msg;

};
    
//==============================================================================
//==============================================================================

} // DT3

#ifdef DT3_DEBUG
	#if DT3_OS == DT3_WINDOWS
		#define ASSERT(cond)    if (!(cond))        DT3::AssertImpl::call_assert_msg((DT3::DTcharacter*)__FILE__, (DT3::DTcharacter*)__FUNCTION__, (DT3::DTint)__LINE__)
	#else
		#define ASSERT(cond)    if (!(cond))        DT3::AssertImpl::call_assert_msg((DT3::DTcharacter*)__FILE__, (DT3::DTcharacter*)__func__, (DT3::DTint)__LINE__)
	#endif

	#if DT3_OS == DT3_WINDOWS
		#define ASSERTBREAK(cond)   if (!(cond))    DT3::AssertImpl::call_assert_break((DT3::DTcharacter*)__FILE__, (DT3::DTcharacter*)__FUNCTION__, (DT3::DTint)__LINE__)
	#else
		#define ASSERTBREAK(cond)   if (!(cond))    DT3::AssertImpl::call_assert_break((DT3::DTcharacter*)__FILE__, (DT3::DTcharacter*)__func__, (DT3::DTint)__LINE__)
	#endif

#else
    #define ASSERT(cond)
    #define ASSERTBREAK(cond)
#endif


#endif
