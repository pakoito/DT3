//==============================================================================
///	
///	File: ImporterSoundWAV.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
/// NOTE: Codecs in this file are adapted from alutCodec.c
///
//==============================================================================

#include "DT3Core/Resources/Importers/ImporterSoundWAV.hpp"
#include "DT3Core/Resources/ResourceTypes/SoundResource.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/Types/Utility/Endian.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_IMPORTER(ImporterSoundWAV,wav)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ImporterSoundWAV::ImporterSoundWAV (void)
	:	_data_start             (0),
		_data_length            (0),
        _preloaded_chunk        (NULL),
        _preloaded_chunk_size	(0)
{    
	
}
				
ImporterSoundWAV::~ImporterSoundWAV (void)
{ 
	delete[] _preloaded_chunk;
}

//==============================================================================
//==============================================================================

void	ImporterSoundWAV::read(BinaryFileStream &infile, DTubyte &v)
{
    infile.read_raw((DTubyte*)&v, sizeof(DTbyte));
}

//==============================================================================
//==============================================================================

void	ImporterSoundWAV::read(BinaryFileStream &infile, DTushort &v)
{
    infile.read_raw((DTubyte*)&v, sizeof(DTushort));
	Endian::from_little_endian(v);
}

//==============================================================================
//==============================================================================

void	ImporterSoundWAV::read(BinaryFileStream &infile, DTshort &v)
{
    infile.read_raw((DTubyte*)&v, sizeof(DTshort));
	Endian::from_little_endian(v);
}

//==============================================================================
//==============================================================================

void	ImporterSoundWAV::read(BinaryFileStream &infile, DTuint &v)
{    
    infile.read_raw((DTubyte*)&v, sizeof(DTuint));
	Endian::from_little_endian(v);
}

//==============================================================================
//==============================================================================

void	ImporterSoundWAV::read(BinaryFileStream &infile, DTint &v)
{    
    infile.read_raw((DTubyte*)&v, sizeof(DTint));
	Endian::from_little_endian(v);
}

//==============================================================================
//==============================================================================

void	ImporterSoundWAV::read(BinaryFileStream &infile, DTcharacter id[4])
{
    infile.read_raw((DTubyte*)&id[0], sizeof(DTcharacter));
    infile.read_raw((DTubyte*)&id[1], sizeof(DTcharacter));
    infile.read_raw((DTubyte*)&id[2], sizeof(DTcharacter));
    infile.read_raw((DTubyte*)&id[3], sizeof(DTcharacter));
}

//==============================================================================
//==============================================================================

void	ImporterSoundWAV::read(BinaryFileStream &infile, ChunkHeader &v)
{
 	read(infile, v.id);
	read(infile, v.length);
}

//==============================================================================
//==============================================================================

DTboolean ImporterSoundWAV::is_type (DTcharacter id[4], const DTcharacter *id_str)
{
	return	id[0] == id_str[0] &&
			id[1] == id_str[1] &&
			id[2] == id_str[2] &&
			id[3] == id_str[3];
}
	
//==============================================================================
//==============================================================================

