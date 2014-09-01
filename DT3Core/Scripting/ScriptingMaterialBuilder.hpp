#ifndef DT3_SCRIPTINGMATERIALBUILDER
#define DT3_SCRIPTINGMATERIALBUILDER
//==============================================================================
///	
///	File: ScriptingMaterialBuilder.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Devices/DeviceGraphics.hpp"
#include "DT3Core/Types/Math/Rectangle.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class TextureResource2D;

//==============================================================================
//==============================================================================

#define FORWARD_FUNCTION(s,g,t) \
    void s(t v)                 {   (*_out_material)->s(v);       }  \
    t g(void) const             {   return (*_out_material)->g(); }

#define FORWARD_UNITS_SETTERS(s,t) \
    void s##_0(t v)              {   (*_out_material)->set_current_unit(0);   (*_out_material)->s(v);       }     \
    void s##_1(t v)              {   (*_out_material)->set_current_unit(1);   (*_out_material)->s(v);       }     \
    void s##_2(t v)              {   (*_out_material)->set_current_unit(2);   (*_out_material)->s(v);       }     \
    void s##_3(t v)              {   (*_out_material)->set_current_unit(3);   (*_out_material)->s(v);       }     \
    void s##_4(t v)              {   (*_out_material)->set_current_unit(4);   (*_out_material)->s(v);       }     \
    void s##_5(t v)              {   (*_out_material)->set_current_unit(5);   (*_out_material)->s(v);       }     \
    void s##_6(t v)              {   (*_out_material)->set_current_unit(6);   (*_out_material)->s(v);       }     \
    void s##_7(t v)              {   (*_out_material)->set_current_unit(7);   (*_out_material)->s(v);       }     \
    void s##_8(t v)              {   (*_out_material)->set_current_unit(7);   (*_out_material)->s(v);       }     \
    void s##_9(t v)              {   (*_out_material)->set_current_unit(7);   (*_out_material)->s(v);       }     \
    void s##_10(t v)             {   (*_out_material)->set_current_unit(7);   (*_out_material)->s(v);       }     \
    void s##_11(t v)             {   (*_out_material)->set_current_unit(7);   (*_out_material)->s(v);       }     \
    void s##_12(t v)             {   (*_out_material)->set_current_unit(7);   (*_out_material)->s(v);       }     \
    void s##_13(t v)             {   (*_out_material)->set_current_unit(7);   (*_out_material)->s(v);       }     \
    void s##_14(t v)             {   (*_out_material)->set_current_unit(7);   (*_out_material)->s(v);       }     \
    void s##_15(t v)             {   (*_out_material)->set_current_unit(7);   (*_out_material)->s(v);       }     \

#define FORWARD_UNITS_GETTERS(g,t) \
    t g##_0(void) const          {   (*_out_material)->set_current_unit(0);   return (*_out_material)->g(); }     \
    t g##_1(void) const          {   (*_out_material)->set_current_unit(1);   return (*_out_material)->g(); }     \
    t g##_2(void) const          {   (*_out_material)->set_current_unit(2);   return (*_out_material)->g(); }     \
    t g##_3(void) const          {   (*_out_material)->set_current_unit(3);   return (*_out_material)->g(); }     \
    t g##_4(void) const          {   (*_out_material)->set_current_unit(4);   return (*_out_material)->g(); }     \
    t g##_5(void) const          {   (*_out_material)->set_current_unit(5);   return (*_out_material)->g(); }     \
    t g##_6(void) const          {   (*_out_material)->set_current_unit(6);   return (*_out_material)->g(); }     \
    t g##_7(void) const          {   (*_out_material)->set_current_unit(7);   return (*_out_material)->g(); }     \
    t g##_8(void) const          {   (*_out_material)->set_current_unit(7);   return (*_out_material)->g(); }     \
    t g##_9(void) const          {   (*_out_material)->set_current_unit(7);   return (*_out_material)->g(); }     \
    t g##_10(void) const         {   (*_out_material)->set_current_unit(7);   return (*_out_material)->g(); }     \
    t g##_11(void) const         {   (*_out_material)->set_current_unit(7);   return (*_out_material)->g(); }     \
    t g##_12(void) const         {   (*_out_material)->set_current_unit(7);   return (*_out_material)->g(); }     \
    t g##_13(void) const         {   (*_out_material)->set_current_unit(7);   return (*_out_material)->g(); }     \
    t g##_14(void) const         {   (*_out_material)->set_current_unit(7);   return (*_out_material)->g(); }     \
    t g##_15(void) const         {   (*_out_material)->set_current_unit(7);   return (*_out_material)->g(); }

#define FORWARD_UNITS(s,g,t) \
    FORWARD_UNITS_SETTERS(s,t) \
    FORWARD_UNITS_GETTERS(g,t)


//==============================================================================
/// Loader for materials.
//==============================================================================

