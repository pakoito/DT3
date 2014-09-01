//==============================================================================
///	
///	File: PlugBase.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Node/Plug.hpp"
#include "DT3Core/Types/Node/PlugNode.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/Error.hpp"
#include "DT3Core/System/Profiler.hpp"
#include <stack>
#include <mutex>
#include <algorithm>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,PlugBase::initialize_static())

//==============================================================================
//==============================================================================

PlugBase::PlugConnections PlugBase::_pool[DT3_PLUG_CONNECTION_POOL_SIZE];
PlugBase::PlugConnections *PlugBase::_free_list = NULL;
std::mutex PlugBase::_free_list_mutex;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

PlugBase::PlugBase (void)
	:	_is_dirty           (true),
		_will_compute       (false),
        _is_computing       (false),
        _info_index         (0),
        _connection_index   (0)
{

}
		
PlugBase::PlugBase (const PlugBase &rhs)
	:	_is_dirty           (true),
		_will_compute       (false),
        _is_computing       (false),
        _info_index         (rhs._info_index),
        _connection_index   (0)
{

}

PlugBase & PlugBase::operator = (const PlugBase &rhs)
{
    return (*this);
}
			
PlugBase::~PlugBase (void)
{
	remove_incoming_connection();
    remove_outgoing_connections();

    free_connections();
}

//==============================================================================
//==============================================================================

std::string PlugBase::full_name (void) const
{
    return owner()->full_name() + "." + name();
}

//==============================================================================
//==============================================================================

PlugBase* PlugBase::incoming_connection (void) const
{
    if (_connection_index == 0)
        return NULL;

    return connections()._incoming;
}

//==============================================================================
//==============================================================================

DTboolean PlugBase::has_incoming_connection (void) const
{
    if (_connection_index == 0)
        return false;

    return connections()._incoming != NULL;
}

//==============================================================================
//==============================================================================

DTboolean PlugBase::set_incoming_connection		(PlugBase* incoming)
{
	PROFILER(SCRIPTING);
    
    //
    // Lock this owner and the incoming owner
    //
    
    std::unique_lock<std::recursive_mutex> lock_this(owner()->lock(),std::try_to_lock);
    std::unique_lock<std::recursive_mutex> lock_incoming(incoming->owner()->lock(),std::try_to_lock);
    
    DTboolean lock_this_status = lock_this.owns_lock();
    DTboolean lock_incoming_status = lock_incoming.owns_lock();
    
    while (!lock_this_status || !lock_incoming_status) {
        if (lock_this_status)       lock_this.unlock();
        if (lock_incoming_status)   lock_incoming.unlock();
    
        lock_this_status = lock_this.owns_lock();
        lock_incoming_status = lock_incoming.owns_lock();
    }
    
    //
    // Do modifications
    //
    
    PlugBase *&incoming_ref = connections()._incoming;

	if (is_compatible(incoming) && incoming_ref != incoming) {
	
		// Disconnect old node
		if (incoming_ref) {
            owner()->incoming_plug_was_disconnected(incoming_ref, this);
			incoming_ref->remove_outgoing_connection(this);
        }
		
		// Connect new node
		incoming_ref = incoming;
		
		// Add this plug to the outgoing connections
		incoming_ref->add_outgoing_connection(this);
		set_dirty();
	
		owner()->incoming_plug_was_attached(incoming_ref, this);

		return true;
	} else {
		if (!is_compatible(incoming))
			LOG_MESSAGE << plug_type() << " doesn't match " << incoming->plug_type();
	}
	
	return false;
}

//==============================================================================
//==============================================================================

