//==============================================================================
///	
///	File: FontResource.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/ResourceTypes/FontResource.hpp"
#include "DT3Core/Resources/Importers/ImporterFont.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/FileBuffer/Stream.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include "DT3Core/System/Factory.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_CREATION(FontResource)

//==============================================================================
//==============================================================================

GLOBAL_STATIC_INITIALIZATION(0,FontResource::initialize_static())
GLOBAL_STATIC_DESTRUCTION(0,FontResource::uninitialize_static())

//==============================================================================
//==============================================================================

std::mutex                                              FontResource::_font_map_lock;
std::map<std::string, std::shared_ptr<FontResource>>   FontResource::_font_map;

FT_Library      FontResource::_library = NULL;
FT_MemoryRec_   FontResource::_memory;

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

FontResource::FontResource (void)
    :	_typeface   (NULL)
{

}

FontResource::~FontResource (void)
{
    FT_Done_Face(_typeface);
}

//==============================================================================
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<FontResource> &r)
{
	if (r) {
		s << r->property_path();
	} else {
		s << "";
	}
	return s;
}

Stream& operator >>(Stream &s, std::shared_ptr<FontResource> &r)
{
	std::string path;
	s >> path;
	
	r = FontResource::import_resource(FilePath(path));
	return s;
}

//==============================================================================
//==============================================================================

void FontResource::reload_if_changed (void)
{
    FOR_EACH (i,_font_map) {
        if (i->second->is_changed()) {
            LOG_MESSAGE << "Reloading : " << i->second->property_path();
            i->second->reload();
        }
    }
}

std::shared_ptr<FontResource> FontResource::import_resource (const FilePath &pathname, std::string args)
{    
    if (!pathname.exists() && !pathname.in_package()) {
        return NULL;
    }

    std::unique_lock<std::mutex> lock(_font_map_lock);
    
    auto i = _font_map.find(pathname.full_path());
    
    // If there is no resource loaded from that path
    if (i == _font_map.end()) {
                        
        std::shared_ptr<FontResource> res = FontResource::create();
        DTerr err;
        if ((err = res->import(pathname,args)) != DT3_ERR_NONE) {
            return NULL;
        }
                    
        _font_map[pathname.full_path()] = res;
        return res;
    }
    
    return i->second;
}

//==============================================================================
//==============================================================================

void FontResource::initialize_static (void)
{
    SystemCallbacks::reload_resources_cb().add(make_callback(&type::reload_if_changed));
}

void FontResource::uninitialize_static (void)
{
    SystemCallbacks::reload_resources_cb().remove(make_callback(&type::reload_if_changed));
}

//==============================================================================
//==============================================================================

DTerr FontResource::import (const FilePath &pathname, std::string args)
{
	DTerr err;
	if ((err = Resource::import (pathname,args)) != DT3_ERR_NONE)
		return err;
			
	FilePath original_path(path());
	std::string extension = original_path.file_ext();
	
	// Build a generic importer for the file type
	std::shared_ptr<BaseClass> generic_importer = Factory::create_importer(extension);
	if (!generic_importer || !generic_importer->isa(ImporterFont::kind())) {
		return DT3_ERR_FILE_WRONG_TYPE;
	}

	// Attempt to convert the importer of a font importer
	std::shared_ptr<ImporterFont> font_importer = checked_static_cast<ImporterFont>(generic_importer);
    err = font_importer->import(this, args);
	
	return err;
}

//==============================================================================
//==============================================================================

FT_Face& FontResource::typeface (void)
{
    return _typeface;
}

//==============================================================================
// Freetype memory allocators
//==============================================================================

void* DT_FT_Alloc_Func(FT_Memory memory, long size)
{
    DTubyte *block = new ("Freetype") DTubyte[size];
    return block;
}
                    
void DT_FT_Free_Func(FT_Memory memory, void* block)
{
    delete (DTubyte*) block;
}

void* DT_FT_Realloc_Func(FT_Memory memory, long cur_size, long new_size, void* block)
{
    if (new_size == 0) {
        DT_FT_Free_Func(memory, block);
        
        return NULL;
    } else {
        void *new_block = DT_FT_Alloc_Func(memory, new_size);
        ::memcpy(new_block, block, MoreMath::min(cur_size,new_size));
        DT_FT_Free_Func(memory, block);
        
        return new_block;
    }
}

//==============================================================================
//==============================================================================

FT_Library FontResource::library (void)
{
    if (!_library) {
        _memory.user = NULL;
        _memory.alloc = &DT_FT_Alloc_Func;
        _memory.free = &DT_FT_Free_Func;
        _memory.realloc = &DT_FT_Realloc_Func;
    
        FT_New_Library(&_memory, &_library);
        FT_Add_Default_Modules(_library);

        // Or we could just use this
        //FT_Init_FreeType(&_library);
    }  
    
    return _library;
}

//==============================================================================
//==============================================================================

} // DT3
