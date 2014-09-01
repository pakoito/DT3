#ifndef DT3_IMPORTERSHADERSHDR
#define DT3_IMPORTERSHADERSHDR
//==============================================================================
///	
///	File: ImporterShaderSHDR.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/Importers/ImporterShader.hpp"
#include "DT3Core/Types/Utility/Tokenizer.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class ShaderResource;

//==============================================================================
/// Class
//==============================================================================

class ImporterShaderSHDR: public ImporterShader {
    public:
        DEFINE_TYPE(ImporterShaderSHDR,ImporterShader)
		DEFINE_CREATE
         
										ImporterShaderSHDR              (void);
    
	private:
										ImporterShaderSHDR              (const ImporterShaderSHDR &rhs);
        ImporterShaderSHDR &            operator =                      (const ImporterShaderSHDR &rhs);
    
    public:
        virtual							~ImporterShaderSHDR             (void);
    
    public: 
		/// Imports a sound group into an ShaderResource
		/// \param target object to import sound into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr					import                          (ShaderResource *target, std::string args);
		
	private:
        void                            parse_fragment_shader_implementation_block  (ShaderResource *target);
        void                            parse_geometry_shader_implementation_block  (ShaderResource *target);
        void                            parse_vertex_shader_implementation_block    (ShaderResource *target);

		void							parse_fragment_shader_block     (ShaderResource *target);
		void							parse_geometry_shader_block     (ShaderResource *target);
		void							parse_vertex_shader_block       (ShaderResource *target);
		
		void							parse_shaders_block             (ShaderResource *target);
		
		void							parse_attrib                    (ShaderResource *target);
		void							parse_standard_attrib           (ShaderResource *target);
    
		void							parse_uniform_v                 (ShaderResource *target);
		void							parse_uniform_m                 (ShaderResource *target);
		void							parse_standard_uniform          (ShaderResource *target);

		void							parse_program_block             (ShaderResource *target);
		
		Tokenizer						_tokenizer;

};

//==============================================================================
//==============================================================================

} // DT3

#endif
