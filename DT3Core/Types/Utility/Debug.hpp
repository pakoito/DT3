#ifndef DT3__DEBUG
#define DT3__DEBUG
//==============================================================================
///	
///	File: Debug.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

#ifdef DT3_DEBUG

	#if DT3_COMPILER == DT3_GCC

		#if DT3_CPU == DT3_INTEL
			#define DT3_DEBUG_BREAK {__asm__("int $3\n" : : );}
		#else
			#define DT3_DEBUG_BREAK
		#endif

	#elif DT3_COMPILER == DT3_VISUALC
        #define DT3_DEBUG_BREAK     {	asm { int 3 };	}
	#else
		#define DT3_DEBUG_BREAK
	#endif	// DT3_COMPILER
	
#else
	#define DT3_DEBUG_BREAK

#endif	// DT3_DEBUG


//==============================================================================
//==============================================================================

} // DT3

#endif


