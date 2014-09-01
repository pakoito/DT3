#ifndef DT3_IMPORTERKEYEDSPRITEANIMATION
#define DT3_IMPORTERKEYEDSPRITEANIMATION
//==============================================================================
///	
///	File: ImporterKeyedSpriteAnimation.hpp
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

class KeyedSpriteAnimationResource;

//==============================================================================
/// Class
//==============================================================================

class ImporterKeyedSpriteAnimation: public BaseClass {
    public:
        DEFINE_TYPE(ImporterKeyedSpriteAnimation,BaseClass)

         
                                        ImporterKeyedSpriteAnimation	(void);	
    
	private:
                                        ImporterKeyedSpriteAnimation	(const ImporterKeyedSpriteAnimation &rhs);
        ImporterKeyedSpriteAnimation &	operator =                      (const ImporterKeyedSpriteAnimation &rhs);
    
    public:
        virtual                         ~ImporterKeyedSpriteAnimation	(void);
    
	public:
		/// Imports a KeyedSpriteAnimation
		/// \param target object to import animation into
		/// \param args arguments to importer
        /// \return error code
		virtual DTerr					import							(KeyedSpriteAnimationResource *target, std::string args) = 0;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
