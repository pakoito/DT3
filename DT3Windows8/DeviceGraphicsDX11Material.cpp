//==============================================================================
///	
///	File: DeviceGraphicsDX11Material.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DeviceGraphicsDX11Material.hpp"

#include "System.hpp"
#include "DeviceConsole.hpp"
#include "Factory.hpp"
#include "MaterialResource.hpp"
#include "ShaderResource.hpp"
#include "FragmentShaderResource.hpp"

#include "Assert.hpp"
#include "DeviceGlobalsManager.hpp"
#include "System.hpp"
#include "DeviceGraphics.hpp"
#include "DeviceGraphicsDX11Renderer.hpp"
#include "DeviceGraphicsDX11Texture.hpp"
#include "DeviceGraphicsDX11Shader.hpp"
#include "CheckedCast.hpp"

//==============================================================================
//==============================================================================

namespace DT2 {

//==============================================================================
//==============================================================================

//DeviceGraphicsDX11MaterialState	DeviceGraphicsDX11Material::_global_state;

//==============================================================================
/// Associations for OpenGL constants and Built in ones
//==============================================================================

namespace {

	DTuint gBlending[] = {
		/*BLEND_ZERO*/					D3D11_BLEND_ZERO,
		/*BLEND_ONE*/					D3D11_BLEND_ONE,
		/*BLEND_SRC_ALPHA*/				D3D11_BLEND_SRC_ALPHA,
		/*BLEND_ONE_MINUS_SRC_ALPHA*/	D3D11_BLEND_INV_SRC_ALPHA,
		/*BLEND_DST_ALPHA*/				D3D11_BLEND_DEST_ALPHA,
		/*BLEND_ONE_MINUS_DST_ALPHA*/	D3D11_BLEND_INV_DEST_ALPHA,
		/*BLEND_SRC_COLOR*/             D3D11_BLEND_SRC_COLOR,
		/*BLEND_DST_COLOR*/             D3D11_BLEND_DEST_COLOR,
		/*BLEND_ONE_MINUS_SRC_COLOR*/	D3D11_BLEND_INV_SRC_COLOR,
		/*BLEND_ONE_MINUS_DST_COLOR*/	D3D11_BLEND_INV_DEST_COLOR
	};

	DTuint gDepth[] = {
		/*DEPTH_ALWAYS*/				D3D11_COMPARISON_ALWAYS,
		/*DEPTH_NEVER*/					D3D11_COMPARISON_NEVER,
		/*DEPTH_EQUAL*/					D3D11_COMPARISON_EQUAL,
		/*DEPTH_NOT_EQUAL*/				D3D11_COMPARISON_NOT_EQUAL,
		/*DEPTH_GREATER*/				D3D11_COMPARISON_GREATER,
        /*DEPTH_GEQUAL*/                D3D11_COMPARISON_GREATER_EQUAL,
		/*DEPTH_LESS*/					D3D11_COMPARISON_LESS,
		/*DEPTH_LEQUAL*/				D3D11_COMPARISON_LESS_EQUAL
	};

	DTuint gCulling[] = {
		/*CULL_FRONT*/					D3D11_CULL_FRONT,
		/*CULL_BACK*/					D3D11_CULL_BACK
	};
	
	DTuint gWrap[] = {
		/*TEXTURE_WRAP_REPEAT*/			D3D11_TEXTURE_ADDRESS_WRAP,
		/*TEXTURE_WRAP_CLAMP*/			D3D11_TEXTURE_ADDRESS_CLAMP
	};

	DTuint gFilterMode[] = {
        /*TEXTURE_FILTER_NEAREST*/		D3D11_FILTER_MIN_MAG_MIP_POINT,
        /*TEXTURE_FILTER_LINEAR*/		D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
		/*TEXTURE_FILTER_MIPMAP_LINEAR*/D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR
	};

    DTuint gStencil[] = {
		/*STENCIL_EQUAL*/		D3D11_COMPARISON_EQUAL,
		/*STENCIL_NOTEQUAL*/	D3D11_COMPARISON_NOT_EQUAL,
		/*STENCIL_LESS*/		D3D11_COMPARISON_LESS,
		/*STENCIL_GREATER*/     D3D11_COMPARISON_GREATER,
		/*STENCIL_LEQUAL*/      D3D11_COMPARISON_LESS_EQUAL,
		/*STENCIL_GEQUAL*/      D3D11_COMPARISON_GREATER_EQUAL,
		/*STENCIL_ALWAYS*/      D3D11_COMPARISON_ALWAYS,
		/*STENCIL_NEVER*/       D3D11_COMPARISON_NEVER
	};
    
