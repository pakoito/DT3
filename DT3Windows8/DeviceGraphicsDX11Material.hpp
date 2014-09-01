#ifndef DT2_DEVICEGRAPHICSDX11MATERIAL
#define DT2_DEVICEGRAPHICSDX11MATERIAL
//==============================================================================
///	
///	File: DeviceGraphicsDX11Material.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "pch.h"

#include "BaseClass.hpp"
#include "TextureResource.hpp"
#include "Color.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"

//==============================================================================
/// Namespace
//==============================================================================

namespace DT2 {

//==============================================================================
/// Forward declarations
//==============================================================================

class MaterialResource;
class ShaderResource;

//==============================================================================
/// Class
//==============================================================================

class DeviceGraphicsDX11MaterialTexture {
	public:
        DEFINE_TYPE_SIMPLE_BASE(DeviceGraphicsDX11MaterialTexture)

		DeviceGraphicsDX11MaterialTexture (void);
		~DeviceGraphicsDX11MaterialTexture (void);

	public:
		TextureResource*			_texture;
		
		DTuint						_wrap_mode_s;
		DTuint						_wrap_mode_t;
		DTuint						_wrap_mode_r;
		DTuint						_filter_mode;
		
		Vector3						_scroll;
		DTfloat						_rotation;
		Vector3						_scale;
		Vector3						_pre_translate;
		Vector3						_post_translate;
		
		Matrix4						_texture_matrix;

        ID3D11SamplerState          *_sampler_state;
};

//==============================================================================
/// Class
//==============================================================================

class DeviceGraphicsDX11MaterialState {
	public:
        DEFINE_TYPE_SIMPLE_BASE(DeviceGraphicsDX11MaterialState)

		DeviceGraphicsDX11MaterialState (void);
		~DeviceGraphicsDX11MaterialState (void);

	public:
		DTboolean						_blending_enabled;
		DTuint							_blending_src;
		DTuint							_blending_dst;
		DTuint							_blending_alpha_src;
		DTuint							_blending_alpha_dst;
        DTboolean						_color_mask_r;
        DTboolean						_color_mask_g;
        DTboolean						_color_mask_b;
        DTboolean						_color_mask_a;

		DTboolean						_depth_test_enabled;
		DTint							_depth_func;
		DTboolean						_depth_mask;
						
		DTboolean						_culling_enabled;
		DTint							_cull_face;
		
        DTboolean                       _stencil_test_enabled;
        DTuint                          _stencil_mask;
        DTint                           _stencil_func;
        DTuint                          _stencil_bit_mask;
    
        DTint                           _stencil_front_sfail;
        DTint                           _stencil_front_dpfail;
        DTint                           _stencil_front_dppass;

        DTint                           _stencil_back_sfail;
        DTint                           _stencil_back_dpfail;
        DTint                           _stencil_back_dppass;

		Color							_color;
				
		ShaderResource					*_shader;

        ID3D11DepthStencilState         *_depth_stencil_state;
        ID3D11BlendState                *_blend_state;
        ID3D11RasterizerState           *_rasterizer_state;

		DeviceGraphicsDX11MaterialTexture		_textures[8];

        ID3D11ShaderResourceView                *_textures_state[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
        ID3D11SamplerState                      *_samplers_state[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];

};

//==============================================================================
/// Class
//==============================================================================

class DeviceGraphicsDX11Material: public BaseClass {
    public:        
        DEFINE_TYPE(DeviceGraphicsDX11Material,BaseClass)
		DEFINE_CREATE
         
										DeviceGraphicsDX11Material			(void);	
    
    private:	
										DeviceGraphicsDX11Material			(const DeviceGraphicsDX11Material &rhs);
        DeviceGraphicsDX11Material &    operator =					        (const DeviceGraphicsDX11Material &rhs);	
    
    public:
        virtual							~DeviceGraphicsDX11Material			(void);
    
