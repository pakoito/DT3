//==============================================================================
///	
///	File: MemoryAllocator.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Memory/MemoryAllocator.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/Types/Utility/Error.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

DTsize MemoryAllocator::_total_allocs = 0;

//==============================================================================
//==============================================================================

void MemoryAllocator::init (void)
{
	_total_allocs = 0;
}
		
void MemoryAllocator::exit (void)
{
	LOG_MESSAGE << "---- MEMORY DUMP -------------------------------------------------------------";
	LOG_MESSAGE << "Total Allocations:   " << _total_allocs << " bytes (" << _total_allocs/(1024.0F*1024.0F) << " MB)";
}

//==============================================================================
//==============================================================================

void* MemoryAllocator::allocate (const DTsize size, const DTcharacter *name)
{    
	void *p = NULL;
	
#if DT3_OS != DT3_ANDROID
    ::posix_memalign(&p, DT3_ALIGN, size);
#else
	p = std::malloc( (size_t) size);
#endif
    
	// Attempt emergency dump of resources if failed
	if (!p)	{
		SystemCallbacks::emergency_free_memory_cb().fire();

#if DT3_OS != DT3_ANDROID
        ::posix_memalign(&p, DT3_ALIGN, size);
#else
		p = std::malloc( (size_t) size);
#endif
	}
	
	// If still not able to allocate memory, we fail
	if (!p) {
        ERRORMSG("Allocation failed!");
        ::exit(1);
    }
	
    // Record stats
    _total_allocs += size;
    
	return p; 
}

void MemoryAllocator::deallocate (void *p)
{
	std::free(p);
}

//==============================================================================
//==============================================================================

} // DT3
