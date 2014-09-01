//==============================================================================
///	
///	File: ImporterImagePVR.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/Importers/ImporterImagePVR.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResource2D.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResource3D.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResourceCube.hpp"
#include "DT3Core/Types/FileBuffer/BinaryFileStream.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/FileManager.hpp"

#if DT3_OS == DT3_IOS

#define PVR_TEXTURE_FLAG_TYPE_MASK    0xff
static char gPVRTexIdentifier[5] = "PVR!";

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_IMPORTER(ImporterImagePVR,pvr)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ImporterImagePVR::ImporterImagePVR (void)
{    

}
		
ImporterImagePVR::~ImporterImagePVR (void)
{ 

}

//==============================================================================
//==============================================================================

DTerr ImporterImagePVR::import (TextureResource2D *target, std::string args)
{
    // Convert path to this platform
	FilePath pathname(target->path());
    
    DTuint                      width;
    DTuint                      height;
    std::shared_ptr<DTubyte>    data;
    DT3GLTextelFormat           format;

    import(pathname, args, width, height, data, format);
    
    DTboolean mipmapped = (MoreStrings::lowercase(args).find("mipmapped") != std::string::npos);

    target->set_textels(width, height, data, format, mipmapped);

	return DT3_ERR_NONE;
}

DTerr ImporterImagePVR::import (TextureResource3D *target, std::string args)
{
    // Convert path to this platform
	FilePath pathname(target->path());
    
    DTuint                      width;
    DTuint                      height;
    std::shared_ptr<DTubyte>    data;
    DT3GLTextelFormat           format;

    import(pathname, args, width, height, data, format);
    
    // TODO: Set textels

	return DT3_ERR_NONE;
}

DTerr ImporterImagePVR::import (TextureResourceCube *target, std::string args)
{
    // Convert path to this platform
	FilePath pathname(target->path());
    
    DTuint                      width;
    DTuint                      height;
    std::shared_ptr<DTubyte>    data;
    DT3GLTextelFormat           format;

    import(pathname, args, width, height, data, format);

    // TODO: Set textels

	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================
	
DTerr ImporterImagePVR::import(const FilePath &pathname, const std::string &args, DTuint &width, DTuint &height, std::shared_ptr<DTubyte> &data, DT3GLTextelFormat &format)
{
	// open the file
	BinaryFileStream file;
	DTerr err;
	if ((err = FileManager::open(file, pathname, true)) != DT3_ERR_NONE)
		return err;
	
	// read the header
	PVRTexHeader header;
	file.read_raw((DTubyte*) &header, sizeof(header));
	
    DTuint pvrTag = header.pvrTag;

    if (gPVRTexIdentifier[0] != ((pvrTag >>  0) & 0xff) ||
        gPVRTexIdentifier[1] != ((pvrTag >>  8) & 0xff) ||
        gPVRTexIdentifier[2] != ((pvrTag >> 16) & 0xff) ||
        gPVRTexIdentifier[3] != ((pvrTag >> 24) & 0xff))
    {
        return DT3_ERR_FILE_WRONG_TYPE;
    }
	
	DTuint flags = header.flags;
	DTuint formatFlags = flags & PVR_TEXTURE_FLAG_TYPE_MASK;
	
	ASSERT (formatFlags == kPVRTextureFlagTypePVR_4 || formatFlags == kPVRTextureFlagTypePVR_2);
	
	// allocate a buffer
    DTuint blockSize = 0, widthBlocks = 0, heightBlocks = 0;
    width = header.width;
    height = header.height;
    DTuint bpp = 4;

	if (formatFlags == kPVRTextureFlagTypePVR_4)
	{
		blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
		widthBlocks = width / 4;
		heightBlocks = height / 4;
		bpp = 4;
	} else {
		blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
		widthBlocks = width / 8;
		heightBlocks = height / 4;
		bpp = 2;
	}
	
	// Clamp to minimum number of blocks
	if (widthBlocks < 2)
		widthBlocks = 2;
	if (heightBlocks < 2)
		heightBlocks = 2;

	DTuint size = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);
    
    std::shared_ptr<DTuint> buffer = std::shared_ptr<DTuint>(new DTuint[size]);

	// Read the entire file
	file.read_raw((DTubyte*) buffer.get(), size);
    
	if (header.bitmaskAlpha) {
        if (formatFlags == kPVRTextureFlagTypePVR_4)		format = DT3GL_FORMAT_PVR4_RGBA;
		else												format = DT3GL_FORMAT_PVR2_RGBA;
	} else {
		if (formatFlags == kPVRTextureFlagTypePVR_4)		format = DT3GL_FORMAT_PVR4_RGB;
		else												format = DT3GL_FORMAT_PVR4_RGB;
	}


	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

} // DT3

#endif //iPhone only
