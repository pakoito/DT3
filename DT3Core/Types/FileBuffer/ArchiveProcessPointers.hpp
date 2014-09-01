#ifndef DT3_ARCHIVEPROCESSPOINTERSREF
#define DT3_ARCHIVEPROCESSPOINTERSREF
//==============================================================================
///	
///	File: ArchiveProcessPointers.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/ArchiveProcess.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include <memory>
#include <map>

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

template <class T>
class ArchiveProcessPointers: public ArchiveProcess {
    public:
        //DEFINE_TYPE(ArchiveProcessPointers,ArchiveProcess)
         
									ArchiveProcessPointers	(const std::shared_ptr<Archive> &archive, std::shared_ptr<T> &ptr);
    
									ArchiveProcessPointers	(const ArchiveProcessPointers &rhs);
        ArchiveProcessPointers &	operator =				(const ArchiveProcessPointers &rhs);
    
		virtual						~ArchiveProcessPointers	(void);
								
	public:
		/// Post process for archiving
		/// \param archive Archive that owns the post process
		/// \param archive Objects Map of objects that have been archived
		virtual void				process			(const std::shared_ptr<Archive> &archive, std::map<DTuint64, std::shared_ptr<BaseClass>> &objects);
		
	private:
		DTuint64            _ref;
		std::shared_ptr<T>  &_ptr;
};

//==============================================================================
//==============================================================================

template<typename T>
std::shared_ptr<ArchiveProcess> archive_process_pointers_creator (const std::shared_ptr<Archive> &archive, std::shared_ptr<T> &ptr)
{
	return std::shared_ptr<ArchiveProcess> (new ("archive_process_pointers_creator") ArchiveProcessPointers<T>(archive, ptr));
}

#define ARCHIVE_PROCESS_POINTERS(A,V)			archive_process_pointers_creator(A, V)

//==============================================================================
//==============================================================================

template <class T>
ArchiveProcessPointers<T>::ArchiveProcessPointers(const std::shared_ptr<Archive> &archive, std::shared_ptr<T> &ptr)
    :   _ptr    (ptr),   // Remember reference
        _ref    (0)
{
	// Writing out pointer immediately
	if (archive->is_writing()) {
	
		if (archive->ignore_streamable_flag() || (ptr && ptr->streamable())) {

			DTuint ref = (ptr != NULL) ? ptr->unique_id() : 0;
			*archive << ARCHIVE_DATA(ref, DATA_PERSISTENT);
			
			// Remember object for later
			_ref = ref;
			
		// Stream a NULL
		} else {
			DTuint ref = 0;  // This way it'll work with reading and writing
			
			*archive << ARCHIVE_DATA(ref, DATA_PERSISTENT);
			
			_ref = ref;
		}
			
	// Reading in pointer reference immediately
	} else {
		DTuint ref = 0;

		*archive << ARCHIVE_DATA(ref, DATA_PERSISTENT);

		if (ref == 0) {
			ASSERT(!_ptr);  // Make sure pointer is NULL
		} else {
            // Do nothing yet
        }
		
		_ref = ref;
	}
}

template <class T>
ArchiveProcessPointers<T>::ArchiveProcessPointers(const ArchiveProcessPointers<T> &rhs)
{

}

template <class T>
ArchiveProcessPointers<T>& ArchiveProcessPointers<T>::operator = (const ArchiveProcessPointers<T> &rhs)
{

}

template <class T>
ArchiveProcessPointers<T>::~ArchiveProcessPointers(void)
{

}

template <class T>
void ArchiveProcessPointers<T>::process (const std::shared_ptr<Archive> &archive, std::map<DTuint64, std::shared_ptr<BaseClass>> &objects)
{
	// write the actual object
	if (archive->is_writing()) {
		// If it's NULL then ignore it
		if (!_ptr || _ref == 0)
			return;
	
		// Check to see if the object is already written
		if (objects.find( _ptr->unique_id() ) != objects.end())
			return;
			
		// If we made it this far, then stream the object

		// Add the object to the list so it won't get streamed multiple times
		objects[_ptr->unique_id()] = _ptr;

		// write the type of the object to the archive so the object can be
		// created when streaming in
	    std::string obj_type = _ptr->class_id_child();
		*archive << ARCHIVE_DATA(obj_type, DATA_PERSISTENT);

        archive->push_domain("obj");
		_ptr->archive(archive);
        archive->pop_domain();

	// Connect pointers
	} else {
    
        if (_ref == 0)
            return;

        // Check to see if the object is already read. It better be!
        auto i = objects.find(_ref);
        ASSERT (i != objects.end());
        
        // Hook up the pointer
        _ptr = checked_cast<T>(i->second);
	}
}

//==============================================================================
//==============================================================================

} // DT3

#endif
