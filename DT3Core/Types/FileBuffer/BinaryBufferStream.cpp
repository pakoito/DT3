//==============================================================================
///	
///	File: BinaryBufferStream.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/FileBuffer/BinaryBufferStream.hpp"
#include "DT3Core/Types/Utility/Endian.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

BinaryBufferStream::BinaryBufferStream (void)
	:	_logical_size	(0),
		_pos_p			(0),
		_pos_g			(0)
{

}
	
BinaryBufferStream::~BinaryBufferStream (void)
{

}

//==============================================================================
//==============================================================================

Stream& BinaryBufferStream::operator<< (const DTboolean n)
{
	return operator << ((DTbyte) n);
}

Stream& BinaryBufferStream::operator<< (const DTbyte nc)
{
	DTbyte n = nc;
	grow (sizeof(DTbyte));
	Endian::to_little_endian(n);
	*(reinterpret_cast<DTbyte*>(&_stream[_pos_p])) = (DTbyte) n;
	_pos_p += sizeof(DTbyte);
	
	return *this;
}

Stream& BinaryBufferStream::operator<< (const DTshort nc)
{
	DTshort n = nc;
	grow (sizeof(DTshort));
	Endian::to_little_endian(n);
	*(reinterpret_cast<DTshort*>(&_stream[_pos_p])) = (DTshort) n;
	_pos_p += sizeof(DTshort);
	
	return *this;
}

Stream& BinaryBufferStream::operator<< (const DTint nc)
{
	DTint n = nc;
	grow (sizeof(DTint));
	Endian::to_little_endian(n);
	*(reinterpret_cast<DTint*>(&_stream[_pos_p])) = (DTint) n;
	_pos_p += sizeof(DTint);

	return *this;
}

Stream& BinaryBufferStream::operator<< (const DTubyte nc)
{
	DTubyte n = nc;
	grow (sizeof(DTubyte));
	Endian::to_little_endian(n);
	*(reinterpret_cast<DTubyte*>(&_stream[_pos_p])) = (DTubyte) n;
	_pos_p += sizeof(DTubyte);

	return *this;
}

Stream& BinaryBufferStream::operator<< (const DTushort nc)
{
	DTushort n = nc;
	grow (sizeof(DTushort));
	Endian::to_little_endian(n);
	*(reinterpret_cast<DTushort*>(&_stream[_pos_p])) = (DTushort) n;
	_pos_p += sizeof(DTushort);

	return *this;
}

Stream& BinaryBufferStream::operator<< (const DTuint nc)
{
	DTuint n = nc;
	grow (sizeof(DTuint));
	Endian::to_little_endian(n);
	*(reinterpret_cast<DTuint*>(&_stream[_pos_p])) = (DTuint) n;
	_pos_p += sizeof(DTuint);

	return *this;
}

Stream& BinaryBufferStream::operator<< (const DTint64 nc)
{
	DTint64 n = nc;
	grow (sizeof(DTuint64));
	Endian::to_little_endian(n);
	*(reinterpret_cast<DTuint64*>(&_stream[_pos_p])) = (DTuint64) n;
	_pos_p += sizeof(DTuint64);

	return *this;
}

Stream& BinaryBufferStream::operator<< (const DTuint64 nc)
{
	DTuint64 n = nc;
	grow (sizeof(DTuint64));
	Endian::to_little_endian(n);
	*(reinterpret_cast<DTuint64*>(&_stream[_pos_p])) = (DTuint64) n;
	_pos_p += sizeof(DTuint64);

	return *this;
}

#if DT3_SIZES_ARE_DISTINCT_TYPES
Stream& BinaryBufferStream::operator<< (const DTsize nc)
{
	DTsize n = nc;
	grow (sizeof(DTsize));
	Endian::to_little_endian(n);
	*(reinterpret_cast<DTsize*>(&_stream[_pos_p])) = (DTsize) n;
	_pos_p += sizeof(DTsize);

	return *this;
}
#endif

Stream& BinaryBufferStream::operator<< (const DTfloat nc)
{
	DTfloat n = nc;
	grow (sizeof(DTfloat));
	Endian::to_little_endian(n);
    
    DTfloat *mem = reinterpret_cast<DTfloat*>(&_stream[_pos_p]);
    ::memcpy(mem, &n, sizeof(DTfloat));
    
	_pos_p += sizeof(DTfloat);

	return *this;
}

Stream& BinaryBufferStream::operator<< (const DTdouble nc)
{
	DTdouble n = nc;
	grow (sizeof(DTdouble));
	Endian::to_little_endian(n);
    
	DTdouble *mem = reinterpret_cast<DTdouble*>(&_stream[_pos_p]);
    ::memcpy(mem, &n, sizeof(DTdouble));
    
	_pos_p += sizeof(DTdouble);

	return *this;
}

Stream& BinaryBufferStream::operator<< (const DTcharacter n)
{
	return operator << ((DTubyte) n);
}

Stream& BinaryBufferStream::operator<< (const char* n)
{
	std::string n_str(n);

	return operator << (n_str);
}

Stream& BinaryBufferStream::operator<< (const std::string &n)
{
	for (DTuint i = 0; i < n.size(); ++i)
		operator << ((DTcharacter) n[i]);

	operator << ('\0');

	return *this;
}

Stream& BinaryBufferStream::operator<< (const BinaryBufferStream& n)
{
	for (DTuint i = 0; i < n._logical_size; ++i)
		operator << (n._stream[i]);
		
	return *this;
}

//==============================================================================
//==============================================================================

Stream& BinaryBufferStream::operator>> (DTboolean &n)
{
	n = *(reinterpret_cast<DTbyte*>(&_stream[_pos_g])) != (DTbyte)0;
	Endian::from_little_endian(n);
	_pos_g += sizeof(DTbyte);

	return *this;
}

