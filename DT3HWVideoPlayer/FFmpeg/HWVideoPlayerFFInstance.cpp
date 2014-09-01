//==============================================================================
///	
///	File: 			HWVideoPlayerFFInstance.cpp
///	Author:			Tod Baudais
///					Copyright (C) 2000-2007. All rights reserved.
///	
///	Date Created:	2/12/2013
///	Changes:		-none-
///	
//==============================================================================

#include "HWVideoPlayerFFInstance.hpp"

#include "Assert.hpp"
#include "ConsoleStream.hpp"
#include "System.hpp"
#include "DeviceAudio.hpp"

#include "HWVideoPlayerFFDataSourceFile.hpp"
#include "HWVideoPlayerFFDataSourceURL.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

const DTint COMMAND_INDEX_PLAY = -1;
const DTint COMMAND_INDEX_PAUSE = -2;
const DTint COMMAND_INDEX_SEEK = -3;

const DTint AUDIO_FREQUENCY = 44100;
const DTdouble AUDIO_READ_AHEAD = 0.5;
const DTdouble AUDIO_PRE_BUFFER_THRESH = 0.2;
const DTint AUDIO_BUFFER_SIZE = AUDIO_PRE_BUFFER_THRESH * AUDIO_FREQUENCY;


//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

HWVideoPlayerFFInstance::HWVideoPlayerFFInstance (void)
    :   _data_source            (NULL),
        _src_frame              (NULL),
        _dst_frame              (NULL),
        _src_audio_frame        (NULL),
        _state                  (STATE_IDLE),
        _buffer_start           (0),
        _buffer_end             (0),
        _playback_thread        (NULL),
        _data_thread            (NULL),
        _playback_speed         (1.0),
        _img_convert_ctx        (NULL),
        _snd_convert_ctx        (NULL),
        _video_timestamp        (-1.0),
        _audio_timestamp        (-1.0)
{
    _sound_source.setEndsOnEmpty(false);
    
    _dst_sound_packet.setFormat(SoundResource::FORMAT_STEREO16);
    _dst_sound_packet.setFrequency(AUDIO_FREQUENCY);
}

HWVideoPlayerFFInstance::~HWVideoPlayerFFInstance (void)
{
    close();
}

//==============================================================================
//==============================================================================

DTerr HWVideoPlayerFFInstance::open (const FilePath &path)
{
    _data_source = new HWVideoPlayerFFDataSourceFile(path);   // Create doesn't accept parameters
    
    DTerr err = setup();
    if (err != DT3_ERR_NONE) {
        _state = STATE_ERROR;
        return err;
    } else {
        _state = STATE_PAUSED;
    }
    
    HAL::createThread(dataThread, this, _data_thread);
    Assert(_data_thread);

    HAL::createThread(playbackThread, this, _playback_thread);
    Assert(_playback_thread);
    
    pause();
        
    return DT3_ERR_NONE;
}

