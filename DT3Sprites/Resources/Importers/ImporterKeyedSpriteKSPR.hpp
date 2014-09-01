#ifndef DT3_IMPORTERKEYEDSPRITEKSPR
#define DT3_IMPORTERKEYEDSPRITEKSPR
//==============================================================================
///	
///	File: ImporterKeyedSpriteKSPR.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Sprites/Resources/Importers/ImporterKeyedSprite.hpp"

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class KeyedSpriteResource;

//==============================================================================
/// Class
//==============================================================================

class ImporterKeyedSpriteKSPR: public ImporterKeyedSprite {
    public:
        DEFINE_TYPE(ImporterKeyedSpriteKSPR,ImporterKeyedSprite)
		DEFINE_CREATE
         
										ImporterKeyedSpriteKSPR		(void);	
    
	private:
										ImporterKeyedSpriteKSPR		(const ImporterKeyedSpriteKSPR &rhs);
        ImporterKeyedSpriteKSPR &		operator =					(const ImporterKeyedSpriteKSPR &rhs);
    
    public:
        virtual							~ImporterKeyedSpriteKSPR	(void);
    
    public: 
		/// Imports a KeyedSpriteResource
		/// \param target object to import animation into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr					import						(KeyedSpriteResource *target, std::string args);
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
