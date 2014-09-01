//==============================================================================
///	
///	File: GameController.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/GameController.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(GameController)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

GameController::GameController (void)
{  

}
	
GameController::GameController (const GameController &rhs)
    :   ObjectBase(rhs)
{

}

GameController & GameController::operator = (const GameController &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
		ObjectBase::operator = (rhs);
    }
    return (*this);
}
		
GameController::~GameController (void)
{

}

//==============================================================================
//==============================================================================

void GameController::archive (const std::shared_ptr<Archive> &archive)
{
    ObjectBase::archive(archive);

    archive->push_domain (class_id ());
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void GameController::add_to_world(World *world)
{
    ObjectBase::add_to_world(world);
}

void GameController::remove_from_world (void)
{
    ObjectBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3

