//==============================================================================
///	
///	File: 			HWVideoPlayerFFDataSourceBase.cpp
///	Author:			Tod Baudais
///					Copyright (C) 2000-2007. All rights reserved.
///	
///	Date Created:	2/12/2013
///	Changes:		-none-
///	
//==============================================================================

#include "HWVideoPlayerFFDataSourceBase.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

HWVideoPlayerFFDataSourceBase::HWVideoPlayerFFDataSourceBase (void)
    :   _buffer                 (NULL),
        _av_io_context          (NULL),
        _format_context         (NULL),
        _video_stream_index     (-1),
        _audio_stream_index     (-1),
        _video_codec_context    (NULL),
        _audio_codec_context    (NULL)
{

}

HWVideoPlayerFFDataSourceBase::~HWVideoPlayerFFDataSourceBase (void)
{

}

//==============================================================================
//==============================================================================

} // DT3

