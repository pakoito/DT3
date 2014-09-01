#ifndef DT3_GLYPHCACHE
#define DT3_GLYPHCACHE
//==============================================================================
///	
///	File: GlyphCache.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Text/GlyphCache.hpp"
#include <memory>
#include <unordered_map>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class GlyphCacheEntry;

//==============================================================================
/// Class
//==============================================================================

class GlyphCache {
    public:
        DEFINE_TYPE_SIMPLE_BASE(GlyphCache)   
	
								GlyphCache			(void);	
								GlyphCache			(const GlyphCache &rhs);
								GlyphCache			(GlyphCache &&rhs);
        GlyphCache &            operator =			(const GlyphCache &rhs);
        GlyphCache &            operator =			(GlyphCache &&rhs);
								~GlyphCache			(void);
                                
    public:
    
		/// Retrieve a cache entry from the Glyph Cache
		/// \param s Glyph cache entry
		/// \return Cached version of glyph entry
        std::shared_ptr<GlyphCacheEntry>&   entry   (GlyphCacheEntry &s);
        
		/// Clear Glyph Cache
        void                                clear   (void);
        
     private:
        std::unordered_map<DTuint, std::shared_ptr<GlyphCacheEntry>>   _hash_table;
};

//==============================================================================
//==============================================================================

} // DT3

#endif

