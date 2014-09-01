//==============================================================================
///	
///	File: ScriptingParticleEmitter.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleEmitter.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Graphics/Particles.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(ScriptingParticleEmitter)
IMPLEMENT_PLUG_NODE(ScriptingParticleEmitter)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleEmitter)

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingParticleEmitter::ScriptingParticleEmitter (void)
{  

}
		
ScriptingParticleEmitter::ScriptingParticleEmitter (const ScriptingParticleEmitter &rhs)
    :   ScriptingBase	(rhs)
{   

}

ScriptingParticleEmitter & ScriptingParticleEmitter::operator = (const ScriptingParticleEmitter &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);
	}
    return (*this);
}
			
ScriptingParticleEmitter::~ScriptingParticleEmitter (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleEmitter::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

} // DT3

