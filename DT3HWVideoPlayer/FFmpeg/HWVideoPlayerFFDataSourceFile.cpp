//==============================================================================
///	
///	File: 			HWVideoPlayerFFDataSourceFile.cpp
///	Author:			Tod Baudais
///					Copyright (C) 2000-2007. All rights reserved.
///	
///	Date Created:	2/12/2013
///	Changes:		-none-
///	
//==============================================================================

#include "HWVideoPlayerFFDataSourceFile.hpp"

#include "FileManager.hpp"
#include "MoreMath.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

const DTsize BUFFER_SIZE = 1024*64;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

HWVideoPlayerFFDataSourceFile::HWVideoPlayerFFDataSourceFile (const FilePath &path)
    :   _path                   (path)
{

}

HWVideoPlayerFFDataSourceFile::~HWVideoPlayerFFDataSourceFile (void)
{
    close();
}

//==============================================================================
//==============================================================================

int HWVideoPlayerFFDataSourceFile::readFunc(void *opaque, uint8_t *buf, int buf_size)
{
    BinaryFileStream *file = (BinaryFileStream*) opaque;
    
    DTsize nbytes = file->readRaw((DTubyte*) buf, buf_size);
    if (file->isEOF())  return 0;
    
    return (int) nbytes;
}

int64_t HWVideoPlayerFFDataSourceFile::seekFunc(void *opaque, int64_t offset, int whence)
{
    BinaryFileStream *file = (BinaryFileStream*) opaque;

    switch (whence) {
        case SEEK_SET:  {
            file->seekG(offset, Stream::FROM_BEGINNING);
            DTsize g = file->getG();
            return g;
        }
        case SEEK_CUR:  {
            file->seekG(offset, Stream::FROM_CURRENT);
            DTsize g = file->getG();
            return g;
        }
        case SEEK_END:  {
            file->seekG(offset, Stream::FROM_END);
            DTsize g = file->getG();
            return g;
        }
        case AVSEEK_SIZE: {
            return file->getLength();
        }
    }
    
    return -1;
}

//==============================================================================
//==============================================================================

DTerr HWVideoPlayerFFDataSourceFile::open   (void)
{
    //
	// Open the file
    //
    
	DTerr err;
	if ((err = FileManager::open(_file, _path, true)) != DT3_ERR_NONE) {
        close();
        return DT3_ERR_FILE_OPEN_FAILED;
    }
    
#if 1
    _buffer = (DTubyte*) ::av_malloc(BUFFER_SIZE + FF_INPUT_BUFFER_PADDING_SIZE);

    _av_io_context = ::avio_alloc_context(_buffer, BUFFER_SIZE, 1, &_file, readFunc, NULL, seekFunc);
    _av_io_context->seekable = 1;
    _av_io_context->write_flag = 0;
    
    // Allocate Format Context
    _format_context = ::avformat_alloc_context();
    _format_context->pb = _av_io_context;
    _format_context->flags = AVFMT_FLAG_CUSTOM_IO;
    
    // We must manually determine file type
    // Determining the input format:
    DTsize nbytes = 0;
    
    // Read a chunk of data
    nbytes = _file.readRaw(_buffer, min2(BUFFER_SIZE, _file.getLength()) );
    _file.seekG(0, Stream::FROM_BEGINNING);

    // Now we set the ProbeData-structure for av_probe_input_format:
    AVProbeData probeData;
    probeData.buf = _buffer;
    probeData.buf_size = (DTint) nbytes;
    probeData.filename = "";
     
    // Determine the input-format:
    _format_context->iformat = ::av_probe_input_format(&probeData, 1);
#endif

    // Open the input
    err = ::avformat_open_input(&_format_context, _path.getPath().cStr(), NULL, NULL);
    if (err < 0) {
        close();
        return DT3_ERR_FILE_OPEN_FAILED;
    }
    
    // Ok we should now be able to read from our archive
    
    // Retrieve stream information
    if(::avformat_find_stream_info(_format_context, NULL) < 0) {
        close();
        return DT3_ERR_FILE_OPEN_FAILED;
    }
    
    // Display information
    ::av_dump_format(_format_context,0,"",false);
    
    // Stream indices
    _video_stream_index = -1;
    _audio_stream_index = -1;

    // Get the streams
    _video_codec_context = NULL;
    _audio_codec_context = NULL;

    for(DTint i=0; i < _format_context->nb_streams; ++i) {
    
        if(_format_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO && _video_stream_index < 0) {
            _video_codec_context = _format_context->streams[i]->codec;
            _video_stream_index = i;
        }

        if(_format_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO && _audio_stream_index < 0) {
            _audio_codec_context = _format_context->streams[i]->codec;
            _audio_stream_index = i;
        }
        
    }
    
    // Validate. If no video then we assume a problem.
    if (!_video_codec_context) {
        close();
        return DT3_ERR_FILE_OPEN_FAILED;
    }
    
    // Get the codecs
    // Find the decoder for the video stream
    
    if (_video_codec_context) {
        AVCodec *video_codec = ::avcodec_find_decoder(_video_codec_context->codec_id);
        if(video_codec) {
        
            if (::avcodec_open2(_video_codec_context, video_codec, NULL) < 0) {
                _video_codec_context = NULL;
                close();
                return DT3_ERR_FILE_OPEN_FAILED;
            }
            
        } else {
            _video_codec_context = NULL;
            close();
            return DT3_ERR_FILE_OPEN_FAILED;
        }
    }

    if (_audio_codec_context) {
        AVCodec *audio_codec = ::avcodec_find_decoder(_audio_codec_context->codec_id);
        if(audio_codec) {
        
            if (::avcodec_open2(_audio_codec_context, audio_codec, NULL) < 0) {
                _audio_codec_context = NULL;
                close();
                return DT3_ERR_FILE_OPEN_FAILED;
            }

        } else {
            _audio_codec_context = NULL;
            close();
            return DT3_ERR_FILE_OPEN_FAILED;
        }
    }
    
    return DT3_ERR_NONE;
}
    
void HWVideoPlayerFFDataSourceFile::close   (void)
{
    if (_video_codec_context)   {   ::avcodec_close(_video_codec_context);      _video_codec_context = NULL;    }
    if (_audio_codec_context)   {   ::avcodec_close(_audio_codec_context);      _audio_codec_context = NULL;    }
    
    if (_format_context)        {   ::avformat_free_context(_format_context);   _format_context = NULL;         }
    if (_av_io_context)         {   ::av_free(_av_io_context);                  _av_io_context = NULL;          }
}

//==============================================================================
//==============================================================================

} // DT3

