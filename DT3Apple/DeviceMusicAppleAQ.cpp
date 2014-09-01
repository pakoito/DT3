//==============================================================================
///	
///	File: DeviceMusicAppleAQ.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================
//
// NOTE: See http://developer.apple.com/iphone/library/qa/qa2009/qa1636.html
//
// EXAMPLE: afconvert -d aac -f caff /System/Library/Sounds/Submarine.aiff /tmp/output.caf
// Note: Use the -b option for bitrate tweaking (better performance) e.g. -b 38000 
//
//==============================================================================

#include "DT3Apple/DeviceMusicAppleAQ.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(DeviceMusicAppleAQ)

//==============================================================================
//==============================================================================

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DeviceMusicAppleAQ::DeviceMusicAppleAQ (void)
    :   _music_master_volume(1.0F),
		_music_volume		(0.0F),
		_music_volume_vel	(0.0F),
		_old_volume			(-1.0F),
		_tick_accum			(0.0F),
        _loop               (true)
{
	_aq_info._audio_file = NULL;
}

DeviceMusicAppleAQ::~DeviceMusicAppleAQ (void)
{
	stop_music();
}

//==============================================================================
//==============================================================================

void DeviceMusicAppleAQ::aq_buffer_callback(	void *					inUserData,
                                                AudioQueueRef			inAQ,
                                                AudioQueueBufferRef		inCompleteAQBuffer)
{
	AQInfo *aq_info = (AQInfo *)inUserData;
	
	// Check if done playing
	if (aq_info->_done) 
		return;
		
	UInt32 numBytes;
	UInt32 nPackets = aq_info->_packets_to_read_per_buffer;

	// read nPackets worth of data into buffer
	OSStatus error_code = ::AudioFileReadPackets(	aq_info->_audio_file,
                                                    false,
                                                    &numBytes, 
                                                    aq_info->_packet_descs.size() ? &aq_info->_packet_descs[0] : NULL, 
                                                    aq_info->_packet_index, 
                                                    &nPackets, 
                                                    inCompleteAQBuffer->mAudioData);
	if (error_code) {
		LOG_ERROR << "AudioFileReadPackets failed";
		return;
	}	
	
	// If packet read successfully, queue it up
	if (nPackets > 0) {
	
        UInt32 trimFramesAtStart = 0;
        UInt32 trimFramesAtEnd = 0;
			
        inCompleteAQBuffer->mAudioDataByteSize = numBytes; 
        inCompleteAQBuffer->mPacketDescriptionCount = nPackets;

        if (aq_info->_packet_index == 0) {
            // at the beginning -- need to trim priming frames
            trimFramesAtStart = aq_info->_num_priming_frames;
        } 

        aq_info->_packet_index += nPackets;
		
        if (aq_info->_packet_index == aq_info->_num_packets) {
            // at the end -- need to trim remainder frames
            trimFramesAtEnd = aq_info->_num_remainder_frames;

            // reset read from the beginning again
            if (aq_info->_loop)
                aq_info->_packet_index = 0;
        }

		// Queues the buffer for audio input/output.
        error_code = ::AudioQueueEnqueueBufferWithParameters(	inAQ,
																inCompleteAQBuffer,
																aq_info->_packet_descs.size() ? nPackets : 0,
																aq_info->_packet_descs.size() ? &aq_info->_packet_descs[0] : NULL, 
																trimFramesAtStart,
																trimFramesAtEnd,
																0, NULL, NULL, NULL);

		if (error_code) {
			LOG_ERROR << "AudioQueueEnqueueBufferWithParameters failed";
		}	
		
	} else {
		error_code = ::AudioQueueStop(aq_info->_queue, false);
		if (error_code) {
			LOG_ERROR << "AudioQueueStop(false) failed";
			return;
		}	
		
		// reading nPackets == 0 is our EOF condition
		aq_info->_done = true;
	}
}

//==============================================================================
//==============================================================================

