//==============================================================================
///	
///	File: EventInfo.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Node/Event.hpp"
#include "DT3Core/Types/Node/PlugNode.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// EventInfo routines
//==============================================================================	

EventInfo EventInfo::_pool[DT3_EVENT_POOL_SIZE];

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

EventInfo::EventInfo (void)
	:	_offset			(0),
		_event_bind		(NULL),
		_next			(NULL),
		_is_input		(false),
		_is_output		(false),
		_is_no_draw		(false)
{  

}
					
EventInfo::~EventInfo (void)
{

}

//==============================================================================
//==============================================================================

EventInfo& EventInfo::init (std::string name, DTsize offset, DTnodekind *event_node_info)
{
	// This assert will trigger if this EventInfo is added more than once. The easiest way for
	// this to happen is if you have a DEFINE_PLUG_NODE macro without a Initialize_Events static 
	// function in your class.
	ASSERT(_next == NULL);
							
	_name = name;
	_offset = offset;
	
	// Add to linked list
	_next = event_node_info->_events;
	event_node_info->_events = this;
	
	return *this;
}

//==============================================================================	
//==============================================================================	

DTushort EventInfo::get_free_index (void)
{
    static DTushort pool_index = 0;
    
    DTint r = pool_index++;
    ASSERT(r < DT3_EVENT_POOL_SIZE);
    
    return static_cast<DTushort>(r);
}

EventInfo& EventInfo::get_info (DTushort i)
{
    ASSERT(i != 0xFFFF);
    return _pool[i];
}

//==============================================================================
//==============================================================================	

} // DT3

