#ifndef DT3_ARCHIVEPROCESSEVENTS
#define DT3_ARCHIVEPROCESSEVENTS
//==============================================================================
///	
///	File: ArchiveProcessEvents.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/ArchiveProcess.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveProcessPointers.hpp"
#include <map>
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

template <class T>
class ArchiveProcessEvents: public ArchiveProcess {
    public:
        //DEFINE_TYPE(ArchiveProcessEvents,ArchiveProcess)
         
									ArchiveProcessEvents	(const std::shared_ptr<Archive> &archive, T dst_event);	
									ArchiveProcessEvents    (const ArchiveProcessEvents &rhs);
        ArchiveProcessEvents &		operator =				(const ArchiveProcessEvents &rhs);	
		virtual						~ArchiveProcessEvents	(void);
								
	public:
		/// Post process for archiving
		/// \param archive Archive that owns the post process
		/// \param archive Objects Map of objects that have been archived
		virtual void				process                 (const std::shared_ptr<Archive> &archive, std::map<DTuint64, std::shared_ptr<BaseClass>> &objects);
		
	private:	
    
        DTuint                      _dst_node_id;
        std::string                 _dst_name;

        struct Connection {	
            DTuint                  _src_node_id;
            std::string             _src_name;
        };
        std::vector<Connection>     _connections;
};

//==============================================================================
//==============================================================================

template<typename T>
std::shared_ptr<ArchiveProcess> archive_process_events_creator (const std::shared_ptr<Archive> &archive, T dst_event)
{
	return std::shared_ptr<ArchiveProcess> (new ("archive_process_events_creator") ArchiveProcessEvents<T>(archive, dst_event));
}

#define ARCHIVE_PROCESS_EVENTS(A,V)			archive_process_events_creator(A, V)

//==============================================================================
//==============================================================================

template <class T>
ArchiveProcessEvents<T>::ArchiveProcessEvents(const std::shared_ptr<Archive> &archive, T dst_event)
{
	// Writing out pointer immediately
	if (archive->is_writing()) {
        const std::vector<Event*>& src_events	= dst_event->incoming_connections();
        
        archive->push_domain ("EventConnection");

        DTsize num_connections = src_events.size();
        *archive << ARCHIVE_DATA(num_connections, DATA_PERSISTENT);

        DTuint	dst_node_id = dst_event->owner()->unique_id();
        std::string  dst_name = dst_event->name();

        *archive << ARCHIVE_DATA(dst_node_id, DATA_PERSISTENT);
        *archive << ARCHIVE_DATA(dst_name, DATA_PERSISTENT);

		for (DTuint i = 0; i < num_connections; ++i) {
			DTuint	src_node_id = src_events[i]->owner()->unique_id();
			std::string	src_name = src_events[i]->name();
		
			*archive << ARCHIVE_DATA(src_node_id, DATA_PERSISTENT);
			*archive << ARCHIVE_DATA(src_name, DATA_PERSISTENT);
        }
        
        archive->pop_domain ();

	
	// Reading in pointer reference immediately
	} else {
		DTuint		src_node_id;
		DTuint		dst_node_id;
		std::string	src_name;
		std::string dst_name;
        
        archive->push_domain ("EventConnection");

        DTsize num_connections;
        *archive << ARCHIVE_DATA(num_connections, DATA_PERSISTENT);

        *archive << ARCHIVE_DATA(dst_node_id, DATA_PERSISTENT);
        *archive << ARCHIVE_DATA(dst_name, DATA_PERSISTENT);

        _dst_node_id = dst_node_id;
        _dst_name = dst_name;

		for (DTuint i = 0; i < num_connections; ++i) {
            *archive << ARCHIVE_DATA(src_node_id, DATA_PERSISTENT);
            *archive << ARCHIVE_DATA(src_name, DATA_PERSISTENT);
            
            Connection c;
            c._src_node_id = src_node_id;
            c._src_name = src_name;
            
            _connections.push_back(c);
        }
			
        archive->pop_domain ();

	}
}

template <class T>
ArchiveProcessEvents<T>::ArchiveProcessEvents(const ArchiveProcessEvents<T> &rhs)
{

}

template <class T>
ArchiveProcessEvents<T>& ArchiveProcessEvents<T>::operator = (const ArchiveProcessEvents<T> &rhs)
{

}

template <class T>
ArchiveProcessEvents<T>::~ArchiveProcessEvents(void)
{

}

template <class T>
void ArchiveProcessEvents<T>::process (const std::shared_ptr<Archive> &archive, std::map<DTuint64, std::shared_ptr<BaseClass>> &objects)
{
	// write the actual object
	if (archive->is_writing()) {
		
		// Nothing to do here.
		
	// Connect events
	} else {
    
		for (std::size_t i = 0; i < _connections.size(); ++i) {
    
            // Good to check but probably won't happen
            if (_connections[i]._src_node_id == 0 || _dst_node_id == 0)
                return;
            
            // Check to see if the object is already read. It better be! If not, check if all 
            // objects have been created. I bet they haven't.
            auto src_obj = objects.find(_connections[i]._src_node_id);
            ERROR(src_obj != objects.end(), "Unable to connect events");
            
            auto dst_obj = objects.find(_dst_node_id);
            ERROR(dst_obj != objects.end(), "Unable to connect events");
            
            // Convert to EventNodes
            std::shared_ptr<PlugNode> src_node = checked_cast<PlugNode>(src_obj->second);
            std::shared_ptr<PlugNode> dst_node = checked_cast<PlugNode>(dst_obj->second);
            
            if (src_node && dst_node) {
            
                // get Events
                Event *src_event = src_node->event_by_name(_connections[i]._src_name);
                Event *dst_event = dst_node->event_by_name(_dst_name);
                                
                // If everything is good, then connect them
                if (src_event && dst_event) {
                    // Check if inputs were swapped
                    if ( (dst_event->is_output() && src_event->is_input()) && !(src_event->is_output() && dst_event->is_input())) {
                        std::swap(src_event,dst_event);
                        WARNINGMSG("Event input and output were swapped.");
                    }

                    dst_event->add_incoming_connection(src_event);
                }
            }
            
        }

	}
}

//==============================================================================
//==============================================================================

} // DT3

#endif