	DTuint gStencilOp[] = {
		/*STENCILOP_KEEP*/		D3D11_STENCIL_OP_KEEP,
		/*STENCILOP_ZERO*/      D3D11_STENCIL_OP_ZERO,
		/*STENCILOP_REPLACE*/   D3D11_STENCIL_OP_REPLACE,
		/*STENCILOP_INCR*/      D3D11_STENCIL_OP_INCR_SAT,
		/*STENCILOP_DECR*/      D3D11_STENCIL_OP_DECR_SAT,
		/*STENCILOP_INCR_WRAP*/ D3D11_STENCIL_OP_INCR,
		/*STENCILOP_DECR_WRAP*/ D3D11_STENCIL_OP_DECR,
		/*STENCILOP_INVERT*/    D3D11_STENCIL_OP_INVERT
	};

};

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(DeviceGraphicsDX11Material)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DeviceGraphicsDX11Material::DeviceGraphicsDX11Material (void)
{    

}
			
DeviceGraphicsDX11Material::~DeviceGraphicsDX11Material (void)
{
	
}

//==============================================================================
//==============================================================================

DeviceGraphicsDX11MaterialTexture::DeviceGraphicsDX11MaterialTexture ()
	:	_texture		(NULL),
		_wrap_mode_s	(TEXTURE_WRAP_REPEAT),
		_wrap_mode_t	(TEXTURE_WRAP_REPEAT),
		_wrap_mode_r	(TEXTURE_WRAP_REPEAT),
		_filter_mode	(TEXTURE_FILTER_LINEAR),
		_scroll			(0.0F,0.0F,0.0F),
		_rotation		(0.0F),
		_scale			(1.0F,1.0F,0.0F),
		_pre_translate	(0.0F,0.0F,0.0F),
		_post_translate	(0.0F,0.0F,0.0F),
        _texture_matrix (   1.0F,0.0F,0.0F,0.0F,
                            0.0F,1.0F,0.0F,0.0F,
                            0.0F,0.0F,1.0F,0.0F,
                            0.0F,0.0F,0.0F,1.0F),
        _sampler_state  (NULL)
{		

}

DeviceGraphicsDX11MaterialTexture::~DeviceGraphicsDX11MaterialTexture ()
{
    SAFE_RELEASE(_sampler_state);
	RELEASE(_texture);
}

//==============================================================================
//==============================================================================

DeviceGraphicsDX11MaterialState::DeviceGraphicsDX11MaterialState ()
{
	_blending_enabled = false;
	_blending_src = BLEND_ONE;
	_blending_dst = BLEND_ZERO;
	_blending_alpha_src = BLEND_UNSPECIFIED;
	_blending_alpha_dst = BLEND_UNSPECIFIED;
    _color_mask_r = true;
    _color_mask_g = true;
    _color_mask_b = true;
    _color_mask_a = true;

	_depth_test_enabled = false;
	_depth_func = DEPTH_LEQUAL;
	_depth_mask = true;
						
	_culling_enabled = false;
	_cull_face = CULL_BACK;

    _stencil_test_enabled   = false;
    _stencil_mask           = 0xFFFFFFFF;
    _stencil_func           = STENCIL_ALWAYS;
    _stencil_bit_mask       = 0xFFFFFFFF;
    _stencil_front_sfail    = STENCILOP_KEEP;
    _stencil_front_dpfail   = STENCILOP_KEEP;
    _stencil_front_dppass   = STENCILOP_KEEP;
    _stencil_back_sfail     = STENCILOP_KEEP;
    _stencil_back_dpfail    = STENCILOP_KEEP;
    _stencil_back_dppass    = STENCILOP_KEEP;

	_color = Color(1.0F,1.0F,1.0F,1.0F);
			
	_shader = NULL;

    _depth_stencil_state = NULL;
    _blend_state = NULL;
    _rasterizer_state = NULL;
}

DeviceGraphicsDX11MaterialState::~DeviceGraphicsDX11MaterialState ()
{
	RELEASE(_shader);
    SAFE_RELEASE(_depth_stencil_state);
    SAFE_RELEASE(_blend_state);
    SAFE_RELEASE(_rasterizer_state);
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Material::syncToResource (MaterialResource *material)
{
	
	if (material->getRecacheParameters()) {
        DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());
        ID3D11Device1 *device = renderer->getD3D11Device();

		setBlending                 (material->getBlending());
		setBlendFuncSrc             (material->getBlendFuncSrc());
		setBlendFuncDst             (material->getBlendFuncDst());
		setAlphaBlendFuncSrc        (material->getAlphaBlendFuncSrc());
		setAlphaBlendFuncDst        (material->getAlphaBlendFuncDst());
		setColorMaskR               (material->getColorMaskR());
		setColorMaskG               (material->getColorMaskG());
		setColorMaskB               (material->getColorMaskB());
		setColorMaskA               (material->getColorMaskA());
		
		setDepthMask				(material->getDepthMask());
		setDepthTest				(material->getDepthTest());
		setDepthFunc				(material->getDepthFunc());
				
		setCulling					(material->getCulling());
		setCullFace                 (material->getCullFace());
		
		setStencilTest              (material->getStencilTest());
		setStencilMask              (material->getStencilMask());
		setStencilFunc              (material->getStencilFunc());
		setStencilBitMask           (material->getStencilBitMask());
		setStencilFrontsFail        (material->getStencilFrontsFail());
		setStencilFrontdpFail       (material->getStencilFrontdpFail());
		setStencilFrontdpPass       (material->getStencilFrontdpPass());
		setStencilBacksFail         (material->getStencilBacksFail());
		setStencilBackdpFail        (material->getStencilBackdpFail());
		setStencilBackdpPass        (material->getStencilBackdpPass());

		setColor					(material->getColor());

		setShader					(material->getShader());


        // Build state object for depth and stencil
        D3D11_DEPTH_STENCIL_DESC dsDesc;

        // Depth test parameters
        dsDesc.DepthEnable = getDepthTest();
        dsDesc.DepthWriteMask = getDepthMask() ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
        dsDesc.DepthFunc = (D3D11_COMPARISON_FUNC) gDepth[getDepthFunc()];

        // Stencil test parameters
        dsDesc.StencilEnable = getStencilTest();
        dsDesc.StencilReadMask = getStencilMask();
        dsDesc.StencilWriteMask = getStencilBitMask();

        // Stencil operations if pixel is front-facing
        dsDesc.FrontFace.StencilFailOp = (D3D11_STENCIL_OP) gStencilOp[getStencilFrontsFail()];
        dsDesc.FrontFace.StencilDepthFailOp = (D3D11_STENCIL_OP) gStencilOp[getStencilFrontdpFail()];
        dsDesc.FrontFace.StencilPassOp = (D3D11_STENCIL_OP) gStencilOp[getStencilFrontdpPass()];
        dsDesc.FrontFace.StencilFunc = (D3D11_COMPARISON_FUNC) gStencil[getStencilFunc()];

        // Stencil operations if pixel is back-facing
        dsDesc.BackFace.StencilFailOp = (D3D11_STENCIL_OP) gStencilOp[getStencilBacksFail()];
        dsDesc.BackFace.StencilDepthFailOp = (D3D11_STENCIL_OP) gStencilOp[getStencilBackdpFail()];
        dsDesc.BackFace.StencilPassOp = (D3D11_STENCIL_OP) gStencilOp[getStencilBackdpPass()];
        dsDesc.BackFace.StencilFunc = (D3D11_COMPARISON_FUNC) gStencil[getStencilFunc()];

        SAFE_RELEASE(_current_state._depth_stencil_state);
        HRESULT hr = device->CreateDepthStencilState(&dsDesc, &_current_state._depth_stencil_state);
        Assert(SUCCEEDED(hr));

        // Build state object for blending
        D3D11_BLEND_DESC blDesc;

        blDesc.AlphaToCoverageEnable = false;
        blDesc.IndependentBlendEnable = false;

        blDesc.RenderTarget[0].BlendEnable = getBlending();
        blDesc.RenderTarget[0].SrcBlend = (D3D11_BLEND) gBlending[getBlendFuncSrc()];
        blDesc.RenderTarget[0].DestBlend = (D3D11_BLEND) gBlending[getBlendFuncDst()];
        blDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blDesc.RenderTarget[0].SrcBlendAlpha = (D3D11_BLEND) ((getAlphaBlendFuncSrc() == BLEND_UNSPECIFIED) ? gBlending[getBlendFuncSrc()] : gBlending[getAlphaBlendFuncSrc()]);
        blDesc.RenderTarget[0].DestBlendAlpha = (D3D11_BLEND) ((getAlphaBlendFuncDst() == BLEND_UNSPECIFIED) ? gBlending[getBlendFuncDst()] : gBlending[getAlphaBlendFuncDst()]);
        blDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

        blDesc.RenderTarget[0].RenderTargetWriteMask = 
            (getColorMaskR() * D3D11_COLOR_WRITE_ENABLE_RED) |
            (getColorMaskG() * D3D11_COLOR_WRITE_ENABLE_GREEN) |
            (getColorMaskB() * D3D11_COLOR_WRITE_ENABLE_BLUE) |
            (getColorMaskA() * D3D11_COLOR_WRITE_ENABLE_ALPHA);

        SAFE_RELEASE(_current_state._blend_state);
        hr = device->CreateBlendState(&blDesc, &_current_state._blend_state);
        Assert(SUCCEEDED(hr));


        // Build state object for rasterizer
        D3D11_RASTERIZER_DESC rsDesc;

        rsDesc.FillMode = D3D11_FILL_SOLID;
        rsDesc.CullMode = (D3D11_CULL_MODE) (getCulling() ? gCulling[getCullFace()] : D3D11_CULL_NONE);
        rsDesc.FrontCounterClockwise = TRUE;
        rsDesc.DepthBias = 0;
        rsDesc.SlopeScaledDepthBias = 0.0f;
        rsDesc.DepthBiasClamp = 0.0f;
        rsDesc.DepthClipEnable = TRUE;
        rsDesc.ScissorEnable = FALSE;
        rsDesc.MultisampleEnable = FALSE;
        rsDesc.AntialiasedLineEnable = FALSE;

        SAFE_RELEASE(_current_state._rasterizer_state);
        hr = device->CreateRasterizerState(&rsDesc, &_current_state._rasterizer_state);
        Assert(SUCCEEDED(hr));

        ::memset(_current_state._samplers_state,0,sizeof(_current_state._samplers_state));

		for (DTuint t = 0; t < ARRAY_SIZE(_current_state._textures); ++t) {


            String texture_name = getShader()->getUniformName(ShaderResource::UNIFORM_TEX0+t);
            if (texture_name.empty())
                continue;

            DTint sampler_index = getShader()->getFragmentShader()->getShaderResourceIndex (texture_name + "_sampler");

			setCurrentUnit(t);
			material->setCurrentUnit(t);
			
			setPreTranslate         (material->getPreTranslate());
			setPostTranslate		(material->getPostTranslate());
			setScroll				(material->getScroll());
			setRotation             (material->getRotation());
			setScale				(material->getScale());
			setWrapS				(material->getWrapS());
			setWrapT				(material->getWrapT());
			setWrapR				(material->getWrapR());
			setFilter				(material->getFilter());

            D3D11_SAMPLER_DESC ssDesc;
            ssDesc.Filter = (D3D11_FILTER) gFilterMode[getFilter()];
            ssDesc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE) gWrap[getWrapS()];
            ssDesc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE) gWrap[getWrapT()];
            ssDesc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE) gWrap[getWrapR()];
            ssDesc.MipLODBias = 0;
            ssDesc.MaxAnisotropy = 0;
            ssDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
            ssDesc.BorderColor[0] = 0.0F;
            ssDesc.BorderColor[1] = 0.0F;
            ssDesc.BorderColor[2] = 0.0F;
            ssDesc.BorderColor[3] = 0.0F;
            ssDesc.MinLOD = -FLT_MAX;
            ssDesc.MaxLOD = FLT_MAX;

