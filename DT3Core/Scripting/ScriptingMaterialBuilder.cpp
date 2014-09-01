//==============================================================================
///	
///	File: ScriptingMaterialBuilder.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingMaterialBuilder.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResource2D.hpp"
#include "DT3Core/Resources/ResourceTypes/ShaderResource.hpp"
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

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingMaterialBuilder,"Materials",NULL)
IMPLEMENT_PLUG_NODE(ScriptingMaterialBuilder)

IMPLEMENT_PLUG_INFO_INDEX(_out_material)

IMPLEMENT_PLUG_INFO_INDEX(_tex_0)
IMPLEMENT_PLUG_INFO_INDEX(_tex_1)
IMPLEMENT_PLUG_INFO_INDEX(_tex_2)
IMPLEMENT_PLUG_INFO_INDEX(_tex_3)
IMPLEMENT_PLUG_INFO_INDEX(_tex_4)
IMPLEMENT_PLUG_INFO_INDEX(_tex_5)
IMPLEMENT_PLUG_INFO_INDEX(_tex_6)
IMPLEMENT_PLUG_INFO_INDEX(_tex_7)
IMPLEMENT_PLUG_INFO_INDEX(_tex_8)
IMPLEMENT_PLUG_INFO_INDEX(_tex_9)
IMPLEMENT_PLUG_INFO_INDEX(_tex_10)
IMPLEMENT_PLUG_INFO_INDEX(_tex_11)
IMPLEMENT_PLUG_INFO_INDEX(_tex_12)
IMPLEMENT_PLUG_INFO_INDEX(_tex_13)
IMPLEMENT_PLUG_INFO_INDEX(_tex_14)
IMPLEMENT_PLUG_INFO_INDEX(_tex_15)

IMPLEMENT_PLUG_INFO_INDEX(_rect_0)
IMPLEMENT_PLUG_INFO_INDEX(_rect_1)
IMPLEMENT_PLUG_INFO_INDEX(_rect_2)
IMPLEMENT_PLUG_INFO_INDEX(_rect_3)
IMPLEMENT_PLUG_INFO_INDEX(_rect_4)
IMPLEMENT_PLUG_INFO_INDEX(_rect_5)
IMPLEMENT_PLUG_INFO_INDEX(_rect_6)
IMPLEMENT_PLUG_INFO_INDEX(_rect_7)
IMPLEMENT_PLUG_INFO_INDEX(_rect_8)
IMPLEMENT_PLUG_INFO_INDEX(_rect_9)
IMPLEMENT_PLUG_INFO_INDEX(_rect_10)
IMPLEMENT_PLUG_INFO_INDEX(_rect_11)
IMPLEMENT_PLUG_INFO_INDEX(_rect_12)
IMPLEMENT_PLUG_INFO_INDEX(_rect_13)
IMPLEMENT_PLUG_INFO_INDEX(_rect_14)
IMPLEMENT_PLUG_INFO_INDEX(_rect_15)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingMaterialBuilder)

	PLUG_INIT(_out_material,"Out_Material")
		.set_output(true);
        
	PLUG_INIT(_tex_0,"Tex_0")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);
        
	PLUG_INIT(_rect_0,"Rect_0")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);
        
	PLUG_INIT(_tex_1,"Tex_1")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);
        
	PLUG_INIT(_rect_1,"Rect_1")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_tex_2,"Tex_2")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);
        
	PLUG_INIT(_rect_2,"Rect_2")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_tex_3,"Tex_3")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);
        
	PLUG_INIT(_rect_3,"Rect_3")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_tex_4,"Tex_4")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);
        
	PLUG_INIT(_rect_4,"Rect_4")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_tex_5,"Tex_5")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);
        
	PLUG_INIT(_rect_5,"Rect_5")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_tex_6,"Tex_6")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);
        
	PLUG_INIT(_rect_6,"Rect_6")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_tex_7,"Tex_7")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_rect_7,"Rect_7")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_tex_8,"Tex_8")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_rect_8,"Rect_8")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_tex_9,"Tex_9")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_rect_9,"Rect_9")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_tex_10,"Tex_10")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_rect_10,"Rect_10")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_tex_11,"Tex_11")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_rect_11,"Rect_11")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_tex_12,"Tex_12")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_rect_12,"Rect_12")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_tex_13,"Tex_13")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_rect_13,"Rect_13")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_tex_14,"Tex_14")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_rect_14,"Rect_14")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_tex_15,"Tex_15")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);

	PLUG_INIT(_rect_15,"Rect_15")
        .affects(PLUG_INFO_INDEX(_out_material))
		.set_input(true);
    
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingMaterialBuilder::ScriptingMaterialBuilder (void)
    :   _out_material	(PLUG_INFO_INDEX(_out_material)),
        _tex_0          (PLUG_INFO_INDEX(_tex_0)),
        _tex_1          (PLUG_INFO_INDEX(_tex_1)),
        _tex_2          (PLUG_INFO_INDEX(_tex_2)),
        _tex_3          (PLUG_INFO_INDEX(_tex_3)),
        _tex_4          (PLUG_INFO_INDEX(_tex_4)),
        _tex_5          (PLUG_INFO_INDEX(_tex_5)),
        _tex_6          (PLUG_INFO_INDEX(_tex_6)),
        _tex_7          (PLUG_INFO_INDEX(_tex_7)),
        _tex_8          (PLUG_INFO_INDEX(_tex_8)),
        _tex_9          (PLUG_INFO_INDEX(_tex_9)),
        _tex_10         (PLUG_INFO_INDEX(_tex_10)),
        _tex_11         (PLUG_INFO_INDEX(_tex_11)),
        _tex_12         (PLUG_INFO_INDEX(_tex_12)),
        _tex_13         (PLUG_INFO_INDEX(_tex_13)),
        _tex_14         (PLUG_INFO_INDEX(_tex_14)),
        _tex_15         (PLUG_INFO_INDEX(_tex_15)),
        _rect_0         (PLUG_INFO_INDEX(_rect_0), Rectangle(0.0F,1.0F,0.0F,1.0F)),
        _rect_1         (PLUG_INFO_INDEX(_rect_1), Rectangle(0.0F,1.0F,0.0F,1.0F)),
        _rect_2         (PLUG_INFO_INDEX(_rect_2), Rectangle(0.0F,1.0F,0.0F,1.0F)),
        _rect_3         (PLUG_INFO_INDEX(_rect_3), Rectangle(0.0F,1.0F,0.0F,1.0F)),
        _rect_4         (PLUG_INFO_INDEX(_rect_4), Rectangle(0.0F,1.0F,0.0F,1.0F)),
        _rect_5         (PLUG_INFO_INDEX(_rect_5), Rectangle(0.0F,1.0F,0.0F,1.0F)),
        _rect_6         (PLUG_INFO_INDEX(_rect_6), Rectangle(0.0F,1.0F,0.0F,1.0F)),
        _rect_7         (PLUG_INFO_INDEX(_rect_7), Rectangle(0.0F,1.0F,0.0F,1.0F)),
        _rect_8         (PLUG_INFO_INDEX(_rect_8), Rectangle(0.0F,1.0F,0.0F,1.0F)),
        _rect_9         (PLUG_INFO_INDEX(_rect_9), Rectangle(0.0F,1.0F,0.0F,1.0F)),
        _rect_10        (PLUG_INFO_INDEX(_rect_10), Rectangle(0.0F,1.0F,0.0F,1.0F)),
        _rect_11        (PLUG_INFO_INDEX(_rect_11), Rectangle(0.0F,1.0F,0.0F,1.0F)),
        _rect_12        (PLUG_INFO_INDEX(_rect_12), Rectangle(0.0F,1.0F,0.0F,1.0F)),
        _rect_13        (PLUG_INFO_INDEX(_rect_13), Rectangle(0.0F,1.0F,0.0F,1.0F)),
        _rect_14        (PLUG_INFO_INDEX(_rect_14), Rectangle(0.0F,1.0F,0.0F,1.0F)),
        _rect_15        (PLUG_INFO_INDEX(_rect_15), Rectangle(0.0F,1.0F,0.0F,1.0F))
{

}
		
