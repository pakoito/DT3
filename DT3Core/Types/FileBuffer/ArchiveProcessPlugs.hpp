#ifndef DT3_ARCHIVEPROCESSPLUGS
#define DT3_ARCHIVEPROCESSPLUGS
//==============================================================================
///	
///	File: ArchiveProcessPlugs.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/ArchiveProcess.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveProcessPointers.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/World/World.hpp"
#include "DT3Core/World/WorldNode.hpp"
#include "DT3Core/Scripting/ScriptingBase.hpp"
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
class ArchiveProcessPlugs: public ArchiveProcess {
    public:
        //DEFINE_TYPE(ArchiveProcessPlugs,ArchiveProcess)
         
									ArchiveProcessPlugs		(const std::shared_ptr<Archive> &archive, T dst_plug);	
									ArchiveProcessPlugs		(const ArchiveProcessPlugs &rhs);
        ArchiveProcessPlugs &		operator =				(const ArchiveProcessPlugs &rhs);	
		virtual						~ArchiveProcessPlugs	(void);
								
	public:
		/// Post process for archiving
		/// \param archive Archive that owns the post process
		/// \param archive Objects Map of objects that have been archived
		virtual void				process                 (const std::shared_ptr<Archive> &archive, std::map<DTuint64, std::shared_ptr<BaseClass>> &objects);
		
	private:		
		DTuint                      _src_node_id;
		DTuint                      _dst_node_id;
		std::string                 _src_name;
		std::string                 _dst_name;
};

//==============================================================================
//==============================================================================

template<typename T>
std::shared_ptr<ArchiveProcess> archive_process_plugs_creator (const std::shared_ptr<Archive> &archive, T dst_plug)
{
	return std::shared_ptr<ArchiveProcess> (new ("archive_process_plugs_creator") ArchiveProcessPlugs<T>(archive, dst_plug));
}

#define ARCHIVE_PROCESS_PLUGS(A,V)			archive_process_plugs_creator(A, V)

//==============================================================================
//==============================================================================

template <class T>
ArchiveProcessPlugs<T>::ArchiveProcessPlugs(const std::shared_ptr<Archive> &archive, T dst_plug)
{
	// Writing out pointer immediately
	if (archive->is_writing()) {
		T	src_plug = dst_plug->incoming_connection();
		
		if (src_plug) {
			DTuint	src_node_id = src_plug->owner()->unique_id();
			DTuint	dst_node_id = dst_plug->owner()->unique_id();
			std::string	src_name = src_plug->name();
			std::string dst_name = dst_plug->name();
		
			archive->push_domain ("PlugConnection");
			*archive << ARCHIVE_DATA(src_node_id, DATA_PERSISTENT);
			*archive << ARCHIVE_DATA(dst_node_id, DATA_PERSISTENT);
			*archive << ARCHIVE_DATA(src_name, DATA_PERSISTENT);
			*archive << ARCHIVE_DATA(dst_name, DATA_PERSISTENT);
			archive->pop_domain ();
			
		// No Plug
		} else {
			_src_node_id = _dst_node_id = 0;	// Shouldn't ever hit this anyway
		}
	
	// Reading in pointer reference immediately
	} else {
		DTuint		src_node_id;
		DTuint		dst_node_id;
		std::string      src_name;
		std::string      dst_name;

		archive->push_domain ("PlugConnection");
		*archive << ARCHIVE_DATA(src_node_id, DATA_PERSISTENT);
		*archive << ARCHIVE_DATA(dst_node_id, DATA_PERSISTENT);
		*archive << ARCHIVE_DATA(src_name, DATA_PERSISTENT);
		*archive << ARCHIVE_DATA(dst_name, DATA_PERSISTENT);
		archive->pop_domain ();
        
#ifndef DT3_DISABLE_PLUG_CONNECTION_HACK
        // TODO: Remove this hack
        if (src_name == "World_Translation" && dst_name == "Parent_Translation") {
            src_name = "World_Transform";
            dst_name = "Parent_Transform";
        }
#endif
				
		_src_node_id = src_node_id;
		_dst_node_id = dst_node_id;
		_src_name = src_name;
		_dst_name = dst_name;
	}
}

template <class T>
ArchiveProcessPlugs<T>::ArchiveProcessPlugs(const ArchiveProcessPlugs<T> &rhs)
{

}

template <class T>
ArchiveProcessPlugs<T>& ArchiveProcessPlugs<T>::operator = (const ArchiveProcessPlugs<T> &rhs)
{

}

template <class T>
ArchiveProcessPlugs<T>::~ArchiveProcessPlugs(void)
{

}

template <class T>
void ArchiveProcessPlugs<T>::process (const std::shared_ptr<Archive> &archive, std::map<DTuint64, std::shared_ptr<BaseClass>> &objects)
{
	// write the actual object
	if (archive->is_writing()) {
		
		// Nothing to do here.
		
	// Connect plugs
	} else {
		// Good to check but probably won't happen
		if (_src_node_id == 0 || _dst_node_id == 0)
			return;
		
		// Check to see if the object is already read. It better be! If not, check if all 
		// objects have been created. I bet they haven't.
		auto src_obj = objects.find(_src_node_id);
        ERROR(src_obj != objects.end(), "Unable to connect plugs");
		
		auto dst_obj = objects.find(_dst_node_id);
        ERROR(dst_obj != objects.end(), "Unable to connect events");
		
		// Convert to PlugNodes
		std::shared_ptr<PlugNode> src_node = checked_static_cast<PlugNode>(src_obj->second);
		std::shared_ptr<PlugNode> dst_node = checked_static_cast<PlugNode>(dst_obj->second);
                
        if (src_node && dst_node) {
		
            // get Plugs
            PlugBase *src_plug = src_node->plug_by_name(_src_name);
            PlugBase *dst_plug = dst_node->plug_by_name(_dst_name);
                        
            // If everything is good, then connect them
            if (src_plug && dst_plug) {
            
                // Check if inputs were swapped
                if ( (dst_plug->is_output() && src_plug->is_input()) && !(src_plug->is_output() && dst_plug->is_input())) {
                    std::swap(src_plug,dst_plug);
                    WARNINGMSG("Plug input and output were swapped.");
                }

                // If the types are correct then connect them
                if ( src_plug->is_compatible(dst_plug) ) {
                    dst_plug->set_incoming_connection(src_plug);
                    
                }
            
            }
        }

	}
}

//==============================================================================
//==============================================================================

} // DT3

#endif
