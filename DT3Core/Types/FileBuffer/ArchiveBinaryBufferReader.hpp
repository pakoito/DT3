#ifndef DT3_ARCHIVEBINARYBUFFERREADER
#define DT3_ARCHIVEBINARYBUFFERREADER
//==============================================================================
///	
///	File: ArchiveBinaryBufferReader.hpp
///
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/BinaryBufferStream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================



//==============================================================================
/// Class
//==============================================================================

class ArchiveBinaryBufferReader: public Archive {
    public:
        DEFINE_TYPE(ArchiveBinaryBufferReader,Archive)
		DEFINE_CREATE
         
                                    ArchiveBinaryBufferReader   (void);	
    
	private:
                                    ArchiveBinaryBufferReader   (const ArchiveBinaryBufferReader &rhs);
        ArchiveBinaryBufferReader &	operator =                  (const ArchiveBinaryBufferReader &rhs);	
    
	public: 
		virtual                     ~ArchiveBinaryBufferReader	(void);
								
	public:		
	
		/// Streams data into or out of this object
		/// \param data data to stream
		/// \return this archiving object so you can chain them together
		virtual Archive &           operator <<                 (const ArchiveData& data);
		
		/// Adds a sub domain for the archive
		/// \param domain name of the domain
		virtual void                push_domain                 (const std::string &domain);

		/// Balances the sub domain for the archive
		virtual void                pop_domain                  (void);
		
		/// Returns wether the archive is reading or not
		/// \return archive is loading
		DTboolean                   is_reading                  (void) const;

		/// Returns wether the arcive is writing or not
		/// \return archive is saving
		DTboolean                   is_writing                  (void) const;

		/// Returns the stream
		/// \return stream
		BinaryBufferStream&         stream                      (void)		{	return _buffer;	}
		
	private:	
		BinaryBufferStream          _buffer;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
