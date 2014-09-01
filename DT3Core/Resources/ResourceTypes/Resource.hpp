#ifndef DT3_RESOURCE
#define DT3_RESOURCE
//==============================================================================
///	
///	File: Resource.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include <vector>
#include <mutex>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Base resource type
//==============================================================================

class Resource: public BaseClass {
    public:
        DEFINE_TYPE(Resource,BaseClass)
		DEFINE_CREATE

         
									Resource            (void);
    private:
									Resource            (const Resource &rhs);
        Resource &					operator =          (const Resource &rhs);
	public:
        virtual						~Resource           (void);
                
    public:        	

		/// Import the resource with an importer into a platform independent format
		/// \param pathname path to resource
		/// \param args importer args
		/// \return error code
		virtual DTerr				import              (const FilePath &pathname, std::string args);

		/// Reload the resource
		/// \return error code
		virtual DTerr				reload              (void);
				
		/// Checks if a reload is needed
		/// \return reload needed
		virtual DTboolean           is_changed        (void) const;

		//
		// Queries for the paths
		//

		/// Gets the path that was types into the properties panel
		/// \return property value
		std::string                 property_path       (void) const                        {	return _file_path.original_path();      }

		/// Gets the path for the resource
		/// \return path
		const FilePath&				path                (void) const                        {	return _file_path;						}

		//
		// Args
		//

		/// Arguments that the resource was loaded with
		/// \return Arguments
		const std::string &         args                (void) const                        {	return _args;							}
		

		//
		// Status flags
		//
		  		
		/// Pass time in the resource if needed
		/// \param dt delta time
		virtual void				tick                (const DTfloat dt)                  {}
		
		/// Resource lock (used in multi threaded applications)
        void                        lock                (void)                              {   _lock.lock();   }

		/// Resource unlock (used in multi threaded applications)
        void                        unlock              (void)                              {   _lock.unlock();   }

		/// Add a file dependency so that the editor can reload all related files if necessary
		/// \param path path to file dependency
        void                        add_dependency      (const FilePath &path)              {   _dependencies.push_back(path);   }

		/// Add file dependencies so that the editor can reload all related files if necessary
		/// \param paths paths to file dependencies
        void                        add_dependencies    (const std::vector<FilePath> &paths);
	
	private:
		DTuint64					_time_loaded;
		FilePath					_file_path;
		std::string                 _args;
        
        std::vector<FilePath>       _dependencies;
        std::mutex                  _lock;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
