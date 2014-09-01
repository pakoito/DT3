#ifndef DT3_ARCHIVEBINARYWRITER
#define DT3_ARCHIVEBINARYWRITER
//==============================================================================
///	
///	File: ArchiveBinaryWriter.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/BinaryFileStream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class ArchiveBinaryWriter: public Archive {
    public:
        DEFINE_TYPE(ArchiveBinaryWriter,Archive)
		DEFINE_CREATE
         
                                ArchiveBinaryWriter		(void);	
    
	private:
                                ArchiveBinaryWriter		(const ArchiveBinaryWriter &rhs);
        ArchiveBinaryWriter &	operator =              (const ArchiveBinaryWriter &rhs);	
    
	public:
		virtual					~ArchiveBinaryWriter	(void);
								
	public:		
		/// Opens an archive
		/// \param pathname path to archive
		DTerr					open					(const FilePath &pathname);

		/// Closes archive
		void					close					(void);
	
		/// Streams data into or out of this object
		/// \param data data to stream
		/// \return this archiving object so you can chain them together
		virtual Archive &		operator <<				(const ArchiveData& data);
		
		/// Adds a sub domain for the archive
		/// \param domain name of the domain
		virtual void			push_domain             (const std::string &domain);

		/// Balances the sub domain for the archive
		virtual void			pop_domain				(void);
		
		/// Returns wether the archive is reading or not
		/// \return archive is loading
		DTboolean				is_reading				(void) const;

		/// Returns wether the arcive is writing or not
		/// \return archive is saving
		DTboolean				is_writing				(void) const;

		/// Returns the stream
		/// \return stream
		BinaryFileStream&		stream                  (void)		{	return _outfile;	}
		
	private:		
		BinaryFileStream		_outfile;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
