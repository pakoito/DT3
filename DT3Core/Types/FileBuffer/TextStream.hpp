#ifndef DT3_TEXTSTREAM
#define DT3_TEXTSTREAM
//==============================================================================
///	
///	File: TextStream.hpp
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

class TextStream: public Stream {
    public:
        DEFINE_TYPE(TextStream,Stream)
		//DEFINE_CREATE_AND_CLONE
         
                                    TextStream		(void);
	private:
                                    TextStream		(const TextStream &rhs);
        TextStream &				operator =		(const TextStream &rhs);
    public:
        virtual						~TextStream		(void);
};

//==============================================================================
//==============================================================================


} // DT3

#endif
