#ifndef DT2_DEVICEGLOBALSMANAGERWINDOWS8
#define DT2_DEVICEGLOBALSMANAGERWINDOWS8
//==============================================================================
///	
///	File: DeviceGlobalsManagerWindows8.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DeviceGlobalsManager.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
//==============================================================================

class DeviceGlobalsManagerWindows8: public DeviceGlobalsManager {
		/// Base class and default implementation for Globals Manager.

    public:
        DEFINE_TYPE(DeviceGlobalsManagerWindows8,DeviceGlobalsManager)
		DEFINE_CREATE
         
									DeviceGlobalsManagerWindows8	(void);	
	private:
									DeviceGlobalsManagerWindows8	(const DeviceGlobalsManagerWindows8 &rhs);
        DeviceGlobalsManagerWindows8 &		operator =				(const DeviceGlobalsManagerWindows8 &rhs);	
    public:
        virtual						~DeviceGlobalsManagerWindows8	(void);
    
	public:
       
		/// save all of the globals
		void						save					(void);
	
};

//==============================================================================
//==============================================================================


} // DT2

#endif
