//==============================================================================
///	
///	File: ScriptingSoundBase.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSoundBase.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

//IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSoundBase,"Resources",NULL)
IMPLEMENT_PLUG_NODE(ScriptingSoundBase)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSoundBase)
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingSoundBase::ScriptingSoundBase (void)
{  

}
		
ScriptingSoundBase::ScriptingSoundBase (const ScriptingSoundBase &rhs)
    :   ScriptingBase	(rhs)
{   

}	

ScriptingSoundBase & ScriptingSoundBase::operator = (const ScriptingSoundBase &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);
	}
    return (*this);
}
			
ScriptingSoundBase::~ScriptingSoundBase (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSoundBase::initialize (void)
{
	ScriptingBase::initialize();
    set_node_color(Color4f(0.360784F,0.729412F,0.92549F,1.0F));
}

//==============================================================================
//==============================================================================

void ScriptingSoundBase::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());													
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

} // DT3

