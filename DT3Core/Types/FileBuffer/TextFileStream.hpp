#ifndef DT3_TEXTFILESTREAM
#define DT3_TEXTFILESTREAM
//==============================================================================
///	
///	File: TextFileStream.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/FileBuffer/TextStream.hpp"
#include <memory>
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class FilePath;
class FileHandle;
class Progress;

//==============================================================================
/// Class
//==============================================================================

class TextFileStream: public TextStream {
    public:
        DEFINE_TYPE(TextFileStream,TextStream)
		DEFINE_CREATE
         
                                            TextFileStream          (void);
        explicit                            TextFileStream          (const FilePath &pathname);
    
    private:
                                            TextFileStream          (const TextFileStream &rhs);
        TextFileStream &                    operator =              (const TextFileStream &rhs);
    
    public:
        virtual                             ~TextFileStream         (void);

	public:
		/// Called to initialize the object
		virtual void                        initialize              (void);


		Stream&                             operator<<              (const Stream::FS)	{	*this << " ";	return *this;	}
		Stream&                             operator<<              (const DTboolean n);
		Stream&                             operator<<              (const DTbyte n);
		Stream&                             operator<<              (const DTshort n);
		Stream&                             operator<<              (const DTint n);
		Stream&                             operator<<              (const DTubyte n);
		Stream&                             operator<<              (const DTushort n);
		Stream&                             operator<<              (const DTuint n);
		Stream&                             operator<<              (const DTint64 n);
		Stream&                             operator<<              (const DTuint64 n);
#if DT3_SIZES_ARE_DISTINCT_TYPES
		Stream&                             operator<<              (const DTsize n);
#endif
		Stream&                             operator<<              (const DTfloat n);
		Stream&                             operator<<              (const DTdouble n);
		Stream&                             operator<<              (const DTcharacter n);
		Stream&                             operator<<              (const char* n);
		Stream&                             operator<<              (const std::string &n);
		Stream&                             operator<<              (const Stream& n);


		Stream&                             operator>>              (DTboolean &n);
		Stream&                             operator>>              (DTbyte &n);
		Stream&                             operator>>              (DTshort &n);
		Stream&                             operator>>              (DTint &n);
		Stream&                             operator>>              (DTubyte &n);
		Stream&                             operator>>              (DTushort &n);
		Stream&                             operator>>              (DTuint &n);
		Stream&                             operator>>              (DTint64 &n);
		Stream&                             operator>>              (DTuint64 &n);
#if DT3_SIZES_ARE_DISTINCT_TYPES
		Stream&                             operator>>              (DTsize &n);
#endif
		Stream&                             operator>>              (DTfloat &n);
		Stream&                             operator>>              (DTdouble &n);
		Stream&                             operator>>              (DTcharacter &n);
		Stream&                             operator>>              (std::string &n);
		
		
		/// Return write position
		/// \return write position
		DTsize                              p                       (void);

		/// Return read position
		/// \return read position
		DTsize                              g                       (void);
		
		/// Change write position
		/// \param p position
		/// \param r relative
		void                                seek_p                  (DToffset p, Relative r);

		/// Change read position
		/// \param p position
		/// \param r relative
		void                                seek_g                  (DToffset g, Relative r);


		/// Peeks ahead at the next byte
		/// \return next byte
		DTcharacter                         peek                    (void);

		/// Ignores the next byte
		void                                ignore                  (void);

		
		/// Checks for end of file
		/// \return End of file
		DTboolean                           is_eof                  (void);

		/// Clears end of file flag
		void                                clear_EOF               (void)  {   _eof = false;   }

		/// Reads a line into a buffer
		/// \param buffer raw buffer
		/// \param size raw buffer length
		void                                line                    (DTcharacter *buffer, DTuint size);

		/// Associate the file handle with the stream. This opens a fragment of a file.
		/// \param file file handle
		/// \param start start of the fragment
		/// \param length length of the fragment
		/// \param progress progress
		void                                open_file_fragment      (const std::shared_ptr<FileHandle> &file, DTsize start, DTsize length, std::shared_ptr<Progress> progress = NULL);

		/// Close the stream
		void                                close                   (void);

		/// Length of the stream
		/// \return Length of the stream
		DTsize                              length                  (void) const			{	return _length;		}

		/// Returns the start of the stream if opening a fragment
		/// \return Start position
		DTsize                              start                   (void) const			{	return _start;		}

		/// Returns the file handle associated with the stream
		/// \return file handle
		const std::shared_ptr<FileHandle>&  file_handle             (void) const;
		
		/// Sets the file handle associated with the stream
		/// \param file_handle File handle
		void                                set_file_handle         (const std::shared_ptr<FileHandle> &file_handle);
	
		/// Sets the ignore whitespace flag. This ignores spaces when reading strings
		/// \param ignore_whitepace ignore whitepace flag
        void                                set_ignore_whitespace   (DTboolean ignore_whitepace)    {   _ignore_whitespace = ignore_whitepace;  }

	private:
        void                                update_progress         (void);
    
		std::shared_ptr<FileHandle>         _file;
        DTboolean                           _ignore_whitespace;
    
		DTsize                              _start;
		DTsize                              _length;
		DTsize                              _last_update;
    
		std::shared_ptr<Progress>           _progress;

        DTboolean                           _eof;
		
        static const DTsize TEMP_BUFFER_SIZE = 1024;
		
		void                                eat_whitespace			(void);
		void                                read_float_string       (DTcharacter buffer[TEMP_BUFFER_SIZE]);
};

//==============================================================================
//==============================================================================


} // DT3

#endif
