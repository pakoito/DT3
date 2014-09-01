//==============================================================================
///	
///	File: Memory.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Memory/Memory.hpp"
#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/System/StaticInitializer.hpp"

#if defined(DT3_DEBUG) && (DT3_OS == DT3_MACOSX)
    #include "DT3Core/Types/Memory/MemoryAllocatorTrace.hpp"
    #define MEMORY_MANAGER	DT3::MemoryAllocatorTrace
#else
    #include "DT3Core/Types/Memory/MemoryAllocator.hpp"
    #define MEMORY_MANAGER	DT3::MemoryAllocator
#endif

#include <new>

//==============================================================================
// Override new and delete
//==============================================================================

void*	operator new	(size_t size)
{
	return MEMORY_MANAGER::allocate(size,NULL);;
}
																
void*	operator new	(size_t size, const char *name)
{
	return MEMORY_MANAGER::allocate(size,name);;
}

//==============================================================================
//==============================================================================

void*	operator new[]	(size_t size)
{
	return MEMORY_MANAGER::allocate(size,NULL);
}
																
void*	operator new[]	(size_t size, const char *name)
{
	return MEMORY_MANAGER::allocate(size,name);
}

//==============================================================================
//==============================================================================
																
void	operator delete	(void *p) noexcept (true)
{			
	MEMORY_MANAGER::deallocate(p);
}	

void	operator delete	(void *p, const char *name) noexcept (true)
{			
	MEMORY_MANAGER::deallocate(p);
}	

//==============================================================================
//==============================================================================

void	operator delete[] (void *p) noexcept (true)
{			
	MEMORY_MANAGER::deallocate(p);
}	

void	operator delete[] (void *p, const char *name) noexcept (true)
{			
	MEMORY_MANAGER::deallocate(p);
}	

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,Memory::initialize_static())
GLOBAL_STATIC_DESTRUCTION(0,Memory::destroy_static())

//==============================================================================
//==============================================================================

void	Memory::initialize_static (void)
{
	MEMORY_MANAGER::init();
}

void	Memory::destroy_static (void)
{
	MEMORY_MANAGER::exit();
}

//==============================================================================
//==============================================================================

void*	Memory::allocate (const size_t size, const char *name)
{
    return MEMORY_MANAGER::allocate(size,name);
}

void	Memory::deallocate (void *p)
{
    MEMORY_MANAGER::deallocate(p);
}

//==============================================================================
//==============================================================================

} // DT3