Stream& BinaryBufferStream::operator>> (DTbyte &n)
{
	n = *(reinterpret_cast<DTbyte*>(&_stream[_pos_g]));
	Endian::from_little_endian(n);
	_pos_g += sizeof(DTbyte);

	return *this;
}

Stream& BinaryBufferStream::operator>> (DTshort &n)
{
	n = *(reinterpret_cast<DTshort*>(&_stream[_pos_g]));
	Endian::from_little_endian(n);
	_pos_g += sizeof(DTshort);

	return *this;
}

Stream& BinaryBufferStream::operator>> (DTint &n)
{
	n = *(reinterpret_cast<DTint*>(&_stream[_pos_g]));
	Endian::from_little_endian(n);
	_pos_g += sizeof(DTint);

	return *this;
}

Stream& BinaryBufferStream::operator>> (DTubyte &n)
{
	n = *(reinterpret_cast<DTubyte*>(&_stream[_pos_g]));
	Endian::from_little_endian(n);
	_pos_g += sizeof(DTubyte);

	return *this;
}

Stream& BinaryBufferStream::operator>> (DTushort &n)
{
	n = *(reinterpret_cast<DTushort*>(&_stream[_pos_g]));
	Endian::from_little_endian(n);
	_pos_g += sizeof(DTushort);

	return *this;
}

Stream& BinaryBufferStream::operator>> (DTuint &n)
{
	n = *(reinterpret_cast<DTuint*>(&_stream[_pos_g]));
	Endian::from_little_endian(n);
	_pos_g += sizeof(DTuint);

	return *this;
}

Stream& BinaryBufferStream::operator>> (DTint64 &n)
{
	n = *(reinterpret_cast<DTuint64*>(&_stream[_pos_g]));
	Endian::from_little_endian(n);
	_pos_g += sizeof(DTuint64);

	return *this;
}

Stream& BinaryBufferStream::operator>> (DTuint64 &n)
{
	n = *(reinterpret_cast<DTuint64*>(&_stream[_pos_g]));
	Endian::from_little_endian(n);
	_pos_g += sizeof(DTuint64);

	return *this;
}

#if DT3_SIZES_ARE_DISTINCT_TYPES
Stream& BinaryBufferStream::operator>> (DTsize &n)
{
	n = *(reinterpret_cast<DTsize*>(&_stream[_pos_g]));
	Endian::from_little_endian(n);
	_pos_g += sizeof(DTsize);

	return *this;
}
#endif

Stream& BinaryBufferStream::operator>> (DTfloat &n)
{
    DTfloat *mem = reinterpret_cast<DTfloat*>(&_stream[_pos_g]);
    ::memcpy(&n, mem, sizeof(DTfloat));

	Endian::from_little_endian(n);
	_pos_g += sizeof(DTfloat);

	return *this;
}

Stream& BinaryBufferStream::operator>> (DTdouble &n)
{
    DTdouble *mem = reinterpret_cast<DTdouble*>(&_stream[_pos_g]);
    ::memcpy(&n, mem, sizeof(DTdouble));

	Endian::from_little_endian(n);
	_pos_g += sizeof(DTdouble);

	return *this;
}

Stream& BinaryBufferStream::operator>> (DTcharacter &n)
{
	n = *(reinterpret_cast<DTcharacter*>(&_stream[_pos_g]));
	Endian::from_little_endian(n);
	_pos_g += sizeof(DTcharacter);

	return *this;
}

Stream& BinaryBufferStream::operator>> (std::string &n)
{
	n.clear();
	
	DTcharacter c = *(reinterpret_cast<DTcharacter*>(&_stream[_pos_g]));
	_pos_g += sizeof(DTcharacter);
	
	while (c != '\0') {
		n += c;
		
		c = *(reinterpret_cast<DTcharacter*>(&_stream[_pos_g]));
		_pos_g += sizeof(DTcharacter);
	}

	return *this;
}

//==============================================================================
//==============================================================================

void BinaryBufferStream::resize (const DTsize size)
{
	const DTuint BUFFER_SIZE = 2048;
	
	std::size_t allocate_size = ((size/BUFFER_SIZE) + 1) * BUFFER_SIZE;
	if (allocate_size > _stream.size()) {
		_stream.resize(allocate_size);
	}
	_logical_size = size;
}

void BinaryBufferStream::grow (DTsize size)
{
	if (_logical_size < (DTuint) (_pos_p + size)) {
		resize(_pos_p+size);
	}
}

//==============================================================================
//==============================================================================

DTsize BinaryBufferStream::p (void)
{
	return _pos_p;
}

DTsize BinaryBufferStream::g (void)
{
	return _pos_g;
}

void BinaryBufferStream::seek_p (DToffset p, BinaryBufferStream::Relative r)
{
	switch (r) {
		case FROM_BEGINNING:
			_pos_p = p;
			break;
		case FROM_END:
			_pos_p = (DTsize) _logical_size - p;
			break;
		case FROM_CURRENT:
			_pos_p += p;
			break;
	};
}

void BinaryBufferStream::seek_g (DToffset g, BinaryBufferStream::Relative r)
{
	switch (r) {
		case FROM_BEGINNING:
			_pos_g = g;
			break;
		case FROM_END:
			_pos_g = (DTsize) _logical_size - g;
			break;
		case FROM_CURRENT:
			_pos_g += g;
			break;
	};
}
		
//==============================================================================
//==============================================================================
		
void BinaryBufferStream::set_buffer (void *b, DTsize size)
{
	// Build and copy data into buffer
	resize(size);
	memcpy(&_stream[0], b, (size_t) size);
}

//==============================================================================
//==============================================================================
		

} // DT3
