//==============================================================================
///	
///	File: ScriptingBase.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

//IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingBase,"Logic",NULL)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingBase)

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingBase::ScriptingBase (void)
{  
    
}
		
ScriptingBase::ScriptingBase (const ScriptingBase &rhs)
    :   WorldNode		(rhs)
{   

}

ScriptingBase & ScriptingBase::operator = (const ScriptingBase &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		WorldNode::operator = (rhs);
	}
    return (*this);
}
			
ScriptingBase::~ScriptingBase (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingBase::archive (const std::shared_ptr<Archive> &archive)
{
    WorldNode::archive(archive);

	archive->push_domain (class_id ());
    
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

std::string ScriptingBase::preferred_name (void) const
{
    std::string name(class_id_child());
    
    name = MoreStrings::find_and_replace(name, "Scripting", "");
    name = MoreStrings::find_and_replace(name, "Keyframes", "Key");
    
    return name;
}

//==============================================================================
//==============================================================================

} // DT3

