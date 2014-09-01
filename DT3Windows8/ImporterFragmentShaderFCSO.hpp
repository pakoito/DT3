#ifndef DT2_IMPORTERFRAGMENTSHADERFCSO
#define DT2_IMPORTERFRAGMENTSHADERFCSO
//==============================================================================
///	
///	File: ImporterFragmentShaderFCSO.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "ImporterFragmentShader.hpp"
#include "Tokenizer.hpp"

//==============================================================================
/// Namespace
//==============================================================================

namespace DT2 {

//==============================================================================
/// Forward declarations
//==============================================================================

class FragmentShaderResource;

//==============================================================================
/// Class
//==============================================================================

class ImporterFragmentShaderFCSO: public ImporterFragmentShader {
    public:
        DEFINE_TYPE(ImporterFragmentShaderFCSO,ImporterFragmentShader)
		DEFINE_CREATE
         
										ImporterFragmentShaderFCSO	(void);	
    
	private:
										ImporterFragmentShaderFCSO	(const ImporterFragmentShaderFCSO &rhs);
        ImporterFragmentShaderFCSO &	operator =						(const ImporterFragmentShaderFCSO &rhs);
    
    public:
        virtual							~ImporterFragmentShaderFCSO	(void);
    
    public: 
		/// Imports a fragment shader into an FragmentShaderResource
		/// \param target object to import sound into
		/// \param path path to sound file
		virtual DTerr					import						(FragmentShaderResource *target, String args);
		
	private:
        void                            parseResourceBlock          (FragmentShaderResource *target);
        void                            parseResourcesBlock         (FragmentShaderResource *target);
        void                            parseShaderBlock            (FragmentShaderResource *target);
        void                            parseProgramBlock           (FragmentShaderResource *target);
		
		Tokenizer						_tokenizer;

};

//==============================================================================
//==============================================================================

} // DT2

#endif
