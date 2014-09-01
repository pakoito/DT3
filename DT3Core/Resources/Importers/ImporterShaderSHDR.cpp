//==============================================================================
///	
///	File: ImporterShaderSHDR.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/Importers/ImporterShaderSHDR.hpp"
#include "DT3Core/Resources/ResourceTypes/ShaderResource.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/System/Factory.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_IMPORTER(ImporterShaderSHDR,shdr)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ImporterShaderSHDR::ImporterShaderSHDR (void)
{    
	
}
			
ImporterShaderSHDR::~ImporterShaderSHDR (void)
{ 

}

//==============================================================================
//==============================================================================

void ImporterShaderSHDR::parse_fragment_shader_implementation_block (ShaderResource *target)
{
	std::string standard = _tokenizer.next_token_string();

	_tokenizer.assume_next_token("{");

	while (true) {
		std::string token = _tokenizer.next_token_string();
	
		// Handle Preprocessor
		if (_tokenizer.parse_preprocessor_macros(token))
			continue;
			
		// Are we at the end of the block
		if (token == "}")	break;
		
		if (MoreStrings::iequals(token,"Shader")) {
            _tokenizer.assume_next_token("=");
            
#if DT3_OS == DT3_ANDROID || DT3_OS == DT3_IOS
            std::string target_standard = "GLES2";
#else
            std::string target_standard = "GLSL";
#endif

            target->add_fragment_shader (target_standard, _tokenizer.next_token_string());
            
            continue;
        }
	};
	
}

//==============================================================================
//==============================================================================

void ImporterShaderSHDR::parse_geometry_shader_implementation_block (ShaderResource *target)
{
	std::string standard = _tokenizer.next_token_string();

	_tokenizer.assume_next_token("{");

	while (true) {
		std::string token = _tokenizer.next_token_string();
	
		// Handle Preprocessor
		if (_tokenizer.parse_preprocessor_macros(token))
			continue;
			
		// Are we at the end of the block
		if (token == "}")	break;
		
		if (MoreStrings::iequals(token,"Shader")) {
            _tokenizer.assume_next_token("=");
            
#if DT3_OS == DT3_ANDROID || DT3_OS == DT3_IOS
            std::string target_standard = "GLES2";
#else
            std::string target_standard = "GLSL";
#endif

            target->add_geometry_shader (target_standard, _tokenizer.next_token_string());

            continue;
        }
	};
	
}

//==============================================================================
//==============================================================================

void ImporterShaderSHDR::parse_vertex_shader_implementation_block (ShaderResource *target)
{
	std::string standard = _tokenizer.next_token_string();

	_tokenizer.assume_next_token("{");

	while (true) {
		std::string token = _tokenizer.next_token_string();
	
		// Handle Preprocessor
		if (_tokenizer.parse_preprocessor_macros(token))
			continue;
			
		// Are we at the end of the block
		if (token == "}")	break;
		
		if (MoreStrings::iequals(token,"Shader")) {
            _tokenizer.assume_next_token("=");

#if DT3_OS == DT3_ANDROID || DT3_OS == DT3_IOS
            std::string target_standard = "GLES2";
#else
            std::string target_standard = "GLSL";
#endif

            target->add_vertex_shader (target_standard, _tokenizer.next_token_string());

            continue;
        }
	};
	
}

//==============================================================================
//==============================================================================

void ImporterShaderSHDR::parse_fragment_shader_block (ShaderResource *target)
{
	_tokenizer.assume_next_token("{");

	while (true) {
		std::string token = _tokenizer.next_token_string();
	
		// Handle Preprocessor
		if (_tokenizer.parse_preprocessor_macros(token))
			continue;
			
		// Are we at the end of the block
		if (token == "}")	break;
		
		if (MoreStrings::iequals(token,"Implementation"))			{	parse_fragment_shader_implementation_block(target);		continue;	}
	};
    
}

