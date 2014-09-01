//==============================================================================
///	
///	File: ScriptingMatrix3ToComponents.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingMatrix3ToComponents.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingMatrix3ToComponents,"Conversion",NULL)
IMPLEMENT_PLUG_NODE(ScriptingMatrix3ToComponents)

IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out_x_axis)
IMPLEMENT_PLUG_INFO_INDEX(_out_y_axis)
IMPLEMENT_PLUG_INFO_INDEX(_out_z_axis)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingMatrix3ToComponents)

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_x_axis))
		.affects(PLUG_INFO_INDEX(_out_y_axis))
		.affects(PLUG_INFO_INDEX(_out_z_axis));

	PLUG_INIT(_out_x_axis,"Out_X_Axis")
		.set_output(true);
		
	PLUG_INIT(_out_y_axis,"Out_Y_Axis")
		.set_output(true);
		
	PLUG_INIT(_out_z_axis,"Out_Z_Axis")
		.set_output(true);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingMatrix3ToComponents::ScriptingMatrix3ToComponents (void)
    :   _in				(PLUG_INFO_INDEX(_in), Matrix3(   1.0F,0.0F,0.0F,
                                                    0.0F,1.0F,0.0F,
                                                    0.0F,0.0F,1.0F)),
		_out_x_axis		(PLUG_INFO_INDEX(_out_x_axis), Vector3(1.0F,0.0F,0.0F)),
		_out_y_axis		(PLUG_INFO_INDEX(_out_y_axis), Vector3(0.0F,1.0F,0.0F)),
		_out_z_axis		(PLUG_INFO_INDEX(_out_z_axis), Vector3(0.0F,0.0F,1.0F))
{  

}
		
ScriptingMatrix3ToComponents::ScriptingMatrix3ToComponents (const ScriptingMatrix3ToComponents &rhs)
    :   ScriptingBase	(rhs),
		_in				(rhs._in),
		_out_x_axis		(rhs._out_x_axis),
		_out_y_axis		(rhs._out_y_axis),
		_out_z_axis		(rhs._out_z_axis)
{   

}

ScriptingMatrix3ToComponents & ScriptingMatrix3ToComponents::operator = (const ScriptingMatrix3ToComponents &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in = rhs._in;
		_out_x_axis = rhs._out_x_axis;
		_out_y_axis = rhs._out_y_axis;
		_out_z_axis = rhs._out_z_axis;
	}
    return (*this);
}
			
ScriptingMatrix3ToComponents::~ScriptingMatrix3ToComponents (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingMatrix3ToComponents::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingMatrix3ToComponents::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());

	*archive << ARCHIVE_PLUG(_in, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out_x_axis, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out_y_axis, DATA_PERSISTENT);
	*archive << ARCHIVE_PLUG(_out_z_axis, DATA_PERSISTENT);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingMatrix3ToComponents::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out_x_axis || plug == &_out_y_axis || plug == &_out_z_axis) {
		_out_x_axis = _in->x_axis();
		_out_y_axis = _in->y_axis();
		_out_z_axis = _in->z_axis();
		_out_x_axis.set_clean();
		_out_y_axis.set_clean();
		_out_z_axis.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3

