//==============================================================================
///	
///	File: ImporterMaterialMAT.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/Importers/ImporterMaterialMAT.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"
#include "DT3Core/Resources/ResourceTypes/ShaderResource.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResource2D.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResource3D.hpp"
#include "DT3Core/Resources/ResourceTypes/TextureResourceCube.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_IMPORTER(ImporterMaterialMAT,mat)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ImporterMaterialMAT::ImporterMaterialMAT (void)
{    
	
}
			
ImporterMaterialMAT::~ImporterMaterialMAT (void)
{ 

}

//==============================================================================
//==============================================================================

void ImporterMaterialMAT::parse_depth_stencil_block (MaterialResource *target)
{
	_tokenizer.assume_next_token("{");

	while (true) {
		std::string token = _tokenizer.next_token_string();
		
		// Handle Preprocessor
		if (_tokenizer.parse_preprocessor_macros(token))
			continue;
		
		// Are we at the end of the block
		if (token == "}")	break;
        
        if (MoreStrings::iequals(token,"DepthEnable"))                {     _tokenizer.assume_next_token("=");
                                                                            target->set_depth_enable(static_cast<DTboolean>(_tokenizer.next_token_number()));
                                                                            continue;   }
        if (MoreStrings::iequals(token,"DepthWriteMask"))             {     _tokenizer.assume_next_token("=");
                                                                            target->set_depth_write_mask(static_cast<DTboolean>(_tokenizer.next_token_number()));
                                                                            continue;   }
        if (MoreStrings::iequals(token,"DepthFunc"))                  {     _tokenizer.assume_next_token("=");
                                                                            target->set_depth_func((DT3GLComparison) _tokenizer.next_token_number());
                                                                            continue;   }

        if (MoreStrings::iequals(token,"StencilEnable"))              {     _tokenizer.assume_next_token("=");
                                                                            target->set_stencil_enable(static_cast<DTboolean>(_tokenizer.next_token_number()));
                                                                            continue;   }
        if (MoreStrings::iequals(token,"StencilReadMask"))            {     _tokenizer.assume_next_token("=");
                                                                            target->set_stencil_read_mask(_tokenizer.next_token_hex());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"StencilWriteMask"))           {     _tokenizer.assume_next_token("=");
                                                                            target->set_stencil_write_mask(_tokenizer.next_token_hex());
                                                                            continue;   }

        if (MoreStrings::iequals(token,"FrontFaceStencilFailOp"))     {     _tokenizer.assume_next_token("=");
                                                                            target->set_front_face_stencil_fail_op((DT3GLStencilOp) _tokenizer.next_token_number());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"FrontFaceStencilDepthFailOp")){     _tokenizer.assume_next_token("=");
                                                                            target->set_front_face_stencil_depth_fail_op((DT3GLStencilOp) _tokenizer.next_token_number());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"FrontFaceStencilPassOp"))     {     _tokenizer.assume_next_token("=");
                                                                            target->set_front_face_stencil_pass_op((DT3GLStencilOp) _tokenizer.next_token_number());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"FrontFaceStencilFunc"))       {     _tokenizer.assume_next_token("=");
                                                                            target->set_front_face_stencil_func((DT3GLComparison) _tokenizer.next_token_number());
                                                                            continue;   }

        if (MoreStrings::iequals(token,"BackFaceStencilFailOp"))      {     _tokenizer.assume_next_token("=");
                                                                            target->set_back_face_stencil_fail_op((DT3GLStencilOp) _tokenizer.next_token_number());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"BackFaceStencilDepthFailOp")) {     _tokenizer.assume_next_token("=");
                                                                            target->set_back_face_stencil_depth_fail_op((DT3GLStencilOp) _tokenizer.next_token_number());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"BackFaceStencilPassOp"))      {     _tokenizer.assume_next_token("=");
                                                                            target->set_back_face_stencil_pass_op((DT3GLStencilOp) _tokenizer.next_token_number());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"BackFaceStencilFunc"))        {     _tokenizer.assume_next_token("=");
                                                                            target->set_back_face_stencil_func((DT3GLComparison) _tokenizer.next_token_number());
                                                                            continue;   }
        
		_tokenizer.syntax_error("Unknown Token " + token);
	};
}