void PlugBase::remove_incoming_connection	(void)
{
	PROFILER(SCRIPTING);

    if (_connection_index == 0)
        return;
    
    PlugBase *&incoming_ref = connections()._incoming;

	// Disconnect old node
	if (incoming_ref) {

        //
        // Lock this owner and the incoming owner
        //
        
        std::unique_lock<std::recursive_mutex> lock_this(owner()->lock(),std::try_to_lock);
        std::unique_lock<std::recursive_mutex> lock_incoming(incoming_ref->owner()->lock(),std::try_to_lock);
        
        DTboolean lock_this_status = lock_this.owns_lock();
        DTboolean lock_incoming_status = lock_incoming.owns_lock();
        
        while (!lock_this_status || !lock_incoming_status) {
            if (lock_this_status)       lock_this.unlock();
            if (lock_incoming_status)   lock_incoming.unlock();
        
            lock_this_status = lock_this.owns_lock();
            lock_incoming_status = lock_incoming.owns_lock();
        }

        //
        // Do modifications
        //

        PlugBase *temp = incoming_ref;
        incoming_ref = NULL;
    
		owner()->incoming_plug_was_disconnected(temp, this);
		temp->remove_outgoing_connection(this);
	}
	
}

//==============================================================================
//==============================================================================

std::vector<PlugBase*> PlugBase::outgoing_connections (void) const
{
    if (_connection_index == 0)
        return std::vector<PlugBase*>();
    
    return connections()._outgoing;
}

//==============================================================================
//==============================================================================

DTboolean PlugBase::has_outgoing_connection (void) const
{
    if (_connection_index == 0)
        return false;

    return connections()._outgoing.size() != 0;
}

//==============================================================================
//==============================================================================

DTboolean PlugBase::add_outgoing_connection (PlugBase* outgoing)
{
	PROFILER(SCRIPTING);


    //
    // Lock this owner and the outgoing owner
    //
    
    std::unique_lock<std::recursive_mutex> lock_this(owner()->lock(),std::try_to_lock);
    std::unique_lock<std::recursive_mutex> lock_outgoing(outgoing->owner()->lock(),std::try_to_lock);

    DTboolean lock_this_status = lock_this.owns_lock();
    DTboolean lock_outgoing_status = lock_outgoing.owns_lock();
    
    while (!lock_this_status || !lock_outgoing_status) {
        if (lock_this_status)       lock_this.unlock();
        if (lock_outgoing_status)   lock_outgoing.unlock();
    
        lock_this_status = lock_this.owns_lock();
        lock_outgoing_status = lock_outgoing.owns_lock();
    }
    
    //
    // Do modifications
    //
    
    std::vector<PlugBase*> &outgoing_ref = connections()._outgoing;

    // Check if only a single output is allowed
    if (!info().is_single_output() || (info().is_single_output() && outgoing_ref.size() == 0)) {
    
        // Check compatibility
        if (is_compatible(outgoing)) {
            auto i = std::find(outgoing_ref.begin(), outgoing_ref.end(), outgoing);
            if (i == outgoing_ref.end()) {
                outgoing_ref.push_back(outgoing);
                outgoing->set_dirty();
                outgoing->set_incoming_connection(this);
                
                // Increasse Ref count for owner
                owner()->outgoing_plug_was_attached(this, outgoing);
            }
        
            return true;
        }
        
    }
	
	return false;
}

//==============================================================================
//==============================================================================

void PlugBase::remove_outgoing_connection	(PlugBase* outgoing)
{
	PROFILER(SCRIPTING);

    if (_connection_index == 0)
        return;
    
    std::vector<PlugBase*> &outgoing_ref = connections()._outgoing;

    auto i = std::find(outgoing_ref.begin(), outgoing_ref.end(), outgoing);
	if (i != outgoing_ref.end()) {
    
        //
        // Lock this owner and the outgoing owner
        //
        
        std::unique_lock<std::recursive_mutex> lock_this(owner()->lock(),std::try_to_lock);
        std::unique_lock<std::recursive_mutex> lock_outgoing(outgoing->owner()->lock(),std::try_to_lock);

        DTboolean lock_this_status = lock_this.owns_lock();
        DTboolean lock_outgoing_status = lock_outgoing.owns_lock();
        
        while (!lock_this_status || !lock_outgoing_status) {
            if (lock_this_status)       lock_this.unlock();
            if (lock_outgoing_status)   lock_outgoing.unlock();
        
            lock_this_status = lock_this.owns_lock();
            lock_outgoing_status = lock_outgoing.owns_lock();
        }
        
        //
        // Do modifications
        //

		owner()->outgoing_plug_was_disconnected(this,outgoing);

		outgoing_ref.erase(i);
		outgoing->remove_incoming_connection();
    }
	
}

