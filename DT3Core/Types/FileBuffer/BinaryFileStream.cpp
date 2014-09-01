//==============================================================================
///	
///	File: BinaryFileStream.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/BinaryFileStream.hpp"
#include "DT3Core/Types/FileBuffer/FileHandle.hpp"
#include "DT3Core/Types/Utility/Endian.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Utility/Progress.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

BinaryFileStream::BinaryFileStream (void)
	:	_start			(0),
		_length			(0),
		_last_update	(0),
        _eof            (false)
{

}
			
BinaryFileStream::~BinaryFileStream (void)
{

}

//==============================================================================
//==============================================================================

void BinaryFileStream::initialize (void)
{

}

//==============================================================================
//==============================================================================

const std::shared_ptr<FileHandle>& BinaryFileStream::file_handle (void) const
{
	return _file;
}

void BinaryFileStream::set_file_handle (const std::shared_ptr<FileHandle> &file_handle)
{
	_file = file_handle;
}

//==============================================================================
//==============================================================================

Stream& BinaryFileStream::operator<< (const DTboolean nc)
{
	DTubyte n = nc;
	Endian::to_little_endian(n);
	write_raw((DTubyte*) &n, sizeof(n));
	return *this;
}

Stream& BinaryFileStream::operator<< (const DTbyte nc)
{
	DTbyte n = nc;
	Endian::to_little_endian(n);
	write_raw((DTubyte*) &n, sizeof(n));
	return *this;
}

Stream& BinaryFileStream::operator<< (const DTshort nc)
{
	DTshort n = nc;
	Endian::to_little_endian(n);
	write_raw((DTubyte*) &n, sizeof(n));
	return *this;
}

Stream& BinaryFileStream::operator<< (const DTint nc)
{
	DTint n = nc;
	Endian::to_little_endian(n);
	write_raw((DTubyte*) &n, sizeof(n));
	return *this;
}

Stream& BinaryFileStream::operator<< (const DTubyte nc)
{
	DTubyte n = nc;
	Endian::to_little_endian(n);
	write_raw((DTubyte*) &n, sizeof(n));
	return *this;
}

Stream& BinaryFileStream::operator<< (const DTushort nc)
{
	DTushort n = nc;
	Endian::to_little_endian(n);
	write_raw((DTubyte*) &n, sizeof(n));
	return *this;
}

Stream& BinaryFileStream::operator<< (const DTuint nc)
{
	DTuint n = nc;
	Endian::to_little_endian(n);
	write_raw((DTubyte*) &n, sizeof(n));
	return *this;
}

Stream& BinaryFileStream::operator<< (const DTint64 nc)
{
	DTint64 n = nc;
	Endian::to_little_endian(n);
	write_raw((DTubyte*) &n, sizeof(n));
	return *this;
}

Stream& BinaryFileStream::operator<< (const DTuint64 nc)
{
	DTuint64 n = nc;
	Endian::to_little_endian(n);
	write_raw((DTubyte*) &n, sizeof(n));
	return *this;
}

#if DT3_SIZES_ARE_DISTINCT_TYPES
Stream& BinaryFileStream::operator<< (const DTsize nc)
{
	DTsize n = nc;
	Endian::to_little_endian(n);
	write_raw((DTubyte*) &n, sizeof(n));
	return *this;
}
#endif

Stream& BinaryFileStream::operator<< (const DTfloat nc)
{
	DTfloat n = nc;
	Endian::to_little_endian(n);
	write_raw((DTubyte*) &n, sizeof(n));
	return *this;
}

Stream& BinaryFileStream::operator<< (const DTdouble nc)
{
	DTdouble n = nc;
	Endian::to_little_endian(n);
	write_raw((DTubyte*) &n, sizeof(n));
	return *this;
}

Stream& BinaryFileStream::operator<< (const DTcharacter n)
{
	return operator << ((DTubyte) n);
}

Stream& BinaryFileStream::operator<< (const char* n)
{
	std::string n_str(n);

	return operator << (n_str);
}

Stream& BinaryFileStream::operator<< (const std::string &n)
{
	for (std::size_t i = 0; i < n.size(); ++i)
		operator << ((DTcharacter) n[i]);

	operator << ('\0');

	return *this;
}

//==============================================================================
//==============================================================================

Stream& BinaryFileStream::operator>> (DTboolean &n)
{
	DTubyte nc;
	read_raw((DTubyte*) &nc, sizeof(nc));
	Endian::from_little_endian(nc);
	n = nc;
	return *this;
}

Stream& BinaryFileStream::operator>> (DTbyte &n)
{
	read_raw((DTubyte*) &n, sizeof(n));
	Endian::from_little_endian(n);
	return *this;
}

Stream& BinaryFileStream::operator>> (DTshort &n)
{
	read_raw((DTubyte*) &n, sizeof(n));
	Endian::from_little_endian(n);
	return *this;
}

