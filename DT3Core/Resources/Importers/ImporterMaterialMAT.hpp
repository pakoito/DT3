#ifndef DT3_IMPORTERMATERIALMAT
#define DT3_IMPORTERMATERIALMAT
//==============================================================================
///	
///	File: ImporterMaterialMAT.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Resources/Importers/ImporterMaterial.hpp"
#include "DT3Core/Types/Utility/Tokenizer.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class MaterialResource;

//==============================================================================
/// Class
//==============================================================================

class ImporterMaterialMAT: public ImporterMaterial {
    public:
        DEFINE_TYPE(ImporterMaterialMAT,ImporterMaterial)
		DEFINE_CREATE
         
										ImporterMaterialMAT         (void);
    
	private:
										ImporterMaterialMAT         (const ImporterMaterialMAT &rhs);
        ImporterMaterialMAT &			operator =                  (const ImporterMaterialMAT &rhs);
    
    public:
        virtual							~ImporterMaterialMAT        (void);
                
    public:        	
		/// Imports a material into an MaterialResource
		/// \param target object to import material into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr					import                      (MaterialResource *target, std::string args);
		
		
	private:

        void                            parse_depth_stencil_block   (MaterialResource *target);
        void                            parse_blend_block           (MaterialResource *target);
        void                            parse_rasterizer_block      (MaterialResource *target);
        void                            parse_sampler_block         (MaterialResource *target);
        void                            parse_tex_block             (MaterialResource *target);
        void                            parse_material_block        (MaterialResource *target);

		Tokenizer						_tokenizer;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
