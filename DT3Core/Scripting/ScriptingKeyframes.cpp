//==============================================================================
///	
///	File: ScriptingKeyframes.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingKeyframes.hpp"
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

//IMPLEMENT_FACTORY_CREATION(ScriptingKeyframes)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingKeyframes::ScriptingKeyframes (void)
{  

}
		
ScriptingKeyframes::ScriptingKeyframes (const ScriptingKeyframes &rhs)
    :   ScriptingBase			(rhs)
{   

}

ScriptingKeyframes & ScriptingKeyframes::operator = (const ScriptingKeyframes &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);		
	}
    return (*this);
}
			
ScriptingKeyframes::~ScriptingKeyframes (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingKeyframes::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());														     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingKeyframes::initialize (void)
{
	ScriptingBase::initialize();
    set_node_color(Color4f(0.4F,0.4F,0.4F,1.0F));
}

//==============================================================================
//==============================================================================

DTint ScriptingKeyframes::key_index (DTint id) const
{
    DTint n = (DTint) num_keys();
    for (DTint i = 0; i < n; ++i) {
    
        if (key_id(i) == id)
            return i;
    }
    
    return -1;
}

//==============================================================================
//==============================================================================

} // DT3