void ImporterMaterialMAT::parse_blend_block (MaterialResource *target)
{
	_tokenizer.assume_next_token("{");

	while (true) {
		std::string token = _tokenizer.next_token_string();
		
		// Handle Preprocessor
		if (_tokenizer.parse_preprocessor_macros(token))
			continue;
		
		// Are we at the end of the block
		if (token == "}")	break;
        
        if (MoreStrings::iequals(token,"BlendEnable"))                {     _tokenizer.assume_next_token("=");
                                                                            target->set_blend_enable(static_cast<DTboolean>(_tokenizer.next_token_number()));
                                                                            continue;   }
        if (MoreStrings::iequals(token,"BlendFunc"))                  {     _tokenizer.assume_next_token("=");
                                                                            target->set_src_blend((DT3GLBlend) _tokenizer.next_token_number());
                                                                            target->set_dest_blend((DT3GLBlend) _tokenizer.next_token_number());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"BlendAlphaFunc"))             {     _tokenizer.assume_next_token("=");
                                                                            target->set_src_blend_alpha((DT3GLBlend) _tokenizer.next_token_number());
                                                                            target->set_dest_blend_alpha((DT3GLBlend) _tokenizer.next_token_number());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"ColorMask"))                  {     _tokenizer.assume_next_token("=");
                                                                            target->set_render_target_write_mask_r(static_cast<DTboolean>(_tokenizer.next_token_number()));
                                                                            target->set_render_target_write_mask_g(static_cast<DTboolean>(_tokenizer.next_token_number()));
                                                                            target->set_render_target_write_mask_b(static_cast<DTboolean>(_tokenizer.next_token_number()));
                                                                            target->set_render_target_write_mask_a(static_cast<DTboolean>(_tokenizer.next_token_number()));
                                                                            continue;   }
    
		_tokenizer.syntax_error("Unknown Token " + token);
	};
}

void ImporterMaterialMAT::parse_rasterizer_block (MaterialResource *target)
{
	_tokenizer.assume_next_token("{");

	while (true) {
		std::string token = _tokenizer.next_token_string();
		
		// Handle Preprocessor
		if (_tokenizer.parse_preprocessor_macros(token))
			continue;
		
		// Are we at the end of the block
		if (token == "}")	break;
        
        if (MoreStrings::iequals(token,"CullMode"))                   {     _tokenizer.assume_next_token("=");
                                                                            target->set_cull_mode((DT3GLCullMode) _tokenizer.next_token_number());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"DepthBias"))                  {     _tokenizer.assume_next_token("=");
                                                                            target->set_depth_bias(_tokenizer.next_token_number());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"SlopeScaledDepthBias"))       {     _tokenizer.assume_next_token("=");
                                                                            target->set_slope_scaled_depth_bias(_tokenizer.next_token_number());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"DepthBiasClamp"))             {     _tokenizer.assume_next_token("=");
                                                                            target->set_depth_bias_clamp(_tokenizer.next_token_number());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"ScissorEnable"))              {     _tokenizer.assume_next_token("=");
                                                                            target->set_scissor_enable(static_cast<DTboolean>(_tokenizer.next_token_number()));
                                                                            continue;   }
        
		_tokenizer.syntax_error("Unknown Token " + token);
	};
}

