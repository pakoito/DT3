#ifndef DT3_EVENT
#define DT3_EVENT
//==============================================================================
///	
///	File: Event.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Node/EventInfo.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class PlugNode;
class PlugNodeInfo;

//==============================================================================
/// Class
//==============================================================================

class Event {
    public:         
        DEFINE_TYPE_SIMPLE_BASE(Event)

                                            Event       (void);
                                            Event       (DTushort info);
                                            Event       (const Event &rhs);
    private:
        Event &                             operator =  (const Event &rhs);
    public:
                                            ~Event      (void);
	
	public:
		/// Called to initialize the class
		static void                         initialize_static           (void);

		/// Return a pointer to the owning node of the Event
		/// \return Owner node
		PlugNode*                           owner                       (void) const    {	return info().event_to_node(this);  }

		/// Returns the name of the event
		/// \return Name of the event
		const std::string&                  name                        (void) const    {	return info().name();           }
		
        /// Returns the full name of object that uniquely identifies it
		/// \return full name of object
        std::string                         full_name                   (void) const;
		
    
    
		/// Returns no draw flag
		/// \return No draw flag
		DTboolean                           is_no_draw                  (void) const    {	return info().is_no_draw();		}
	
		/// Returns is input flag
		/// \return is input
		DTboolean                           is_input                    (void) const    {	return info().is_input();		}
    
		/// Returns is output flag
		/// \return is output
		DTboolean                           is_output                   (void) const    {	return info().is_output();		}
		
		
    
		/// Returns all incoming connections
		/// \return incoming connections
		std::vector<Event*>                 incoming_connections        (void) const;

		/// Add an incoming event connection
		/// \param incoming incoming event
		void                                add_incoming_connection     (Event* incoming);

		/// Remove an incoming connection
		/// \param incoming incoming event
		void                                remove_incoming_connection	(Event* incoming);

		/// Remove all incoming connections
		void                                remove_incoming_connections	(void);

		/// Returns true if there is at least one incoming connection
		/// \return has incoming connection
        DTboolean                           has_incoming_connection		(void) const;


		/// Returns all outgoing connections
		/// \return outgoing connections
        std::vector<Event*>                 outgoing_connections        (void) const;

		/// Add an outgoing event connection
		/// \param outgoing outgoing event
		void                                add_outgoing_connection		(Event* outgoing);
		
		/// Remove an outgoing connection
		/// \param outgoing outgoing event
		void                                remove_outgoing_connection	(Event* outgoing);

		/// Remove all outgoing connections
		void                                remove_outgoing_connections	(void);
		
		/// Returns true if there is at least one outgoing connection
		/// \return has outgoing connection
        DTboolean                           has_outgoing_connection		(void) const;
		
        
    
		/// Send the event to all outgoing connections
		/// \param sender Node that sent the event
		void                                send                        (PlugNode *sender = NULL);

	private:
        /// Return a pointer to the owning node of the Plug
		/// \return Owner node
		EventInfo&                          info                        (void) const    {	return EventInfo::get_info(_info_index); }

        // Trigger this event
		void                                trigger                     (PlugNode *sender);
    
    
        // Data indices
		DTuint                          _info_index:14;         // Index into Event info pool
		DTuint                          _connection_index:15;   // Index into connections pool
    
        // Flags
        DTuint                          _is_computing:1;        // Is computing flag
        DTuint                          _spare_0:1;             // Spare flag
        DTuint                          _spare_1:1;             // Spare flag
    
    
        //
        // Connection list pool
        //
    
        struct EventConnections {
            std::vector<Event*>         _incoming;
            std::vector<Event*>         _outgoing;
            EventConnections            *_next_free;
        };

        static EventConnections         _pool[DT3_EVENT_CONNECTION_POOL_SIZE];
        static EventConnections         *_free_list;
        static std::mutex               _free_list_mutex;

        EventConnections&               connections                     (void);
        EventConnections&               connections                     (void) const;
        void                            free_connections                (void);

};

//==============================================================================
//==============================================================================

} // DT3

#endif
