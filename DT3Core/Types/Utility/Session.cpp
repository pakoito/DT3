//==============================================================================
///	
///	File: Session.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/Session.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/System/Factory.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(Session)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Session::Session (void)
{

}

Session::Session (const Session& rhs)
{

}

Session & Session::operator = (const Session& rhs)
{
	BaseClass::operator = (rhs);
    return (*this);
}

Session::~Session (void)
{

}

//==============================================================================
//==============================================================================

void Session::archive (const std::shared_ptr<Archive> &archive)
{
    BaseClass::archive(archive);

    archive->push_domain (class_id ());
		
	archive->pop_domain ();
}

//==============================================================================
//==============================================================================

} // DT3
