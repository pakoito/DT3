//==============================================================================
///	
///	File: ScriptingSlerp.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSlerp.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Math/Quaternion.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSlerp,"Math",NULL)
IMPLEMENT_PLUG_NODE(ScriptingSlerp)

IMPLEMENT_PLUG_INFO_INDEX(_in_orientation_0);
IMPLEMENT_PLUG_INFO_INDEX(_in_translation_0);
IMPLEMENT_PLUG_INFO_INDEX(_in_scale_0);

IMPLEMENT_PLUG_INFO_INDEX(_in_orientation_1);
IMPLEMENT_PLUG_INFO_INDEX(_in_translation_1);
IMPLEMENT_PLUG_INFO_INDEX(_in_scale_1);

IMPLEMENT_PLUG_INFO_INDEX(_t);

IMPLEMENT_PLUG_INFO_INDEX(_out_orientation);
IMPLEMENT_PLUG_INFO_INDEX(_out_translation);
IMPLEMENT_PLUG_INFO_INDEX(_out_scale);

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSlerp)

	PLUG_INIT(_in_orientation_0,"In_Orientation_0")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_orientation))
		.affects(PLUG_INFO_INDEX(_out_translation))
		.affects(PLUG_INFO_INDEX(_out_scale));

	PLUG_INIT(_in_translation_0,"In_Translation_0")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_orientation))
		.affects(PLUG_INFO_INDEX(_out_translation))
		.affects(PLUG_INFO_INDEX(_out_scale));

	PLUG_INIT(_in_scale_0,"In_Scale_0")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_orientation))
		.affects(PLUG_INFO_INDEX(_out_translation))
		.affects(PLUG_INFO_INDEX(_out_scale));


	PLUG_INIT(_in_orientation_1,"In_Orientation_1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_orientation))
		.affects(PLUG_INFO_INDEX(_out_translation))
		.affects(PLUG_INFO_INDEX(_out_scale));

	PLUG_INIT(_in_translation_1,"In_Translation_1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_orientation))
		.affects(PLUG_INFO_INDEX(_out_translation))
		.affects(PLUG_INFO_INDEX(_out_scale));

	PLUG_INIT(_in_scale_1,"In_Scale_1")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_orientation))
		.affects(PLUG_INFO_INDEX(_out_translation))
		.affects(PLUG_INFO_INDEX(_out_scale));


	PLUG_INIT(_t,"t")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out_orientation))
		.affects(PLUG_INFO_INDEX(_out_translation))
		.affects(PLUG_INFO_INDEX(_out_scale));
        
        
	PLUG_INIT(_out_orientation,"Out_Orientation")
		.set_output(true);
        
	PLUG_INIT(_out_translation,"Out_Translation")
		.set_output(true);

	PLUG_INIT(_out_scale,"Out_Scale")
		.set_output(true);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingSlerp::ScriptingSlerp (void)
    :   _in_orientation_0	(PLUG_INFO_INDEX(_in_orientation_0),    Matrix3::identity()),
		_in_translation_0	(PLUG_INFO_INDEX(_in_translation_0),    Vector3(0.0F,0.0F,0.0F)),
		_in_scale_0			(PLUG_INFO_INDEX(_in_scale_0),          Vector3(1.0F,1.0F,1.0F)),
        
        _in_orientation_1	(PLUG_INFO_INDEX(_in_orientation_1),    Matrix3::identity()),
		_in_translation_1	(PLUG_INFO_INDEX(_in_translation_1),    Vector3(0.0F,0.0F,0.0F)),
		_in_scale_1			(PLUG_INFO_INDEX(_in_scale_1),          Vector3(1.0F,1.0F,1.0F)),
        
        _out_orientation	(PLUG_INFO_INDEX(_out_orientation),     Matrix3::identity()),
		_out_translation	(PLUG_INFO_INDEX(_out_translation),     Vector3(0.0F,0.0F,0.0F)),
		_out_scale			(PLUG_INFO_INDEX(_out_scale),           Vector3(1.0F,1.0F,1.0F)),
        
		_t                  (PLUG_INFO_INDEX(_t), 0.0F)
{  

}
		
ScriptingSlerp::ScriptingSlerp (const ScriptingSlerp &rhs)
    :   ScriptingBase           (rhs),
		_in_orientation_0		(rhs._in_orientation_0),
		_in_translation_0		(rhs._in_translation_0),
		_in_scale_0             (rhs._in_scale_0),
		_in_orientation_1		(rhs._in_orientation_1),
		_in_translation_1		(rhs._in_translation_1),
		_in_scale_1             (rhs._in_scale_1),
		_out_orientation		(rhs._out_orientation),
		_out_translation		(rhs._out_translation),
		_out_scale              (rhs._out_scale),
		_t                      (rhs._t)
{   

}

ScriptingSlerp & ScriptingSlerp::operator = (const ScriptingSlerp &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

		_in_orientation_0 = rhs._in_orientation_0;
		_in_translation_0 = rhs._in_translation_0;
		_in_scale_0 = rhs._in_scale_0;
		_in_orientation_1 = rhs._in_orientation_1;
		_in_translation_1 = rhs._in_translation_1;
		_in_scale_1 = rhs._in_scale_1;
		_out_orientation = rhs._out_orientation;
		_out_translation = rhs._out_translation;
		_out_scale = rhs._out_scale;
		_t = rhs._t;
	}
    return (*this);
}
			
ScriptingSlerp::~ScriptingSlerp (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSlerp::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingSlerp::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
    
	*archive << ARCHIVE_PLUG(_in_orientation_0, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_translation_0, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_scale_0, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_in_orientation_1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_translation_1, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_in_scale_1, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_t, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_out_orientation, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out_translation, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_PLUG(_out_scale, DATA_PERSISTENT | DATA_SETTABLE);
    														     					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingSlerp::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out_scale) {
		
        // lerp Scale
        _out_scale = _in_scale_0 + (_in_scale_1 - _in_scale_0) * _t;
		_out_scale.set_clean();
		return true;
        
	} else if (plug == &_out_translation) {
		
        // lerp translation
        _out_translation = _in_translation_0 + (_in_translation_1 - _in_translation_0) * _t;
		_out_translation.set_clean();
		return true;
        
	} else if (plug == &_out_orientation) {
        
        Quaternion q0(_in_orientation_0);
        Quaternion q1(_in_orientation_1);
        Quaternion r = Quaternion::lerp(q0, q1, _t);
        
        _out_orientation = Matrix3(r);
		_out_orientation.set_clean();
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3

