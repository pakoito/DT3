#ifndef DT3_MATERIALRESOURCE
#define DT3_MATERIALRESOURCE
//==============================================================================
///	
///	File: MaterialResource.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/ResourceTypes/Resource.hpp"
#include "DT3Core/Devices/DeviceGraphics.hpp"
#include <string>
#include <map>
#include <mutex>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class ShaderResource;
class TextureResource2D;
class TextureResource3D;
class TextureResourceCube;

//==============================================================================
//==============================================================================

#define DEFINE_ACCESSORS_DEPTH_STENCIL(G,S,T,V)                                                             \
inline T G(void) const			{	return _depth_stencil_state.V;                                      }	\
inline void S(T var_)			{	_depth_stencil_state.V = var_;   _depth_stencil_state_dirty = true; }

#define DEFINE_ACCESSORS_BLEND(G,S,T,V)                                                                     \
inline T G(void) const			{	return _blend_state.V;                                              }	\
inline void S(T var_)			{	_blend_state.V = var_;   _blend_state_dirty = true;                 }

#define DEFINE_ACCESSORS_RASTERIZER(G,S,T,V)                                                                \
inline T G(void) const			{	return _rasterizer_state.V;                                         }	\
inline void S(T var_)			{	_rasterizer_state.V = var_;   _rasterizer_state_dirty = true;       }

#define DEFINE_ACCESSORS_TEXTURE_SLOT(G,S,T,V)                                                                                           \
inline T G(void) const			{	return _textures[_unit].V;  }	\
inline void S(T var_)			{	_textures[_unit].V = var_;  }

#define DEFINE_ACCESSORS_SAMPLER(G,S,T,V)                                                                                           \
inline T G(void) const			{	return _textures[_unit]._sampler_state.V;                                                   }	\
inline void S(T var_)			{	_textures[_unit]._sampler_state.V = var_;   _textures[_unit]._sampler_state_dirty = true;   }

//==============================================================================
/// Material resource type
//==============================================================================

class MaterialResource: public Resource {
    public:
        DEFINE_TYPE(MaterialResource,Resource)
		DEFINE_CREATE
  
										MaterialResource			(void);	
	private:
										MaterialResource			(const MaterialResource &rhs);
        MaterialResource &				operator =					(const MaterialResource &rhs);
	public:
		virtual							~MaterialResource			(void);
                
    public:
		/// Called to initialize the class
		static void                     initialize_static   (void);

		/// Called to uninitialize the class
		static void                     uninitialize_static (void);

		/// Called to initialize the object
		virtual void                    initialize          (void);


		/// Import the resource with an importer into a platform independent format
		/// \param pathname path to resource
		/// \param args importer args
		/// \return error code
		DTerr							import						(const FilePath &pathname, std::string args);
		
		/// Checks if a reload is needed
		/// \return reload needed
		virtual DTboolean               is_changed                (void) const;

		/// Description
		/// \param param description
		/// \return description
		void							reset						(void);




        //
        // Depth Stencil State
        //
    
        DEFINE_ACCESSORS_DEPTH_STENCIL(depth_enable,set_depth_enable,DTboolean,depth_enable)
        DEFINE_ACCESSORS_DEPTH_STENCIL(depth_write_mask,set_depth_write_mask,DTboolean,depth_write_mask)
        DEFINE_ACCESSORS_DEPTH_STENCIL(depth_func,set_depth_func,DT3GLComparison,depth_func)
    
        DEFINE_ACCESSORS_DEPTH_STENCIL(stencil_enable,set_stencil_enable,DTboolean,stencil_enable)
        DEFINE_ACCESSORS_DEPTH_STENCIL(stencil_read_mask,set_stencil_read_mask,DTuint,stencil_read_mask)
        DEFINE_ACCESSORS_DEPTH_STENCIL(stencil_write_mask,set_stencil_write_mask,DTuint,stencil_write_mask)
    
        DEFINE_ACCESSORS_DEPTH_STENCIL(front_face_stencil_fail_op,set_front_face_stencil_fail_op,DT3GLStencilOp,front_face_stencil_fail_op)
        DEFINE_ACCESSORS_DEPTH_STENCIL(front_face_stencil_depth_fail_op,set_front_face_stencil_depth_fail_op,DT3GLStencilOp,front_face_stencil_depth_fail_op)
        DEFINE_ACCESSORS_DEPTH_STENCIL(front_face_stencil_pass_op,set_front_face_stencil_pass_op,DT3GLStencilOp,front_face_stencil_pass_op)
        DEFINE_ACCESSORS_DEPTH_STENCIL(front_face_stencil_func,set_front_face_stencil_func,DT3GLComparison,front_face_stencil_func)
    
        DEFINE_ACCESSORS_DEPTH_STENCIL(back_face_stencil_fail_op,set_back_face_stencil_fail_op,DT3GLStencilOp,back_face_stencil_fail_op)
        DEFINE_ACCESSORS_DEPTH_STENCIL(back_face_stencil_depth_fail_op,set_back_face_stencil_depth_fail_op,DT3GLStencilOp,back_face_stencil_depth_fail_op)
        DEFINE_ACCESSORS_DEPTH_STENCIL(back_face_stencil_pass_op,set_back_face_stencil_pass_op,DT3GLStencilOp,back_face_stencil_pass_op)
        DEFINE_ACCESSORS_DEPTH_STENCIL(back_face_stencil_func,set_back_face_stencil_func,DT3GLComparison,back_face_stencil_func)
    
        //
        // Blend State
        //
    