DTerr ImporterSoundWAV::import(SoundResource *target, std::string args)
{
	// Convert path to this platform
	_pathname = target->path();

	//
    // open the file
	//
	
	FileManager::open(_infile, _pathname, true);

	//
	// Check file header
	//
	
	ChunkHeader chunk_riff;
	read(_infile, chunk_riff);

	if (!is_type(chunk_riff.id,"RIFF"))
        return DT3_ERR_FILE_WRONG_TYPE;


	DTcharacter	id_wave[4];
	read(_infile, id_wave);

	if (!is_type(id_wave,"WAVE"))
        return DT3_ERR_FILE_WRONG_TYPE;


	DTint size_remaining = chunk_riff.length - 4; // 4 for "WAVE"
	while(size_remaining > 0) {
		// read the next chunk
		ChunkHeader chunk;
		read(_infile, chunk);

		//
		// Format Chunk
		//
		if (is_type(chunk.id,"fmt ")) {
			// Common format
			read(_infile, _format._format_tag);
			read(_infile, _format._channels);
			read(_infile, _format._samples_per_sec);
			read(_infile, _format._avg_bytes_per_sec);
			read(_infile, _format._block_align);
			
			// Specific format
			switch (_format._format_tag) {
				case FMT_WAVE_FORMAT_PCM:	read(_infile, _format._wave_format_PCM._bits_per_sample);
										break;
				case FMT_FORMAT_MULAW:
				case FMT_IBM_FORMAT_ALAW:
				case FMT_IBM_FORMAT_ADPCM:
				default:				LOG_MESSAGE << "Failed to read WAV file. Currently only Microsoft PCM is supported.";
										return DT3_ERR_FILE_OPEN_FAILED;
										break;
			};
            
            ASSERT( (_format._wave_format_PCM._bits_per_sample == 16) || (_format._wave_format_PCM._bits_per_sample == 8) );
            
            // Some WAV's require this
			_infile.seek_g(chunk.length - sizeof(_format),BinaryStream::FROM_CURRENT);
			
		//
		// Data chunk
		//
		} else if (is_type(chunk.id,"data")) {
		
			// Record the position and length of data in the file
			_data_start = _infile.g();
			_data_length = chunk.length;
		
			// Seek past data for now
			_infile.seek_g(chunk.length,BinaryStream::FROM_CURRENT);
		
		//
		// Unknown chunk
		//
		} else {
			_infile.seek_g(chunk.length,BinaryStream::FROM_CURRENT);
		}

		size_remaining -= sizeof(ChunkHeader) + chunk.length;
	}
	
	//
	// Post process data into OpenAL format
	//

	switch (_format._format_tag) {
		case FMT_WAVE_FORMAT_PCM:	if (_format._wave_format_PCM._bits_per_sample == 16 && _format._channels == 1)	target->set_format(SoundResource::FORMAT_MONO16);
                                    else if (_format._wave_format_PCM._bits_per_sample == 16 && _format._channels == 2)	target->set_format(SoundResource::FORMAT_STEREO16);
                                    else return DT3_ERR_FILE_OPEN_FAILED;
                                    break;
		case FMT_FORMAT_MULAW:
		case FMT_IBM_FORMAT_ALAW:
		case FMT_IBM_FORMAT_ADPCM:
		default:                    return DT3_ERR_FILE_OPEN_FAILED;
                                    break;
	};

	// set final frequency
	target->set_frequency(_format._samples_per_sec);
    
    
    //
    // Preload some data
    //
	
	// Check large enough for streaming
    DTsize preloaded_chunk_size = MoreMath::min( (DTsize) DT3_AUDIO_STREAMING_BUFFER_SIZE, length());
    _preloaded_chunk = new ("SoundBuffer") DTubyte[preloaded_chunk_size];

#ifdef DT3_DEBUG
    DTsize num_bytes = stream(_preloaded_chunk, 0, preloaded_chunk_size);
    ASSERT(num_bytes == preloaded_chunk_size);
#else
    stream(_preloaded_chunk, 0, preloaded_chunk_size);
#endif
    
    _preloaded_chunk_size = preloaded_chunk_size;

	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

DTsize ImporterSoundWAV::stream (DTubyte *data, DTsize start, DTsize length)
{

    DTsize actual_length = MoreMath::min(length, _data_length - start);
    DTsize end = start + actual_length;
    
    // Check if we can load from the preloaded chunk
    if (end <= _preloaded_chunk_size) {
        ::memcpy(data, _preloaded_chunk+start, (size_t) actual_length);

    // Otherwise load from file
    } else {
        _infile.seek_g(_data_start + start, BinaryStream::FROM_BEGINNING);
        _infile.read_raw(data, actual_length);
    }
	
#if DT3_BYTEORDER != DT3_LIL_ENDIAN
	// 8 bits per sample
	if (_format.waveFormatPCM.bitsPerSample == 8) {
	
		// Do nothing
		
	// 16 bits per sample
	} else if (_format.waveFormatPCM.bitsPerSample == 16) {
	
		DTshort *d = (DTshort *) data;
		DTuint l = length / 2;
		for (DTuint i = 0; i < l; i++) {
            Endian::fromLittleEndian(d[i]);
		}
		
	}
#endif

    return actual_length;
}

//==============================================================================
//==============================================================================

DTsize ImporterSoundWAV::length (void)
{
	return _data_length;
}

//==============================================================================
//==============================================================================

} // DT3

