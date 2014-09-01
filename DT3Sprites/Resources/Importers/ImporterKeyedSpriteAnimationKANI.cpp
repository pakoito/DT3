//==============================================================================
///	
///	File: ImporterKeyedSpriteAnimationKANI.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Sprites/Resources/Importers/ImporterKeyedSpriteAnimationKANI.hpp"
#include "DT3Sprites/Resources/ResourceTypes/KeyedSpriteAnimationResource.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/Utility/CheckedCast.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_IMPORTER(ImporterKeyedSpriteAnimationKANI,kani)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ImporterKeyedSpriteAnimationKANI::ImporterKeyedSpriteAnimationKANI (void)
{    
	
}
			
ImporterKeyedSpriteAnimationKANI::~ImporterKeyedSpriteAnimationKANI (void)
{ 

}

//==============================================================================
//==============================================================================

DTerr ImporterKeyedSpriteAnimationKANI::import(KeyedSpriteAnimationResource *target, std::string args)
{
    // Convert path to this platform
	FilePath pathname(target->path());
    
    std::shared_ptr<BaseClass> animation = Factory::create_object_from_stream(pathname);
    if (animation && animation->is_a(KeyedSpriteAnimationResource::kind()))
        target->copy(*checked_static_cast<KeyedSpriteAnimationResource*>(animation));

	target->set_streamable(true);
	
	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

} // DT3