        DEFINE_ACCESSORS_BLEND(blend_enable,set_blend_enable,DTboolean,blend_enable)
        DEFINE_ACCESSORS_BLEND(src_blend,set_src_blend,DT3GLBlend,src_blend)
        DEFINE_ACCESSORS_BLEND(dest_blend,set_dest_blend,DT3GLBlend,dest_blend)
        DEFINE_ACCESSORS_BLEND(src_blend_alpha,set_src_blend_alpha,DT3GLBlend,src_blend_alpha)
        DEFINE_ACCESSORS_BLEND(dest_blend_alpha,set_dest_blend_alpha,DT3GLBlend,dest_blend_alpha)
        DEFINE_ACCESSORS_BLEND(render_target_write_mask_r,set_render_target_write_mask_r,DTboolean,render_target_write_mask_r)
        DEFINE_ACCESSORS_BLEND(render_target_write_mask_g,set_render_target_write_mask_g,DTboolean,render_target_write_mask_g)
        DEFINE_ACCESSORS_BLEND(render_target_write_mask_b,set_render_target_write_mask_b,DTboolean,render_target_write_mask_b)
        DEFINE_ACCESSORS_BLEND(render_target_write_mask_a,set_render_target_write_mask_a,DTboolean,render_target_write_mask_a)
    
        //
        // Rasterizer State
        //
    
        DEFINE_ACCESSORS_RASTERIZER(cull_mode,set_cull_mode,DT3GLCullMode,cull_mode)
        DEFINE_ACCESSORS_RASTERIZER(depth_bias,set_depth_bias,DTfloat,depth_bias)
        DEFINE_ACCESSORS_RASTERIZER(slope_scaled_depth_bias,set_slope_scaled_depth_bias,DTfloat,slope_scaled_depth_bias)
        DEFINE_ACCESSORS_RASTERIZER(depth_bias_clamp,set_depth_bias_clamp,DTfloat,depth_bias_clamp)
        DEFINE_ACCESSORS_RASTERIZER(scissor_enable,set_scissor_enable,DTboolean,scissor_enable)

        //
        // Texture State
        //

        DEFINE_ACCESSORS(current_unit, set_current_unit, DTuint, _unit)

        DEFINE_ACCESSORS_TEXTURE_SLOT(texture_2D,set_texture_2D,std::shared_ptr<TextureResource2D>,_texture_2D)
        DEFINE_ACCESSORS_TEXTURE_SLOT(texture_3D,set_texture_3D,std::shared_ptr<TextureResource3D>,_texture_3D)
        DEFINE_ACCESSORS_TEXTURE_SLOT(texture_cube,set_texture_cube,std::shared_ptr<TextureResourceCube>,_texture_cube)
    
        DEFINE_ACCESSORS_SAMPLER(filter,set_filter,DT3GLFilterMode,filter)
        DEFINE_ACCESSORS_SAMPLER(address_u,set_address_u,DT3GLAddressMode,address_u)
        DEFINE_ACCESSORS_SAMPLER(address_v,set_address_v,DT3GLAddressMode,address_v)
        DEFINE_ACCESSORS_SAMPLER(address_w,set_address_w,DT3GLAddressMode,address_w)
        DEFINE_ACCESSORS_SAMPLER(mip_lod_bias,set_mip_lod_bias,DTfloat,mip_lod_bias)
        DEFINE_ACCESSORS_SAMPLER(max_anisotropy,set_max_anisotropy,DTfloat,max_anisotropy)

        DEFINE_ACCESSORS_TEXTURE_SLOT(transform,set_transform,Matrix4,_transform)
    
        // Convenience functions
        void                                set_rectangle           (const Rectangle &rectangle);
        void                                set_translation         (const Vector3 &translation);
        void                                set_scale               (const Vector3 &scale);
    
		/// Activate the resource
        void                                activate                (void);

		/// Load the resource
		/// \param pathname path to file
		/// \param args arguments to importer
		/// \return resource
		static std::shared_ptr<MaterialResource>   import_resource  (const FilePath &pathname, std::string args = "");

	private:
        static std::mutex                                                   _material_map_lock;
		static std::map<std::string, std::shared_ptr<MaterialResource>>     _material_map;

        static void                         reload_if_changed       (void);

        void                                screen_opened           (DTuint width, DTuint height);
        void                                screen_closed           (void);
    
    
        DT3GLDepthStencilState                      _depth_stencil_state;
        DTboolean                                   _depth_stencil_state_dirty;
        std::shared_ptr<DT3GLDepthStencilResource>  _depth_stencil_resource;

        DT3GLBlendState                             _blend_state;
        DTboolean                                   _blend_state_dirty;
        std::shared_ptr<DT3GLBlendResource>         _blend_state_resource;

        DT3GLRasterizerState                        _rasterizer_state;
        DTboolean                                   _rasterizer_state_dirty;
        std::shared_ptr<DT3GLRasterizerResource>    _rasterizer_resource;
    
        struct TextureSlot {
            std::shared_ptr<TextureResource2D>      _texture_2D;
            std::shared_ptr<TextureResource3D>      _texture_3D;
            std::shared_ptr<TextureResourceCube>    _texture_cube;
            
            DT3GLSamplerState                       _sampler_state;
            DTboolean                               _sampler_state_dirty;
            std::shared_ptr<DT3GLSamplerResource>   _sampler_state_resource;
            
            std::shared_ptr<DT3GLUniformResource>   _transform_uniform;
            Matrix4                                 _transform;
        };
		TextureSlot                                 _textures[16];

		DTuint                                      _unit;
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const std::shared_ptr<MaterialResource> &r);
Stream& operator >>(Stream &s, std::shared_ptr<MaterialResource> &r);

//==============================================================================
//==============================================================================

} // DT3

#endif
