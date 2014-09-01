//==============================================================================
///	
///	File: ImporterKeyedSpriteKSPR.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Sprites/Resources/Importers/ImporterKeyedSpriteKSPR.hpp"
#include "DT3Sprites/Resources/ResourceTypes/KeyedSpriteResource.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/Utility/CheckedCast.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_IMPORTER(ImporterKeyedSpriteKSPR,kspr)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ImporterKeyedSpriteKSPR::ImporterKeyedSpriteKSPR (void)
{    
	
}
			
ImporterKeyedSpriteKSPR::~ImporterKeyedSpriteKSPR (void)
{ 

}

//==============================================================================
//==============================================================================

DTerr ImporterKeyedSpriteKSPR::import(KeyedSpriteResource *target, std::string args)
{
    // Convert path to this platform
	FilePath pathname(target->path());
    
    std::shared_ptr<BaseClass> = Factory::create_object_from_stream(pathname);
    if (sprite && sprite->is_a(KeyedSpriteResource::kind()))
        target->copy(*checked_static_cast<KeyedSpriteResource*>(sprite));

	target->set_streamable(true);
	
	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

} // DT3

