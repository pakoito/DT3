//==============================================================================
///	
///	File: DeviceGraphics.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Devices/DeviceGraphics.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/Types/Math/Vector4.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

//IMPLEMENT_FACTORY_CREATION(DeviceGraphics)

//==============================================================================
//==============================================================================

DeviceGraphics::DeviceGraphics (void)
{

}

DeviceGraphics::~DeviceGraphics (void)
{

}

//==============================================================================
//==============================================================================

void DeviceGraphics::init_display (void)
{
    SystemCallbacks::screen_inited_cb().fire();
}

void DeviceGraphics::open_display (DTint width, DTint height)
{
    SystemCallbacks::screen_opened_cb().fire(width, height);
}

void DeviceGraphics::change_display (DTuint width, DTuint height)
{
    SystemCallbacks::screen_changed_cb().fire(width,height);
}

void DeviceGraphics::close_display (void)
{
    SystemCallbacks::screen_closed_cb().fire();
}

//==============================================================================
//==============================================================================

void DeviceGraphics::reset(DT3GLDepthStencilState &s)
{
    s.depth_enable                    = true;
    s.depth_write_mask                = true;
    s.depth_func                      = DT3GL_COMPARE_LEQUAL;
    s.stencil_enable                  = false;
    s.stencil_read_mask               = 0xFFFFFFFF;
    s.stencil_write_mask              = 0xFFFFFFFF;
    s.front_face_stencil_fail_op      = DT3GL_STENCILOP_KEEP;
    s.front_face_stencil_depth_fail_op= DT3GL_STENCILOP_KEEP;
    s.front_face_stencil_pass_op      = DT3GL_STENCILOP_KEEP;
    s.front_face_stencil_func         = DT3GL_COMPARE_LEQUAL;
    s.back_face_stencil_fail_op       = DT3GL_STENCILOP_KEEP;
    s.back_face_stencil_depth_fail_op = DT3GL_STENCILOP_KEEP;
    s.back_face_stencil_pass_op       = DT3GL_STENCILOP_KEEP;
    s.back_face_stencil_func          = DT3GL_COMPARE_LEQUAL;
}

void DeviceGraphics::reset(DT3GLBlendState &s)
{
    s.blend_enable                    = false;
    s.src_blend                       = DT3GL_BLEND_SRC_ALPHA;
    s.dest_blend                      = DT3GL_BLEND_ONE_MINUS_SRC_ALPHA;
    s.src_blend_alpha                 = DT3GL_BLEND_SRC_ALPHA;
    s.dest_blend_alpha                = DT3GL_BLEND_ONE_MINUS_SRC_ALPHA;
    s.render_target_write_mask_r      = true;
    s.render_target_write_mask_g      = true;
    s.render_target_write_mask_b      = true;
    s.render_target_write_mask_a      = true;
}

void DeviceGraphics::reset(DT3GLRasterizerState &s)
{
    s.cull_mode                       = DT3GL_CULL_BACK;
    s.depth_bias                      = 0.0F;
    s.slope_scaled_depth_bias         = 0.0F;
    s.depth_bias_clamp                = 0.0F;
    s.scissor_enable                  = false;
}

void DeviceGraphics::reset(DT3GLSamplerState &s)
{
    s.filter                          = DT3GL_FILTER_LINEAR;
    s.address_u                       = DT3GL_ADDRESS_CLAMP_TO_EDGE;
    s.address_v                       = DT3GL_ADDRESS_CLAMP_TO_EDGE;
    s.address_w                       = DT3GL_ADDRESS_CLAMP_TO_EDGE;
    s.mip_lod_bias                    = 0.0F;
    s.max_anisotropy                  = 0.0F;
}

//==============================================================================
//==============================================================================

} // DT3
