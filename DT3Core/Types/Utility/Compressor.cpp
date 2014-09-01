//==============================================================================
///	
///	File: Compressor.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/Compressor.hpp"
#include "DT3Core/Types/Utility/Error.hpp"

extern "C" {
    #include "zlib.h"
}

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

DTsize Compressor::deflate (    DTubyte *src_data, DTsize src_data_size,
                                DTubyte *dst_data, DTsize dst_data_size )
{
    DTint ret;
    z_stream strm;
    
    // Allocate deflate state
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    ret = ::deflateInit(&strm, Z_BEST_COMPRESSION);
    if (ret != Z_OK)
        return ret;

    // Compress until end of file
    strm.avail_in = (uInt) src_data_size;
    strm.next_in = src_data;

    strm.avail_out = (uInt) dst_data_size;
    strm.next_out = dst_data;
    
    ret = ::deflate(&strm, Z_FINISH);    // no bad return value
    
    ERROR(ret != Z_STREAM_ERROR, "deflate returned Z_STREAM_ERROR");  // state not clobbered
    
    ::deflateEnd(&strm);

    if (ret == Z_STREAM_END)
        return 0;   // Output buffer not large enough

    // clean up and return
    return (dst_data_size - strm.avail_out);    // Calculate actual size of deflated data
}

//==============================================================================
//==============================================================================

DTsize Compressor::inflate (    DTubyte *src_data, DTsize src_data_size,
                                DTubyte *dst_data, DTsize dst_data_size )
{
    DTint ret;
    z_stream strm;

    // allocate inflate state
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    ret = ::inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    // decompress until deflate stream ends or end of file
    strm.avail_in = (uInt) src_data_size;
    strm.next_in = src_data;

    strm.avail_out = (uInt) dst_data_size;
    strm.next_out = dst_data;
    
    ret = ::inflate(&strm, Z_NO_FLUSH);
    
    ERROR(ret != Z_STREAM_ERROR, "inflate returned Z_STREAM_ERROR");  // state not clobbered
    
    // clean up and return
    ::inflateEnd(&strm);

    switch (ret) {
        case Z_NEED_DICT:
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            return 0;
    }
            
    return (dst_data_size - strm.avail_out);    // Calculate actual size of inflated data
}

//==============================================================================
//==============================================================================

} // DT3
