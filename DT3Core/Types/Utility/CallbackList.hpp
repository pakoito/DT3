#ifndef DT3_CALLBACKLIST
#define DT3_CALLBACKLIST
//==============================================================================
///	
///	File: CallbackList.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/Callback.hpp"
#include <list>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

template <typename... T>
class CallbackList {
    public:
        CallbackList(void)
        {
        }
        
        ~CallbackList(void)
        {

        }
    
        void add (std::shared_ptr<Callback<T...>> cb)
        {
            for (auto &i : _callbacks) {
                if (*i == *cb) {
                    return;
                }
            }            

            _callbacks.push_back(cb);
        }
    
        void remove (std::shared_ptr<Callback<T...>> cb)
        {
            for (auto i = _callbacks.begin(); i != _callbacks.end(); ++i) {
                if (**i == *cb) {
                    _callbacks.erase(i);
                    return;
                }
            }            
        }
        
        void fire (T... t)
        {
            for (auto &i : _callbacks)
                (*i)(t...);
        }
        
        DTsize size (void) const  {   return _callbacks.size();   }

    private:
        std::list<std::shared_ptr<Callback<T...>>> _callbacks;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
