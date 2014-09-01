//==============================================================================
///	
///	File: TextFileStream.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/TextFileStream.hpp"
#include "DT3Core/Types/FileBuffer/FileHandle.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"
#include "DT3Core/Types/Utility/Progress.hpp"
#include <sstream>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

TextFileStream::TextFileStream (void)
	:	_start			(0),
		_length			(0),
		_last_update	(0),
        _eof            (false),
        _ignore_whitespace  (false)
{

}
			
TextFileStream::~TextFileStream (void)
{

}

//==============================================================================
//==============================================================================

void TextFileStream::initialize (void)
{

}

//==============================================================================
//==============================================================================

const std::shared_ptr<FileHandle>& TextFileStream::file_handle (void) const
{
	return _file;
}

void TextFileStream::set_file_handle (const std::shared_ptr<FileHandle> &file_handle)
{
	_file = file_handle;
}

//==============================================================================
//==============================================================================

void TextFileStream::eat_whitespace	(void)
{
	if (_eof)
		return;
		
	DTcharacter c = peek();

#if DT3_COMPILER == DT3_VISUALC || DT3_OS == DT3_ANDROID
	while (!isgraph(c) && _file->g() < (_start + _length)) {
#else
    while (!std::isgraph(c) && _file->g() < (_start + _length)) {
#endif
		ignore();
		c = peek();
	}
}

void TextFileStream::read_float_string (DTcharacter buffer[TEMP_BUFFER_SIZE])
{
	eat_whitespace();

    DTsize file_g = _file->g();
	_eof = (file_g == -1) || (file_g >= (_start + _length));
	if (_eof)
		return;
		
	DTcharacter c = peek();
	DTushort i = 0;
	
	while (	i < TEMP_BUFFER_SIZE && 
			(c == '+' || c == '-' || (c >= '0' && c <= '9') || c == '.' || c == 'e' || c == 'E')  && 
			_file->g() < (_start + _length)) {
		_file->read((DTubyte*) &buffer[i++], 1);
		c = peek();
	}
	buffer[i++] = '\0';
    
    update_progress();
}

//==============================================================================
//==============================================================================

Stream& TextFileStream::operator<< (const DTboolean n)
{
	std::stringstream ss;
	ss << n;
	_file->write((const DTubyte*) ss.str().c_str(), ss.str().size());
	return *this;
}

Stream& TextFileStream::operator<< (const DTbyte n)
{
	std::stringstream ss;
	ss << n;
	_file->write((const DTubyte*) ss.str().c_str(), ss.str().size());
	return *this;
}

Stream& TextFileStream::operator<< (const DTshort n)
{
	std::stringstream ss;
	ss << n;
	_file->write((const DTubyte*) ss.str().c_str(), ss.str().size());
	return *this;
}

Stream& TextFileStream::operator<< (const DTint n)
{
	std::stringstream ss;
	ss << n;
	_file->write((const DTubyte*) ss.str().c_str(), ss.str().size());
	return *this;
}

Stream& TextFileStream::operator<< (const DTubyte n)
{
	std::stringstream ss;
	ss << n;
	_file->write((const DTubyte*) ss.str().c_str(), ss.str().size());
	return *this;
}

Stream& TextFileStream::operator<< (const DTushort n)
{
	std::stringstream ss;
	ss << n;
	_file->write((const DTubyte*) ss.str().c_str(), ss.str().size());
	return *this;
}

Stream& TextFileStream::operator<< (const DTuint n)
{
	std::stringstream ss;
	ss << n;
	_file->write((const DTubyte*) ss.str().c_str(), ss.str().size());
	return *this;
}

Stream& TextFileStream::operator<< (const DTint64 n)
{
	std::stringstream ss;
	ss << n;
	_file->write((const DTubyte*) ss.str().c_str(), ss.str().size());
	return *this;
}

Stream& TextFileStream::operator<< (const DTuint64 n)
{
	std::stringstream ss;
	ss << n;
	_file->write((const DTubyte*) ss.str().c_str(), ss.str().size());
	return *this;
}

#if DT3_SIZES_ARE_DISTINCT_TYPES
Stream& TextFileStream::operator<< (const DTsize n)
{
	std::stringstream ss;
	ss << n;
	_file->write((const DTubyte*) ss.str().c_str(), ss.str().size());
	return *this;
}
#endif

Stream& TextFileStream::operator<< (const DTfloat n)
{
	std::stringstream ss;
    ss.precision(7);
    ss.setf(std::ios::showpoint);
	ss << n;
	_file->write((const DTubyte*) ss.str().c_str(), ss.str().size());
	return *this;
}

Stream& TextFileStream::operator<< (const DTdouble n)
{
	std::stringstream ss;
    ss.precision(17);
    ss.setf(std::ios::showpoint);
	ss << n;
	_file->write((const DTubyte*) ss.str().c_str(), ss.str().size());
	return *this;
}

Stream& TextFileStream::operator<< (const DTcharacter n)
{
	std::stringstream ss;
	ss << n;
	_file->write((const DTubyte*) ss.str().c_str(), ss.str().size());
	return *this;
}

Stream& TextFileStream::operator<< (const char* n)
{
	std::stringstream ss;
	ss << n;
	_file->write((const DTubyte*) ss.str().c_str(), ss.str().size());
	return *this;
}

Stream& TextFileStream::operator<< (const std::string &n)
{
	std::stringstream ss;
	ss << n.c_str();
	_file->write((const DTubyte*) ss.str().c_str(), ss.str().size());
	return *this;
}

//==============================================================================
//==============================================================================

Stream& TextFileStream::operator>> (DTboolean &n)
{
	DTcharacter buffer[TEMP_BUFFER_SIZE];
	read_float_string (buffer);
	n = MoreStrings::cast_from_string<DTboolean>(buffer);
	return *this;
}

Stream& TextFileStream::operator>> (DTbyte &n)
{
	DTcharacter buffer[TEMP_BUFFER_SIZE];
	read_float_string (buffer);
	n = MoreStrings::cast_from_string<DTbyte>(buffer);
	return *this;
}

Stream& TextFileStream::operator>> (DTshort &n)
{
	DTcharacter buffer[TEMP_BUFFER_SIZE];
	read_float_string (buffer);
	n = MoreStrings::cast_from_string<DTshort>(buffer);
	return *this;
}

Stream& TextFileStream::operator>> (DTint &n)
{
	DTcharacter buffer[TEMP_BUFFER_SIZE];
	read_float_string (buffer);
	n = MoreStrings::cast_from_string<DTint>(buffer);
	return *this;
}

Stream& TextFileStream::operator>> (DTubyte &n)
{
	DTcharacter buffer[TEMP_BUFFER_SIZE];
	read_float_string (buffer);
	n = MoreStrings::cast_from_string<DTubyte>(buffer);
	return *this;
}

Stream& TextFileStream::operator>> (DTushort &n)
{
	DTcharacter buffer[TEMP_BUFFER_SIZE];
	read_float_string (buffer);
	n = MoreStrings::cast_from_string<DTushort>(buffer);
	return *this;
}

Stream& TextFileStream::operator>> (DTuint &n)
{
	DTcharacter buffer[TEMP_BUFFER_SIZE];
	read_float_string (buffer);
	n = MoreStrings::cast_from_string<DTuint>(buffer);
	return *this;
}

Stream& TextFileStream::operator>> (DTint64 &n)
{
	DTcharacter buffer[TEMP_BUFFER_SIZE];
	read_float_string (buffer);
	n = MoreStrings::cast_from_string<DTint64>(buffer);
	return *this;
}

Stream& TextFileStream::operator>> (DTuint64 &n)
{
	DTcharacter buffer[TEMP_BUFFER_SIZE];
	read_float_string (buffer);
	n = MoreStrings::cast_from_string<DTuint64>(buffer);
	return *this;
}

#if DT3_SIZES_ARE_DISTINCT_TYPES
Stream& TextFileStream::operator>> (DTsize &n)
{
	DTcharacter buffer[TEMP_BUFFER_SIZE];
	read_float_string (buffer);
	n = MoreStrings::cast_from_string<DTsize>(buffer);
	return *this;
}
#endif

Stream& TextFileStream::operator>> (DTfloat &n)
{
	DTcharacter buffer[TEMP_BUFFER_SIZE];
	read_float_string (buffer);
	n = MoreStrings::cast_from_string<DTfloat>(buffer);
	return *this;
}

Stream& TextFileStream::operator>> (DTdouble &n)
{
	DTcharacter buffer[TEMP_BUFFER_SIZE];
	read_float_string (buffer);
	n = MoreStrings::cast_from_string<DTdouble>(buffer);
	return *this;
}

Stream& TextFileStream::operator>> (DTcharacter &n)
{
    DTsize file_g = _file->g();
	_eof = (file_g == -1) || (file_g >= (_start + _length));
	_file->read((DTubyte*) &n, 1);
    
    update_progress();

	return *this;
}

Stream& TextFileStream::operator>> (std::string &n)
{
    if (_ignore_whitespace) {
        eat_whitespace();
         
        DTsize file_g = _file->g();
        _eof = (file_g == -1) || (file_g >= (_start + _length));
        if (_eof)
            return *this;
            
        DTcharacter buffer[TEMP_BUFFER_SIZE];
        DTcharacter c = peek();
        DTushort i = 0;
        
        while (i < TEMP_BUFFER_SIZE-1 && c != '\n') {
            _file->read((DTubyte*) &buffer[i++], 1);
            c = peek();
        }
        buffer[i++] = '\0';

        n = buffer;
        
        update_progress();

        return *this;
        
    } else {

        eat_whitespace();

        DTsize file_g = _file->g();
        _eof = (file_g == -1) || (file_g >= (_start + _length));
        if (_eof)
            return *this;
            
        DTcharacter buffer[TEMP_BUFFER_SIZE];
        DTcharacter c = peek();
        DTushort i = 0;
        
#if DT3_COMPILER == DT3_VISUALC || DT3_OS == DT3_ANDROID
        while (i < TEMP_BUFFER_SIZE-1 && !isspace(c) ) {
#else
        while (i < TEMP_BUFFER_SIZE-1 && !std::isspace(c) ) {
#endif
            _file->read((DTubyte*) &buffer[i++], 1);
            c = peek();
        }
        buffer[i++] = '\0';

        n = buffer;
        
        update_progress();

        return *this;
    }
}

//==============================================================================
//==============================================================================

DTsize TextFileStream::p (void)
{
	return _file->p() - _start;
}

DTsize TextFileStream::g (void)
{
	return _file->g() - _start;
}

void TextFileStream::seek_p (DToffset p, Relative r)
{	
	switch (r) {
		case FROM_BEGINNING:	_file->seek_p(_start+p, FileHandle::FROM_BEGINNING);			break;
		case FROM_CURRENT:		_file->seek_p(p, FileHandle::FROM_CURRENT);						break;
		case FROM_END:			_file->seek_p(_start+_length-p, FileHandle::FROM_BEGINNING);	break;
	};
}

void TextFileStream::seek_g (DToffset g, Relative r)
{	
	switch (r) {
		case FROM_BEGINNING:	_file->seek_g(_start+g, FileHandle::FROM_BEGINNING);			break;
		case FROM_CURRENT:		_file->seek_g(g, FileHandle::FROM_CURRENT);						break;
		case FROM_END:			_file->seek_g(_start+_length-g, FileHandle::FROM_BEGINNING);	break;
	};
}

DTcharacter TextFileStream::peek (void)
{
	DTcharacter c = _file->peek();
	return c;
}

void TextFileStream::ignore (void)
{
    DTsize file_g = _file->g();
	_eof = (file_g == -1) || (file_g >= (_start + _length));
	_file->ignore();
}

DTboolean TextFileStream::is_eof (void)
{
	return _eof;
}

void TextFileStream::line (DTcharacter *buffer, DTuint size)
{
    DTsize file_g = _file->g();
	_eof = (file_g == -1) || (file_g >= (_start + _length));
	if (_eof)
		return;

	DTcharacter c = peek();
	DTuint i = 0;
	
	while (i < (size-1) && _file->g() < (_start + _length) && (c != '\n' && c != '\r' && c != -1) ) {
	
		// read the character
		_file->read((DTubyte*) &buffer[i++], 1);

		c = peek();
	}
	
	// ignore following newline characters
	while (_file->g() < (_start + _length) && (c == '\n' || c == '\r') && (c != -1) ) {
		ignore();
		c = peek();
	}
	
	buffer[i] = '\0';
	
	ASSERT(!_eof);
    
    update_progress();
}

//==============================================================================
//==============================================================================

void TextFileStream::open_file_fragment	(const std::shared_ptr<FileHandle> &file, DTsize start, DTsize length, std::shared_ptr<Progress> progress)
{	
	set_file_handle(file);
	
	_start = start;
	_length = length;
    
    _file->seek_g(_start, FileHandle::FROM_BEGINNING);
	
    _progress = progress;
}

//==============================================================================
//==============================================================================

void TextFileStream::close (void)
{
	_file.reset();
}

//==============================================================================
//==============================================================================

void TextFileStream::update_progress (void)
{
	// Update Progress
	if (_progress) {
		_progress->update(_file->g(), _length);
		_last_update = 0;
	}
}

//==============================================================================
//==============================================================================

} // DT3