//==============================================================================
//==============================================================================

void PlugBase::remove_outgoing_connections	(void)
{
	PROFILER(SCRIPTING);

    if (_connection_index == 0)
        return;
    
    std::unique_lock<std::recursive_mutex> lock(owner()->lock());
    std::vector<PlugBase*> &outgoing_ref = connections()._outgoing;

	while (outgoing_ref.begin() != outgoing_ref.end()) {
		remove_outgoing_connection(*(outgoing_ref.begin()));
	}
				
}

//==============================================================================
//==============================================================================

DTboolean PlugBase::try_set_dirty(void)
{
    std::list<PlugBase*> visited_plugs;
    
    //
    // Phase I: Lock all plugs and mark dirty
    //

    visited_plugs.push_back(this);
    
    // Lock and set dirty flags on all dependents
    // If it fails to get a lock, it resets all of the flags and exits
    
    FOR_EACH (i,visited_plugs) {
        PlugBase *plug = (*i);
        PlugNode *owner = plug->owner();
    
        if (!owner->lock().try_lock()) {
            
            // LOCK FAILED so we have to undo all previous locks
            
            // Unlock all previously locked nodes
            for (auto j = visited_plugs.begin(); j != i; ++j) {
                PlugBase *plug2 = (*j);
                plug2->_is_dirty = false;
                plug2->owner()->lock().unlock();
            }
                            
            return false;   // Try again
        }
        
        plug->_is_dirty = true;
        
        // Traverse affects first
        const std::vector<PlugInfo*>& affects = plug->info().affects();
        
        FOR_EACH (k,affects) {
            PlugBase *plug2 = (*k)->node_to_plug(owner);
            if (!plug2->_is_dirty)
                visited_plugs.push_back(plug2);
        }

        // Outgoing connections second
        if (plug->_connection_index != 0) {
        
            FOR_EACH (l,plug->connections()._outgoing) {
                PlugBase *plug2 = *l;
                if (!plug2->_is_dirty)
                    visited_plugs.push_back(plug2);
            }
            
        }
    
    }
    
    //
    // Phase II: Unlock all plugs
    //
    
    // Unlock dirtied nodes
    FOR_EACH (i,visited_plugs) {
        PlugBase *plug = (*i);
        plug->owner()->lock().unlock();
    }

    return true;    // Success
}

void PlugBase::set_dirty (void)
{	
    if (!_is_dirty) {        
        // This will only loop if there's some thread contention for the nodes
        while (!try_set_dirty())
            ;
    }
}

//==============================================================================
//==============================================================================

