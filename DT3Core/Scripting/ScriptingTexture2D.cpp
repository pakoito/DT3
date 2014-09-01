//==============================================================================
///	
///	File: ScriptingTexture2D.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingTexture2D.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResource2D.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingTexture2D,"Materials",NULL)
IMPLEMENT_PLUG_NODE(ScriptingTexture2D)

IMPLEMENT_PLUG_INFO_INDEX(_out_texture)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingTexture2D)

	PLUG_INIT(_out_texture,"Out_Texture")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingTexture2D::ScriptingTexture2D (void)
    :   _texture		(NULL),
		_out_texture	(PLUG_INFO_INDEX(_out_texture))
{  

}
		
ScriptingTexture2D::ScriptingTexture2D (const ScriptingTexture2D &rhs)
    :   ScriptingBase	(rhs),
		_texture		(rhs._texture),
		_out_texture	(rhs._out_texture)
{   

}

ScriptingTexture2D & ScriptingTexture2D::operator = (const ScriptingTexture2D &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_texture = rhs._texture;
		_out_texture = rhs._out_texture;
	}
    return (*this);
}
			
ScriptingTexture2D::~ScriptingTexture2D (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingTexture2D::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingTexture2D::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	        
	*archive << ARCHIVE_DATA_ACCESSORS("Texture", ScriptingTexture2D::texture_property, ScriptingTexture2D::set_texture_property, DATA_PERSISTENT | DATA_SETTABLE);
													
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingTexture2D::set_texture_property (const std::shared_ptr<TextureResource2D> &texture)
{
	_texture = texture;
	_out_texture.set_dirty();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingTexture2D::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out_texture) {
		_out_texture = _texture;
		_out_texture.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3

