#ifndef DT3_PLUGNODE
#define DT3_PLUGNODE
//==============================================================================
///	
///	File: PlugNode.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/Node/PlugInfo.hpp"
#include "DT3Core/Types/Node/EventInfo.hpp"
#include <string>
#include <mutex>
#include <list>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Event;
class PlugInfo;
class EventInfo;

//==============================================================================
/// Class
//==============================================================================

// Class kind structure. Used for various class information
struct DTnodekind {
	DTnodekind(DTnodekind *super_class, void (*fn)(DTnodekind *)) {
		_plugs = (super_class ? super_class->_plugs : NULL);	// initialize with nodes from super class
		_events = (super_class ? super_class->_events : NULL);	// initialize with nodes from super class
		fn(this);	
	}
	
	PlugInfo    *_plugs;
	EventInfo   *_events;
};

#define DEFINE_PLUG_NODE																			\
	inline static DTnodekind*	plug_info_static    (void) {                                        \
		static DTnodekind plug_info(super_type::plug_info_static(), &type::initialize_plugs);       \
		return &plug_info;																			\
	}																								\
	virtual DTnodekind*			plug_info_child     (void) const {									\
		return plug_info_static();                                                                  \
	}																								\
	DTnodekind*					plug_info           (void) const {									\
		return plug_info_static();                                                                  \
	}                                                                                               \
    static void                 initialize_plugs    (DTnodekind *plug_node_info);
    
#define DEFINE_PLUG_NODE_BASE                                                       \
	inline static DTnodekind*	plug_info_static    (void) {                        \
		static DTnodekind plug_info(NULL, &type::initialize_plugs);                 \
		return &plug_info;                                                          \
	}                                                                               \
	virtual DTnodekind*			plug_info_child     (void) const {                  \
		return plug_info_static();                                                  \
	}                                                                               \
	DTnodekind*					plug_info           (void) const {                  \
		return plug_info_static();                                                  \
	}                                                                               \
    static void                 initialize_plugs    (DTnodekind *plug_node_info);

	
	
#define IMPLEMENT_PLUG_NODE(Class)                                                  \
	namespace {                                                                     \
		struct registration_node_class##Class: public StaticInitializerCallback {   \
			void initialize() {                                                     \
				Class::plug_info_static();                                          \
			}                                                                       \
		} registration_node_obj##Class;                                             \
	}
    
#define BEGIN_IMPLEMENT_PLUGS(Class)                                                \
    void Class::initialize_plugs (DTnodekind *plug_node_info) {
    
#define END_IMPLEMENT_PLUGS                                                         \
    }
				
//==============================================================================
/// Forward declarations
//==============================================================================

class PlugBase;

//==============================================================================
/// Class
//==============================================================================

class PlugNode: public BaseClass {
    public:
        DEFINE_TYPE(PlugNode,BaseClass)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE_BASE
		
										PlugNode                    (void);
										PlugNode                    (const PlugNode &rhs);
        PlugNode &						operator =                  (const PlugNode &rhs);
        virtual							~PlugNode                   (void);
    
        virtual void					archive                     (const std::shared_ptr<Archive> &archive);
        virtual void					archive_done                (const std::shared_ptr<Archive> &archive);
		
	public:	
	
		DEFINE_ACCESSORS                (name, set_name, std::string, _name);
        
        /// Returns the full name of object that uniquely identifies it
		/// \return full name of object
        virtual std::string             full_name                           (void) const;

        /// Returns the preferred name of the object for when it is created
		/// \return preferred name of object
        virtual std::string             preferred_name                      (void) const;

		/// Returns a plug with matching name
		/// \return matching plug
		PlugBase*						plug_by_name                        (const std::string &name);
	
		/// Builds a list of all of the plugs
		/// \param plugs all plugs in the node
		void							all_plugs                           (std::list<PlugBase*> &plugs);

		/// Returns an event with matching name
		/// \return matching event
		Event*                          event_by_name                       (const std::string &name);
	
		/// Builds a list of all of the events
		/// \param events all events in the node
		void							all_events                          (std::list<Event*> &events);

		
		/// Compute function called when a plug value needs to be calculated
		/// \param plug plug to compute
		/// \return Compute successful
		virtual DTboolean				compute                             (const PlugBase *plug);
		
		
		/// Disconnect all of the plugs
		void							disconnect_all_plugs                (void);
		
