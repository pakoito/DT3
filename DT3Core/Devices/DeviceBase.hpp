#ifndef DT3_DEVICEBASE
#define DT3_DEVICEBASE
//==============================================================================
///	
///	File: DeviceBase.hpp
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
/// Class
//==============================================================================

class DeviceBase: public BaseClass {
		/// Base class for all drivers.

    public:
        DEFINE_TYPE(DeviceBase,BaseClass)
		//DEFINE_CREATE
         
								DeviceBase				(void);	
	private:
								DeviceBase				(const DeviceBase &rhs);
        DeviceBase &			operator =				(const DeviceBase &rhs);	
    public:
        virtual					~DeviceBase             (void);
   
};

//==============================================================================
//==============================================================================

} // DT3

#endif
