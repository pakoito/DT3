#ifndef DT3_IMPORTERKEYEDSPRITEANIMATIONKANI
#define DT3_IMPORTERKEYEDSPRITEANIMATIONKANI
//==============================================================================
///	
///	File: ImporterKeyedSpriteAnimationKANI.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Sprites/Resources/Importers/ImporterKeyedSpriteAnimation.hpp"
#include "Tokenizer.hpp"

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class SpriteAnimationFrame;

//==============================================================================
/// Class
//==============================================================================

class ImporterKeyedSpriteAnimationKANI: public ImporterKeyedSpriteAnimation {
    public:
        DEFINE_TYPE(ImporterKeyedSpriteAnimationKANI,ImporterKeyedSpriteAnimation)
		DEFINE_CREATE
         
											ImporterKeyedSpriteAnimationKANI	(void);	
    
	private:
											ImporterKeyedSpriteAnimationKANI	(const ImporterKeyedSpriteAnimationKANI &rhs);
        ImporterKeyedSpriteAnimationKANI &	operator =							(const ImporterKeyedSpriteAnimationKANI &rhs);
    
    public:
        virtual								~ImporterKeyedSpriteAnimationKANI	(void);
    
    public: 
		/// Imports a KeyedSpriteAnimationResource
		/// \param target object to import animation into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr						import								(KeyedSpriteAnimationResource *target, std::string args);
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
