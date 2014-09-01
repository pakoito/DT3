#ifndef DT3_TEXTCHARACTER
#define DT3_TEXTCHARACTER
//==============================================================================
///	
///	File: TextCharacter.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"
#include "DT3Core/Types/Math/Color4f.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class GlyphCacheEntry;

//==============================================================================
//==============================================================================

class TextCharacter {
    public:	
								TextCharacter			(void);	
								TextCharacter			(const TextCharacter &rhs);
        TextCharacter &         operator =              (const TextCharacter &rhs);
                                ~TextCharacter          (void);
                                
    public:
         
        enum Justification {
            JUSTIFY_LEFT = 0,
            JUSTIFY_RIGHT = 1,
            JUSTIFY_CENTER = 2,
            JUSTIFY_FULL = 3
        };

		/// Set the tab size
		/// \param size tab size
        void                    set_tab_size            (DTuint size);

		/// Returns the tab size
		/// \return tab size
        DTuint                  tab_size                (void) const                    {   return _tab_size;   }
    
		/// Set the baseline shift
		/// \param bl baseline shift
        void                    set_baseline_shift      (DTuint bl);

		/// Returns the baseline shift
		/// \return baseline shift
        DTfloat                 baseline_shift          (void) const                    {   return _baseline;   }
    
		/// Set the kerning
		/// \param k kerning
        void                    set_kern                (DTfloat k);

		/// Returns the kerning
		/// \return kerning
        DTfloat                 kern                    (void) const                    {   return _kerning;   }

        enum Wrap {
            WRAP_WORD,
            WRAP_CHARACTER,
            WRAP_NONE
        };

		/// Sets word or character wrapping
		/// \param word_wrap true for word wrap, false for character wrap
        void                    set_word_wrap           (Wrap word_wrap);

		/// Returns wehter word wrap is enabled
		/// \return word wrap enabled
        Wrap                    word_wrap               (void) const                    {   return _word_wrap;   }
    
		/// Sets the justification
		/// \param justification justification
        void                    set_justification       (Justification justification)   {   _justification = justification;    }

		/// Returns the justification
		/// \return justification
        Justification           justification           (void) const                    {   return _justification;   }
    
		/// Set the text direction
		/// \param direction text direction
        enum Direction {
            DIR_HORZ_LEFT_TO_RIGHT = 0,
            DIR_HORZ_RIGHT_TO_LEFT = 1,
            DIR_VERT_LEFT_TO_RIGHT = 2,
            DIR_VERT_RIGHT_TO_LEFT = 3
        };
        void                    set_direction           (Direction direction);
    
		/// Returns the text direction
		/// \return text direction
        Direction               direction               (void) const                    {   return _direction;          }


		/// Sets the color of the text
		/// \param color text color
        void                    set_color               (const Color4f &color)          {   _color = color;     }

		/// Returns the color of the text
		/// \return text color
        const Color4f&          color                   (void) const                    {   return _color;      }


		/// Sets the color of the outline
		/// \param color text color
        void                    set_outline_color       (const Color4f &outline_color)  {   _outline_color = outline_color;     }

		/// Returns the color of the outline
		/// \return outline color
        const Color4f&          outline_color           (void) const                    {   return _outline_color;      }


		/// Sets the size of the outline
		/// \param size outline size
        void                    set_outline_size        (const DTfloat outline_size)    {   _outline_size = outline_size;     }

		/// Returns the size of the outline
		/// \return outline size
        DTfloat                 outline_size            (void) const                    {   return _outline_size;      }


        
		/// Set the glyph for the character
		/// \param glyph glyph for the character
        void                                    set_glyph_entry (std::shared_ptr<GlyphCacheEntry> &glyph);

		/// Returns the glyph for the character
		/// \return character glyph
        const std::shared_ptr<GlyphCacheEntry>& glyph_entry     (void) const                {   return _glyph;   }


  		DEFINE_ACCESSORS		(advance_X, set_advance_X, DTfloat, _advance_x);
  		DEFINE_ACCESSORS		(advance_Y, set_advance_Y, DTfloat, _advance_y);
        
		/// The extent of the character. For left-right fonts this is width
		/// \return extent
        DTfloat                 extent                  (void) const;

		/// The extent of the character. For left-right fonts this is height between lines
		/// \return extent
        DTfloat                 extent_perp             (void) const;

    private:            
        Justification                       _justification;
        DTuint                              _tab_size;
        DTfloat                             _baseline;
        DTfloat                             _kerning;
        Color4f                             _color;
        Color4f                             _outline_color;
        DTfloat                             _outline_size;
        DTfloat                             _advance_x;
        DTfloat                             _advance_y;
        Direction                           _direction;
        Wrap                                _word_wrap;

        std::shared_ptr<GlyphCacheEntry>    _glyph;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

