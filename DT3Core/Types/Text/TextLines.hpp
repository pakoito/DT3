#ifndef DT3_TEXTLINES
#define DT3_TEXTLINES
//==============================================================================
///	
///	File: TextLines.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Text/TextCharacter.hpp"
#include <list>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class GlyphCache;
class FontResource;

//==============================================================================
//==============================================================================

class TextLine {
    public:
        DEFINE_TYPE_SIMPLE_BASE(TextLine)   

                                            TextLine        (void);
                                            TextLine        (const TextLine &rhs);
        TextLine &                          operator =      (const TextLine &rhs);
                                            ~TextLine       (void);
      
    public:
        /// List of glyphs on the line
		/// \return glyphs
        const std::list<TextCharacter>&     glyphs          (void) const  {   return _glyphs; }
        
        /// Direction of line (i.e. left-right or up-down)
		/// \return Line direction
        TextCharacter::Direction            direction       (void)  const {   return _glyphs.size() > 0 ? _glyphs.front().direction() : TextCharacter::DIR_HORZ_LEFT_TO_RIGHT; }

        /// Justification of line
		/// \return Justification
        TextCharacter::Justification        justification   (void)  const {   return _glyphs.size() > 0 ? _glyphs.front().justification() : TextCharacter::JUSTIFY_LEFT; }

        /// Description
		/// \param param description
		/// \return description
        DTboolean                           is_blank        (void) const;
        
        /// Extents of line
		DEFINE_ACCESSORS (extents_offset, set_extents_offset, DTfloat, _extents_offset);
		DEFINE_ACCESSORS (extents_perp_offset, set_extents_perp_offset, DTfloat, _extents_perp_offset);

		DEFINE_ACCESSORS (auto_split, set_auto_split, DTboolean, _auto_split);

    private:
        friend class TextLines;
    
        std::list<TextCharacter>    _glyphs;
        DTfloat                     _extents_offset;
        DTfloat                     _extents_perp_offset;
        DTboolean                   _auto_split;
};

//==============================================================================
//==============================================================================

class TextLines: public BaseClass {
    public:
        DEFINE_TYPE(TextLines, BaseClass)   
	
                                        TextLines			(void);
                                        TextLines			(const TextLines &rhs);
        TextLines &                     operator =			(const TextLines &rhs);
                                        ~TextLines			(void);
                                
    public:
                                           
        /// Add text to the lines
		/// \param c Glyph Cache
		/// \param transform Text transform
		/// \param scale Text scale
		/// \param resolution_scale Resolution independent scale factor
		/// \param text_raw Raw text
        void                            add_line            (   GlyphCache &c,
                                                                const Matrix4 &transform,
                                                                const Vector3 &scale,
                                                                DTfloat resolution_scale,
                                                                const std::string &text_raw);

        /// Add text to the lines
		/// \param c Glyph Cache
		/// \param transform Text transform
		/// \param scale Text scale
		/// \param resolution_scale Resolution independent scale factor
		/// \param text_raw Raw text
		/// \param font Font to render
		/// \param font_size Font size
		/// \param color Font color
		/// \param justification Font justification
        void                            add_line            (   GlyphCache &c,
                                                                const Matrix4 &transform,
                                                                const Vector3 &scale,
                                                                DTfloat resolution_scale,
                                                                const std::string &text_raw,
                                                                const std::shared_ptr<FontResource> &font,
                                                                DTuint font_size,
                                                                const Color4f &color,
                                                                TextCharacter::Justification justification);

        /// Clear the lines of text
        void                            clear               (void);


        /// Wrap the lines of text to the text box
		/// \param width width of text box
		/// \param height height of text box
        enum Alignment {
            ALIGN_TOP,
            ALIGN_MIDDLE,
            ALIGN_BOTTOM
        };
        
        void                            wrap                (DTfloat width, DTfloat height, Alignment alignment);
           
        /// Wrap and paginate the lines of text to the text box
		/// \param width width of text box
		/// \param height height of text box
        /// \return pages of text
        std::list<TextLines>            paginate            (DTfloat width, DTfloat height);
           
           
        /// Extent of the line of text
		/// \return extent
        DTfloat                         extents             (void) const;

        /// Perpendicular Extent of the line of text
		/// \return extent
        DTfloat                         extents_perp        (void) const;
        
        
        /// Lines of text
		/// \return lines of text
        const std::list<TextLine>&      lines               (void) const    {   return _lines;  }

    public:
        DTfloat                         line_extents        (const TextLine &line) const; // Width in Horizontal text
        DTfloat                         line_extents_perp   (const TextLine &line) const; // Height in Horizontal text
        DTfloat                         line_baseline_shift (const TextLine &line) const;

        std::list<TextLine>             _lines;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

