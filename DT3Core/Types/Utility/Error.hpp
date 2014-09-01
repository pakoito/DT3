#ifndef DT3_ERROR
#define DT3_ERROR
//==============================================================================
///	
///	File: Error.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

template <typename... T> class Callback;

//==============================================================================
/// Class
//==============================================================================

class ErrorImpl {
 	private:
                            ErrorImpl                   (void);
                            ErrorImpl                   (const ErrorImpl &rhs);
        ErrorImpl&          operator =                  (const ErrorImpl &rhs);
		virtual             ~ErrorImpl                  (void);

	public:
    
        //
        // Callbacks
        //
        
        static void         set_callback_error_msg      (std::shared_ptr<Callback<const DTcharacter*, const DTcharacter*, DTint, const DTcharacter*>> cb);
        static void         call_error_msg              (const DTcharacter* file, const DTcharacter* func, DTint line, const DTcharacter *msg);

        static void         set_callback_warning_msg    (std::shared_ptr<Callback<const DTcharacter*, const DTcharacter*, DTint, const DTcharacter*>> cb);
        static void         call_warning_msg            (const DTcharacter* file, const DTcharacter* func, DTint line, const DTcharacter *msg);

    private:
        static std::shared_ptr<Callback<const DTcharacter*, const DTcharacter*, DTint, const DTcharacter *>>  _callback_error_msg;
        static std::shared_ptr<Callback<const DTcharacter*, const DTcharacter*, DTint, const DTcharacter *>>  _callback_warning_msg;

};
    
//==============================================================================
//==============================================================================

} // DT3


#if DT3_OS == DT3_WINDOWS
    #define ERROR(cond, msg)    if (!(cond))        ErrorImpl::call_error_msg   ((DT3::DTcharacter*)__FILE__, (DT3::DTcharacter*)__FUNCTION__, (DT3::DTint)__LINE__, msg)
#else
    #define ERROR(cond, msg)    if (!(cond))        ErrorImpl::call_error_msg   ((DT3::DTcharacter*)__FILE__, (DT3::DTcharacter*)__func__, (DT3::DTint)__LINE__, msg)
#endif


#if DT3_OS == DT3_WINDOWS
    #define ERRORMSG(msg)       ErrorImpl::call_error_msg ((DT3::DTcharacter*)__FILE__, (DT3::DTcharacter*)__FUNCTION__, (DT3::DTint)__LINE__, msg)
#else
    #define ERRORMSG(msg)       ErrorImpl::call_error_msg ((DT3::DTcharacter*)__FILE__, (DT3::DTcharacter*)__func__, (DT3::DTint)__LINE__, msg)
#endif


#if DT3_OS == DT3_WINDOWS
    #define WARNING(cond, msg)  if (!(cond))        ErrorImpl::call_warning_msg ((DT3::DTcharacter*)__FILE__, (DT3::DTcharacter*)__FUNCTION__, (DT3::DTint)__LINE__, msg)
#else
    #define WARNING(cond, msg)  if (!(cond))        ErrorImpl::call_warning_msg ((DT3::DTcharacter*)__FILE__, (DT3::DTcharacter*)__func__, (DT3::DTint)__LINE__, msg)
#endif


#if DT3_OS == DT3_WINDOWS
    #define WARNINGMSG(msg)     ErrorImpl::call_warning_msg ((DT3::DTcharacter*)__FILE__, (DT3::DTcharacter*)__FUNCTION__, (DT3::DTint)__LINE__, msg)
#else
    #define WARNINGMSG(msg)     ErrorImpl::call_warning_msg ((DT3::DTcharacter*)__FILE__, (DT3::DTcharacter*)__func__, (DT3::DTint)__LINE__, msg)
#endif


#endif