void ImporterShaderSHDR::parse_geometry_shader_block (ShaderResource *target)
{
	_tokenizer.assume_next_token("{");

	while (true) {
		std::string token = _tokenizer.next_token_string();
	
		// Handle Preprocessor
		if (_tokenizer.parse_preprocessor_macros(token))
			continue;
			
		// Are we at the end of the block
		if (token == "}")	break;
		
		if (MoreStrings::iequals(token,"Implementation"))			{	parse_geometry_shader_implementation_block(target);		continue;	}
	};
    
}

void ImporterShaderSHDR::parse_vertex_shader_block (ShaderResource *target)
{
	_tokenizer.assume_next_token("{");

	while (true) {
		std::string token = _tokenizer.next_token_string();
	
		// Handle Preprocessor
		if (_tokenizer.parse_preprocessor_macros(token))
			continue;
			
		// Are we at the end of the block
		if (token == "}")	break;
		
		if (MoreStrings::iequals(token,"Implementation"))			{	parse_vertex_shader_implementation_block(target);		continue;	}
	};
    
}

//==============================================================================
//==============================================================================

void ImporterShaderSHDR::parse_shaders_block (ShaderResource *target)
{
	_tokenizer.assume_next_token("{");

	while (true) {
		std::string token = _tokenizer.next_token_string();
	
		// Handle Preprocessor
		if (_tokenizer.parse_preprocessor_macros(token))
			continue;
			
		// Are we at the end of the block
		if (token == "}")	break;
		
		if (MoreStrings::iequals(token,"FragmentShaderBlock"))      {	parse_fragment_shader_block(target);	continue;	}
		if (MoreStrings::iequals(token,"GeometryShaderBlock"))      {	parse_geometry_shader_block(target);	continue;	}
		if (MoreStrings::iequals(token,"VertexShaderBlock"))        {	parse_vertex_shader_block(target);		continue;	}
	};
	
}

//==============================================================================
//==============================================================================

void ImporterShaderSHDR::parse_attrib (ShaderResource *target)
{
	_tokenizer.assume_next_token("=");
    std::string name = _tokenizer.next_token_string();
	target->add_attrib(name);
}

void ImporterShaderSHDR::parse_standard_attrib (ShaderResource *target)
{
	_tokenizer.assume_next_token("=");
    DTint standard_attrib = static_cast<DTint>(_tokenizer.next_token_number());
    std::string name = _tokenizer.next_token_string();
	target->add_attrib(name, (DT3GLStandardAttrib) standard_attrib);
}

//==============================================================================
//==============================================================================

void ImporterShaderSHDR::parse_uniform_v (ShaderResource *target)
{
	_tokenizer.assume_next_token("=");
    
    DTuint array_size = 1;
    
    std::string uniform = _tokenizer.next_token_string();
    std::string::size_type open_bracket = uniform.find('[');
    std::string::size_type close_bracket = uniform.find(']');
    
    if (open_bracket != std::string::npos && close_bracket != std::string::npos && close_bracket > open_bracket) {
        std::string num = uniform.substr(open_bracket+1, close_bracket-open_bracket-1);
        array_size = MoreStrings::cast_from_string<DTuint>(num);
        
        uniform = uniform.substr(open_bracket);
    }
    
    target->add_uniform (uniform,DT3GL_UNIFORM_FORMAT_4_FLOAT,array_size);
}

void ImporterShaderSHDR::parse_uniform_m (ShaderResource *target)
{
	_tokenizer.assume_next_token("=");

    DTuint array_size = 1;
        
    std::string uniform = _tokenizer.next_token_string();
    std::string::size_type open_bracket = uniform.find('[');
    std::string::size_type close_bracket = uniform.find(']');
    
    if (open_bracket != std::string::npos && close_bracket != std::string::npos && close_bracket > open_bracket) {
        std::string num = uniform.substr(open_bracket+1, close_bracket-open_bracket-1);
        array_size = MoreStrings::cast_from_string<DTuint>(num);
        
        uniform = uniform.substr(open_bracket);
    }

    target->add_uniform (uniform,DT3GL_UNIFORM_FORMAT_16_FLOAT,array_size);
}

