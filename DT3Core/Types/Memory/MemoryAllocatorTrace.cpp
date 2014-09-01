//==============================================================================
///	
///	File: MemoryAllocatorTrace.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Memory/MemoryAllocatorTrace.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/Error.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

std::mutex  MemoryAllocatorTrace::_alloc_mutex;
DTboolean   MemoryAllocatorTrace::_alloc_recording = false;
DTboolean   MemoryAllocatorTrace::_alloc_checking = false;

MemoryAllocatorTrace::allocations_map_type& MemoryAllocatorTrace::allocations(void)
{
    static allocations_map_type allocations;
    return allocations;
}

DTsize MemoryAllocatorTrace::_total_allocs = 0;
DTsize MemoryAllocatorTrace::_current_allocs = 0;
DTsize MemoryAllocatorTrace::_peak_allocs = 0;

//==============================================================================
//==============================================================================

void MemoryAllocatorTrace::record_allocation (void* p, const DTcharacter *name, DTsize size)
{
	if (_alloc_recording) {
        std::unique_lock<std::mutex> lock(_alloc_mutex);
		
		Allocation a;
		a._name = const_cast<DTcharacter*>(name);
		a._size = size;
		
		allocations()[p] = a;
        
		// Record stats
		_total_allocs += size;
		_current_allocs += size;
		_peak_allocs = MoreMath::max(_current_allocs, _peak_allocs);
	} 
}

void MemoryAllocatorTrace::record_deallocation (void* p)
{
	if (p && _alloc_recording) {
        std::unique_lock<std::mutex> lock(_alloc_mutex);
        
		auto i = allocations().find(p);
		if (i != allocations().end()) {
			
			// Record stats
			_current_allocs -= i->second._size;
			
			allocations().erase(i);
		} 
	}
}

//==============================================================================
//==============================================================================

void MemoryAllocatorTrace::check_allocations (void)
{
#if DT3_GUARD_ALLOCATIONS && defined(DT3_DEBUG)
    if (!_alloc_checking)
        return;

    for (auto &i : allocations()) {
        void *pp = i.first;
    
        DTubyte *p = (DTubyte*) pp - sizeof(Header);

        // Useful pointers
        Header *header = (Header*) p;
        Footer *footer = (Footer*) (p + sizeof(Header) + header->size);
    
        ASSERTBREAK(header->magic = MAGIC);
        ASSERTBREAK(footer->magic = MAGIC);

    }
#endif
}

void MemoryAllocatorTrace::enable_check_allocations(DTboolean check)
{
    _alloc_checking = check;
}

//==============================================================================
//==============================================================================

void MemoryAllocatorTrace::init (void)
{
	allocations().clear();
	_alloc_recording = true;
	
	_total_allocs = 0;
	_peak_allocs = 0;
	_current_allocs = 0;
}
		
void MemoryAllocatorTrace::exit (void)
{
    check_allocations();

	_alloc_recording = false;
    
	LOG_MESSAGE << "---- MEMORY DUMP -------------------------------------------------------------";
	LOG_MESSAGE << "Total Allocations:   " << _total_allocs << " bytes (" << _total_allocs/(1024.0F*1024.0F) << " MB)";
	LOG_MESSAGE << "Peak Allocations:    " << _peak_allocs << " bytes (" << _peak_allocs/(1024.0F*1024.0F) << " MB)";
	LOG_MESSAGE << "Current Allocations: " << _current_allocs << " bytes (" << _current_allocs/(1024.0F*1024.0F) << " MB)";

    //
    // Named objects
    //

    std::multimap<char *, Allocation*, std::less<char *>> grouped;

	LOG_MESSAGE << "Named objects leaked: ";
    FOR_EACH (i,allocations()) {
	    if (i->second._name) {
            grouped.insert(std::make_pair(i->second._name, &(i->second)));
		}
	}	
	
	for (auto grouped_i = grouped.begin(); grouped_i != grouped.end(); grouped_i = grouped.upper_bound(grouped_i->first) ) {
		
		auto lower = grouped.lower_bound(grouped_i->first);
		auto upper = grouped.upper_bound(grouped_i->first);

        DTuint count = 0;
        DTsize size = 0;
		for (auto grouped_j = lower; grouped_j != upper; ++grouped_j) {
            ++count;
            size += grouped_j->second->_size;
		}	
	
        LOG_MESSAGE << " Kind " << grouped_i->first <<   "   Num " << count << "  Total Size: " << size;
	}

    //
    // Unnamed objects
    //

	LOG_MESSAGE << "Unnamed objects leaked: ";
    
    std::multimap<DTuint, Allocation*, std::greater<DTuint>> ordered;
    FOR_EACH (i,allocations()) {
	    if (!i->second._name) {
            ordered.insert(std::make_pair(i->second._size, &(i->second)));
        }
	}
    
	for (auto ordered_i = ordered.begin(); ordered_i != ordered.end(); ordered_i = ordered.upper_bound(ordered_i->first) ) {
		
		auto lower = ordered.lower_bound(ordered_i->first);
		auto upper = ordered.upper_bound(ordered_i->first);

        DTuint count = 0;
		for (auto ordered_j = lower; ordered_j != upper; ++ordered_j) {
            ++count;
		}	
	
        LOG_MESSAGE << " Num bytes " << ordered_i->first <<   "   Num " << count;
	}
    
}
		
//==============================================================================
//==============================================================================

void* MemoryAllocatorTrace::allocate (const DTsize size, const DTcharacter *name)
{
#if DT3_GUARD_ALLOCATIONS
    check_allocations();

	DTubyte *p = (DTubyte*) std::malloc( sizeof(Header) + size + sizeof(Footer) );
    
    // Useful pointers
    Header *header = (Header*) p;
    void *pp = (void*) (p + sizeof(Header));
    Footer *footer = (Footer*) (p + sizeof(Header) + size);
    
    header->magic = MAGIC;
    header->size = size;
    
    footer->magic = MAGIC;
#else
	void *pp = (void*) std::malloc(size);
#endif

#if defined(DT3_DEBUG)
	::memset(pp,0xFF, (size_t) size);
#endif
														
	record_allocation (pp, name, size);
	return pp;
}

void MemoryAllocatorTrace::deallocate (void *pp)
{
#if DT3_GUARD_ALLOCATIONS
    check_allocations();

	DTubyte *p = (DTubyte*) pp - sizeof(Header);
#else
    void *p = pp;
#endif

	record_deallocation (pp);

	::free(p);
}

//==============================================================================
//==============================================================================

} // DT3
