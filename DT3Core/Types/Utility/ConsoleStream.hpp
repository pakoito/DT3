#ifndef DT3_CONSOLESTREAM
#define DT3_CONSOLESTREAM
//==============================================================================
///	
///	File: ConsoleStream.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/FileBuffer/TextBufferStream.hpp"
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class ConsoleStream {
	public:
	
                        ConsoleStream   (DTuint type);
                        ConsoleStream   (ConsoleStream &rhs);
		
	private:
		ConsoleStream&	operator =      (const ConsoleStream &rhs);
	public:
                        ~ConsoleStream  (void);
	
	public:
		// Final destination of buffer
		enum {
			SS_ERROR,
			SS_MESSAGE,
			SS_DEBUG
		};

		template <class V>
		ConsoleStream& operator << (const V& v) 
		{
			_ss->_buffer << v;
			return *this;
		}

	private:
		struct SS {		
			TextBufferStream    _buffer;
			DTuint				_type;
		};	
		
		std::shared_ptr<SS>     _ss;
};

//==============================================================================
//==============================================================================

class ConsoleErrorStream {
	public:
		template <class V>
		ConsoleStream operator << (const V& v) {
			ConsoleStream ss(ConsoleStream::SS_ERROR);
			ss << v;
			return ss;
		}
};

class ConsoleMessageStream {
	public:
		template <class V>
		ConsoleStream operator << (const V& v) {
			ConsoleStream ss(ConsoleStream::SS_MESSAGE);
			ss << v;
			return ss;
		}
};

class ConsoleDebugStream {
	public:
		template <class V>
		ConsoleStream operator << (const V& v) {
			ConsoleStream ss(ConsoleStream::SS_DEBUG);
			ss << v;
			return ss;
		}
};

class ConsoleNullStream {
	public:
		template <class V>
		ConsoleNullStream& operator << (const V& v) {
			return *this;	// Does nothing
		}
};

//==============================================================================
//==============================================================================

extern ConsoleErrorStream ConsoleErrorVar;
extern ConsoleMessageStream ConsoleMessageVar;
extern ConsoleDebugStream ConsoleDebugVar;
extern ConsoleNullStream ConsoleNullVar;

#define LOG_NULL ConsoleNullVar

#if defined(DT3_DEBUG) || defined(DT3_EDITOR)
	#define LOG_ERROR ConsoleErrorVar
#else
	#define LOG_ERROR LOG_NULL
#endif

#if defined(DT3_DEBUG) || defined(DT3_EDITOR)
	#define LOG_MESSAGE ConsoleMessageVar
#else
	#define LOG_MESSAGE LOG_NULL
#endif

#if defined(DT3_DEBUG) || defined(DT3_EDITOR)
	#define LOG_DEBUG ConsoleDebugVar
#else
	#define LOG_DEBUG LOG_NULL
#endif

//==============================================================================
//==============================================================================

} // DT3

#endif