Stream& BinaryFileStream::operator>> (DTint &n)
{
	read_raw((DTubyte*) &n, sizeof(n));
	Endian::from_little_endian(n);
	return *this;
}

Stream& BinaryFileStream::operator>> (DTubyte &n)
{
	read_raw((DTubyte*) &n, sizeof(n));
	Endian::from_little_endian(n);
	return *this;
}

Stream& BinaryFileStream::operator>> (DTushort &n)
{
	read_raw((DTubyte*) &n, sizeof(n));
	Endian::from_little_endian(n);
	return *this;
}

Stream& BinaryFileStream::operator>> (DTuint &n)
{
	read_raw((DTubyte*) &n, sizeof(n));
	Endian::from_little_endian(n);
	return *this;
}

Stream& BinaryFileStream::operator>> (DTint64 &n)
{
	read_raw((DTubyte*) &n, sizeof(n));
	Endian::from_little_endian(n);
	return *this;
}

Stream& BinaryFileStream::operator>> (DTuint64 &n)
{
	read_raw((DTubyte*) &n, sizeof(n));
	Endian::from_little_endian(n);
	return *this;
}

#if DT3_SIZES_ARE_DISTINCT_TYPES
Stream& BinaryFileStream::operator>> (DTsize &n)
{
	read_raw((DTubyte*) &n, sizeof(n));
	Endian::from_little_endian(n);
	return *this;
}
#endif

Stream& BinaryFileStream::operator>> (DTfloat &n)
{
	read_raw((DTubyte*) &n, sizeof(n));
	Endian::from_little_endian(n);
	return *this;
}

Stream& BinaryFileStream::operator>> (DTdouble &n)
{
	read_raw((DTubyte*) &n, sizeof(n));
	Endian::from_little_endian(n);
	return *this;
}

Stream& BinaryFileStream::operator>> (DTcharacter &n)
{
	read_raw((DTubyte*) &n, sizeof(n));
	Endian::from_little_endian(n);
	return *this;
}

Stream& BinaryFileStream::operator>> (std::string &n)
{
	n.clear();
	
	DTcharacter c;
	read_raw((DTubyte*) &c, sizeof(c));

	while (c != '\0') {
		n += c;
		read_raw((DTubyte*) &c, sizeof(c));
	}

	return *this;
}

//==============================================================================
//==============================================================================

DTsize BinaryFileStream::read_raw (DTubyte *data, DTsize size)
{
    DTsize file_g = _file->g();
	_eof = (file_g == -1) || (file_g >= (_start + _length));
	
	DTsize count = _file->read(data, size);	
	
	// Update Progress
	if (_progress) {
		_progress->update(_file->g(), _length);
		_last_update = 0;
	}

	//return min2( (_start + _length) - (_file->getG() - count), count); // WHY WAS THIS HERE BEFORE?? It is wrong though.
    return count;
}

void BinaryFileStream::write_raw (DTubyte *data, DTsize size)
{
	_file->write(data, size);
}

//==============================================================================
//==============================================================================

DTsize BinaryFileStream::p (void)
{
	return _file->p() - _start;
}

DTsize BinaryFileStream::g (void)
{
	return _file->g() - _start;
}

void BinaryFileStream::seek_p (DToffset p, Relative r)
{	
	switch (r) {
		case FROM_BEGINNING:	_file->seek_p(_start+p, FileHandle::FROM_BEGINNING);			break;
		case FROM_CURRENT:		_file->seek_p(p, FileHandle::FROM_CURRENT);                     break;
		case FROM_END:			_file->seek_p(_start+_length-p, FileHandle::FROM_BEGINNING);	break;
	};
}

void BinaryFileStream::seek_g (DToffset g, Relative r)
{	
	switch (r) {
		case FROM_BEGINNING:	_file->seek_g(_start+g, FileHandle::FROM_BEGINNING);			break;
		case FROM_CURRENT:		_file->seek_g(g, FileHandle::FROM_CURRENT);                     break;
		case FROM_END:			_file->seek_g(_start+_length-g, FileHandle::FROM_BEGINNING);	break;
	};
}

DTboolean BinaryFileStream::is_eof (void)
{
	return _eof;
}

//==============================================================================
//==============================================================================

void BinaryFileStream::open_file_fragment (const std::shared_ptr<FileHandle> &file, DTsize start, DTsize length, std::shared_ptr<Progress> progress)
{	
	set_file_handle(file);
		
	_start = start;
	_length = length;
	
    _file->seek_g(_start, FileHandle::FROM_BEGINNING);

	_progress = progress;
}

//==============================================================================
//==============================================================================

void BinaryFileStream::close (void)
{
    _file.reset();
}
		
//==============================================================================
//==============================================================================

} // DT3
