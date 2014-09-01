#ifndef DT3_BITFONT
#define DT3_BITFONT
//==============================================================================
///	
///	File: BitFont.hpp
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

class BitFont {
 	private:
                                    BitFont					(void);	
                                    BitFont					(const BitFont &rhs);
        BitFont &                   operator =              (const BitFont &rhs);
		virtual                     ~BitFont                (void);

	public:

		/// Returns an 8x8 bitmap for the character that is passed in.
		/// \param c character to get bitmap for
		/// \return 8 bytes of bitmap data
        static const DTubyte*       character_bitmap        (DTcharacter c)   {   return bit_font_data[c];    }

	private:
        static const DTubyte bit_font_data[128][8];

};

//==============================================================================
//==============================================================================

} // DT3

#endif
