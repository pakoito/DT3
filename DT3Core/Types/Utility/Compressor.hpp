#ifndef DT3_COMPRESSOR
#define DT3_COMPRESSOR
//==============================================================================
///	
///	File: Compressor.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class Compressor {
    private:	
		inline					Compressor			(void)  {}
								Compressor			(const Compressor &rhs);
        Compressor &			operator =			(const Compressor &rhs);		
								~Compressor			(void);
    
    public:	
            
        /// Deflate compressed data stream
		/// \param src_data compressed data
		/// \param src_data_size compressed data size
		/// \param dst_data uncompressed data
		/// \param dst_data_size uncompressed data size
		/// \return size of compressed data or zero if not enough space in destination buffer
        static DTsize           deflate             (   DTubyte *src_data, DTsize src_data_size,
                                                        DTubyte *dst_data, DTsize dst_data_size );

        /// inflate compressed data stream
		/// \param src_data uncompressed data
		/// \param src_data_size uncompressed data size
		/// \param dst_data compressed data
		/// \param dst_data_size compressed data size
		/// \return size of decompressed data or zero if not enough space in destination buffer
        static DTsize           inflate             (   DTubyte *src_data, DTsize src_data_size,
                                                        DTubyte *dst_data, DTsize dst_data_size );
        
};

//==============================================================================
//==============================================================================

} // DT3

#endif