void DeviceMusicAppleAQ::derive_buffer_size (	AudioStreamBasicDescription &ASBDesc,
                                                UInt32                      maxPacketSize,
                                                Float64                     seconds,
                                                UInt32                      *outBufferSize,
                                                UInt32                      *outNumPacketsToRead)
{
    const UInt32 maxBufferSize = 0x50000;
    const UInt32 minBufferSize = 0x4000;
 
    if (ASBDesc.mFramesPerPacket != 0) {
        Float64 numPacketsForTime = ASBDesc.mSampleRate / ASBDesc.mFramesPerPacket * seconds;
        *outBufferSize = (UInt32) numPacketsForTime * maxPacketSize;
    } else {
        *outBufferSize = maxBufferSize > maxPacketSize ? maxBufferSize : maxPacketSize;
    }
 
    if (*outBufferSize > maxBufferSize && *outBufferSize > maxPacketSize) {
        *outBufferSize = maxBufferSize;
    } else {                                
        if (*outBufferSize < minBufferSize)
            *outBufferSize = minBufferSize;
    }
 
    *outNumPacketsToRead = *outBufferSize / maxPacketSize;
}

//==============================================================================
//==============================================================================

OSStatus DeviceMusicAppleAQ::read_proc( void     *inClientData,
                                        SInt64   inPosition,
                                        UInt32   requestCount,
                                        void     *buffer,
                                        UInt32   *actualCount)
{
    BinaryFileStream *stream = (BinaryFileStream*) inClientData;
    
    stream->seek_g( static_cast<DToffset>(inPosition), Stream::FROM_BEGINNING);
    (*actualCount) = (UInt32) stream->read_raw( (DTubyte*) buffer, requestCount);
    
    return noErr;
}

SInt64 DeviceMusicAppleAQ::size_proc ( void  *inClientData )
{
    BinaryFileStream *stream = (BinaryFileStream*) inClientData;
    return stream->length();
}

//==============================================================================
//==============================================================================