            SAFE_RELEASE(_current_state._textures[_unit]._sampler_state);
            hr = device->CreateSamplerState(&ssDesc,&_current_state._textures[_unit]._sampler_state);
            Assert(SUCCEEDED(hr));

            _current_state._samplers_state[sampler_index] = _current_state._textures[_unit]._sampler_state;
		}
		
		material->setRecacheParameters(false);
	}

    if (material->getRecacheData()) {
        ::memset(_current_state._textures_state,0,sizeof(_current_state._textures_state));
		
		for (DTuint t = 0; t < ARRAY_SIZE(_current_state._textures); ++t) {
        	setCurrentUnit(t);
			material->setCurrentUnit(t);
			
			setTex (material->getTex());

            String texture_name = getShader()->getUniformName(ShaderResource::UNIFORM_TEX0+t);
            if (texture_name.empty())
                continue;

            DTint texture_index = getShader()->getFragmentShader()->getShaderResourceIndex (texture_name);

            DeviceGraphicsDX11Texture *texture = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer())->getTextureCached(_current_state._textures[_unit]._texture);
            _current_state._textures_state[texture_index] = texture->getTextureView();

		}
		
		material->setRecacheData(false);	
	}
}

//==============================================================================
//==============================================================================
		
void DeviceGraphicsDX11Material::setTex (TextureResource *tex)
{				
	SAFE_ASSIGN(_current_state._textures[_unit]._texture, tex);
}