void ImporterShaderSHDR::parse_standard_uniform (ShaderResource *target)
{
	_tokenizer.assume_next_token("=");

    DTint standard_attrib = static_cast<DTint>(_tokenizer.next_token_number());
    std::string name = _tokenizer.next_token_string();
    
    DT3GLUniformFormat fmt;
    
    switch ((DT3GLStandardUniform) standard_attrib) {
        
        case DT3GL_UNIFORM_MODELVIEW:
        case DT3GL_UNIFORM_PROJECTION:
            fmt = DT3GL_UNIFORM_FORMAT_16_FLOAT;
            break;
        
        case DT3GL_UNIFORM_TEX0:
        case DT3GL_UNIFORM_TEX1:
        case DT3GL_UNIFORM_TEX2:
        case DT3GL_UNIFORM_TEX3:
        case DT3GL_UNIFORM_TEX4:
        case DT3GL_UNIFORM_TEX5:
        case DT3GL_UNIFORM_TEX6:
        case DT3GL_UNIFORM_TEX7:
        case DT3GL_UNIFORM_TEX8:
        case DT3GL_UNIFORM_TEX9:
        case DT3GL_UNIFORM_TEX10:
        case DT3GL_UNIFORM_TEX11:
        case DT3GL_UNIFORM_TEX12:
        case DT3GL_UNIFORM_TEX13:
        case DT3GL_UNIFORM_TEX14:
        case DT3GL_UNIFORM_TEX15:
            fmt = DT3GL_UNIFORM_FORMAT_1_INT;
            break;
        
        case DT3GL_UNIFORM_TEX_MATRIX0:
        case DT3GL_UNIFORM_TEX_MATRIX1:
        case DT3GL_UNIFORM_TEX_MATRIX2:
        case DT3GL_UNIFORM_TEX_MATRIX3:
        case DT3GL_UNIFORM_TEX_MATRIX4:
        case DT3GL_UNIFORM_TEX_MATRIX5:
        case DT3GL_UNIFORM_TEX_MATRIX6:
        case DT3GL_UNIFORM_TEX_MATRIX7:
        case DT3GL_UNIFORM_TEX_MATRIX8:
        case DT3GL_UNIFORM_TEX_MATRIX9:
        case DT3GL_UNIFORM_TEX_MATRIX10:
        case DT3GL_UNIFORM_TEX_MATRIX11:
        case DT3GL_UNIFORM_TEX_MATRIX12:
        case DT3GL_UNIFORM_TEX_MATRIX13:
        case DT3GL_UNIFORM_TEX_MATRIX14:
        case DT3GL_UNIFORM_TEX_MATRIX15:
            fmt = DT3GL_UNIFORM_FORMAT_16_FLOAT;
            break;

        default:
            fmt = DT3GL_UNIFORM_FORMAT_1_INT;
            ASSERT(0);
            break;
    }

    target->add_uniform (name,fmt,1, (DT3GLStandardUniform) standard_attrib);
}

//==============================================================================
//==============================================================================

void ImporterShaderSHDR::parse_program_block (ShaderResource *target)
{
	_tokenizer.assume_next_token("{");

	while (true) {
		std::string token = _tokenizer.next_token_string();
	
		// Handle Preprocessor
		if (_tokenizer.parse_preprocessor_macros(token))
			continue;
			
		// Are we at the end of the block
		if (token == "}")	break;
		
		if (MoreStrings::iequals(token,"Attrib"))               {	parse_attrib(target);                   continue;	}
		if (MoreStrings::iequals(token,"StandardAttrib"))       {	parse_standard_attrib(target);          continue;	}
		if (MoreStrings::iequals(token,"UniformV"))             {	parse_uniform_v(target);                continue;	}
		if (MoreStrings::iequals(token,"UniformM"))             {	parse_uniform_m(target);                continue;	}
		if (MoreStrings::iequals(token,"StandardUniform"))      {	parse_standard_uniform(target);         continue;	}

        // NOTE: shaders block is optional
		if (MoreStrings::iequals(token,"Shaders"))              {	parse_shaders_block(target);            continue;	}
        
		if (MoreStrings::iequals(token,"FragmentShaderBlock"))  {	parse_fragment_shader_block(target);	continue;	}
		if (MoreStrings::iequals(token,"GeometryShaderBlock"))  {	parse_geometry_shader_block(target);    continue;	}
		if (MoreStrings::iequals(token,"VertexShaderBlock"))	{	parse_vertex_shader_block(target);		continue;	}
	};
	
}

