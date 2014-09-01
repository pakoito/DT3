#ifndef DT3_THREADTASKREF
#define DT3_THREADTASKREF
//==============================================================================
///	
///	File: ThreadTaskRef.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class ThreadTaskRef: public BaseClass {
    public:
        DEFINE_TYPE(ThreadTaskRef, BaseClass)
		DEFINE_CREATE
    
	public:
                                    ThreadTaskRef  (void)  {   _status = STATUS_NONE;  }
    
    private:
                                    ThreadTaskRef  (const ThreadTaskRef &rhs);
        ThreadTaskRef &        operator =          (const ThreadTaskRef &rhs);
    
	public:
        virtual                     ~ThreadTaskRef (void)  {}

    public:
    
        enum Status {
            STATUS_NONE,
            STATUS_QUEUED,
            STATUS_RUNNING,
            STATUS_COMPLETE,
            STATUS_CANCELLED
        };
    
        DEFINE_ACCESSORS(status, set_status, Status, _status)
    
    private:
        Status _status;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