ScriptingMaterialBuilder::ScriptingMaterialBuilder (const ScriptingMaterialBuilder &rhs)
    :   ScriptingBase	(rhs),
		_out_material	(rhs._out_material),
		_tex_0          (rhs._tex_0),
		_tex_1          (rhs._tex_1),
		_tex_2          (rhs._tex_2),
		_tex_3          (rhs._tex_3),
		_tex_4          (rhs._tex_4),
		_tex_5          (rhs._tex_5),
		_tex_6          (rhs._tex_6),
		_tex_7          (rhs._tex_7),
		_tex_8          (rhs._tex_8),
		_tex_9          (rhs._tex_9),
		_tex_10         (rhs._tex_10),
		_tex_11         (rhs._tex_11),
		_tex_12         (rhs._tex_12),
		_tex_13         (rhs._tex_13),
		_tex_14         (rhs._tex_14),
		_tex_15         (rhs._tex_15),
        _rect_0         (rhs._rect_0),
        _rect_1         (rhs._rect_1),
        _rect_2         (rhs._rect_2),
        _rect_3         (rhs._rect_3),
        _rect_4         (rhs._rect_4),
        _rect_5         (rhs._rect_5),
        _rect_6         (rhs._rect_6),
        _rect_7         (rhs._rect_7),
        _rect_8         (rhs._rect_8),
        _rect_9         (rhs._rect_9),
        _rect_10        (rhs._rect_10),
        _rect_11        (rhs._rect_11),
        _rect_12        (rhs._rect_12),
        _rect_13        (rhs._rect_13),
        _rect_14        (rhs._rect_14),
        _rect_15        (rhs._rect_15)
{

}	

ScriptingMaterialBuilder & ScriptingMaterialBuilder::operator = (const ScriptingMaterialBuilder &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);
        
		_tex_0 = rhs._tex_0;
		_tex_1 = rhs._tex_1;
		_tex_2 = rhs._tex_2;
		_tex_3 = rhs._tex_3;
		_tex_4 = rhs._tex_4;
		_tex_5 = rhs._tex_5;
		_tex_6 = rhs._tex_6;
		_tex_7 = rhs._tex_7;
		_tex_8 = rhs._tex_8;
		_tex_9 = rhs._tex_9;
		_tex_10 = rhs._tex_10;
		_tex_11 = rhs._tex_11;
		_tex_12 = rhs._tex_12;
		_tex_13 = rhs._tex_13;
		_tex_14 = rhs._tex_14;
		_tex_15 = rhs._tex_15;
        
		_rect_0 = rhs._rect_0;
		_rect_1 = rhs._rect_1;
		_rect_2 = rhs._rect_2;
		_rect_3 = rhs._rect_3;
		_rect_4 = rhs._rect_4;
		_rect_5 = rhs._rect_5;
		_rect_6 = rhs._rect_6;
		_rect_7 = rhs._rect_7;
		_rect_8 = rhs._rect_8;
		_rect_9 = rhs._rect_9;
		_rect_10 = rhs._rect_10;
		_rect_11 = rhs._rect_11;
		_rect_12 = rhs._rect_12;
		_rect_13 = rhs._rect_13;
		_rect_14 = rhs._rect_14;
		_rect_15 = rhs._rect_15;
	}
    return (*this);
}
			