    public:
		/// Description
		/// \param param description
		/// \return description
		void							syncToResource			(MaterialResource *material);

		/// Description
		/// \param param description
		/// \return description	
		void							activateMaterial			(void);
			
		// Blending
        DEFINE_ACCESSORS(getBlending, setBlending, DTboolean, _current_state._blending_enabled)
        DEFINE_ACCESSORS(getBlendFuncSrc, setBlendFuncSrc, DTint, _current_state._blending_src)
        DEFINE_ACCESSORS(getBlendFuncDst, setBlendFuncDst, DTint, _current_state._blending_dst)
        DEFINE_ACCESSORS(getAlphaBlendFuncSrc, setAlphaBlendFuncSrc, DTint, _current_state._blending_alpha_src)
        DEFINE_ACCESSORS(getAlphaBlendFuncDst, setAlphaBlendFuncDst, DTint, _current_state._blending_alpha_dst)
        DEFINE_ACCESSORS(getColorMaskR, setColorMaskR, DTboolean, _current_state._color_mask_r)
        DEFINE_ACCESSORS(getColorMaskG, setColorMaskG, DTboolean, _current_state._color_mask_g)
        DEFINE_ACCESSORS(getColorMaskB, setColorMaskB, DTboolean, _current_state._color_mask_b)
        DEFINE_ACCESSORS(getColorMaskA, setColorMaskA, DTboolean, _current_state._color_mask_a)

        // Depth
        DEFINE_ACCESSORS(getDepthMask, setDepthMask, DTboolean, _current_state._depth_mask)
        DEFINE_ACCESSORS(getDepthTest, setDepthTest, DTboolean, _current_state._depth_test_enabled)
        DEFINE_ACCESSORS(getDepthFunc, setDepthFunc, DTint, _current_state._depth_func)				
    
        // Culling
        DEFINE_ACCESSORS(getCulling, setCulling, DTboolean, _current_state._culling_enabled)
        DEFINE_ACCESSORS(getCullFace, setCullFace, DTint, _current_state._cull_face)
        
        // Stenciling
        DEFINE_ACCESSORS(getStencilTest, setStencilTest, DTboolean, _current_state._stencil_test_enabled)
        DEFINE_ACCESSORS(getStencilMask, setStencilMask, DTuint, _current_state._stencil_mask)
        DEFINE_ACCESSORS(getStencilFunc, setStencilFunc, DTint, _current_state._stencil_func)
        DEFINE_ACCESSORS(getStencilBitMask, setStencilBitMask, DTuint, _current_state._stencil_bit_mask)

        DEFINE_ACCESSORS(getStencilFrontsFail, setStencilFrontsFail, DTint, _current_state._stencil_front_sfail)
        DEFINE_ACCESSORS(getStencilFrontdpFail, setStencilFrontdpFail, DTint, _current_state._stencil_front_dpfail)
        DEFINE_ACCESSORS(getStencilFrontdpPass, setStencilFrontdpPass, DTint, _current_state._stencil_front_dppass)

        DEFINE_ACCESSORS(getStencilBacksFail, setStencilBacksFail, DTint, _current_state._stencil_back_sfail)
        DEFINE_ACCESSORS(getStencilBackdpFail, setStencilBackdpFail, DTint, _current_state._stencil_back_dpfail)
        DEFINE_ACCESSORS(getStencilBackdpPass, setStencilBackdpPass, DTint, _current_state._stencil_back_dppass)

        // Color
        DEFINE_ACCESSORS(getColor, setColor, Color, _current_state._color)


		/// Description
		/// \param param description
		/// \return description
		void							setShader					(ShaderResource *p);

		/// Description
		/// \param param description
		/// \return description
		ShaderResource*&				getShader					(void)					{	return _current_state._shader;				}

		/// Description
		/// \param param description
		/// \return description
		void							setTex						(TextureResource *tex);

