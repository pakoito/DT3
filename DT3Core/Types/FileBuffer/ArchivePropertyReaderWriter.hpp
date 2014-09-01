#ifndef DT3_ARCHIVEPROPERTYREADERWRITER
#define DT3_ARCHIVEPROPERTYREADERWRITER
//==============================================================================
///	
///	File: ArchivePropertyReaderWriter.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include <vector>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class ArchiveData;

//==============================================================================
/// Class
//==============================================================================

class ArchivePropertyReaderWriter: public Archive {
    public:
        DEFINE_TYPE(ArchivePropertyReaderWriter,Archive)
		DEFINE_CREATE
         
                                                    ArchivePropertyReaderWriter		(void);
    
	private:
                                                    ArchivePropertyReaderWriter		(const ArchivePropertyReaderWriter &rhs);
        ArchivePropertyReaderWriter &               operator =						(const ArchivePropertyReaderWriter &rhs);
    
	public:
		virtual                                     ~ArchivePropertyReaderWriter	(void);
								
	public:			
		/// Streams data into or out of this object
		/// \param data data to stream
		/// \return reference to this
		virtual Archive &                           operator <<						(const ArchiveData& data);
    
		/// Adds a sub domain for the archive
		/// \param domain name of the domain
		virtual void                                push_domain						(const std::string &domain);

		/// Balances the sub domain for the archive
		virtual void                                pop_domain						(void);
		
		/// Returns wether the archive is reading or not
		/// \return archive is loading
		DTboolean                                   is_reading						(void) const;

		/// Returns wether the arcive is writing or not
		/// \return archive is saving
		DTboolean                                   is_writing						(void) const;
		
		/// Call before loading data
		void                                        begin_reading					(void);
		
		/// Call before saving data
		void                                        begin_writing					(void);
		
		/// Resets the state of the class
		void                                        clear							(void);
		
		/// Returns number of properties saved in the class
		/// \return number of properties
		std::vector<std::shared_ptr<ArchiveData>>&  properties                      (void)      {   return _properties; }
		
        /// Returns a property with a certain name
		/// \param name name of property
		/// \return property
        std::shared_ptr<ArchiveData>                property                        (const std::string &name);
        
	private:		
		std::vector<std::shared_ptr<ArchiveData>>   _properties;
		DTboolean                                   _loading;
		DTuint                                      _loading_index;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
