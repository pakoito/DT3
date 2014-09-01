#ifndef DT3_TEXTRENDERER
#define DT3_TEXTRENDERER
//==============================================================================
///	
///	File: TextRenderer.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Text/TextLines.hpp"
#include "DT3Core/Types/Math/Rectangle.hpp"
#include "DT3Core/Types/Math/Color4f.hpp"
#include <memory>

extern "C" {
    #include "ft2build.h"
    #include "freetype/freetype.h"
    #include "freetype/ftmodapi.h"
    #include "freetype/ftglyph.h"
    #include "freetype/ftoutln.h"
    #include "freetype/ftstroke.h"
}

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class TextureResource2D;

//==============================================================================
//==============================================================================

class TextRenderer {
	private:
								TextRenderer				(void);	
								TextRenderer				(const TextRenderer &rhs);
		TextRenderer &			operator =                  (const TextRenderer &rhs);		
								~TextRenderer				(void);
	
	public:
                                                                        
		/// Renders the text to a texture
		/// \param destination destination texture
		/// \param lines lines to render
		/// \param offset offset for rendering
		/// \return rectangle rendered to
        static Rectangle        render_lines                (   std::shared_ptr<TextureResource2D>  &destination,
                                                                TextLines                           &lines,
                                                                const Vector2                       &offset = Vector2(0.0F,0.0F));

		/// Begin rendering to a texture. Clears it to transparent
		/// \param destination destination texture
		static void             begin_render                (std::shared_ptr<TextureResource2D> &destination);

		/// End rendering to a texture
		/// \param destination destination texture
		static void             end_render                  (std::shared_ptr<TextureResource2D> &destination);

    private:
    
        struct RasterInfo {
            TextureResource2D   *_destination;
            FT_Vector           _offset;
            Color4f             _color;
            DTboolean           _alpha_blend;
        };

        static void             raster_callback             (   const int               y,
                                                                const int               count,
                                                                const FT_Span * const   spans,
                                                                void * const            user);


};

//==============================================================================
//==============================================================================


} // DT3

#endif