DTerr HWVideoPlayerFFInstance::open (const URL &url)
{
    _data_source = new HWVideoPlayerFFDataSourceURL(url);   // Create doesn't accept parameters

    DTerr err = setup();
    if (err != DT3_ERR_NONE) {
        _state = STATE_ERROR;
        return err;
    } else {
        _state = STATE_PAUSED;
    }

    HAL::createThread(dataThread, this, _data_thread);
    Assert(_data_thread);
    
    HAL::createThread(playbackThread, this, _playback_thread);
    Assert(_playback_thread);

    pause();

    return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

DTerr HWVideoPlayerFFInstance::setup (void)
{
    //
    // Open the stream
    //
    
    // FFMpeg stream information
    DTerr err = _data_source->open();
    if (err != DT3_ERR_NONE)
        return err;
    
    AVFormatContext *format_context = _data_source->formatContext();
    AVCodecContext *video_codec_context = _data_source->videoCodecContext();
    AVCodecContext *audio_codec_context = _data_source->audioCodecContext();
    AVStream *video_stream = _data_source->videoStream();
    AVStream *audio_stream = _data_source->audioStream();

    //
    // Get video parameters
    //
    
    // Reset the playhead and get movie duration
    _current_time = 0.0F;
    _length = (DTdouble) format_context->duration / (DTdouble) AV_TIME_BASE;
    
    // Timebase
    _video_time_base = ::av_q2d(video_stream->time_base);
    _video_time_start = video_stream->start_time;

    if (audio_stream) {
        _audio_time_base = ::av_q2d(audio_stream->time_base);
        _audio_time_start = audio_stream->start_time;
    }

    // Get dimensions
    _width = video_codec_context->width;
    _height = video_codec_context->height;

    
    //
    // Build software scaler
    //
    
    // Software scaler
    _img_convert_ctx = ::sws_getContext(    _width, _height, video_codec_context->pix_fmt,
                                            _width, _height, AV_PIX_FMT_RGB24,
                                            SWS_POINT,
                                            NULL, NULL, NULL);

    //
    // Build audio resampler
    //
    
    // Software scaler
    if (audio_codec_context) {
        _snd_convert_ctx = ::swr_alloc_set_opts	(	NULL,
                                                    AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, AUDIO_FREQUENCY,
                                                    audio_codec_context->channel_layout,
                                                    audio_codec_context->sample_fmt,
                                                    audio_codec_context->sample_rate,
                                                    0, NULL);

        ::swr_init(_snd_convert_ctx);
    }
    
    
    //
    // Allocate source frame
    //

    _src_frame = ::avcodec_alloc_frame();
    if (!_src_frame) {
        return DT3_ERR_FILE_OPEN_FAILED;
    }
    

    //
    // Allocate destination frame
    //

    _dst_frame = ::avcodec_alloc_frame();
    if (!_dst_frame) {
        return DT3_ERR_FILE_OPEN_FAILED;
    }
    
    // Allocate a texture to hold the final frames
    DTsize size = ::avpicture_get_size(PIX_FMT_RGB24, _width, _height);
    
    _tex_buffer = makeSmartPtr(ArrayBlock<DTubyte>::create());
    _tex_buffer->resize(size, 0);

    _tex = makeSmartPtr(TextureResource::create());
    _tex->setTextels2D(_width, _height, _tex_buffer, DT3GL_FORMAT_RGB, false, DT3GL_ACCESS_CPU_WRITE | DT3GL_ACCESS_GPU_READ);
    
    // Associate frame to buffer
    ::avpicture_fill( (AVPicture *) _dst_frame, _tex_buffer->getBuffer(), AV_PIX_FMT_RGB24, _width, _height);
    
    // Flip the frame
    for (int i = 0; i < 4; i++) {
        _dst_frame->data[i] += _dst_frame->linesize[i] * (_height-1);
        _dst_frame->linesize[i] = -_dst_frame->linesize[i];
    }

    
    //
    // Allocate destination audio frame
    //
    
    _src_audio_frame = ::avcodec_alloc_frame();
    if (!_src_audio_frame) {
        return DT3_ERR_FILE_OPEN_FAILED;
    }

    // Now in the loaded state
    _state = STATE_PAUSED;
    
    return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

DTdouble HWVideoPlayerFFInstance::bufferStartTime (void) const
{
    return (DTdouble) (_buffer_start - _video_time_start) * _video_time_base;
}

DTdouble HWVideoPlayerFFInstance::bufferEndTime (void) const
{
    return (DTdouble) (_buffer_end - _video_time_start) * _video_time_base;
}

//==============================================================================
// This is the data thread!!
//==============================================================================

void HWVideoPlayerFFInstance::dataThread (void *hwvp)
{
    HWVideoPlayerFFInstance* inst = reinterpret_cast<HWVideoPlayerFFInstance*>(hwvp);
    Assert(inst);

    //
    // Fill up the packet queue
    //
    
    AVFormatContext *format_context = inst->_data_source->formatContext();
    
    AVCodecContext *video_codec_context = inst->_data_source->videoCodecContext();
    AVCodecContext *audio_codec_context = inst->_data_source->audioCodecContext();

    DTint video_stream_index = inst->_data_source->videoStreamIndex();
    DTint audio_stream_index = inst->_data_source->audioStreamIndex();

    AVPacket packet;
    ::av_init_packet(&packet);
    
    while (inst->_state != STATE_CLOSING) {
    
        DTboolean command_processed = false;
        DTboolean packet_processed = false;
    
        //
        // Read in commands
        //
        
        HWVideoPlayerFFCommandQueue::Command command;
        DTdouble param;
        
        while (inst->_data_command_queue.popCommand(command, param)) {
        
            switch (command) {
            
                case HWVideoPlayerFFCommandQueue::CMD_PLAY: {
                    inst->_playback_command_queue.pushPlay();
                } break;

                case HWVideoPlayerFFCommandQueue::CMD_PAUSE: {
                    inst->_playback_command_queue.pushPause();
                } break;

                case HWVideoPlayerFFCommandQueue::CMD_SEEK: {

                    // Seeking cases all of the packets to be flushed
                    inst->_video_packet_queue.clear();
                    inst->_audio_packet_queue.clear();
                    
                    ::avcodec_flush_buffers(video_codec_context);
                    
                    if (audio_codec_context)
                        ::avcodec_flush_buffers(audio_codec_context);
                    
                    // Seek the data source
                    int64_t pos = param * AV_TIME_BASE;
                    DTint err = ::av_seek_frame(format_context, -1, pos, AVSEEK_FLAG_ANY | AVSEEK_FLAG_BACKWARD);    // Should flag be AVSEEK_FLAG_BACKWARD?
                    if (err < 0) {
                        LOG_MESSAGE << "Unable to seek";
                    }
                    
                    inst->_playback_command_queue.pushSeek(param);

                } break;

            }
            
            // Bump the playback thread so it'll process the command packet
            if (inst->_playback_thread)
                HAL::resumeThread(inst->_playback_thread);
            
            command_processed = true;
        }
    
        //
        // Read in more packets
        //
        
        DTint err;
        if ( (err = ::av_read_frame(format_context, &packet)) >= 0) {
        
            if (packet.pts < inst->_buffer_start) inst->_buffer_start = packet.pts;
            if (packet.pts < inst->_buffer_end)   inst->_buffer_end = packet.pts;
            
            // Insert into packet queue
            if (packet.stream_index == video_stream_index)
                inst->_video_packet_queue.pushBack(&packet);
            else if (packet.stream_index == audio_stream_index)
                inst->_audio_packet_queue.pushBack(&packet);
            
            // Bump the playback thread so it'll process the command packet
            if (inst->_playback_thread)
                HAL::resumeThread(inst->_playback_thread);

            packet_processed = true;
        }
        
        //
        // If there wasn't anything to process, suspend the thread
        //
        
        if (!command_processed && !packet_processed) {
            // If there's no more packets, suspend the thread
            HAL::suspendThread(inst->_data_thread);
        }
        
    }
    
}

//==============================================================================
// This is the playback thread!!
//==============================================================================

void HWVideoPlayerFFInstance::playbackThread (void *hwvp)
{
    HWVideoPlayerFFInstance* inst = reinterpret_cast<HWVideoPlayerFFInstance*>(hwvp);
    Assert(inst);

    //
    // Process playback queue
    //
    
    AVCodecContext *video_codec_context = inst->_data_source->videoCodecContext();
    AVCodecContext *audio_codec_context = inst->_data_source->audioCodecContext();

    AVPacket packet;
    ::av_init_packet(&packet);
    
    
    // Flag for determining if the output texture contains valid frame data. The
    // loop tries to keep this true after seeking and loading
    DTboolean valid_frame = false;


    // Loop until done
    while (inst->_state != STATE_CLOSING) {

        //
        // Read in commands
        //

        HWVideoPlayerFFCommandQueue::Command command;
        DTdouble param;
        
        while (inst->_playback_command_queue.popCommand(command, param)) {
        
            switch (command) {
            
                case HWVideoPlayerFFCommandQueue::CMD_PLAY: {
                
                    if (inst->_state == STATE_PAUSED) {
                        inst->_state = STATE_PLAYING;
                        inst->_timer.getDeltaTime();    // Just to reset delta time
                        
                        inst->_sound_source.clearPackets();
                        
                        inst->_audio_timestamp = -1.0;
                        inst->_video_timestamp = -1.0;
                        
                        if (audio_codec_context)
                            System::getAudioRenderer()->play(&inst->_sound_source, NULL);

                    }
                    
                } break;

                case HWVideoPlayerFFCommandQueue::CMD_PAUSE: {
                
                    if (inst->_state == STATE_PLAYING) {
                        inst->_state = STATE_PAUSED;

                        inst->_sound_source.clearPackets();
                        
                        if (audio_codec_context)
                            System::getAudioRenderer()->stop(&inst->_sound_source);
                        
                        HAL::suspendThread(inst->_playback_thread);
                    }
                    
                } break;

                case HWVideoPlayerFFCommandQueue::CMD_SEEK: {
                    if (audio_codec_context && inst->_state == STATE_PLAYING) {
                        System::getAudioRenderer()->stop(&inst->_sound_source);
                    }
                    
                    inst->_current_time = param;
                    valid_frame = false;
                    
                    inst->_audio_timestamp = -1.0;
                    inst->_video_timestamp = -1.0;
                    
                    inst->_sound_source.clearPackets();

                    if (audio_codec_context && inst->_state == STATE_PLAYING) {
                        System::getAudioRenderer()->play(&inst->_sound_source, NULL);
                    }

                } break;

            }
            
        }
    

        //
        // Read in more packets if playing or if we need a valid frame
        //
        
        if (inst->_state == STATE_PLAYING || !valid_frame) {
        
            // Increment playback timer
            inst->_current_time += inst->_timer.getDeltaTime() * inst->_playback_speed * (inst->_state == STATE_PLAYING ? 1.0 : 0.0);
            
            // Force to pause when done
            if (inst->_current_time >= inst->_length) {
                inst->_playback_command_queue.pushPause();
            }

            //
            // Process video frames
            //

            while ( (inst->_video_timestamp < inst->_current_time) && inst->_video_packet_queue.popFront(&packet)) {
            
                // Decode video frame
                int got_frame;
                ::avcodec_decode_video2(video_codec_context, inst->_src_frame, &got_frame, &packet);
                
                // Did we get a video frame?
                if(got_frame) {
                
                    // Get the best timestamp
                    int64_t presentation_time_stamp = ::av_frame_get_best_effort_timestamp(inst->_src_frame);
                    inst->_video_timestamp = (DTdouble) (presentation_time_stamp - inst->_video_time_start) * inst->_video_time_base;
                    
                    //LOG_MESSAGE << "Video Playback Time: " << inst->_current_time << " Presentation Time: " << inst->_video_timestamp;
                
                    // Convert the image from its native format to RGB
                    ::sws_scale(    inst->_img_convert_ctx,
                                    inst->_src_frame->data,
                                    inst->_src_frame->linesize,
                                    0,
                                    inst->_height,
                                    inst->_dst_frame->data,
                                    inst->_dst_frame->linesize);
                    
                    // We now have a valid frame
                    valid_frame = true;
                }
            
                if(video_codec_context->refcounted_frames == 1)
                    ::av_frame_unref(inst->_src_frame);
                
                ::av_free_packet(&packet);
            }
            
            //
            // Process Audio Frames
            //

            // Note: Queue up audio a bit ahead
            while ((inst->_audio_timestamp < (inst->_current_time + AUDIO_READ_AHEAD) ) && inst->_audio_packet_queue.popFront(&packet)) {
                
                ::avcodec_get_frame_defaults(inst->_src_audio_frame);

                // Decode audio frame
                int got_frame;
                ::avcodec_decode_audio4(audio_codec_context, inst->_src_audio_frame, &got_frame, &packet);

                if (got_frame) {

                    // Get the best timestamp
                    int64_t presentation_time_stamp = packet.pts;
                    inst->_audio_timestamp = (DTdouble) (presentation_time_stamp - inst->_audio_time_start) * inst->_audio_time_base;

                    LOG_MESSAGE << "Audio Playback Time: " << inst->_current_time << " Presentation Time: " << inst->_audio_timestamp;

//                    DTint data_size = ::av_samples_get_buffer_size( NULL, 2 /* two channels */,
//                                                                    inst->_src_audio_frame->nb_samples,
//                                                                    AV_SAMPLE_FMT_S16, 0);
                    
                    
                    DTsize old_num_samples = inst->_dst_sound_packet.getNumSamples();
                    
                    // Append new samples
                    inst->_dst_sound_packet.setNumSamples(old_num_samples + inst->_src_audio_frame->nb_samples);
                    
                    const uint8_t **in_buf = (const uint8_t**) inst->_src_audio_frame->extended_data;
                    uint8_t* out_buf[SWR_CH_MAX] = { (uint8_t*) inst->_dst_sound_packet.getBuffer() + old_num_samples*4, NULL};  // 4 bytes per sample

                    DTint num_samples = ::swr_convert(	inst->_snd_convert_ctx,
                                                        out_buf,
                                                        inst->_src_audio_frame->nb_samples,
                                                        in_buf,
                                                        inst->_src_audio_frame->nb_samples);
                    
                    // Readjust num samples
                    inst->_dst_sound_packet.setNumSamples(old_num_samples + num_samples);
                    
                    // Queue up dound data
                    if (inst->_dst_sound_packet.getNumSamples() > AUDIO_BUFFER_SIZE ) {
                        inst->_sound_source.pushPacket(inst->_dst_sound_packet);
                        inst->_dst_sound_packet.setNumBytes(0);
                    }
                    
                }
            
                ::av_free_packet(&packet);
            }

        } else if (inst->_state == STATE_CLOSING) {
            // Dont suspend
        } else {
            HAL::suspendThread(inst->_playback_thread);
        }
        
        
        // Figure out if we can sleep
        DTfloat video_delay = inst->_video_timestamp - inst->_current_time;
        DTfloat audio_delay = inst->_audio_timestamp - (inst->_current_time + AUDIO_PRE_BUFFER_THRESH);
        DTfloat delay = min2(video_delay, audio_delay);
        if (delay > 0.0) {
            HAL::sleepThread(delay * 1000);
        }
        
    }
    
}

//==============================================================================
//==============================================================================

void HWVideoPlayerFFInstance::play (void)
{
    if (_state != STATE_PAUSED && _state != STATE_PLAYING)
        return;

    _data_command_queue.pushPlay();
    HAL::resumeThread(_data_thread);    // Make sure thread is going
}

void HWVideoPlayerFFInstance::pause (void)
{
    if (_state != STATE_PAUSED && _state != STATE_PLAYING)
        return;

    _data_command_queue.pushPause();
    HAL::resumeThread(_data_thread);    // Make sure thread is going
}

void HWVideoPlayerFFInstance::seek (DTdouble t)
{
    if (_state != STATE_PAUSED && _state != STATE_PLAYING)
        return;

    _data_command_queue.pushSeek(t);
    HAL::resumeThread(_data_thread);    // Make sure thread is going
}

//==============================================================================
//==============================================================================

void HWVideoPlayerFFInstance::close (void)
{
    if (_data_source) {
        AVCodecContext *audio_codec_context = _data_source->audioCodecContext();
        if (audio_codec_context)
            System::getAudioRenderer()->stop(&_sound_source);
    }

    // Set state to closing
    _state = STATE_CLOSING;
    
    if (_playback_thread) {
        HAL::resumeThread(_playback_thread);
        HAL::joinThread(_playback_thread);
        _playback_thread = NULL;
    }
    
    if (_data_thread) {
        HAL::resumeThread(_data_thread);
        HAL::joinThread(_data_thread);
        _data_thread = NULL;
    }

    // Free scaler context
    if (_img_convert_ctx)   {   ::sws_freeContext(_img_convert_ctx);    _img_convert_ctx = NULL;    }
    if (_snd_convert_ctx)   {   ::swr_free(&_snd_convert_ctx);          _snd_convert_ctx = NULL;    }

    // Free the frames
    ::av_frame_free(&_src_frame);
    ::av_frame_free(&_dst_frame);
    ::av_frame_free(&_src_audio_frame);
    

    RELEASE(_data_source);
    
    _state = STATE_IDLE;
}

//==============================================================================
//==============================================================================

} // DT3

