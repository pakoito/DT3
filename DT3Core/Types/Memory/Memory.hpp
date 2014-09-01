#ifndef DT3_MEMORY
#define DT3_MEMORY
//==============================================================================
///	
///	File: Memory.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include <new>
#include <cstdlib>

//==============================================================================
// Override new and delete
//==============================================================================

void*	operator new		(size_t size);
void*	operator new		(size_t size, const char *name);

void*	operator new[]		(size_t size);
void*	operator new[]		(size_t size, const char *name);

void	operator delete		(void *p) noexcept (true);
void	operator delete		(void *p, const char *name) noexcept (true);

void	operator delete[]	(void *p) noexcept (true);
void	operator delete[]	(void *p, const char *name) noexcept (true);

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class Memory {
		/// Memory Manager for managing heap allocations.

	private:
                            Memory              (void);	
                            Memory              (const Memory &rhs);
        Memory&             operator =          (const Memory &rhs);
        virtual             ~Memory             (void);

    public:
    
		/// Static initializer for memory manager
        static void         initialize_static   (void);
    
		/// Static uninitializer for memory manager
        static void         destroy_static      (void);
    
		/// allocate memory
		/// \param size size of the chunk of memory to allocate
		static void*        allocate            (const size_t size, const char *name = NULL);

		/// allocate memory
		/// \param size size of the chunk of memory to allocate
		static void         deallocate          (void *p);

};

//==============================================================================
//==============================================================================

} // DT3

#endif