DTboolean PlugBase::try_compute(PlugBase *p)
{
    std::list<PlugBase*> visited_plugs;
    std::stack<PlugBase*> stack;
    
    //
    // Phase I: Lock all plugs
    //
    
    // Prime the stack
    stack.push(p);
    
    // If it fails to get a lock, it resets all of the flags and exits
    while(!stack.empty()) {
    
        // Get the top of the stack
        PlugBase *plug = stack.top();
        stack.pop();
        
        PlugNode *owner = plug->owner();
    
        if (!owner->lock().try_lock()) {
            
            // LOCK FAILED so we have to undo all previous locks
            
            // Unlock all previously locked nodes
            FOR_EACH (j,visited_plugs) {
                PlugBase *plug2 = (*j);
                plug2->_will_compute = false;
                plug2->owner()->lock().unlock();
            }
                            
            return false;   // Try again
        }
        
        plug->_will_compute = true;
        visited_plugs.push_back(plug);
  
        // If there is an incoming connection, then we have to validate
        // those connections first. We always add them to the stack again
        // so they actually get evaluated first later too.
        if (plug->_connection_index != 0) {
            PlugBase *&incoming_ref = plug->connections()._incoming;
            
            if (incoming_ref) {
                PlugBase *plug2 = incoming_ref;
                if (/*!plug2->_will_compute &&*/ plug2->_is_dirty)
                    stack.push(plug2);
            }
        }

        // Validate plugs that affect this one
        const std::vector<PlugInfo*>& affected_by = plug->info().affected_by();

        FOR_EACH (k,affected_by) {
            PlugBase *plug2 = (*k)->node_to_plug(owner);
            if (!plug2->_will_compute && plug2->_is_dirty)
                stack.push(plug2);
        }
        
    }
    
    //
    // Phase II: Compute all plugs
    //
    
    // Compute all nodes in reverse order
    for (auto i = visited_plugs.rbegin(); i != visited_plugs.rend(); ++i) {
        PlugBase *plug2 = (*i);
        
        // Since computing plugs may validate more than one, we have to recheck dirty flag
        if (plug2->_will_compute) {
            PlugInfo *info2 = &plug2->info();

            // Copy value from incoming connection if there is one
            if (plug2->_connection_index != 0) {
            
                PlugBase *&incoming_ref = plug2->connections()._incoming;
                
                if (incoming_ref) {
                    info2->copy(plug2, incoming_ref);
                } else {
                    plug2->set_clean();
                }
            
            } else {
                plug2->set_clean();
            }
            
            

            // Compute the plug if computable... probably does nothing for incoming nodes but we try anyway
            if (info2->requires_compute()) {
            
                // Recursive checks
                if (plug2->_is_computing != true) {
                    plug2->_is_computing = true;
                    info2->set_requires_compute(plug2->owner()->compute(plug2));	// This will turn off computing if it does nothing
                    plug2->_is_computing = false;
                }

#if DT3_DEBUG
                if (!info2->requires_compute())
                    LOG_DEBUG << "Note: Compute turned off for " << plug2->owner()->class_id_child() << "." << plug2->name().c_str();
#endif			
            }
        }

        // Done computing this plug so unlock it
        plug2->_will_compute = false;
        plug2->owner()->lock().unlock();
    }
    
    return true;    // Success
}


void PlugBase::compute (void) const
{
    // Simple case is to skip everything if this plug doesn't need computing
    if (!_is_dirty)
        return;

    // This is a nasty workaround. I had to violate constness because reading
    // a plug might actually cause it to update. I'll change it if I find
    // a standard pattern for dealing with copy-on-read.
    PlugBase* this_plug = const_cast<PlugBase*>(this);

    // This will only loop if there's some thread contention for the nodes
    while (!try_compute(this_plug))
        ;   // Do nothing
}

//==============================================================================
//==============================================================================		

void PlugBase::initialize_static (void)
{
    // Link up free list on first use
    for (DTuint i = 1; i < DT3_PLUG_CONNECTION_POOL_SIZE; ++i) {
        _pool[i-1]._next_free = &_pool[i];
    }
    _free_list = _pool;

    _pool[DT3_PLUG_CONNECTION_POOL_SIZE-1]._next_free = NULL;
}

PlugBase::PlugConnections& PlugBase::connections (void)
{
    std::unique_lock<std::mutex> lock(_free_list_mutex);

    if (_connection_index == 0) {
        ERROR(_free_list != NULL, "Exhausted Connections Cache. Increase DT3_PLUG_CONNECTION_POOL_SIZE");
        
        PlugConnections *c = _free_list;
        _free_list = _free_list->_next_free;
        
        c->_incoming = NULL;
        c->_outgoing.clear();
        
        _connection_index = static_cast<DTushort>(c - _pool) + 1;
    }
    
    return _pool[_connection_index-1];
}

void PlugBase::free_connections (void)
{
    std::unique_lock<std::mutex> lock(_free_list_mutex);

    if (_connection_index != 0) {
        PlugConnections &c = _pool[_connection_index-1];
        
        c._incoming = NULL;
        c._outgoing.clear();
        
        c._next_free = _free_list;
        _free_list = &c;
    }
}

PlugBase::PlugConnections& PlugBase::connections (void) const
{
    return _pool[_connection_index-1];
}

//==============================================================================
//==============================================================================

} // DT3

