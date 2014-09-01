#ifndef DT3_ARCHIVE
#define DT3_ARCHIVE
//==============================================================================
///	
///	File: Archive.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/Utility/Config.hpp"
#include <list>
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class ArchiveData;
class ArchiveProcess;

//==============================================================================
/// Class
//==============================================================================

class Archive: public BaseClass {
    public:
        DEFINE_TYPE(Archive,BaseClass)
         
                                        Archive                     (void);
    
	private:
                                        Archive                     (const Archive &rhs);
        Archive &                       operator =                  (const Archive &rhs);
    
	public:
		virtual                         ~Archive                    (void);
								
	public:		
		/// Streams data into or out of this object
		/// \param data data to stream
		/// \return this archiving object so you can chain them together
		virtual Archive &               operator <<                 (const ArchiveData& data) = 0;

		/// Adds a post process to the reading/writing operation
		/// \param process process to add
		/// \return this archiving object so you can chain them together
		virtual Archive &               add_post_process            (const std::shared_ptr<ArchiveProcess> &process);

		/// Adds a sub domain for the archive
		/// \param domain name of the domain
		virtual void                    push_domain                 (const std::string &domain) = 0;

		/// Balances the sub domain for the archive
		virtual void                    pop_domain                  (void) = 0;
		

		/// Returns the version of the archive
		/// \return the version of the archive
		virtual DTuint                  version                     (void) const                {	return Config::engine_version();			}

		/// Returns the version of the archive
		/// \return the version of the archive
		virtual DTuint                  app_version                 (void) const                {	return AppConfig::app_version();	}

		/// Returns wether the archive is reading or not
		/// \return archive is loading
		virtual DTboolean               is_reading                  (void) const = 0;

		/// Returns wether the arcive is writing or not
		/// \return archive is saving
		virtual DTboolean               is_writing                  (void) const = 0;
        
		/// set this flag to ignore the streaming flag while archiving objects
		/// \param ignore flag to ignore streamable objects or not
        void                            set_ignore_streamable_flag  (const DTboolean ignore)    {   _ignore_streamable_flag = ignore;	}
		
		/// Returns wether streamable objects are ignored or not
		/// \return if streamable flag is ignored
        DTboolean                       ignore_streamable_flag      (void) const                {   return _ignore_streamable_flag;		}

		/// set this flag to ignore the streaming and pointers to other objects
		/// \param ignore flag to ignore pointers to other objects		
        void                            set_recursive				(const DTboolean recursive) {   _recursive = recursive;				}

		/// Returns wether pointers to other objects are ignored or not
		/// \return if pointers to other objects are ignored or not
        DTboolean                       recursive                   (void) const                {   return _recursive;					}
        		
		/// Returns and removes next post process
		/// \return post process	
		std::shared_ptr<ArchiveProcess> pop_post_process            (void);
    
    private:
        friend class ArchiveObjectQueue;
		
		std::list<std::shared_ptr<ArchiveProcess>>		_processes;
        
        DTboolean                       _ignore_streamable_flag;
        DTboolean                       _recursive;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
