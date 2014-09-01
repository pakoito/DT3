#ifndef DT3_IMPORTERKEYEDSPRITE
#define DT3_IMPORTERKEYEDSPRITE
//==============================================================================
///	
///	File: ImporterKeyedSprite.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class KeyedSpriteResource;

//==============================================================================
/// Class
//==============================================================================

class ImporterKeyedSprite: public BaseClass {
    public:
        DEFINE_TYPE(ImporterKeyedSprite,BaseClass)

         
                                        ImporterKeyedSprite     (void);	
    
	private:
                                        ImporterKeyedSprite     (const ImporterKeyedSprite &rhs);
        ImporterKeyedSprite &           operator =              (const ImporterKeyedSprite &rhs);
    
    public:
        virtual                         ~ImporterKeyedSprite	(void);
    
	public:
		/// Imports a Keyed Sprite
		/// \param target object to import animation into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr				import						(KeyedSpriteResource *target, std::string args) = 0;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
