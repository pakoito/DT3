//==============================================================================
///	
///	File: ImporterFontTTF.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/Importers/ImporterFontTTF.hpp"
#include "DT3Core/Resources/ResourceTypes/FontResource.hpp"
#include "DT3Core/Types/FileBuffer/BinaryFileStream.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/FileManager.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_IMPORTER(ImporterFontTTF,ttf)
IMPLEMENT_FACTORY_IMPORTER(ImporterFontTTF,ttc)
IMPLEMENT_FACTORY_IMPORTER(ImporterFontTTF,otf)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ImporterFontTTF::ImporterFontTTF (void)
{    
	
}
			
ImporterFontTTF::~ImporterFontTTF (void)
{ 

}

//==============================================================================
//==============================================================================

unsigned long ImporterFontTTF::ft_io_func   (   FT_Stream       stream,
                                                unsigned long   offset,
                                                unsigned char*  buffer,
                                                unsigned long   count)
{
	BinaryFileStream *read_ptr = reinterpret_cast<BinaryFileStream*>(stream->descriptor.pointer);
    read_ptr->seek_g(offset, Stream::FROM_BEGINNING);
    
    if (count == 0)
        return 0;
	return (unsigned long) read_ptr->read_raw(buffer,count);
}

void ImporterFontTTF::ft_close_func(   FT_Stream    stream)
{
	BinaryFileStream *read_ptr = reinterpret_cast<BinaryFileStream*>(stream->descriptor.pointer);
    delete read_ptr;
}

//==============================================================================
//==============================================================================

DTerr ImporterFontTTF::import(FontResource *target, std::string args)
{
    // Open up the stream for the font file
	BinaryFileStream *file = new BinaryFileStream();    // This pointer has to go through a C-API so no shared_ptr
	DTerr err = FileManager::open(*file, target->path(), true);
    if (err != DT3_ERR_NONE) {
        LOG_MESSAGE << "Unable to open font " << target->path().full_path();
        delete file;
        return DT3_ERR_FILE_OPEN_FAILED;
    }
    
    // Send the stream to freetype
    FT_Open_Args ftargs;

    ::memset((void *)&ftargs,0,sizeof(ftargs));
    
    ftargs.flags = FT_OPEN_STREAM;
    ftargs.stream=(FT_Stream)malloc(sizeof *(ftargs.stream));
    
    ftargs.stream->base = NULL;
    ftargs.stream->size = static_cast<DTuint>(file->length());
    ftargs.stream->pos = 0;
    ftargs.stream->descriptor.pointer = (void*) (file);
    ftargs.stream->pathname.pointer = NULL;
    ftargs.stream->read = &ImporterFontTTF::ft_io_func;
    ftargs.stream->close = &ImporterFontTTF::ft_close_func;

	FT_Error error = ::FT_Open_Face(FontResource::library(),
                                    &ftargs,
                                    0,
                                    &(target->typeface()));
    
    ::FT_Select_Charmap(target->typeface(),FT_ENCODING_UNICODE);
    
	return error == 0 ? DT3_ERR_NONE : DT3_ERR_FILE_OPEN_FAILED;
}

//==============================================================================
//==============================================================================

} // DT3