ScriptingMaterialBuilder::~ScriptingMaterialBuilder (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingMaterialBuilder::initialize (void)
{
	ScriptingBase::initialize();
    
    _out_material = MaterialResource::create();
}

//==============================================================================
//==============================================================================

void ScriptingMaterialBuilder::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

    archive->push_domain (class_id ());
    
    *archive << ARCHIVE_DATA_ACCESSORS("Depth_Enable", ScriptingMaterialBuilder::depth_enable,ScriptingMaterialBuilder::set_depth_enable, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Depth_Write_Mask", ScriptingMaterialBuilder::depth_write_mask,ScriptingMaterialBuilder::set_depth_write_mask, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Depth_Func", ScriptingMaterialBuilder::depth_func,ScriptingMaterialBuilder::set_depth_func, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("DEPTH_ALWAYS")
        .add_enum("DEPTH_NEVER")
        .add_enum("DEPTH_EQUAL")
        .add_enum("DEPTH_NOT_EQUAL")
        .add_enum("DEPTH_GREATER")
        .add_enum("DEPTH_LESS")
        .add_enum("DEPTH_LEQUAL");

    *archive << ARCHIVE_DATA_ACCESSORS("Stencil_Enable", ScriptingMaterialBuilder::stencil_enable,ScriptingMaterialBuilder::set_stencil_enable, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Stencil_Read_Mask", ScriptingMaterialBuilder::stencil_read_mask,ScriptingMaterialBuilder::set_stencil_read_mask, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Stencil_Write_Mask", ScriptingMaterialBuilder::stencil_write_mask,ScriptingMaterialBuilder::set_stencil_write_mask, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Front_Face_Stencil_Fail_Op", ScriptingMaterialBuilder::front_face_stencil_fail_op,ScriptingMaterialBuilder::set_front_face_stencil_fail_op, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("STENCILOP_KEEP")
        .add_enum("STENCILOP_ZERO")
        .add_enum("STENCILOP_REPLACE")
        .add_enum("STENCILOP_INCR")
        .add_enum("STENCILOP_DECR")
        .add_enum("STENCILOP_INCR_WRAP")
        .add_enum("STENCILOP_DECR_WRAP")
        .add_enum("STENCILOP_INVERT");
    
    *archive << ARCHIVE_DATA_ACCESSORS("Front_Face_Stencil_Depth_Fail_Op", ScriptingMaterialBuilder::front_face_stencil_depth_fail_op,ScriptingMaterialBuilder::set_front_face_stencil_depth_fail_op, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("STENCILOP_KEEP")
        .add_enum("STENCILOP_ZERO")
        .add_enum("STENCILOP_REPLACE")
        .add_enum("STENCILOP_INCR")
        .add_enum("STENCILOP_DECR")
        .add_enum("STENCILOP_INCR_WRAP")
        .add_enum("STENCILOP_DECR_WRAP")
        .add_enum("STENCILOP_INVERT");

    *archive << ARCHIVE_DATA_ACCESSORS("Front_Face_Stencil_Pass_Op", ScriptingMaterialBuilder::front_face_stencil_pass_op,ScriptingMaterialBuilder::set_front_face_stencil_pass_op, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("STENCILOP_KEEP")
        .add_enum("STENCILOP_ZERO")
        .add_enum("STENCILOP_REPLACE")
        .add_enum("STENCILOP_INCR")
        .add_enum("STENCILOP_DECR")
        .add_enum("STENCILOP_INCR_WRAP")
        .add_enum("STENCILOP_DECR_WRAP")
        .add_enum("STENCILOP_INVERT");

    *archive << ARCHIVE_DATA_ACCESSORS("Front_Face_Stencil_Func", ScriptingMaterialBuilder::front_face_stencil_func,ScriptingMaterialBuilder::set_front_face_stencil_func, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("COMPARE_ALWAYS")
        .add_enum("COMPARE_NEVER")
        .add_enum("COMPARE_EQUAL")
        .add_enum("COMPARE_NOTEQUAL")
        .add_enum("COMPARE_GREATER")
        .add_enum("COMPARE_GEQUAL")
        .add_enum("COMPARE_LESS")
        .add_enum("COMPARE_LEQUAL");

    *archive << ARCHIVE_DATA_ACCESSORS("Back_Face_Stencil_Fail_Op", ScriptingMaterialBuilder::back_face_stencil_fail_op,ScriptingMaterialBuilder::set_back_face_stencil_fail_op, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("STENCILOP_KEEP")
        .add_enum("STENCILOP_ZERO")
        .add_enum("STENCILOP_REPLACE")
        .add_enum("STENCILOP_INCR")
        .add_enum("STENCILOP_DECR")
        .add_enum("STENCILOP_INCR_WRAP")
        .add_enum("STENCILOP_DECR_WRAP")
        .add_enum("STENCILOP_INVERT");

    *archive << ARCHIVE_DATA_ACCESSORS("Back_Face_Stencil_Depth_Fail_Op", ScriptingMaterialBuilder::back_face_stencil_depth_fail_op,ScriptingMaterialBuilder::set_back_face_stencil_depth_fail_op, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("STENCILOP_KEEP")
        .add_enum("STENCILOP_ZERO")
        .add_enum("STENCILOP_REPLACE")
        .add_enum("STENCILOP_INCR")
        .add_enum("STENCILOP_DECR")
        .add_enum("STENCILOP_INCR_WRAP")
        .add_enum("STENCILOP_DECR_WRAP")
        .add_enum("STENCILOP_INVERT");

    *archive << ARCHIVE_DATA_ACCESSORS("Back_Face_Stencil_Pass_Op", ScriptingMaterialBuilder::back_face_stencil_pass_op,ScriptingMaterialBuilder::set_back_face_stencil_pass_op, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("STENCILOP_KEEP")
        .add_enum("STENCILOP_ZERO")
        .add_enum("STENCILOP_REPLACE")
        .add_enum("STENCILOP_INCR")
        .add_enum("STENCILOP_DECR")
        .add_enum("STENCILOP_INCR_WRAP")
        .add_enum("STENCILOP_DECR_WRAP")
        .add_enum("STENCILOP_INVERT");

    *archive << ARCHIVE_DATA_ACCESSORS("Back_Face_Stencil_Func", ScriptingMaterialBuilder::back_face_stencil_func,ScriptingMaterialBuilder::set_back_face_stencil_func, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("COMPARE_ALWAYS")
        .add_enum("COMPARE_NEVER")
        .add_enum("COMPARE_EQUAL")
        .add_enum("COMPARE_NOTEQUAL")
        .add_enum("COMPARE_GREATER")
        .add_enum("COMPARE_GEQUAL")
        .add_enum("COMPARE_LESS")
        .add_enum("COMPARE_LEQUAL");

    //
    // Blend State
    //

    *archive << ARCHIVE_DATA_ACCESSORS("Blend_Enable", ScriptingMaterialBuilder::blend_enable,ScriptingMaterialBuilder::set_blend_enable, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Src_Blend", ScriptingMaterialBuilder::src_blend,ScriptingMaterialBuilder::set_src_blend, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("BLEND_ZERO")
        .add_enum("BLEND_ONE")
        .add_enum("BLEND_SRC_ALPHA")
        .add_enum("BLEND_ONE_MINUS_SRC_ALPHA")
        .add_enum("BLEND_DST_ALPHA")
        .add_enum("BLEND_ONE_MINUS_DST_ALPHA")
        .add_enum("BLEND_SRC_COLOR")
        .add_enum("BLEND_DST_COLOR")
        .add_enum("BLEND_ONE_MINUS_SRC_COLOR")
        .add_enum("BLEND_ONE_MINUS_DST_COLOR");

    *archive << ARCHIVE_DATA_ACCESSORS("Dest_Blend", ScriptingMaterialBuilder::dest_blend,ScriptingMaterialBuilder::set_dest_blend, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("BLEND_ZERO")
        .add_enum("BLEND_ONE")
        .add_enum("BLEND_SRC_ALPHA")
        .add_enum("BLEND_ONE_MINUS_SRC_ALPHA")
        .add_enum("BLEND_DST_ALPHA")
        .add_enum("BLEND_ONE_MINUS_DST_ALPHA")
        .add_enum("BLEND_SRC_COLOR")
        .add_enum("BLEND_DST_COLOR")
        .add_enum("BLEND_ONE_MINUS_SRC_COLOR")
        .add_enum("BLEND_ONE_MINUS_DST_COLOR");

    *archive << ARCHIVE_DATA_ACCESSORS("Src_Blend_Alpha", ScriptingMaterialBuilder::src_blend_alpha,ScriptingMaterialBuilder::set_src_blend_alpha, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("BLEND_ZERO")
        .add_enum("BLEND_ONE")
        .add_enum("BLEND_SRC_ALPHA")
        .add_enum("BLEND_ONE_MINUS_SRC_ALPHA")
        .add_enum("BLEND_DST_ALPHA")
        .add_enum("BLEND_ONE_MINUS_DST_ALPHA")
        .add_enum("BLEND_SRC_COLOR")
        .add_enum("BLEND_DST_COLOR")
        .add_enum("BLEND_ONE_MINUS_SRC_COLOR")
        .add_enum("BLEND_ONE_MINUS_DST_COLOR");

    *archive << ARCHIVE_DATA_ACCESSORS("Dest_Blend_Alpha", ScriptingMaterialBuilder::dest_blend_alpha,ScriptingMaterialBuilder::set_dest_blend_alpha, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("BLEND_ZERO")
        .add_enum("BLEND_ONE")
        .add_enum("BLEND_SRC_ALPHA")
        .add_enum("BLEND_ONE_MINUS_SRC_ALPHA")
        .add_enum("BLEND_DST_ALPHA")
        .add_enum("BLEND_ONE_MINUS_DST_ALPHA")
        .add_enum("BLEND_SRC_COLOR")
        .add_enum("BLEND_DST_COLOR")
        .add_enum("BLEND_ONE_MINUS_SRC_COLOR")
        .add_enum("BLEND_ONE_MINUS_DST_COLOR");

    *archive << ARCHIVE_DATA_ACCESSORS("Render_Target_Write_Mask_R", ScriptingMaterialBuilder::render_target_write_mask_r,ScriptingMaterialBuilder::set_render_target_write_mask_r, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Render_Target_Write_Mask_G", ScriptingMaterialBuilder::render_target_write_mask_g,ScriptingMaterialBuilder::set_render_target_write_mask_g, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Render_Target_Write_Mask_B", ScriptingMaterialBuilder::render_target_write_mask_b,ScriptingMaterialBuilder::set_render_target_write_mask_b, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Render_Target_Write_Mask_A", ScriptingMaterialBuilder::render_target_write_mask_a,ScriptingMaterialBuilder::set_render_target_write_mask_a, DATA_PERSISTENT | DATA_SETTABLE);

    //
    // Rasterizer State
    //

    *archive << ARCHIVE_DATA_ACCESSORS("Cull_Mode", ScriptingMaterialBuilder::cull_mode,ScriptingMaterialBuilder::set_cull_mode, DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("DT3GL_CULL_FRONT")
        .add_enum("DT3GL_CULL_BACK")
        .add_enum("DT3GL_CULL_NONE");

    *archive << ARCHIVE_DATA_ACCESSORS("Depth_Bias", ScriptingMaterialBuilder::depth_bias,ScriptingMaterialBuilder::set_depth_bias, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Slope_Scaled_Depth_Bias", ScriptingMaterialBuilder::slope_scaled_depth_bias,ScriptingMaterialBuilder::set_slope_scaled_depth_bias, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Depth_Bias_Clamp", ScriptingMaterialBuilder::depth_bias_clamp,ScriptingMaterialBuilder::set_depth_bias_clamp, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Scissor_Enable", ScriptingMaterialBuilder::scissor_enable,ScriptingMaterialBuilder::set_scissor_enable, DATA_PERSISTENT | DATA_SETTABLE);

    //
    // Texture units
    //

    *archive << ARCHIVE_PLUG(_tex_0,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_rect_0,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Filter_0", ScriptingMaterialBuilder::filter_0,ScriptingMaterialBuilder::set_filter_0,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("FILTER_NEAREST")
        .add_enum("FILTER_LINEAR")
        .add_enum("FILTER_LINEAR_MIPMAP_LINEAR");

    *archive << ARCHIVE_DATA_ACCESSORS("Address_U_0", ScriptingMaterialBuilder::address_u_0,ScriptingMaterialBuilder::set_address_u_0,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_V_0", ScriptingMaterialBuilder::address_v_0,ScriptingMaterialBuilder::set_address_v_0,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_W_0", ScriptingMaterialBuilder::address_w_0,ScriptingMaterialBuilder::set_address_w_0,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Mip_LOD_Bias_0", ScriptingMaterialBuilder::mip_lod_bias_0,ScriptingMaterialBuilder::set_mip_lod_bias_0,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Max_Anisotropy_0", ScriptingMaterialBuilder::max_anisotropy_0,ScriptingMaterialBuilder::set_max_anisotropy_0,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Transform_0", ScriptingMaterialBuilder::transform_0,ScriptingMaterialBuilder::set_transform_0,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_tex_1,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_rect_1,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Filter_1", ScriptingMaterialBuilder::filter_1,ScriptingMaterialBuilder::set_filter_1,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("FILTER_NEAREST")
        .add_enum("FILTER_LINEAR")
        .add_enum("FILTER_LINEAR_MIPMAP_LINEAR");

    *archive << ARCHIVE_DATA_ACCESSORS("Address_U_1", ScriptingMaterialBuilder::address_u_1,ScriptingMaterialBuilder::set_address_u_1,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_V_1", ScriptingMaterialBuilder::address_v_1,ScriptingMaterialBuilder::set_address_v_1,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_W_1", ScriptingMaterialBuilder::address_w_1,ScriptingMaterialBuilder::set_address_w_1,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Mip_LOD_Bias_1", ScriptingMaterialBuilder::mip_lod_bias_1,ScriptingMaterialBuilder::set_mip_lod_bias_1,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Max_Anisotropy_1", ScriptingMaterialBuilder::max_anisotropy_1,ScriptingMaterialBuilder::set_max_anisotropy_1,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Transform_1", ScriptingMaterialBuilder::transform_1,ScriptingMaterialBuilder::set_transform_1,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_tex_2,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_rect_2,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Filter_2", ScriptingMaterialBuilder::filter_2,ScriptingMaterialBuilder::set_filter_2,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("FILTER_NEAREST")
        .add_enum("FILTER_LINEAR")
        .add_enum("FILTER_LINEAR_MIPMAP_LINEAR");

    *archive << ARCHIVE_DATA_ACCESSORS("Address_U_2", ScriptingMaterialBuilder::address_u_2,ScriptingMaterialBuilder::set_address_u_2,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_V_2", ScriptingMaterialBuilder::address_v_2,ScriptingMaterialBuilder::set_address_v_2,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_W_2", ScriptingMaterialBuilder::address_w_2,ScriptingMaterialBuilder::set_address_w_2,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Mip_LOD_Bias_2", ScriptingMaterialBuilder::mip_lod_bias_2,ScriptingMaterialBuilder::set_mip_lod_bias_2,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Max_Anisotropy_2", ScriptingMaterialBuilder::max_anisotropy_2,ScriptingMaterialBuilder::set_max_anisotropy_2,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Transform_2", ScriptingMaterialBuilder::transform_2,ScriptingMaterialBuilder::set_transform_2,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_tex_3,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_rect_3,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Filter_3", ScriptingMaterialBuilder::filter_3,ScriptingMaterialBuilder::set_filter_3,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("FILTER_NEAREST")
        .add_enum("FILTER_LINEAR")
        .add_enum("FILTER_LINEAR_MIPMAP_LINEAR");

    *archive << ARCHIVE_DATA_ACCESSORS("Address_U_3", ScriptingMaterialBuilder::address_u_3,ScriptingMaterialBuilder::set_address_u_3,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_V_3", ScriptingMaterialBuilder::address_v_3,ScriptingMaterialBuilder::set_address_v_3,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_W_3", ScriptingMaterialBuilder::address_w_3,ScriptingMaterialBuilder::set_address_w_3,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Mip_LOD_Bias_3", ScriptingMaterialBuilder::mip_lod_bias_3,ScriptingMaterialBuilder::set_mip_lod_bias_3,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Max_Anisotropy_3", ScriptingMaterialBuilder::max_anisotropy_3,ScriptingMaterialBuilder::set_max_anisotropy_3,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Transform_3", ScriptingMaterialBuilder::transform_3,ScriptingMaterialBuilder::set_transform_3,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_tex_4,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_rect_4,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Filter_4", ScriptingMaterialBuilder::filter_4,ScriptingMaterialBuilder::set_filter_4,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("FILTER_NEAREST")
        .add_enum("FILTER_LINEAR")
        .add_enum("FILTER_LINEAR_MIPMAP_LINEAR");

    *archive << ARCHIVE_DATA_ACCESSORS("Address_U_4", ScriptingMaterialBuilder::address_u_4,ScriptingMaterialBuilder::set_address_u_4,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_V_4", ScriptingMaterialBuilder::address_v_4,ScriptingMaterialBuilder::set_address_v_4,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_W_4", ScriptingMaterialBuilder::address_w_4,ScriptingMaterialBuilder::set_address_w_4,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Mip_LOD_Bias_4", ScriptingMaterialBuilder::mip_lod_bias_4,ScriptingMaterialBuilder::set_mip_lod_bias_4,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Max_Anisotropy_4", ScriptingMaterialBuilder::max_anisotropy_4,ScriptingMaterialBuilder::set_max_anisotropy_4,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Transform_4", ScriptingMaterialBuilder::transform_4,ScriptingMaterialBuilder::set_transform_4,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_tex_5,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_rect_5,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Filter_5", ScriptingMaterialBuilder::filter_5,ScriptingMaterialBuilder::set_filter_5,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("FILTER_NEAREST")
        .add_enum("FILTER_LINEAR")
        .add_enum("FILTER_LINEAR_MIPMAP_LINEAR");

    *archive << ARCHIVE_DATA_ACCESSORS("Address_U_5", ScriptingMaterialBuilder::address_u_5,ScriptingMaterialBuilder::set_address_u_5,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_V_5", ScriptingMaterialBuilder::address_v_5,ScriptingMaterialBuilder::set_address_v_5,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_W_5", ScriptingMaterialBuilder::address_w_5,ScriptingMaterialBuilder::set_address_w_5,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Mip_LOD_Bias_5", ScriptingMaterialBuilder::mip_lod_bias_5,ScriptingMaterialBuilder::set_mip_lod_bias_5,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Max_Anisotropy_5", ScriptingMaterialBuilder::max_anisotropy_5,ScriptingMaterialBuilder::set_max_anisotropy_5,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Transform_5", ScriptingMaterialBuilder::transform_5,ScriptingMaterialBuilder::set_transform_5,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_tex_6,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_rect_6,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Filter_6", ScriptingMaterialBuilder::filter_6,ScriptingMaterialBuilder::set_filter_6,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("FILTER_NEAREST")
        .add_enum("FILTER_LINEAR")
        .add_enum("FILTER_LINEAR_MIPMAP_LINEAR");

    *archive << ARCHIVE_DATA_ACCESSORS("Address_U_6", ScriptingMaterialBuilder::address_u_6,ScriptingMaterialBuilder::set_address_u_6,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_V_6", ScriptingMaterialBuilder::address_v_6,ScriptingMaterialBuilder::set_address_v_6,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_W_6", ScriptingMaterialBuilder::address_w_6,ScriptingMaterialBuilder::set_address_w_6,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Mip_LOD_Bias_6", ScriptingMaterialBuilder::mip_lod_bias_6,ScriptingMaterialBuilder::set_mip_lod_bias_6,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Max_Anisotropy_6", ScriptingMaterialBuilder::max_anisotropy_6,ScriptingMaterialBuilder::set_max_anisotropy_6,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Transform_6", ScriptingMaterialBuilder::transform_6,ScriptingMaterialBuilder::set_transform_6,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_tex_7,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_rect_7,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Filter_7", ScriptingMaterialBuilder::filter_7,ScriptingMaterialBuilder::set_filter_7,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("FILTER_NEAREST")
        .add_enum("FILTER_LINEAR")
        .add_enum("FILTER_LINEAR_MIPMAP_LINEAR");

    *archive << ARCHIVE_DATA_ACCESSORS("Address_U_7", ScriptingMaterialBuilder::address_u_7,ScriptingMaterialBuilder::set_address_u_7,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_V_7", ScriptingMaterialBuilder::address_v_7,ScriptingMaterialBuilder::set_address_v_7,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_W_7", ScriptingMaterialBuilder::address_w_7,ScriptingMaterialBuilder::set_address_w_7,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Mip_LOD_Bias_7", ScriptingMaterialBuilder::mip_lod_bias_7,ScriptingMaterialBuilder::set_mip_lod_bias_7,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Max_Anisotropy_7", ScriptingMaterialBuilder::max_anisotropy_7,ScriptingMaterialBuilder::set_max_anisotropy_7,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Transform_7", ScriptingMaterialBuilder::transform_7,ScriptingMaterialBuilder::set_transform_7,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_tex_8,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_rect_8,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Filter_8", ScriptingMaterialBuilder::filter_8,ScriptingMaterialBuilder::set_filter_8,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("FILTER_NEAREST")
        .add_enum("FILTER_LINEAR")
        .add_enum("FILTER_LINEAR_MIPMAP_LINEAR");

    *archive << ARCHIVE_DATA_ACCESSORS("Address_U_8", ScriptingMaterialBuilder::address_u_8,ScriptingMaterialBuilder::set_address_u_8,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_V_8", ScriptingMaterialBuilder::address_v_8,ScriptingMaterialBuilder::set_address_v_8,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_W_8", ScriptingMaterialBuilder::address_w_8,ScriptingMaterialBuilder::set_address_w_8,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Mip_LOD_Bias_8", ScriptingMaterialBuilder::mip_lod_bias_8,ScriptingMaterialBuilder::set_mip_lod_bias_8,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Max_Anisotropy_8", ScriptingMaterialBuilder::max_anisotropy_8,ScriptingMaterialBuilder::set_max_anisotropy_8,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Transform_8", ScriptingMaterialBuilder::transform_8,ScriptingMaterialBuilder::set_transform_8,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_tex_9,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_rect_9,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Filter_9", ScriptingMaterialBuilder::filter_9,ScriptingMaterialBuilder::set_filter_9,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("FILTER_NEAREST")
        .add_enum("FILTER_LINEAR")
        .add_enum("FILTER_LINEAR_MIPMAP_LINEAR");

    *archive << ARCHIVE_DATA_ACCESSORS("Address_U_9", ScriptingMaterialBuilder::address_u_9,ScriptingMaterialBuilder::set_address_u_9,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_V_9", ScriptingMaterialBuilder::address_v_9,ScriptingMaterialBuilder::set_address_v_9,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_W_9", ScriptingMaterialBuilder::address_w_9,ScriptingMaterialBuilder::set_address_w_9,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Mip_LOD_Bias_9", ScriptingMaterialBuilder::mip_lod_bias_9,ScriptingMaterialBuilder::set_mip_lod_bias_9,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Max_Anisotropy_9", ScriptingMaterialBuilder::max_anisotropy_9,ScriptingMaterialBuilder::set_max_anisotropy_9,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Transform_9", ScriptingMaterialBuilder::transform_9,ScriptingMaterialBuilder::set_transform_9,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_tex_10,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_rect_10,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Filter_10", ScriptingMaterialBuilder::filter_10,ScriptingMaterialBuilder::set_filter_10,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("FILTER_NEAREST")
        .add_enum("FILTER_LINEAR")
        .add_enum("FILTER_LINEAR_MIPMAP_LINEAR");

    *archive << ARCHIVE_DATA_ACCESSORS("Address_U_10", ScriptingMaterialBuilder::address_u_10,ScriptingMaterialBuilder::set_address_u_10,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_V_10", ScriptingMaterialBuilder::address_v_10,ScriptingMaterialBuilder::set_address_v_10,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_W_10", ScriptingMaterialBuilder::address_w_10,ScriptingMaterialBuilder::set_address_w_10,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Mip_LOD_Bias_10", ScriptingMaterialBuilder::mip_lod_bias_10,ScriptingMaterialBuilder::set_mip_lod_bias_10,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Max_Anisotropy_10", ScriptingMaterialBuilder::max_anisotropy_10,ScriptingMaterialBuilder::set_max_anisotropy_10,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Transform_10", ScriptingMaterialBuilder::transform_10,ScriptingMaterialBuilder::set_transform_10,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_tex_11,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_rect_11,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Filter_11", ScriptingMaterialBuilder::filter_11,ScriptingMaterialBuilder::set_filter_11,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("FILTER_NEAREST")
        .add_enum("FILTER_LINEAR")
        .add_enum("FILTER_LINEAR_MIPMAP_LINEAR");

    *archive << ARCHIVE_DATA_ACCESSORS("Address_U_11", ScriptingMaterialBuilder::address_u_11,ScriptingMaterialBuilder::set_address_u_11,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_V_11", ScriptingMaterialBuilder::address_v_11,ScriptingMaterialBuilder::set_address_v_11,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_W_11", ScriptingMaterialBuilder::address_w_11,ScriptingMaterialBuilder::set_address_w_11,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Mip_LOD_Bias_11", ScriptingMaterialBuilder::mip_lod_bias_11,ScriptingMaterialBuilder::set_mip_lod_bias_11,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Max_Anisotropy_11", ScriptingMaterialBuilder::max_anisotropy_11,ScriptingMaterialBuilder::set_max_anisotropy_11,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Transform_11", ScriptingMaterialBuilder::transform_11,ScriptingMaterialBuilder::set_transform_11,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_tex_12,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_rect_12,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Filter_12", ScriptingMaterialBuilder::filter_12,ScriptingMaterialBuilder::set_filter_12,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("FILTER_NEAREST")
        .add_enum("FILTER_LINEAR")
        .add_enum("FILTER_LINEAR_MIPMAP_LINEAR");

    *archive << ARCHIVE_DATA_ACCESSORS("Address_U_12", ScriptingMaterialBuilder::address_u_12,ScriptingMaterialBuilder::set_address_u_12,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_V_12", ScriptingMaterialBuilder::address_v_12,ScriptingMaterialBuilder::set_address_v_12,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_W_12", ScriptingMaterialBuilder::address_w_12,ScriptingMaterialBuilder::set_address_w_12,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Mip_LOD_Bias_12", ScriptingMaterialBuilder::mip_lod_bias_12,ScriptingMaterialBuilder::set_mip_lod_bias_12,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Max_Anisotropy_12", ScriptingMaterialBuilder::max_anisotropy_12,ScriptingMaterialBuilder::set_max_anisotropy_12,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Transform_12", ScriptingMaterialBuilder::transform_12,ScriptingMaterialBuilder::set_transform_12,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_tex_13,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_rect_13,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Filter_13", ScriptingMaterialBuilder::filter_13,ScriptingMaterialBuilder::set_filter_13,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("FILTER_NEAREST")
        .add_enum("FILTER_LINEAR")
        .add_enum("FILTER_LINEAR_MIPMAP_LINEAR");

    *archive << ARCHIVE_DATA_ACCESSORS("Address_U_13", ScriptingMaterialBuilder::address_u_13,ScriptingMaterialBuilder::set_address_u_13,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_V_13", ScriptingMaterialBuilder::address_v_13,ScriptingMaterialBuilder::set_address_v_13,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_W_13", ScriptingMaterialBuilder::address_w_13,ScriptingMaterialBuilder::set_address_w_13,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Mip_LOD_Bias_13", ScriptingMaterialBuilder::mip_lod_bias_13,ScriptingMaterialBuilder::set_mip_lod_bias_13,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Max_Anisotropy_13", ScriptingMaterialBuilder::max_anisotropy_13,ScriptingMaterialBuilder::set_max_anisotropy_13,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Transform_13", ScriptingMaterialBuilder::transform_13,ScriptingMaterialBuilder::set_transform_13,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_tex_14,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_rect_14,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Filter_14", ScriptingMaterialBuilder::filter_14,ScriptingMaterialBuilder::set_filter_14,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("FILTER_NEAREST")
        .add_enum("FILTER_LINEAR")
        .add_enum("FILTER_LINEAR_MIPMAP_LINEAR");

    *archive << ARCHIVE_DATA_ACCESSORS("Address_U_14", ScriptingMaterialBuilder::address_u_14,ScriptingMaterialBuilder::set_address_u_14,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_V_14", ScriptingMaterialBuilder::address_v_14,ScriptingMaterialBuilder::set_address_v_14,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_W_14", ScriptingMaterialBuilder::address_w_14,ScriptingMaterialBuilder::set_address_w_14,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Mip_LOD_Bias_14", ScriptingMaterialBuilder::mip_lod_bias_14,ScriptingMaterialBuilder::set_mip_lod_bias_14,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Max_Anisotropy_14", ScriptingMaterialBuilder::max_anisotropy_14,ScriptingMaterialBuilder::set_max_anisotropy_14,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Transform_14", ScriptingMaterialBuilder::transform_14,ScriptingMaterialBuilder::set_transform_14,DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_tex_15,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_PLUG(_rect_15,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Filter_15", ScriptingMaterialBuilder::filter_15,ScriptingMaterialBuilder::set_filter_15,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("FILTER_NEAREST")
        .add_enum("FILTER_LINEAR")
        .add_enum("FILTER_LINEAR_MIPMAP_LINEAR");

    *archive << ARCHIVE_DATA_ACCESSORS("Address_U_15", ScriptingMaterialBuilder::address_u_15,ScriptingMaterialBuilder::set_address_u_15,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_V_15", ScriptingMaterialBuilder::address_v_15,ScriptingMaterialBuilder::set_address_v_15,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Address_W_15", ScriptingMaterialBuilder::address_w_15,ScriptingMaterialBuilder::set_address_w_15,DATA_PERSISTENT | DATA_SETTABLE)
        .add_enum("ADDRESS_REPEAT")
        .add_enum("ADDRESS_CLAMP_TO_EDGE");
    *archive << ARCHIVE_DATA_ACCESSORS("Mip_LOD_Bias_15", ScriptingMaterialBuilder::mip_lod_bias_15,ScriptingMaterialBuilder::set_mip_lod_bias_15,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Max_Anisotropy_15", ScriptingMaterialBuilder::max_anisotropy_15,ScriptingMaterialBuilder::set_max_anisotropy_15,DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Transform_15", ScriptingMaterialBuilder::transform_15,ScriptingMaterialBuilder::set_transform_15,DATA_PERSISTENT | DATA_SETTABLE);
    
    
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

DTboolean ScriptingMaterialBuilder::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);

    if (super_type::compute(plug))  return true;

	if (plug == &_out_material) {
        
        set_texture_2D_0(_tex_0);
        set_texture_2D_1(_tex_1);
        set_texture_2D_2(_tex_2);
        set_texture_2D_3(_tex_3);
        set_texture_2D_4(_tex_4);
        set_texture_2D_5(_tex_5);
        set_texture_2D_6(_tex_6);
        set_texture_2D_7(_tex_7);
        set_texture_2D_8(_tex_8);
        set_texture_2D_9(_tex_9);
        set_texture_2D_10(_tex_10);
        set_texture_2D_11(_tex_11);
        set_texture_2D_12(_tex_12);
        set_texture_2D_13(_tex_13);
        set_texture_2D_14(_tex_14);
        set_texture_2D_15(_tex_15);
    
        set_rectangle_0(_rect_0);
        set_rectangle_1(_rect_1);
        set_rectangle_2(_rect_2);
        set_rectangle_3(_rect_3);
        set_rectangle_4(_rect_4);
        set_rectangle_5(_rect_5);
        set_rectangle_6(_rect_6);
        set_rectangle_7(_rect_7);
        set_rectangle_8(_rect_8);
        set_rectangle_9(_rect_9);
        set_rectangle_10(_rect_10);
        set_rectangle_11(_rect_11);
        set_rectangle_12(_rect_12);
        set_rectangle_13(_rect_13);
        set_rectangle_14(_rect_14);
        set_rectangle_15(_rect_15);

		_out_material.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

} // DT3

