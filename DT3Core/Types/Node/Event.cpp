//==============================================================================
///	
///	File: Event.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Node/Event.hpp"
#include "DT3Core/Types/Node/PlugNode.hpp"
#include "DT3Core/System/Profiler.hpp"
#include <algorithm>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,Event::initialize_static())

//==============================================================================
//==============================================================================

Event::EventConnections Event::_pool[DT3_EVENT_CONNECTION_POOL_SIZE];
Event::EventConnections *Event::_free_list = NULL;
std::mutex Event::_free_list_mutex;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Event::Event (void)
	:	_is_computing       (false),
        _info_index         (0),
        _connection_index   (0)
{  

}

Event::Event (DTushort info)
	:	_is_computing       (false),
        _info_index         (info),
        _connection_index   (0)
{	

}
		
Event::Event (const Event &rhs)
	:	_is_computing       (false),
        _info_index         (rhs._info_index),
        _connection_index   (0)
{   

}
			
Event::~Event (void)
{
    remove_incoming_connections();
    remove_outgoing_connections();
    
    free_connections();
}

//==============================================================================
//==============================================================================

std::string Event::full_name (void) const
{
    return owner()->full_name() + "." + name();
}

//==============================================================================
//==============================================================================

std::vector<Event*> Event::incoming_connections (void) const
{
    if (_connection_index == 0)
        return std::vector<Event*>();
    
    return connections()._incoming;
}

DTboolean Event::has_incoming_connection (void) const
{
    if (_connection_index == 0)
        return false;

    return connections()._incoming.size() != 0;
}

void Event::add_incoming_connection (Event* incoming)
{
	PROFILER(SCRIPTING);
    
    std::vector<Event*> &incoming_ref = connections()._incoming;
    
    if (std::find(incoming_ref.begin(), incoming_ref.end(), incoming) == incoming_ref.end()) {
        incoming_ref.push_back(incoming);
        incoming->add_outgoing_connection(this);
        
        owner()->incoming_event_was_attached(incoming, this);
    }
}

void Event::remove_incoming_connection (Event* incoming)
{
	PROFILER(SCRIPTING);
    
    if (_connection_index == 0)
        return;
    
    std::vector<Event*> &incoming_ref = connections()._incoming;

    auto i = std::find(incoming_ref.begin(), incoming_ref.end(), incoming);
	if (i != incoming_ref.end()) {
		owner()->incoming_event_was_disconnected(incoming, this);

		incoming_ref.erase(i);
		incoming->remove_outgoing_connection(this);
    }
	
}

void Event::remove_incoming_connections (void)
{
    if (_connection_index == 0)
        return;
    
    std::vector<Event*> &incoming_ref = connections()._incoming;

	while (incoming_ref.begin() != incoming_ref.end()) {
		remove_incoming_connection(*(incoming_ref.begin()));
	}
}

//==============================================================================
//==============================================================================

std::vector<Event*> Event::outgoing_connections (void) const
{
    if (_connection_index == 0)
        return std::vector<Event*>();
    
    return connections()._incoming;
}

DTboolean Event::has_outgoing_connection (void) const
{
    if (_connection_index == 0)
        return false;

    return connections()._outgoing.size() != 0;
}

void Event::add_outgoing_connection (Event* outgoing)
{
	PROFILER(SCRIPTING);

    std::vector<Event*> &outgoing_ref = connections()._outgoing;

    if (std::find(outgoing_ref.begin(), outgoing_ref.end(), outgoing) == outgoing_ref.end()) {
        outgoing_ref.push_back(outgoing);
        outgoing->add_incoming_connection(this);
        
        owner()->outgoing_event_was_attached(this, outgoing);
    }

}

void Event::remove_outgoing_connection (Event* outgoing)
{
	PROFILER(SCRIPTING);
	
    if (_connection_index == 0)
        return;
    
    std::vector<Event*> &outgoing_ref = connections()._outgoing;

    auto i = std::find(outgoing_ref.begin(), outgoing_ref.end(), outgoing);
	if (i != outgoing_ref.end()) {
		owner()->outgoing_event_was_disconnected(this, outgoing);

		outgoing_ref.erase(i);
		outgoing->remove_incoming_connection(this);
    }
	
}

void Event::remove_outgoing_connections (void)
{
    if (_connection_index == 0)
        return;
    
    std::vector<Event*> &outgoing_ref = connections()._outgoing;

	while (outgoing_ref.begin() != outgoing_ref.end()) {
		remove_outgoing_connection(*(outgoing_ref.begin()));
	}
}

//==============================================================================
//==============================================================================

void Event::send (PlugNode *sender)
{
    if (_connection_index == 0)
        return;
    
    std::vector<Event*> &outgoing_ref = connections()._outgoing;

    FOR_EACH (i, outgoing_ref) {
        (**i).trigger(sender ? sender : owner());
    }
}

void Event::trigger (PlugNode *sender)
{
    if (!_is_computing) {
        std::shared_ptr<EventBindBase> event_bind = info().event();
        ASSERT(event_bind);

        _is_computing = true;
        PlugNode *other = info().event_to_node(this);
        event_bind->trigger( other, sender);
        _is_computing = false;
    }
}


//==============================================================================
//==============================================================================		

void Event::initialize_static (void)
{
    // Link up free list on first use
    for (DTuint i = 1; i < DT3_EVENT_CONNECTION_POOL_SIZE; ++i) {
        _pool[i-1]._next_free = &_pool[i];
    }
    _free_list = _pool;

    _pool[DT3_EVENT_CONNECTION_POOL_SIZE-1]._next_free = NULL;
}

Event::EventConnections& Event::connections (void)
{
    std::unique_lock<std::mutex> lock(_free_list_mutex);

    if (_connection_index == 0) {
        ERROR(_free_list != NULL, "Exhausted Connections Cache. Increase DT3_EVENT_CONNECTION_POOL_SIZE");
        
        EventConnections *c = _free_list;
        _free_list = _free_list->_next_free;
        
        c->_incoming.clear();
        c->_outgoing.clear();

        _connection_index = static_cast<DTushort>(c - _pool) + 1;
    }
    
    return _pool[_connection_index-1];
}

void Event::free_connections (void)
{
    std::unique_lock<std::mutex> lock(_free_list_mutex);

    if (_connection_index != 0) {
        EventConnections &c = _pool[_connection_index-1];
        
        c._incoming.clear();
        c._outgoing.clear();
        
        c._next_free = _free_list;
        _free_list = &c;
    }
}

Event::EventConnections& Event::connections (void) const
{
    return _pool[_connection_index-1];
}

//==============================================================================		
//==============================================================================		

} // DT3

