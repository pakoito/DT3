//==============================================================================
///	
///	File: GlyphCache.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Text/GlyphCache.hpp"
#include "DT3Core/Types/Text/GlyphCacheEntry.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

GlyphCache::GlyphCache (void)
{
    _hash_table.rehash(1021);
}

GlyphCache::GlyphCache (const GlyphCache &rhs)
    :   _hash_table (rhs._hash_table)
{

}

GlyphCache::GlyphCache (GlyphCache &&rhs)
    :   _hash_table (std::move(rhs._hash_table))
{

}
    
GlyphCache& GlyphCache::operator = (const GlyphCache &rhs)
{
    _hash_table = rhs._hash_table;
    return (*this);
}	

GlyphCache& GlyphCache::operator = (GlyphCache &&rhs)
{	
    _hash_table = std::move(rhs._hash_table);
    return (*this);
}	

GlyphCache::~GlyphCache (void)
{

}

//==============================================================================
//==============================================================================

std::shared_ptr<GlyphCacheEntry>& GlyphCache::entry (GlyphCacheEntry &s)
{
    DTuint h = s.hash();
    
    auto i = _hash_table.find(h);
    if (i != _hash_table.end()) {
        if (s != *(i->second) )
            LOG_ERROR << "Glyph Cache Collision!!!!";
        return i->second;
    }
        
    _hash_table[h] = std::shared_ptr<GlyphCacheEntry>(new GlyphCacheEntry(s));
    
    i = _hash_table.find(h);
    if (s != *(i->second) )
        LOG_ERROR << "Glyph Cache Collision!!!!";
    
    return i->second;
}

void GlyphCache::clear (void)
{
    _hash_table.clear();
}

//==============================================================================
//==============================================================================

} // DT3
