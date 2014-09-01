#ifndef DT3_ARCHIVETEXTWRITER
#define DT3_ARCHIVETEXTWRITER
//==============================================================================
///	
///	File: ArchiveTextWriter.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/TextFileStream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class ArchiveTextWriter: public Archive {
    public:
        DEFINE_TYPE(ArchiveTextWriter,Archive)
		DEFINE_CREATE
         
                                ArchiveTextWriter		(void);	
    
	private:
                                ArchiveTextWriter		(const ArchiveTextWriter &rhs);
        ArchiveTextWriter &		operator =              (const ArchiveTextWriter &rhs);	
    
	public:
		virtual					~ArchiveTextWriter		(void);
								
	public:		
		/// Opens an archive
		/// \param pathname path to archive
		DTerr					open					(const FilePath &pathname);

		/// Closes archive
		void					close					(void);
	
		/// Streams data into or out of this object
		/// \param data data to stream
		/// \return this archiving object so you can chain them together
		virtual Archive &		operator <<             (const ArchiveData& data);
		
		/// Adds a sub domain for the archive
		/// \param domain name of the domain
		virtual void			push_domain             (const std::string &domain);

		/// Balances the sub domain for the archive
		virtual void			pop_domain				(void);
		
		/// Returns wether the archive is reading or not
		/// \return archive is loading
		DTboolean				is_reading				(void) const;

		/// Returns wether the archive is writing or not
		/// \return archive is saving
		DTboolean				is_writing				(void) const;
		
	private:
		std::string             indent					(void) const;
		
		DTint					_indent;
		TextFileStream			_outfile;

};

//==============================================================================
//==============================================================================

} // DT3

#endif
