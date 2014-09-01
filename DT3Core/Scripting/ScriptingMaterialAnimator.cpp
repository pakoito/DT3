//==============================================================================
///	
///	File: ScriptingMaterialAnimator.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingMaterialAnimator.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/World/World.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingMaterialAnimator,"Materials",NULL)
IMPLEMENT_PLUG_NODE(ScriptingMaterialAnimator)

IMPLEMENT_PLUG_INFO_INDEX(_active)
IMPLEMENT_PLUG_INFO_INDEX(_period)
IMPLEMENT_PLUG_INFO_INDEX(_out)
		
//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingMaterialAnimator)

	PLUG_INIT(_active,"Active")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_period,"Period")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingMaterialAnimator::ScriptingMaterialAnimator (void)
    :   _active			(PLUG_INFO_INDEX(_active), true),
        _period			(PLUG_INFO_INDEX(_period), 0.01F),
        _out_time       (0.0F),
		_out			(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingMaterialAnimator::ScriptingMaterialAnimator (const ScriptingMaterialAnimator &rhs)
    :   ScriptingBase	(rhs),
		_active			(rhs._active),
		_period         (rhs._period),
        _out_time       (rhs._out_time),
		_out			(rhs._out),
        _materials      (rhs._materials)
{   

}

ScriptingMaterialAnimator & ScriptingMaterialAnimator::operator = (const ScriptingMaterialAnimator &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_active = rhs._active;
		_period = rhs._period;
        _out_time = rhs._out_time;
		_out = rhs._out;
        _materials = rhs._materials;
	}
    return (*this);
}
			
ScriptingMaterialAnimator::~ScriptingMaterialAnimator (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingMaterialAnimator::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingMaterialAnimator::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
    
    *archive << ARCHIVE_PLUG(_active, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_period, DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_DATA_ACCESSORS("Num_Materials", ScriptingMaterialAnimator::num_materials, ScriptingMaterialAnimator::set_num_materials, DATA_PERSISTENT | DATA_SETTABLE | DATA_FLUSH_UI);
    
    for (std::size_t i = 0; i < _materials.size(); ++i) {
        *archive << ARCHIVE_DATA_NAMED("Material_" + MoreStrings::cast_to_string(static_cast<DTsize>(i)), _materials[i], DATA_PERSISTENT | DATA_SETTABLE);
    }

    *archive << ARCHIVE_PLUG(_out, DATA_PERSISTENT);
    
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingMaterialAnimator::tick (const DTfloat dt)
{
	PROFILER(SCRIPTING);

    _out_time += dt;
    _out.set_dirty();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingMaterialAnimator::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
    
        if (_materials.size() > 0) {
            DTsize index = (DTsize)(_out_time / _period) % _materials.size();
            _out = _materials[index];
        }
        
		_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

void ScriptingMaterialAnimator::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingMaterialAnimator::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3