//==============================================================================
//==============================================================================

DTerr ImporterShaderSHDR::import(ShaderResource *target, std::string args)
{
    // Load in defines for material
    const DTcharacter* header =
        "#define ATTRIB_NONE -1\n"
        "#define ATTRIB_POSITION 0\n"
        "#define ATTRIB_NORMAL 1\n"
        "#define ATTRIB_TEXCOORD0 2\n"
        "#define ATTRIB_TEXCOORD1 3\n"
        "#define ATTRIB_COLOR 4\n"
        "#define UNIFORM_NONE -1\n"
        "#define UNIFORM_MODELVIEW 0\n"
        "#define UNIFORM_PROJECTION 1\n"
        "#define UNIFORM_TEX0 2\n"
        "#define UNIFORM_TEX1 3\n"
        "#define UNIFORM_TEX2 4\n"
        "#define UNIFORM_TEX3 5\n"
        "#define UNIFORM_TEX4 6\n"
        "#define UNIFORM_TEX5 7\n"
        "#define UNIFORM_TEX6 8\n"
        "#define UNIFORM_TEX7 9\n"
        "#define UNIFORM_TEX8 10\n"
        "#define UNIFORM_TEX9 11\n"
        "#define UNIFORM_TEX10 12\n"
        "#define UNIFORM_TEX11 13\n"
        "#define UNIFORM_TEX12 14\n"
        "#define UNIFORM_TEX13 15\n"
        "#define UNIFORM_TEX14 16\n"
        "#define UNIFORM_TEX15 17\n"
        "#define UNIFORM_TEX_MATRIX0 18\n"
        "#define UNIFORM_TEX_MATRIX1 19\n"
        "#define UNIFORM_TEX_MATRIX2 20\n"
        "#define UNIFORM_TEX_MATRIX3 21\n"
        "#define UNIFORM_TEX_MATRIX4 22\n"
        "#define UNIFORM_TEX_MATRIX5 23\n"
        "#define UNIFORM_TEX_MATRIX6 24\n"
        "#define UNIFORM_TEX_MATRIX7 25\n"
        "#define UNIFORM_TEX_MATRIX8 26\n"
        "#define UNIFORM_TEX_MATRIX9 27\n"
        "#define UNIFORM_TEX_MATRIX10 28\n"
        "#define UNIFORM_TEX_MATRIX11 29\n"
        "#define UNIFORM_TEX_MATRIX12 30\n"
        "#define UNIFORM_TEX_MATRIX13 31\n"
        "#define UNIFORM_TEX_MATRIX14 32\n"
        "#define UNIFORM_TEX_MATRIX15 33\n";
    
    _tokenizer.set_token_stream(header, true);
    
	DTerr err;
	if ((err = _tokenizer.load_token_stream (target->path(), true)) != DT3_ERR_NONE)
		return DT3_ERR_FILE_OPEN_FAILED;
	
	while (!_tokenizer.is_done()) {
		std::string token = _tokenizer.next_token_string();
		
		// Handle Preprocessor
		if (_tokenizer.parse_preprocessor_macros(token))
			continue;
		
		if (MoreStrings::iequals(token,"shader"))		{	parse_program_block(target);		continue;	}
		
	};
	
    target->add_dependencies(_tokenizer.dependencies());

	return DT3_ERR_NONE;
}

//==============================================================================
//==============================================================================

} // DT3

