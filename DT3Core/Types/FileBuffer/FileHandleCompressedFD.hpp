#ifndef DT3_FILEHANDLECOMPRESSEDFD
#define DT3_FILEHANDLECOMPRESSEDFD
//==============================================================================
///	
///	File: FileHandleCompressedFD.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/FileBuffer/FileHandle.hpp"
#include <vector>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class FilePath;

//==============================================================================
/// Class
//==============================================================================

class FileHandleCompressedFD: public FileHandle {
    public:
        DEFINE_TYPE(FileHandleCompressedFD,FileHandle)
		//DEFINE_CREATE_AND_CLONE
		DEFINE_CREATE
         
                                FileHandleCompressedFD  (void);
	private:
                                FileHandleCompressedFD  (const FileHandleCompressedFD &rhs);
        FileHandleCompressedFD &  operator =            (const FileHandleCompressedFD &rhs);
	public:
        virtual                 ~FileHandleCompressedFD (void);

	public:
		//
		// File access routines
		//
		
		/// Opens a compressed file
		/// \param pathname path to file
		/// \param start start of file in fragment
		/// \param length length of file in fragment
		/// \param uncompressed_length uncompressed length of file in fragment
		virtual DTerr           open_file               (const FilePath &pathname, DTsize start, DTsize length, DTsize uncompressed_length);

		/// Sets an existing file descriptor for file
		/// \param fd file descriptor
		/// \return Error
		virtual DTerr           set_fd                  (DTint fd, DTsize start, DTsize length, DTsize uncompressed_length);

		/// Close the file handle
		virtual void            close                   (void);

		/// Returns the length of the file
		/// \return Length of the file
		virtual DTsize          length                  (void) const	{	return _data.size();			}
		
		
		/// Return write position
		/// \return write position
		virtual DTsize          p                       (void)	{	return 0;	}

		/// Return read position
		/// \return read position
		virtual DTsize          g                       (void)	{	return _file_g;	}
		
		/// Change write position
		/// \param p position
		/// \param r relative
		virtual void            seek_p                  (DToffset p, Relative r);

		/// Change read position
		/// \param p position
		/// \param r relative
		virtual void            seek_g                  (DToffset g, Relative r);


		/// Peeks ahead at the next byte
		/// \return next byte
		virtual DTcharacter     peek                    (void)	{	return _data[_file_g];	}

		/// Ignores the next byte
		virtual void            ignore                  (void)	{	DTubyte b; read(&b, 1);	}

		
		/// Checks for end of file
		/// \return End of file
		virtual DTboolean       is_eof                  (void)	{	return _eof;		}
		
	
		/// Reads a chunk of raw binary data
		/// \param buffer raw buffer
		/// \param size size of raw buffer
		/// \return actual number of bytes read
		virtual DTsize          read                    (DTubyte *buffer, DTsize size);

		/// Writes a chunk of raw binary data
		/// \param buffer raw buffer
		/// \param size size of raw buffer
		virtual void            write                   (const DTubyte *buffer, DTsize size);

		
	private:	
        std::vector<DTubyte>    _data;
        DTboolean               _eof;
        DTsize                  _file_g;
};

//==============================================================================
//==============================================================================


} // DT3

#endif
