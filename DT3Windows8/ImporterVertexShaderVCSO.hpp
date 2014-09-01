#ifndef DT2_IMPORTERVERTEXSHADERVCSO
#define DT2_IMPORTERVERTEXSHADERVCSO
//==============================================================================
///	
///	File: ImporterVertexShaderVCSO.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "ImporterVertexShader.hpp"
#include "Tokenizer.hpp"

//==============================================================================
/// Namespace
//==============================================================================

namespace DT2 {

//==============================================================================
/// Forward declarations
//==============================================================================

class VertexShaderResource;

//==============================================================================
/// Class
//==============================================================================

class ImporterVertexShaderVCSO: public ImporterVertexShader {
    public:
        DEFINE_TYPE(ImporterVertexShaderVCSO,ImporterVertexShader)
		DEFINE_CREATE
         
										ImporterVertexShaderVCSO	(void);	
    
	private:
										ImporterVertexShaderVCSO	(const ImporterVertexShaderVCSO &rhs);
        ImporterVertexShaderVCSO &	    operator =					(const ImporterVertexShaderVCSO &rhs);
    
    public:
        virtual							~ImporterVertexShaderVCSO	(void);
    
    public: 
		/// Imports a sound group into an VertexShaderResource
		/// \param target object to import sound into
		/// \param path path to sound file
		virtual DTerr					import						(VertexShaderResource *target, String args);
		
	private:
        void                            parseResourceBlock          (VertexShaderResource *target);
        void                            parseResourcesBlock         (VertexShaderResource *target);
        void                            parseShaderBlock            (VertexShaderResource *target);
        void                            parseProgramBlock           (VertexShaderResource *target);
		
		Tokenizer						_tokenizer;

};

//==============================================================================
//==============================================================================

} // DT2

#endif