		/// Disconnect all of the events
		void							disconnect_all_events               (void);

        
		/// Called when an outgoing plug is disconnected
		/// \param outgoing outgoing plug
		/// \param incoming other incoming plug
		virtual void					outgoing_plug_was_disconnected      (PlugBase *outgoing, PlugBase *incoming);

		/// Called when an outgoing plug is attached
		/// \param outgoing outgoing plug
		/// \param incoming other incoming plug
		virtual void					outgoing_plug_was_attached          (PlugBase *outgoing, PlugBase *incoming);

		/// Called when an incoming plug is disconnected
		/// \param outgoing other outgoing plug
		/// \param incoming incoming plug
		virtual void					incoming_plug_was_disconnected      (PlugBase *outgoing, PlugBase *incoming);

		/// Called when an outgoing plug is attached
		/// \param outgoing other outgoing plug
		/// \param incoming incoming plug
		virtual void					incoming_plug_was_attached          (PlugBase *outgoing, PlugBase *incoming);


		/// Called when an outgoing event is disconnected
		/// \param outgoing outgoing event
		/// \param incoming other incoming event
		virtual void					outgoing_event_was_disconnected     (Event *outgoing, Event *incoming);

		/// Called when an outgoing event is attached
		/// \param outgoing outgoing event
		/// \param incoming other incoming event
		virtual void					outgoing_event_was_attached         (Event *outgoing, Event *incoming);

		/// Called when an incoming event is disconnected
		/// \param outgoing other outgoing event
		/// \param incoming incoming event
		virtual void					incoming_event_was_disconnected     (Event *outgoing, Event *incoming);

		/// Called when an outgoing event is attached
		/// \param outgoing other outgoing event
		/// \param incoming incoming event
		virtual void					incoming_event_was_attached         (Event *outgoing, Event *incoming);


		/// Returns a mutex for computing this plug
		/// \return mutex
        std::recursive_mutex&           lock                                (void)  {   return _lock;   }
    
    
		/// This is a known plug pointer that is created for use in the unit tests for testing the compute
        /// chain. It doesn't get actually used anywhere.
        /// \return standard pointer
        static PlugBase*                test_compute_chain_plug             (void)  {   return reinterpret_cast<PlugBase*>(0xFFFFFFFF); }

	private:		
        std::string                     _name;
        mutable std::recursive_mutex    _lock;
        
};

//==============================================================================
//==============================================================================

class PlugIter {
	public:
									PlugIter					(PlugNode* o)   {	_node = o;	_info = _node->plug_info_child()->_plugs;	}
																		
	private:
									PlugIter					(const PlugIter &rhs);
        PlugIter &					operator =					(const PlugIter &rhs);	

	public:
        virtual						~PlugIter					(void)			{}
		
		// Increment the iterator to the next item
		inline PlugIter&			operator ++					(void)			{	_info = _info->next_info(); return *this;	}
		
		// Return the current item
		inline PlugBase*			operator ->					(void) const	{	return _info->node_to_plug(_node);	}
		inline PlugBase*			operator ()					(void) const	{	return _info->node_to_plug(_node);	}
		
		// Returns non-zero value if the iterator is valid
		inline 						operator const void *		(void) const	{	return (void*) (_info != NULL);	}
		
	private:
		PlugInfo        *_info;
		PlugNode        *_node;
};

//==============================================================================
//==============================================================================

class EventIter {
	public:
									EventIter					(PlugNode *o)   {	_node = o;	_info = _node->plug_info_child()->_events;	}
																		
	private:
									EventIter					(const EventIter &rhs);
        EventIter &					operator =					(const EventIter &rhs);	

	public:
        virtual						~EventIter					(void)			{}
		
		// Increment the iterator to the next item
		inline EventIter&			operator ++					(void)			{	_info = _info->next_info(); return *this;	}
		
		// Return the current item
		inline Event*               operator ->					(void) const	{	return _info->node_to_event(_node);	}
		inline Event*               operator ()					(void) const	{	return _info->node_to_event(_node);	}
		
		// Returns non-zero value if the iterator is valid
		inline 						operator const void *		(void) const	{	return (void*) (_info != NULL);	}
		
	private:
		EventInfo       *_info;
		PlugNode        *_node;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
