#ifndef DT3_GLYPHCACHEENTRY
#define DT3_GLYPHCACHEENTRY
//==============================================================================
///	
///	File: GlyphCacheEntry.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Resources/ResourceTypes/FontResource.hpp"
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class GlyphCacheEntry: public BaseClass {
    public:
        DEFINE_TYPE(GlyphCacheEntry, BaseClass)   
	
                                                GlyphCacheEntry         (void);
                                                GlyphCacheEntry         (const GlyphCacheEntry &rhs);
        GlyphCacheEntry &                       operator =              (const GlyphCacheEntry &rhs);
                                                ~GlyphCacheEntry        (void);
                                
    public:
    
		DTboolean                               operator ==             (const GlyphCacheEntry& rhs) const;
		DTboolean                               operator !=             (const GlyphCacheEntry& rhs) const;

        /// Check to see if the Glyph is part of the same family
		/// \param rhs other glyph
		/// \return Is the same
        DTboolean                               is_same_family          (const std::shared_ptr<GlyphCacheEntry> &rhs);
    
        /// Returns a hash of the glyph entry for faster searching and comparison
		/// \return Hash
        DTuint                                  hash                    (void) const;
            

		/// Set the character
		/// \param c character
        void                                    set_character           (DTuint c);

		/// Returns the character
		/// \return character
        DTuint                                  character               (void) const                {   return _c;   }


		/// Set the font
		/// \param font font
        void                                    set_font                (const std::shared_ptr<FontResource> &font);

		/// Returns the font
		/// \return font
        const std::shared_ptr<FontResource>&    font                    (void) const                {   return _font;   }


		/// Set the size
		/// \param font size
        void                                    set_size                (DTuint size);

		/// Returns the size
		/// \return size
        DTuint                                  size                    (void) const                {   return _size;   }


		/// Set the resolution scale
		/// \param resolution resolution scale
        void                                    set_resolution_scale    (DTfloat resolution);

		/// Returns the resolution scale
		/// \return resolution scale
        DTfloat                                 resolution_scale        (void) const                {   return _resolution_scale;   }
    
    
		/// Set the resolution transform
		/// \param transform transform
        void                                    set_transform           (const Matrix4 &transform);
        
		/// Returns the transform
		/// \return transform
        const Matrix4 &                         transform               (void) const                {   return _transform;      }


		/// Set the scale
		/// \param scale scale
        void                                    set_scale               (const Vector3 &scale);
        
		/// Returns the scale
		/// \return scale
        const Vector3 &                         scale                   (void) const                {   return _scale;      }


    
		/// Return the X advance
		/// \return X advance
        DTfloat                                 advance_X               (void) const                {   glyph();    return _advance_x;  }
    
		/// Return the Y advance
		/// \return Y advance
        DTfloat                                 advance_Y               (void) const                {   glyph();    return _advance_y;  }

        /// Returns the freetype glyph
		/// \return freetype glyphs
        FT_Glyph                                glyph                   (void) const;
        
                        

    private:
        void                                    clear_glyph             (void);
    
        mutable DTuint                          _hash;
        DTuint                                  _c;
        
        std::shared_ptr<FontResource>           _font;
        DTuint                                  _size;
        DTfloat                                 _resolution_scale;

        Matrix4                                 _transform;
        Vector3                                 _scale;
                
        mutable DTfloat                         _advance_x;
        mutable DTfloat                         _advance_y;
        
        mutable FT_Glyph                        _glyph;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

