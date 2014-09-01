#ifndef DT3_TEXTBUFFERSTREAM
#define DT3_TEXTBUFFERSTREAM
//==============================================================================
///	
///	File: TextBufferStream.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/FileBuffer/TextStream.hpp"
#include <string>
#include <sstream>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Stream;

//==============================================================================
/// Class
//==============================================================================

class TextBufferStream: public TextStream {
    public:
        DEFINE_TYPE(TextBufferStream,TextStream)
		DEFINE_CREATE
         
                                TextBufferStream		(void);
        explicit                TextBufferStream		(const std::string &s)  {	set_buffer(s);	}
        explicit                TextBufferStream		(const DTcharacter *s)	{	set_buffer(s);	}
	private:
                                TextBufferStream		(const TextBufferStream &rhs);
        TextBufferStream &		operator =				(const TextBufferStream &rhs);
	public:
        virtual					~TextBufferStream		(void);

	public:

		Stream&				operator<<			(const Stream::FS)	{	*this << " ";	return *this;	}
		Stream&				operator<<			(const DTboolean n);
		Stream&				operator<<			(const DTbyte n);
		Stream&				operator<<			(const DTshort n);
		Stream&				operator<<			(const DTint n);
		Stream&				operator<<			(const DTubyte n);
		Stream&				operator<<			(const DTushort n);
		Stream&				operator<<			(const DTuint n);
		Stream&				operator<<			(const DTint64 n);
		Stream&				operator<<			(const DTuint64 n);
#if DT3_SIZES_ARE_DISTINCT_TYPES
		Stream&				operator<<			(const DTsize n);
#endif
		Stream&				operator<<			(const DTfloat n);
		Stream&				operator<<			(const DTdouble n);
		Stream&				operator<<			(const DTcharacter n);
		Stream&				operator<<			(const char* n);
		Stream&				operator<<			(const std::string &n);
		Stream&				operator<<			(const TextBufferStream& n);
    

		Stream&				operator>>			(DTboolean &n);
		Stream&				operator>>			(DTbyte &n);
		Stream&				operator>>			(DTshort &n);
		Stream&				operator>>			(DTint &n);
		Stream&				operator>>			(DTubyte &n);
		Stream&				operator>>			(DTushort &n);
		Stream&				operator>>			(DTuint &n);
		Stream&				operator>>			(DTint64 &n);
		Stream&				operator>>			(DTuint64 &n);
#if DT3_SIZES_ARE_DISTINCT_TYPES
		Stream&				operator>>			(DTsize &n);
#endif
		Stream&				operator>>			(DTfloat &n);
		Stream&				operator>>			(DTdouble &n);
		Stream&				operator>>			(DTcharacter &n);
		Stream&				operator>>			(std::string &n);
	

		/// Return write position
		/// \return write position
		DTsize                          p                   (void);

		/// Return read position
		/// \return read position
		DTsize                          g                   (void);
		
		/// Change write position
		/// \param p position
		/// \param r relative
		void                            seek_p              (DToffset p, Relative r);

		/// Change read position
		/// \param p position
		/// \param r relative
		void                            seek_g              (DToffset g, Relative r);

			
		/// Peeks ahead at the next byte
		/// \return next byte
        DTcharacter                     peek                (void);
        
		/// Ignores the next byte
        void                            ignore              (void);
        
		/// Reads a line into a buffer
		/// \param buffer raw buffer
		/// \param size raw buffer length
        void                            line                (DTcharacter *buffer, DTsize size);
            
		/// Returns the buffer
		/// \return buffer
		std::string						buffer              (void)                              {	return std::string(_buffer.str());		}

		/// Sets the buffer contents
		/// \param buffer new buffer contents
		void							set_buffer			(std::string buffer);
				
		/// Sets the ignore whitespace flag. This ignores spaces when reading strings
		/// \param ignore_whitepace ignore whitepace flag
        void                            set_ignore_whitespace   (DTboolean ignore_whitepace)    {   _ignore_whitespace = ignore_whitepace;  }

	private:
		std::stringstream				_buffer;
        DTboolean                       _ignore_whitespace;
		
};

//==============================================================================
//==============================================================================


} // DT3

#endif
