//==============================================================================
///	
///	File: ArchiveObjectQueue.cpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/ArchiveObjectQueue.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveProcess.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveProcessPointers.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/System/Factory.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

std::shared_ptr<BaseClass> ArchiveObjectQueue::queue_in_tree (  const std::shared_ptr<Archive> &archive,
                                                                std::shared_ptr<Callback<std::shared_ptr<BaseClass>>> obj_loaded_cb)
{    
	std::shared_ptr<BaseClass> first = NULL;
	std::map<DTuint64, std::shared_ptr<BaseClass>>  newly_created_objects;
		
	std::string obj_type;
	*archive << ARCHIVE_DATA(obj_type, DATA_PERSISTENT);
	
	while (obj_type != "end") {
    
        archive->push_domain("obj");
    
		LOG_MESSAGE << "queueInTree - Creating object of type: " << obj_type;

		// Create the object and stream in parameters
		std::shared_ptr<BaseClass> obj = Factory::create_object(obj_type);
        if (!obj) {
            LOG_MESSAGE << "Unable to create object of type: " << obj_type;
            break;
        }
        			
		// Remember the first object
		if (!first) {
			first = obj;
		}
	
		obj->archive (archive);
        
        // Report that an object was created
        if (obj_loaded_cb) {
            (*obj_loaded_cb)(obj);
        }

		// Store new object for reference
		newly_created_objects[obj->unique_id()] = obj;
	
        archive->pop_domain();

		// get next type
		*archive << ARCHIVE_DATA(obj_type, DATA_PERSISTENT);
    }
		
	// run all post processes
	std::shared_ptr<ArchiveProcess> process = archive->pop_post_process();
	while (process != NULL) {
		process->process(archive,newly_created_objects);
		process = archive->pop_post_process();
	}
	
	
	// Finish up reading
	for (auto o = newly_created_objects.rbegin(); o != newly_created_objects.rend(); ++o) {
		o->second->new_unique_id();	// make sure ID is unique
		o->second->archive_done(archive);
	}
	
	return first;
}

//==============================================================================
//==============================================================================

void ArchiveObjectQueue::queue_out_tree (const std::shared_ptr<Archive> &archive, BaseClass *ptr)
{
    std::map<DTuint64, std::shared_ptr<BaseClass>>  queued_objects;
	
	// Write out first object
	std::string obj_type = ptr->class_id_child();
	*archive << ARCHIVE_DATA(obj_type, DATA_PERSISTENT);
    
    archive->push_domain("obj");
    ptr->archive(archive);
    archive->pop_domain();

	// run all post processes which should write the remainder of the objects
    std::shared_ptr<ArchiveProcess> process(archive->pop_post_process());
	while (process != NULL) {
		process->process(archive,queued_objects);
		process = archive->pop_post_process();
	}
 
	// This denotes the end of the objects in the archive
	obj_type = "end";
	*archive << ARCHIVE_DATA(obj_type, DATA_PERSISTENT);
	
	//
    // Archive_Write_Done message
	//
	    
    for (auto &i : queued_objects) {
        i.second->archive_done(archive);
    }
        
}

void ArchiveObjectQueue::queue_out_tree (const std::shared_ptr<Archive> &archive, const std::shared_ptr<BaseClass> &ptr)
{
    ArchiveObjectQueue::queue_out_tree (archive, ptr.get());
}

//==============================================================================
//==============================================================================

} // DT3

