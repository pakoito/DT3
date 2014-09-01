#ifndef DT3_FILEHANDLE
#define DT3_FILEHANDLE
//==============================================================================
///	
///	File: FileHandle.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class FileHandle: public BaseClass {
    public:
        DEFINE_TYPE(FileHandle,BaseClass)
		//DEFINE_CREATE_AND_CLONE
		//DEFINE_CREATE
         
							FileHandle			(void)  {}
	private:
							FileHandle			(const FileHandle &rhs);
        FileHandle &		operator =			(const FileHandle &rhs);
	public:
        virtual				~FileHandle			(void)  {}

	public:
		enum Relative {
			FROM_BEGINNING,
			FROM_END,
			FROM_CURRENT
		};
		
		//
		// File access routines
		//
		
		/// Close the file handle
		virtual void            close				(void) = 0;

		/// Returns the length of the file
		/// \return Length of the file
		virtual DTsize          length              (void) const = 0;
		
		
		/// Return write position
		/// \return write position
		virtual DTsize          p                   (void) = 0;

		/// Return read position
		/// \return read position
		virtual DTsize          g                   (void) = 0;
		
		/// Change write position
		/// \param p position
		/// \param r relative
		virtual void            seek_p              (DToffset p, Relative r) = 0;
    
		/// Change read position
		/// \param p position
		/// \param r relative
		virtual void            seek_g              (DToffset g, Relative r) = 0;


		/// Peeks ahead at the next byte
		/// \return next byte
		virtual DTcharacter     peek				(void) = 0;

		/// Ignores the next byte
		virtual void            ignore				(void) = 0;

		
		/// Checks for end of file
		/// \return End of file
		virtual DTboolean       is_eof				(void) = 0;
		
	
		/// Reads a chunk of raw binary data
		/// \param buffer raw buffer
		/// \param size size of raw buffer
		/// \return actual number of bytes read
		virtual DTsize          read				(DTubyte *buffer, DTsize size) = 0;

		/// Writes a chunk of raw binary data
		/// \param buffer raw buffer
		/// \param size size of raw buffer
		virtual void            write				(const DTubyte *buffer, DTsize size) = 0;
};

//==============================================================================
//==============================================================================


} // DT3

#endif
