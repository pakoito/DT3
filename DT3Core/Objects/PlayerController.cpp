//==============================================================================
///	
///	File: PlayerController.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/PlayerController.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(PlayerController)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

PlayerController::PlayerController (void)
{
	set_streamable(false);
}

PlayerController::PlayerController (const PlayerController &rhs)
    :   ObjectBase(rhs)
{   

}

PlayerController & PlayerController::operator = (const PlayerController &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
		ObjectBase::operator = (rhs);
    }
    return (*this);
}
			
PlayerController::~PlayerController (void)
{

}

//==============================================================================
//==============================================================================

void PlayerController::archive (const std::shared_ptr<Archive> &archive)
{
    ObjectBase::archive(archive);

    archive->push_domain (class_id ());
	archive->pop_domain ();
}

//==============================================================================
//==============================================================================



} // DT3

