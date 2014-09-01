#ifndef DT3_WORLDENTRY
#define DT3_WORLDENTRY
//==============================================================================
///	
///	File: WorldEntry.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/Callback.hpp"
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class WorldNode;
class TouchEvent;
class CameraObject;

//==============================================================================
/// Base object for the different placeable types of objects in the engine.
//==============================================================================

template <class CB>
class WorldEntry {
    public:
    
        WorldEntry (WorldNode *node, const std::shared_ptr<CB> &cb)
            :   _node   (node),
                _cb     (cb)
        {}

        WorldEntry (const WorldEntry<CB> &rhs)
            :   _node   (rhs._node),
                _cb     (rhs._cb)
        {}

        WorldEntry& operator = (const WorldEntry<CB> &rhs)
        {
            _node = rhs._node;
            _cb = rhs._cb;
            
            return *this;
        }

        ~WorldEntry (void)  {}

    public:
        DTboolean       operator ==         (const WorldEntry<CB> &rhs) const
        {
            return (_node == rhs._node) && (*_cb == *rhs._cb);
        }
    
        template <typename... ARGS>
        void            operator ()         (ARGS... args)
        {
            (*_cb)(args...);
        }
    
        WorldNode*          node    (void)          {   return _node;   }
        const WorldNode*    node    (void) const    {   return _node;   }
    
    private:
        WorldNode               *_node;
        std::shared_ptr<CB>     _cb;
        
};

//==============================================================================
//==============================================================================

typedef Callback<const std::shared_ptr<CameraObject> &, DTfloat> DrawCallbackType;
typedef Callback<DTfloat> TickCallbackType;
typedef Callback<const TouchEvent*, const std::shared_ptr<CameraObject> &, WorldNode*&> TouchCallbackType;

typedef WorldEntry<DrawCallbackType> DrawCallbackEntry;
typedef WorldEntry<TickCallbackType> TickCallbackEntry;
typedef WorldEntry<TouchCallbackType> TouchCallbackEntry;

//==============================================================================
//==============================================================================

} // DT3

#endif
