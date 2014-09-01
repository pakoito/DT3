#ifndef DT3_ARCHIVEBINARYREADER
#define DT3_ARCHIVEBINARYREADER
//==============================================================================
///	
///	File: ArchiveBinaryReader.hpp
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

class ArchiveBinaryReader: public Archive {
    public:
        DEFINE_TYPE(ArchiveBinaryReader,Archive)
		DEFINE_CREATE
         
                                ArchiveBinaryReader		(void);	
    
	private:
                                ArchiveBinaryReader		(const ArchiveBinaryReader &rhs);
        ArchiveBinaryReader &	operator =              (const ArchiveBinaryReader &rhs);	
    
	public:
		virtual					~ArchiveBinaryReader	(void);
								
	public:		

		/// Opens an archive
		/// \param pathname path to archive
		DTerr					open					(const FilePath &pathname, std::shared_ptr<Progress> progress = NULL);

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
		
		/// Returns the engine version of the archive
		/// \return the engine version of the archive
		DTuint					get_engine_version      (void) const;

		/// Returns the app version of the archive
		/// \return the app version of the archive
		DTuint					get_app_version			(void) const;

		/// Returns wether the archive is reading or not
		/// \return archive is loading
		DTboolean				is_reading				(void) const;

		/// Returns wether the arcive is writing or not
		/// \return archive is saving
		DTboolean				is_writing				(void) const;

		/// Returns the stream
		/// \return stream
		BinaryFileStream&		stream                  (void)		{	return _infile;	}
		
	private:	
		DTuint					_engine_version;
		DTuint					_app_version;
		BinaryFileStream		_infile;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
