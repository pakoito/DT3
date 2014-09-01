#ifndef DT3_MEMORYALLOCATOR
#define DT3_MEMORYALLOCATOR
//==============================================================================
///	
///	File: MemoryAllocator.hpp
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
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class MemoryAllocator {
    public:
        DEFINE_TYPE_SIMPLE_BASE(MemoryAllocator)

    private:
									MemoryAllocator			(void);	
									MemoryAllocator			(const MemoryAllocator &rhs);
        MemoryAllocator &			operator =				(const MemoryAllocator &rhs);	
        virtual						~MemoryAllocator		(void);
    
	public:
		
		/// allocate memory
		/// \param size size of the chunk of memory to allocate
		static void*				allocate				(const DTsize size, const DTcharacter *name = NULL);

		/// allocate memory
		/// \param size size of the chunk of memory to allocate
		static void					deallocate				(void *p);

		/// init Routine
		static void					init					(void);
		
		/// exit Routine
		static void					exit					(void);

    private:
		static DTsize               _total_allocs;

};

//==============================================================================
//==============================================================================


} // DT3

#endif
