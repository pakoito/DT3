#ifndef DT3_WAVWRITER
#define DT3_WAVWRITER
//==============================================================================
///	
///	File: WAVWriter.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <fstream>

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

class WAVWriter {
    public:
								WAVWriter			(void);	
								WAVWriter			(const WAVWriter &rhs);
        WAVWriter &             operator =			(const WAVWriter &rhs);
								~WAVWriter			(void);

	public:
		/// Open a blank sound file
		/// \param pathname Path of file to write
		/// \param num_channels number of channels in the file
		/// \param sample_rate sample rate of sound data
		/// \param bits_per_sample number of bits per sample
        /// \return error
        DTerr                   begin_write         (   const FilePath &pathname,
                                                        DTushort num_channels = 2,
                                                        DTint sample_rate = 44100,
                                                        DTushort bits_per_sample = 16);
    
        /// Write some sound data
        /// \param pointer to raw sound data
        /// \param size of raw sound data
        /// \return error
        DTerr                   write               (DTubyte *data, DTuint size);

		/// Finish writing sound file
        /// \return error
        DTerr                   end_write           (void);

    private:
		struct ChunkHeader {
			DTcharacter	id[4];
			DTuint		length;
		};
        
		enum FORMAT_TAG {
			FMT_WAVE_FORMAT_PCM		= 0x0001
		};
        
		//
		// Format chunk
		//
		        
		struct Format {
			DTushort	formatTag;			// Format category
			DTushort	channels;			// Number of channels
			DTuint		samplesPerSec;		// Sampling rate
			DTuint		avgBytesPerSec;		// For buffer estimation
			DTushort	blockAlign;			// Data block size
			DTushort	bitsPerSample;
		};

        void					write				(std::ofstream &outfile, DTubyte &v);
        void					write				(std::ofstream &outfile, DTushort &v);
        void					write				(std::ofstream &outfile, DTshort &v);
        void					write				(std::ofstream &outfile, DTint &v);
        void					write				(std::ofstream &outfile, DTuint &v);
        void					write				(std::ofstream &outfile, DTcharacter id[4]);
        void					write				(std::ofstream &outfile, ChunkHeader &v);
        void                    set_type            (DTcharacter id[4], const DTcharacter *id_str);
    
        std::ofstream           _outfile;
        DTuint                  _header_length_offset;
        DTuint                  _data_length_offset;
        DTuint                  _data_size;
        

};

//==============================================================================
//==============================================================================

} // DT3

#endif
