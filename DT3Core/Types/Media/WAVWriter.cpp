//==============================================================================
///	
///	File: WAVWriter.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Media/WAVWriter.hpp"
#include "DT3Core/Types/Utility/Endian.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

WAVWriter::WAVWriter (void)
{

}

WAVWriter::WAVWriter (const WAVWriter &rhs)
{

}
            
WAVWriter& WAVWriter::operator = (const WAVWriter &rhs)
{	
    return (*this);
}

WAVWriter::~WAVWriter (void)
{

}
//==============================================================================
//==============================================================================

void	WAVWriter::write(std::ofstream &outfile, DTubyte &v)
{
    outfile.write((const char*)&v, sizeof(DTbyte));
}

//==============================================================================
//==============================================================================

void	WAVWriter::write(std::ofstream &outfile, DTushort &v)
{
	Endian::to_little_endian(v);
    outfile.write((const char*)&v, sizeof(DTushort));
}

//==============================================================================
//==============================================================================

void	WAVWriter::write(std::ofstream &outfile, DTshort &v)
{
	Endian::to_little_endian(v);
    outfile.write((const char*)&v, sizeof(DTshort));
}

//==============================================================================
//==============================================================================

void	WAVWriter::write(std::ofstream &outfile, DTuint &v)
{    
	Endian::to_little_endian(v);
    outfile.write((const char*)&v, sizeof(DTuint));
}

//==============================================================================
//==============================================================================

void	WAVWriter::write(std::ofstream &outfile, DTint &v)
{    
	Endian::to_little_endian(v);
    outfile.write((const char*)&v, sizeof(DTint));
}

//==============================================================================
//==============================================================================

void	WAVWriter::write(std::ofstream &outfile, DTcharacter id[4])
{
    outfile.write((const char*)&id[0], sizeof(DTcharacter));
    outfile.write((const char*)&id[1], sizeof(DTcharacter));
    outfile.write((const char*)&id[2], sizeof(DTcharacter));
    outfile.write((const char*)&id[3], sizeof(DTcharacter));
}

//==============================================================================
//==============================================================================

void	WAVWriter::write(std::ofstream &outfile, ChunkHeader &v)
{
 	write(outfile, v.id);
	write(outfile, v.length);
}

//==============================================================================
//==============================================================================

void WAVWriter::set_type (DTcharacter id[4], const DTcharacter *id_str)
{
	id[0] = id_str[0];
    id[1] = id_str[1];
    id[2] = id_str[2];
    id[3] = id_str[3];
}

//==============================================================================
//==============================================================================

DTerr WAVWriter::begin_write (const FilePath &pathname, DTushort num_channels, DTint sample_rate, DTushort bits_per_sample)
{
    // open the file
    _outfile.open(pathname.full_path().c_str(), std::ios::binary);
    if (_outfile.bad() || _outfile.fail())
        return DT3_ERR_FILE_OPEN_FAILED;
        
    _header_length_offset = 0;
    _data_length_offset = 0;
    _data_size = 0;
    
    // write header
	ChunkHeader chunk_riff;
    set_type(chunk_riff.id,"RIFF");
    chunk_riff.length = 0;   // To be filled in later
    write(_outfile, chunk_riff);
    _header_length_offset = static_cast<DTuint>(_outfile.tellp()) - 4;
    
    // write WAVE data
	DTcharacter	id_wave[4];
    set_type(id_wave,"WAVE");
    write(_outfile, id_wave);

    // Format Chunk
	ChunkHeader chunk_format;
    set_type(chunk_format.id,"fmt ");
    chunk_format.length = sizeof(Format);
    write(_outfile, chunk_format);
    
    Format format;
    format.formatTag = FMT_WAVE_FORMAT_PCM;
    format.channels = num_channels;
    format.samplesPerSec = sample_rate;
    format.avgBytesPerSec = 0;
    format.blockAlign = 0;
    format.bitsPerSample = bits_per_sample;
    write(_outfile, format.formatTag);
    write(_outfile, format.channels);
    write(_outfile, format.samplesPerSec);
    write(_outfile, format.avgBytesPerSec);
    write(_outfile, format.blockAlign);
    write(_outfile, format.bitsPerSample);
    
    // Data Chunk
	ChunkHeader chunk_data;
    set_type(chunk_data.id,"data");
    chunk_data.length = 0;  // To be filled in later
    write(_outfile, chunk_data);
    _data_length_offset = static_cast<DTuint>(_outfile.tellp()) - 4;

    return DT3_ERR_NONE;
}

DTerr WAVWriter::write (DTubyte *data, DTuint size)
{
    _data_size += size;
    _outfile.write((const char*)data,size);

    return DT3_ERR_NONE;
}

DTerr WAVWriter::end_write (void)
{
    _outfile.seekp(_data_length_offset, std::ios_base::beg);
    write(_outfile, _data_size);
    
    _outfile.seekp(_header_length_offset, std::ios_base::beg);
    
    DTuint header_size = _data_size + _data_length_offset - _header_length_offset;
    write(_outfile, header_size);
    
    _outfile.close();

    return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

} // DT3