void ImporterMaterialMAT::parse_sampler_block (MaterialResource *target)
{
	_tokenizer.assume_next_token("{");

	while (true) {
		std::string token = _tokenizer.next_token_string();
		
		// Handle Preprocessor
		if (_tokenizer.parse_preprocessor_macros(token))
			continue;
		
		// Are we at the end of the block
		if (token == "}")	break;
        
        if (MoreStrings::iequals(token,"Filter"))                     {     _tokenizer.assume_next_token("=");
                                                                            target->set_filter((DT3GLFilterMode) _tokenizer.next_token_number());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"AddressU"))                   {     _tokenizer.assume_next_token("=");
                                                                            target->set_address_u((DT3GLAddressMode) _tokenizer.next_token_number());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"AddressV"))                   {     _tokenizer.assume_next_token("=");
                                                                            target->set_address_v((DT3GLAddressMode) _tokenizer.next_token_number());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"AddressW"))                   {     _tokenizer.assume_next_token("=");
                                                                            target->set_address_w((DT3GLAddressMode) _tokenizer.next_token_number());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"MipLODBias"))                 {     _tokenizer.assume_next_token("=");
                                                                            target->set_mip_lod_bias(_tokenizer.next_token_number());
                                                                            continue;   }
        if (MoreStrings::iequals(token,"MaxAnisotropy"))              {     _tokenizer.assume_next_token("=");
                                                                            target->set_max_anisotropy(_tokenizer.next_token_number());
                                                                            continue;   }
        
		_tokenizer.syntax_error("Unknown Token " + token);
	};
}

void ImporterMaterialMAT::parse_tex_block (MaterialResource *target)
{
    DTint tex_slot = static_cast<DTint>(_tokenizer.next_token_number());
    
    target->set_current_unit(tex_slot);

	_tokenizer.assume_next_token("{");

	while (true) {
		std::string token = _tokenizer.next_token_string();
		
		// Handle Preprocessor
		if (_tokenizer.parse_preprocessor_macros(token))
			continue;
		
		// Are we at the end of the block
		if (token == "}")	break;
        
        if (MoreStrings::iequals(token,"Tex2DArgs"))                  {     _tokenizer.assume_next_token("=");
                                                                            std::string file = _tokenizer.next_token_string_no_substitute();
                                                                            std::string args = _tokenizer.next_token_string();
                                                                            target->set_texture_2D( TextureResource2D::import_resource(FilePath(file), args) );
                                                                            continue;   }
        if (MoreStrings::iequals(token,"Tex3DArgs"))                  {     _tokenizer.assume_next_token("=");
                                                                            std::string file = _tokenizer.next_token_string_no_substitute();
                                                                            std::string args = _tokenizer.next_token_string();
                                                                            target->set_texture_3D( TextureResource3D::import_resource(FilePath(file), args) );
                                                                            continue;   }
        if (MoreStrings::iequals(token,"TexCubeArgs"))                {     _tokenizer.assume_next_token("=");
                                                                            std::string file = _tokenizer.next_token_string_no_substitute();
                                                                            std::string args = _tokenizer.next_token_string();
                                                                            target->set_texture_cube( TextureResourceCube::import_resource(FilePath(file), args) );
                                                                            continue;   }
        if (MoreStrings::iequals(token,"Sampler"))                    {     parse_sampler_block (target);   continue;   }
        if (MoreStrings::iequals(token,"SubRect"))                    {     _tokenizer.assume_next_token("=");
                                                                            DTfloat x = _tokenizer.next_token_number();
                                                                            DTfloat y = _tokenizer.next_token_number();
                                                                            DTfloat scale_x = _tokenizer.next_token_number();
                                                                            DTfloat scale_y = _tokenizer.next_token_number();
                                                                            
                                                                            target->set_translation(Vector3(x,y,0.0F));
                                                                            target->set_scale(Vector3(scale_x,scale_y,1.0F));
                                                                            continue;   }
        
		_tokenizer.syntax_error("Unknown Token " + token);
	};
}

//==============================================================================
//==============================================================================