		/// Description
		/// \param param description
		/// \return description
		TextureResource*				getTex						(void) const;

		
		/// Description
		/// \param param description
		/// \return description
		void							setScroll					(const Vector3 &scroll)				{	_current_state._textures[_unit]._scroll = scroll;				}
		
		/// Description
		/// \param param description
		/// \return description
		Vector3							getScroll					(void) const						{	return _current_state._textures[_unit]._scroll;					}

		/// Description
		/// \param param description
		/// \return description
		void							setPreTranslate			(const Vector3 &translate)			{	_current_state._textures[_unit]._pre_translate = translate;		}
		
		/// Description
		/// \param param description
		/// \return description
		Vector3							getPreTranslate			(void) const						{	return _current_state._textures[_unit]._pre_translate;			}


		/// Description
		/// \param param description
		/// \return description
		void							setPostTranslate			(const Vector3 &translate)			{	_current_state._textures[_unit]._post_translate = translate;	}
		
		/// Description
		/// \param param description
		/// \return description
		Vector3							getPostTranslate			(void) const						{	return _current_state._textures[_unit]._post_translate;			}


		/// Description
		/// \param param description
		/// \return description
		void							setRotation				(const DTfloat rotation)			{	_current_state._textures[_unit]._rotation = rotation;			}
		
		/// Description
		/// \param param description
		/// \return description
		DTfloat							getRotation				(void) const						{	return _current_state._textures[_unit]._rotation;				}

		/// Description
		/// \param param description
		/// \return description
		void							setScale					(const Vector3 &scale)				{	_current_state._textures[_unit]._scale = scale;					}

		/// Description
		/// \param param description
		/// \return description
		Vector3							getScale					(void) const						{	return _current_state._textures[_unit]._scale;					}

		/// Description
		/// \param param description
		/// \return description
		const Matrix4&					getTextureMatrix			(void) const						{	return _current_state._textures[_unit]._texture_matrix;			}


		
		/// Description
		/// \param param description
		/// \return description
		void							setWrapS					(DTuint wrap_mode_s)				{	_current_state._textures[_unit]._wrap_mode_s = wrap_mode_s;		}

		/// Description
		/// \param param description
		/// \return description
		DTuint							getWrapS					(void) const						{	return _current_state._textures[_unit]._wrap_mode_s;			}

		/// Description
		/// \param param description
		/// \return description
		void							setWrapT					(DTuint wrap_mode_t)				{	_current_state._textures[_unit]._wrap_mode_t = wrap_mode_t;		}

		/// Description
		/// \param param description
		/// \return description
		DTuint							getWrapT					(void) const						{	return _current_state._textures[_unit]._wrap_mode_t;			}

		/// Description
		/// \param param description
		/// \return description
		void							setWrapR					(DTuint wrap_mode_r)				{	_current_state._textures[_unit]._wrap_mode_r = wrap_mode_r;		}

		/// Description
		/// \param param description
		/// \return description
		DTuint							getWrapR					(void) const						{	return _current_state._textures[_unit]._wrap_mode_r;			}

		/// Description
		/// \param param description
		/// \return description
		void							setFilter					(DTuint filter_mode)				{	_current_state._textures[_unit]._filter_mode = filter_mode;		}

		/// Description
		/// \param param description
		/// \return description
		DTuint							getFilter					(void) const						{	return _current_state._textures[_unit]._filter_mode;			}

		/// Description
		/// \param param description
		/// \return description
		void							setMode					(DTint mode)						{}

		/// Description
		/// \param param description
		/// \return description
		DTint							getMode					(void) const						{	return 0;					}
		

		/// Description
		/// \param param description
		/// \return description
		void							setCurrentUnit			(const DTuint unit)					{	_unit = unit;													}
		
		
	private:		
		DTuint							_unit;

		DeviceGraphicsDX11MaterialState	_current_state;
		
		void							syncTexture				(DTuint unit);
};

//==============================================================================
//==============================================================================

} // DT2

#endif