class ScriptingMaterialBuilder: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingMaterialBuilder,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingMaterialBuilder	(void);	
									ScriptingMaterialBuilder	(const ScriptingMaterialBuilder &rhs);
        ScriptingMaterialBuilder &	operator =                  (const ScriptingMaterialBuilder &rhs);	
        virtual                     ~ScriptingMaterialBuilder	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);
    
        //
        // Depth Stencil State
        //
    
        FORWARD_FUNCTION(set_depth_enable,depth_enable,DTboolean)
        FORWARD_FUNCTION(set_depth_write_mask,depth_write_mask,DTboolean)
        FORWARD_FUNCTION(set_depth_func,depth_func,DT3GLComparison)
        FORWARD_FUNCTION(set_stencil_enable,stencil_enable,DTboolean)
        FORWARD_FUNCTION(set_stencil_read_mask,stencil_read_mask,DTuint)
        FORWARD_FUNCTION(set_stencil_write_mask,stencil_write_mask,DTuint)
        FORWARD_FUNCTION(set_front_face_stencil_fail_op,front_face_stencil_fail_op,DT3GLStencilOp)
        FORWARD_FUNCTION(set_front_face_stencil_depth_fail_op,front_face_stencil_depth_fail_op,DT3GLStencilOp)
        FORWARD_FUNCTION(set_front_face_stencil_pass_op,front_face_stencil_pass_op,DT3GLStencilOp)
        FORWARD_FUNCTION(set_front_face_stencil_func,front_face_stencil_func,DT3GLComparison)
        FORWARD_FUNCTION(set_back_face_stencil_fail_op,back_face_stencil_fail_op,DT3GLStencilOp)
        FORWARD_FUNCTION(set_back_face_stencil_depth_fail_op,back_face_stencil_depth_fail_op,DT3GLStencilOp)
        FORWARD_FUNCTION(set_back_face_stencil_pass_op,back_face_stencil_pass_op,DT3GLStencilOp)
        FORWARD_FUNCTION(set_back_face_stencil_func,back_face_stencil_func,DT3GLComparison)

        //
        // Blend State
        //

        FORWARD_FUNCTION(set_blend_enable,blend_enable,DTboolean)
        FORWARD_FUNCTION(set_src_blend,src_blend,DT3GLBlend)
        FORWARD_FUNCTION(set_dest_blend,dest_blend,DT3GLBlend)
        FORWARD_FUNCTION(set_src_blend_alpha,src_blend_alpha,DT3GLBlend)
        FORWARD_FUNCTION(set_dest_blend_alpha,dest_blend_alpha,DT3GLBlend)
        FORWARD_FUNCTION(set_render_target_write_mask_r,render_target_write_mask_r,DTboolean)
        FORWARD_FUNCTION(set_render_target_write_mask_g,render_target_write_mask_g,DTboolean)
        FORWARD_FUNCTION(set_render_target_write_mask_b,render_target_write_mask_b,DTboolean)
        FORWARD_FUNCTION(set_render_target_write_mask_a,render_target_write_mask_a,DTboolean)

        //
        // Rasterizer State
        //

        FORWARD_FUNCTION(set_cull_mode,cull_mode,DT3GLCullMode)
        FORWARD_FUNCTION(set_depth_bias,depth_bias,DTfloat)
        FORWARD_FUNCTION(set_slope_scaled_depth_bias,slope_scaled_depth_bias,DTfloat)
        FORWARD_FUNCTION(set_depth_bias_clamp,depth_bias_clamp,DTfloat)
        FORWARD_FUNCTION(set_scissor_enable,scissor_enable,DTboolean)

        //
        // Texture State
        //

        FORWARD_UNITS(set_texture_2D,texture_2D,std::shared_ptr<TextureResource2D>)
        FORWARD_UNITS(set_filter,filter,DT3GLFilterMode)
        FORWARD_UNITS(set_address_u,address_u,DT3GLAddressMode)
        FORWARD_UNITS(set_address_v,address_v,DT3GLAddressMode)
        FORWARD_UNITS(set_address_w,address_w,DT3GLAddressMode)
        FORWARD_UNITS(set_mip_lod_bias,mip_lod_bias,DTfloat)
        FORWARD_UNITS(set_max_anisotropy,max_anisotropy,DTfloat)
    
        FORWARD_UNITS(set_transform,transform,Matrix4)
    
        FORWARD_UNITS_SETTERS(set_rectangle, Rectangle)
    
    private:		
		Plug<std::shared_ptr<MaterialResource>>	_out_material;

		Plug<std::shared_ptr<TextureResource2D>>		_tex_0;
		Plug<std::shared_ptr<TextureResource2D>>		_tex_1;
		Plug<std::shared_ptr<TextureResource2D>>		_tex_2;
		Plug<std::shared_ptr<TextureResource2D>>		_tex_3;
		Plug<std::shared_ptr<TextureResource2D>>		_tex_4;
		Plug<std::shared_ptr<TextureResource2D>>		_tex_5;
		Plug<std::shared_ptr<TextureResource2D>>		_tex_6;
		Plug<std::shared_ptr<TextureResource2D>>		_tex_7;
		Plug<std::shared_ptr<TextureResource2D>>		_tex_8;
		Plug<std::shared_ptr<TextureResource2D>>		_tex_9;
		Plug<std::shared_ptr<TextureResource2D>>		_tex_10;
		Plug<std::shared_ptr<TextureResource2D>>		_tex_11;
		Plug<std::shared_ptr<TextureResource2D>>		_tex_12;
		Plug<std::shared_ptr<TextureResource2D>>		_tex_13;
		Plug<std::shared_ptr<TextureResource2D>>		_tex_14;
		Plug<std::shared_ptr<TextureResource2D>>		_tex_15;

		Plug<Rectangle>		_rect_0;
		Plug<Rectangle>		_rect_1;
		Plug<Rectangle>		_rect_2;
		Plug<Rectangle>		_rect_3;
		Plug<Rectangle>		_rect_4;
		Plug<Rectangle>		_rect_5;
		Plug<Rectangle>		_rect_6;
		Plug<Rectangle>		_rect_7;
		Plug<Rectangle>		_rect_8;
		Plug<Rectangle>		_rect_9;
		Plug<Rectangle>		_rect_10;
		Plug<Rectangle>		_rect_11;
		Plug<Rectangle>		_rect_12;
		Plug<Rectangle>		_rect_13;
		Plug<Rectangle>		_rect_14;
		Plug<Rectangle>		_rect_15;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
