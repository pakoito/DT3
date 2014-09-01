#ifndef DT3_BINARYSTREAM
#define DT3_BINARYSTREAM
//==============================================================================
///	
///	File: BinaryStream.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class BinaryStream: public Stream {
    public:
        DEFINE_TYPE(BinaryStream,Stream)
		//DEFINE_CREATE_AND_CLONE
         
                                    BinaryStream		(void);
	private:
                                    BinaryStream		(const BinaryStream &rhs);
        BinaryStream &				operator =			(const BinaryStream &rhs);
    public:
        virtual						~BinaryStream		(void);
};

//==============================================================================
//==============================================================================


} // DT3

#endif