void DeviceMusicAppleAQ::start_music (const FilePath &pathname)
{		
	if (pathname.is_blank())
		return;	
	
	OSStatus	error_code;
    
    // Open up the stream for the font file
	DTerr error = FileManager::open(_file, pathname, true);
    if (error != DT3_ERR_NONE)
        return;
    
    error_code = AudioFileOpenWithCallbacks (   (void*) &_file,
                                                &read_proc,                 // inReadFunc
                                                NULL,                       // inWriteFunc
                                                NULL,                       // inGetSizeFunc
                                                NULL,                       // inSetSizeFunc
                                                0,                          // inFileTypeHint
                                                &_aq_info._audio_file);     // outAudioFile
	if (error_code) {
		LOG_ERROR << "Unable to open file";
		return;
	}

	UInt32 size = sizeof(_aq_info._data_format);
	
	error_code = ::AudioFileGetProperty(_aq_info._audio_file, kAudioFilePropertyDataFormat, &size, &_aq_info._data_format);
	if (error_code) {
		::AudioFileClose(_aq_info._audio_file);
		_aq_info._audio_file = NULL;

		LOG_ERROR << "Couldn't get file's data format";
		return;
	}
	
	// get Priming and remainder packets as per http://developer.apple.com/iphone/library/qa/qa2009/qa1636.html
	AudioFilePacketTableInfo thePacketTableInfo;

	size = sizeof(thePacketTableInfo);
	error_code = ::AudioFileGetProperty(_aq_info._audio_file, kAudioFilePropertyPacketTableInfo, &size, &thePacketTableInfo);
	if (error_code) {
		_aq_info._num_priming_frames = 0;
		_aq_info._num_remainder_frames = 0;
	} else {
		_aq_info._num_priming_frames = thePacketTableInfo.mPrimingFrames;
		_aq_info._num_remainder_frames = thePacketTableInfo.mRemainderFrames;
	}

	// get packet count
	size = sizeof(_aq_info._num_packets);

	error_code = ::AudioFileGetProperty(_aq_info._audio_file, kAudioFileStreamProperty_AudioDataPacketCount,
										&size, &_aq_info._num_packets);
	if (error_code) {
		::AudioFileClose(_aq_info._audio_file);
		LOG_ERROR << "Couldn't get file's packet count";
		return;
	}

	// Specify the callback
	error_code = ::AudioQueueNewOutput(&_aq_info._data_format, aq_buffer_callback, &_aq_info, NULL, NULL, 0, &_aq_info._queue);
	if (error_code) {
		::AudioFileClose(_aq_info._audio_file);
		_aq_info._audio_file = NULL;

		LOG_ERROR << "AudioQueueNewOutput failed";
		return;
	}
	
	// set the default volume	
	DTfloat music_gain = Globals::has_global("SYS_MUSIC_GAIN") ? MoreStrings::cast_from_string<DTfloat>(Globals::global("SYS_MUSIC_GAIN")) : 1.0F;
	DTfloat new_vol = _music_volume * _music_master_volume * music_gain;
	::AudioQueueSetParameter(_aq_info._queue, kAudioQueueParam_Volume, new_vol);
	_old_volume = new_vol;
	

	
	// We have a couple of things to take care of now
	// (1) Setting up the conditions around VBR or a CBR format - affects how we will read from the file
	
	// first check to see what the max size of a packet is - if it is bigger
	// than our allocation default size, that needs to become larger
	UInt32 max_packet_size;
	size = sizeof(max_packet_size);
	
	error_code = ::AudioFileGetProperty(_aq_info._audio_file, kAudioFilePropertyPacketSizeUpperBound, &size, &max_packet_size);
	if (error_code) {
		::AudioQueueDispose(_aq_info._queue, true);
		::AudioFileClose(_aq_info._audio_file);
		_aq_info._audio_file = NULL;

		LOG_ERROR << "Couldn't get file's max packet size";
		return;
	}
	
	// Derive a new buffer size
	derive_buffer_size ( _aq_info._data_format, max_packet_size, 0.5, &_buffer_size, &_aq_info._packets_to_read_per_buffer);
	
	// Packet descriptions array if VBR file	 
	if (_aq_info._data_format.mBytesPerPacket == 0 || _aq_info._data_format.mFramesPerPacket == 0) {                                         // 2
		_aq_info._packet_descs.resize(_aq_info._packets_to_read_per_buffer);
	}				
	

	// (2) If the file has a cookie, we should get it and set it on the AQ
	size = sizeof(UInt32);
	OSStatus result = ::AudioFileGetPropertyInfo (_aq_info._audio_file, kAudioFilePropertyMagicCookieData, &size, NULL);
	if (!result && size) {
		std::vector<DTcharacter> cookie;
		cookie.resize(size);
			
		error_code = ::AudioFileGetProperty (_aq_info._audio_file, kAudioFilePropertyMagicCookieData, &size, &cookie[0]);
		if (error_code) {
			LOG_ERROR << "get cookie from file";
		}

		error_code = ::AudioQueueSetProperty(_aq_info._queue, kAudioQueueProperty_MagicCookie, &cookie[0], size);
		if (error_code) {
			LOG_ERROR << "set cookie on queue";
		}

	}
	
	
	// Prime the queue with some data before starting
	_aq_info._done = false;
    _aq_info._loop = _loop;
	_aq_info._packet_index = 0;
	
	for (int i = 0; i < NUM_BUFFERS; ++i) {
		error_code = ::AudioQueueAllocateBuffer(_aq_info._queue, _buffer_size, &_aq_info._buffers[i]);
		if (error_code) {
			::AudioQueueDispose(_aq_info._queue, true);
			::AudioFileClose(_aq_info._audio_file);
			_aq_info._audio_file = NULL;
		
			LOG_ERROR << "AudioQueueAllocateBuffer failed";
			return;
		}
	
		aq_buffer_callback (&_aq_info, _aq_info._queue, _aq_info._buffers[i]);
		
		if (_aq_info._done) 
			break;
	}	
		
	// Lets start playing now - stop is called in the aQBufferCallback when there's
	// no more to read from the file
	error_code = ::AudioQueueStart(_aq_info._queue, NULL);
	if (error_code) {
		::AudioQueueDispose(_aq_info._queue, true);
		::AudioFileClose(_aq_info._audio_file);
		_aq_info._audio_file = NULL;
		
		LOG_ERROR << "AudioQueueStart failed";
		return;
	}
	
	// set the current music
	_music_current = pathname;
}

//==============================================================================
//==============================================================================

void DeviceMusicAppleAQ::play_music	(const FilePath &pathname, DTfloat transition, DTboolean loop)
{
	_music_volume_vel = 1.0F / transition;
	_music_pending = pathname;
    _loop = loop;
}

//==============================================================================
//==============================================================================

void DeviceMusicAppleAQ::stop_music (DTfloat transition)
{
	_music_volume_vel = -1.0F / transition;
}