TextureResource* DeviceGraphicsDX11Material::getTex (void) const
{	
	return _current_state._textures[_unit]._texture;
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Material::activateMaterial (void)
{
	// Activate shader for this material
	if (!_current_state._shader || !_current_state._shader->getFragmentShader() || !_current_state._shader->getVertexShader())
		return;

    DeviceGraphicsDX11Renderer *renderer = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer());
    ID3D11DeviceContext1 *context = renderer->getD3D11Context();

	DeviceGraphicsDX11Shader *shader = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer())->getShaderCached(_current_state._shader);
	DeviceGraphicsDX11PixelShader *pixel_shader = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer())->getPixelShaderCached(_current_state._shader->getFragmentShader());
	DeviceGraphicsDX11VertexShader *vertex_shader = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer())->getVertexShaderCached(_current_state._shader->getVertexShader());

    // Set Shaders
    context->VSSetShader(vertex_shader->getShader(),NULL,0);
    context->PSSetShader(pixel_shader->getShader(),NULL,0);

    // Configure pipeline states
    context->OMSetDepthStencilState(_current_state._depth_stencil_state, System::getRenderer()->glGetStencilRef());
    context->OMSetBlendState(_current_state._blend_state,NULL,0xFFFFFFFF);
    context->RSSetState(_current_state._rasterizer_state);

    // Configure shaders
    float c[4] = { _current_state._color.getRFloat(), _current_state._color.getGFloat(), _current_state._color.getBFloat(), _current_state._color.getAFloat() };
    DeviceGraphicsDX11Shader::setConstantValue(shader->getConstant(ShaderResource::UNIFORM_COLOR), (DTubyte*) c, sizeof(c));
 
    // Configure Vertex Shader
    context->IASetInputLayout(shader->getLayout());

    // Configure constant buffers
    ID3D11Buffer** vs_constants = shader->getVSConstants();
    ID3D11Buffer** ps_constants = shader->getPSConstants();

    context->VSSetConstantBuffers(0,D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, vs_constants);
    context->PSSetConstantBuffers(0,D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, ps_constants);
	
    context->PSSetShaderResources(0,D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT,_current_state._textures_state);
    context->PSSetSamplers(0,D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT,_current_state._samplers_state);

    // Update textures
	// Textures
	DTint num_units = min2(8,System::getRenderer()->getNumTextureUnits());
	for (DTint t = 0; t < num_units; ++t) {
        setCurrentUnit(t);

    	DeviceGraphicsDX11MaterialTexture &current = _current_state._textures[_unit];

        if (!current._texture)
            continue;

        // This line is here to reset the timers for the texture resource so they don't get cleared out
        DeviceGraphicsDX11Texture *texture = checkedCast<DeviceGraphicsDX11Renderer*>(System::getRenderer())->getTextureCached(current._texture);

        syncTexture(_unit);
    }

}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Material::syncTexture (DTuint unit)
{
	DeviceGraphicsDX11MaterialTexture &current = _current_state._textures[unit];

    static TimerLores time;  // For scrolling
	DTfloat t = (DTfloat) time.getAbsTime();	
    
    if (current._texture->is2D()) {
                    
        DTfloat sina, cosa;
        if (current._rotation) {
            sina = std::sin(t * current._rotation);
            cosa = std::cos(t * current._rotation);
        } else {
            cosa = 1.0F;
            sina = 0.0F;
        }
            
        DTfloat pretx,prety;
        pretx = current._pre_translate.x;
        prety = current._pre_translate.y;

        DTfloat posttx,postty;
        posttx = t * current._scroll.x + current._post_translate.x;
        postty = t * current._scroll.y + current._post_translate.y;
            
        DTfloat sinasx,sinasy,cosasx,cosasy;
        sinasx = sina * current._scale.x;
        sinasy = sina * current._scale.y;
        cosasx = cosa * current._scale.x;
        cosasy = cosa * current._scale.y;
            
        Matrix4 transform	(	cosasx,		sinasx,		0.0F,	cosasx*pretx + sinasx*prety + posttx,
                                -sinasy,	cosasy,		0.0F,	-sinasy*pretx + cosasy*prety + postty,
                                0.0F,		0.0F,		1.0F,	0.0F,
                                0.0F,		0.0F,		0.0F,	1.0F	);

        current._texture_matrix = transform;   
    }
	
}

//==============================================================================
//==============================================================================

void DeviceGraphicsDX11Material::setShader (ShaderResource *p)
{
	SAFE_ASSIGN(_current_state._shader, p);
}

//==============================================================================
//==============================================================================

} // DT2

