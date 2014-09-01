#ifndef DT3_MEMORYALLOCATORTRACE
#define DT3_MEMORYALLOCATORTRACE
//==============================================================================
///	
///	File: MemoryAllocatorTrace.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <mutex>
#include <map>
#include <cstdlib>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
// An STL allocator that uses malloc so that it doesn't interfere with our
// new and delete overrides which would cause an endless loop.
//==============================================================================
	
template< typename T > struct MallocAllocator : public std::allocator<T>
{
	typedef std::allocator<T> base ;
	typedef typename base::size_type size_type;
	typedef typename base::difference_type  difference_type;
	typedef typename base::pointer pointer;
	typedef typename base::const_pointer const_pointer;
	typedef typename base::reference reference;
	typedef typename base::const_reference const_reference;
	typedef typename base::value_type value_type;
    
	MallocAllocator() throw() {}
	MallocAllocator( const MallocAllocator& a ) throw() : base(a) {}
	template <typename X> MallocAllocator( const MallocAllocator<X>& ) throw() {}
	~MallocAllocator() throw() {}
	
	template <typename X> struct rebind
    {
        typedef MallocAllocator<X> other;
    };
	
	pointer allocate( size_type sz, const void* hint = 0 )
	{
		return reinterpret_cast<pointer>(::malloc(sizeof(T)));
	}
    
	void deallocate( pointer p, size_type n )
	{
		return ::free(p);
	}
};

template<typename T> inline bool operator==(const MallocAllocator<T>&, const MallocAllocator<T>& )	{ return true; }
template<typename T> inline bool operator!=(const MallocAllocator<T>&, const MallocAllocator<T>& )	{ return false; }

//==============================================================================
/// Class
//==============================================================================

class MemoryAllocatorTrace {
    public:
        DEFINE_TYPE_SIMPLE_BASE(MemoryAllocatorTrace)

    private:
									MemoryAllocatorTrace	(void);	
									MemoryAllocatorTrace	(const MemoryAllocatorTrace &rhs);
        MemoryAllocatorTrace &		operator =				(const MemoryAllocatorTrace &rhs);	
        virtual						~MemoryAllocatorTrace	(void);
    
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
    
        /// check allocations
        static void                 check_allocations       (void);

        /// check allocations
        static void                 enable_check_allocations(DTboolean check);

    private:
    
    
        static std::mutex           _alloc_mutex;
        static DTboolean            _alloc_recording;
        static DTboolean            _alloc_checking;

        static const DTuint MAGIC = 0xDEADBEEF;

        struct Header {
            DTuint magic;
            DTsize size;
        };
    
        struct Footer {
            DTuint magic;
        };

        struct Allocation {
            DTcharacter*	_name;
            DTsize			_size;
        };
    

		static DTsize               _total_allocs;
		static DTsize               _current_allocs;
		static DTsize               _peak_allocs;
   
        typedef std::map<
            void*,
            Allocation,
            std::less<void*>,
            MallocAllocator<std::pair<void* const, MemoryAllocatorTrace::Allocation>>
        > allocations_map_type;
    
        static allocations_map_type&    allocations         (void);
    
        static void                     record_allocation   (void* p, const DTcharacter *name, DTsize size);
        static void                     record_deallocation (void* p);

};

//==============================================================================
//==============================================================================


} // DT3

#endif
