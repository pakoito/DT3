#ifndef DT3_PROGRESS
#define DT3_PROGRESS
//==============================================================================
///	
///	File: Progress.hpp
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

class Progress: public BaseClass {
    public:
        DEFINE_TYPE(Progress,BaseClass)
		DEFINE_CREATE_AND_CLONE
         
										Progress			(void);	
										Progress			(const Progress &rhs);
        Progress &						operator =			(const Progress &rhs);
        virtual							~Progress			(void);

	public:
		
		/// Called when progress has started a new process
		/// \param current current progress
		/// \param length total progress length
		virtual void					update				(DTsize current, DTsize length)		{}
		
		/// Called when progress has started a new process
		virtual void					started				(void)		{}
		
};

//==============================================================================
//==============================================================================


} // DT3

#endif