void DeviceMusicAppleAQ::stop_music (void)
{
	if (_aq_info._audio_file) {
		OSStatus error_code;
		error_code = ::AudioQueueDispose(_aq_info._queue, true);
		if (error_code) {
			LOG_ERROR << "AudioQueueDispose(true) failed";
			return;
		}
	
		error_code = ::AudioFileClose(_aq_info._audio_file);
		if (error_code) {
			LOG_ERROR << "AudioQueueDispose(false) failed";
			return;
		}
		
		_aq_info._audio_file = NULL;
        _file.close();
		_music_current.clear();
	}
	
}

//==============================================================================
//==============================================================================

void DeviceMusicAppleAQ::set_volume (DTfloat volume)
{
	_music_master_volume = volume;
}

//==============================================================================
//==============================================================================

DTfloat DeviceMusicAppleAQ::progress (void)
{
    if (playing())
        return static_cast<DTfloat>( _aq_info._packet_index) / static_cast<DTfloat>(_aq_info._num_packets);
    else
        return 0.0F;
        
}

DTboolean DeviceMusicAppleAQ::playing (void)
{
    return (_aq_info._audio_file && !_aq_info._done);
}

//==============================================================================
//==============================================================================

FilePath DeviceMusicAppleAQ::current_music (void)
{
    return _music_current;
}

//==============================================================================
//==============================================================================

void DeviceMusicAppleAQ::tick (const DTfloat dt)
{
	// Only tick at 10 FPS
	const DTfloat TICK_PERIOD = 0.1F;
	_tick_accum += dt;
	if (_tick_accum > TICK_PERIOD) {
		_tick_accum -= TICK_PERIOD;

		//
		// Music
		//
		
		DTboolean music_on = MoreStrings::cast_from_string<DTboolean>(Globals::global("SYS_MUSIC"));
		DTfloat music_gain = Globals::has_global("SYS_MUSIC_GAIN") ? MoreStrings::cast_from_string<DTfloat>(Globals::global("SYS_MUSIC_GAIN")) : 1.0F;

		// Check for music off
		if (!music_on || music_gain <= 0.01F) {
			if (_music_current.full_path().size() > 0)
				_music_pending = _music_current;
			_music_current.clear();
			stop_music ();
			return;
		}

		// Trying to play song already playing so ignore it
		if (_music_pending == _music_current)
			_music_pending.clear();

		// Swapping music
		if (!_music_pending.is_blank() && !_music_current.is_blank()) {
				
			// transition to new music
			_music_volume -= _music_volume_vel * TICK_PERIOD;
			
			if (_music_volume < 0.0F) {
				stop_music ();

				_music_volume = 0.0F;
				start_music (_music_pending);
				_music_pending.clear();

			}
            
            DTfloat new_vol = _music_volume * _music_master_volume * music_gain;
            if (_aq_info._audio_file && new_vol != _old_volume) {
                ::AudioQueueSetParameter(_aq_info._queue, kAudioQueueParam_Volume, new_vol);
                _old_volume = new_vol;
            }

		// Ramping music
		} else if (!_music_current.is_blank()) {
		
			_music_volume += _music_volume_vel * TICK_PERIOD;
			if (_music_volume > 1.0F) {
				_music_volume = 1.0F;
                _music_volume_vel = 0.0F;
			} else if (_music_volume < 0.0F) {
                stop_music ();

				_music_volume = 0.0F;
                _music_volume_vel = 0.0F;
			}
			
			DTfloat new_vol = _music_volume * _music_master_volume * music_gain;
			if (_aq_info._audio_file && new_vol != _old_volume) {
				::AudioQueueSetParameter(_aq_info._queue, kAudioQueueParam_Volume, new_vol);
				_old_volume = new_vol;
			}
				
		// Music pending and existing music is playing
		} else if (!_music_pending.is_blank()) {
			stop_music ();

			_music_volume = 1.0F;
			_music_volume_vel = 0.0F;
			start_music (_music_pending);
			_music_pending.clear();

			DTfloat new_vol = _music_volume * _music_master_volume * music_gain;
			if (_aq_info._audio_file && new_vol != _old_volume) {
				::AudioQueueSetParameter(_aq_info._queue, kAudioQueueParam_Volume, new_vol);
				_old_volume = new_vol;
			}
		}
		
		// Loop music
		if (_aq_info._audio_file && _aq_info._done) {
			FilePath music_current = _music_current;	// Stopping music clears this
			stop_music ();
            
            if (_loop)
                start_music (music_current);
		}
	}
}

//==============================================================================
//==============================================================================

} // DT3