void ImporterMaterialMAT::parse_material_block (MaterialResource *target)
{
	_tokenizer.assume_next_token("{");

	while (true) {
		std::string token = _tokenizer.next_token_string();
	
		// Handle Preprocessor
		if (_tokenizer.parse_preprocessor_macros(token))
			continue;
			
		// Are we at the end of the block
		if (token == "}")	break;
		
		if (MoreStrings::iequals(token,"DepthStencil"))    {	parse_depth_stencil_block(target);  continue;	}
		if (MoreStrings::iequals(token,"Blend"))           {	parse_blend_block(target);          continue;	}
		if (MoreStrings::iequals(token,"Rasterizer"))      {	parse_rasterizer_block(target);     continue;	}
		if (MoreStrings::iequals(token,"Tex"))             {	parse_tex_block(target);            continue;	}
	};
	
}

//==============================================================================
//==============================================================================

DTerr ImporterMaterialMAT::import(MaterialResource *target, std::string args)
{
    // Load in defines for material
    const DTcharacter* header =
        "#define TRUE 1\n"
        "#define FALSE 0\n"

        "#define ACCESS_CPU_NONE 0\n"
        "#define ACCESS_CPU_READ 1\n"
        "#define ACCESS_CPU_WRITE 2\n"
        "#define ACCESS_GPU_NONE 0\n"
        "#define ACCESS_GPU_READ 4\n"
        "#define ACCESS_GPU_WRITE 8\n"

        "#define COMPARE_ALWAYS 0\n"
        "#define COMPARE_NEVER 1\n"
        "#define COMPARE_EQUAL 2\n"
        "#define COMPARE_NOTEQUAL 3\n"
        "#define COMPARE_GREATER 4\n"
        "#define COMPARE_GEQUAL 5\n"
        "#define COMPARE_LESS 6\n"
        "#define COMPARE_LEQUAL 7\n"

        "#define STENCILOP_KEEP 0\n"
        "#define STENCILOP_ZERO 1\n"
        "#define STENCILOP_REPLACE 2\n"
        "#define STENCILOP_INCR 3\n"
        "#define STENCILOP_DECR 4\n"
        "#define STENCILOP_INCR_WRAP 5\n"
        "#define STENCILOP_DECR_WRAP 6\n"
        "#define STENCILOP_INVERT 7\n"

        "#define BLEND_ZERO 0\n"
        "#define BLEND_ONE 1\n"
        "#define BLEND_SRC_ALPHA 2\n"
        "#define BLEND_ONE_MINUS_SRC_ALPHA 3\n"
        "#define BLEND_DST_ALPHA 4\n"
        "#define BLEND_ONE_MINUS_DST_ALPHA 5\n"
        "#define BLEND_SRC_COLOR 6\n"
        "#define BLEND_DST_COLOR 7\n"
        "#define BLEND_ONE_MINUS_SRC_COLOR 8\n"
        "#define BLEND_ONE_MINUS_DST_COLOR 9\n"

        "#define CULL_FRONT 0\n"
        "#define CULL_BACK 1\n"
        "#define CULL_NONE 2\n"

        "#define FILTER_NEAREST 0\n"
        "#define FILTER_LINEAR 1\n"
        "#define FILTER_NEAREST_MIPMAP 2\n"
        "#define FILTER_LINEAR_MIPMAP 3\n"

        "#define ADDRESS_REPEAT 0\n"
        "#define ADDRESS_CLAMP_TO_EDGE 1\n";
    
    _tokenizer.set_token_stream(header, true);


	DTerr err;
	if ((err = _tokenizer.load_token_stream (target->path(), true)) != DT3_ERR_NONE)
		return DT3_ERR_FILE_OPEN_FAILED;
			
	while (!_tokenizer.is_done()) {
		std::string token = _tokenizer.next_token_string();
		
		// Handle Preprocessor
		if (_tokenizer.parse_preprocessor_macros(token))
			continue;
		
		if (MoreStrings::iequals(token,"Material"))		{	parse_material_block(target);		continue;	}
		
	};
	
    target->add_dependencies(_tokenizer.dependencies());

	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

} // DT3

