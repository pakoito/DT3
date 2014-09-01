#ifndef DT3_EVENTINFO
#define DT3_EVENTINFO
//==============================================================================
///	
///	File: EventInfo.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"
#include "DT3Core/Types/Utility/CheckedCast.hpp"
#include <string>
#include <vector>
#include <mutex>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class PlugNode;
class Event;
struct DTnodekind;

//==============================================================================
// Event binding code
//==============================================================================

class EventBindBase
{
	public:
		virtual void		trigger	(PlugNode *node, PlugNode *sender) = 0;
};

template <class CLASS>
class EventBind: public EventBindBase
{
	private:
		typedef void (CLASS::*Fn)(PlugNode*);

	public:
		EventBind(Fn fn)    {   _fn = fn;   }

		virtual void trigger (PlugNode *node, PlugNode *sender)
		{
			CLASS *node_obj = checked_static_cast<CLASS*>(node);
			(*node_obj.*_fn)(sender);
		}

	private:
		Fn _fn;
};

//==============================================================================
/// Class
//==============================================================================

class EventInfo {
	public:
                                        EventInfo		(void);
	private:
                                        EventInfo		(const EventInfo &rhs);
        EventInfo &                     operator =		(const EventInfo &rhs);
		
	public:
        virtual                         ~EventInfo		(void);
		
		
	public:
		/// Initialize the static EventInfo structure
		/// \param name Name of event
		/// \param offset Byte offset to the event in the object
		/// \param event_node_info next event info in linked list
		/// \return This event info
		EventInfo&                      init			(std::string name, DTsize offset, DTnodekind *event_node_info);

		/// Returns event name
		/// \return event name
		inline const std::string &		name            (void) const				{	return _name;									}


		/// Returns no draw flag
		/// \return No draw flag
		inline DTboolean                is_no_draw		(void) const				{	return _is_no_draw;							}
		
		/// Sets the no draw flag
		/// \param flag no draw flag
		/// \return This event info
		inline EventInfo&               set_no_draw		(const DTboolean flag)		{	_is_no_draw = flag;	return *this;			}

		
		/// Returns is input flag
		/// \return is input
		inline DTboolean                is_input        (void) const				{	return _is_input;							}
		
		/// Sets the is input flag
		/// \param flag is input flag
		/// \return This event info
		inline EventInfo&               set_input		(const DTboolean flag)		{	_is_input = flag;	return *this;			}


		/// Returns is output flag
		/// \return is output
		inline DTboolean                is_output		(void) const				{	return _is_output;							}
		
		/// Sets the is output flag
		/// \param flag is output flag
		/// \return This event info
		inline EventInfo&               set_output		(const DTboolean flag)		{	_is_output = flag;	return *this;			}


		/// Returns the next info object in the list
		/// \return Next info object
		EventInfo*                      next_info       (void) const				{	return _next;				}


		/// Create the event binding object
		/// \param fn event binding funtion pointer
        template<typename CLASS>
		void                            set_event		(void (CLASS::*fn)(PlugNode*))
        {
            _event_bind = std::shared_ptr<EventBindBase>(new ("EventBind") EventBind<CLASS>(fn));
        }

		/// Returns the event binding object
		/// \return event binding object
		std::shared_ptr<EventBindBase>  event           (void) const                            {	return _event_bind;         }


		/// Given the event, return a pointer to the node
		/// \param evt event
		/// \return owner node
		PlugNode*                       event_to_node   (const Event *evt) {
			ASSERT(_offset != 0);
			return reinterpret_cast<PlugNode*>( (DTubyte*) evt - _offset);
		}

		/// Given the node, return a pointer to the event
		/// \param node node
		/// \return Event
		Event*                          node_to_event   (const PlugNode *node){
			ASSERT(_offset != 0);
			return reinterpret_cast<Event*>( (DTubyte*) node + _offset);
		}
    
    
        //
        // Event Info pool
        //

        /// Returns the next free index in the pool
		/// \return Next free index
        static DTushort                 get_free_index      (void);

        /// Returns the info at the specified index
		/// \param i index
		/// \return Info
        static EventInfo&               get_info            (DTushort i);

    private:
		DTubyte                         _is_input:1;
		DTubyte                         _is_output:1;
		DTubyte                         _is_no_draw:1;

		DTsize                          _offset;

		EventInfo                       *_next;

		std::string                     _name;
		std::shared_ptr<EventBindBase>  _event_bind;

        static EventInfo                _pool[DT3_EVENT_POOL_SIZE];
};

//==============================================================================
//==============================================================================

#define EVENT_INFO_INDEX(Evt)               Evt##Index

#define IMPLEMENT_EVENT_INFO_INDEX(Evt)     namespace {	DTushort EVENT_INFO_INDEX(Evt) = EventInfo::get_free_index();	}

#define EVENT_OFFSET(Evt)                   (	(DTubyte*) (static_cast<Event*>(&(reinterpret_cast<type*>(0x99999999)->Evt))) - \
                                                (DTubyte*) (static_cast<PlugNode*>(reinterpret_cast<type*>(0x99999999)))	)

#define EVENT_INIT(Evt, Name)               EventInfo::get_info(EVENT_INFO_INDEX(Evt)).init(    \
                                                Name,                                           \
                                                EVENT_OFFSET(Evt),                              \
                                                plug_node_info                                  \
                                            )

//==============================================================================
//==============================================================================

} // DT3

#endif
