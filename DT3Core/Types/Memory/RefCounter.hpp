#ifndef DT3_REF_COUNTER
#define DT3_REF_COUNTER
//==============================================================================
///	
///	File: Callback.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/CheckedCast.hpp"
#include <atomic>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Handy class for ref counting in callbacks. Allows you to maintain your own
/// ref count for an object.
//==============================================================================

template <typename T>
class RefCounter {
    public:
        RefCounter  (const T &obj)
            :   _ref_count  (1),
                _obj        (obj)
        {}
    
        ~RefCounter (void)
        {}
    
    public:

		/// Returns managed object
		/// \return object Object
        const T&    get         (void) const
        {
            return _obj;
        }
    
        /// Retains the object
        void        retain      (void)
        {
            _ref_count.fetch_add(1, std::memory_order_relaxed);
        }

        /// Releases the object. If ref count goes to zero, then delete this is called
        void        release     (void)
        {
            _ref_count.fetch_add(-1, std::memory_order_relaxed);
            if (_ref_count == 0) {
                delete this;
            }
        }

    private:
        std::atomic<DTint>  _ref_count;
        T                   _obj;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
