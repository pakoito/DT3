#ifndef DT3_SCRIPTINGTEXTURE
#define DT3_SCRIPTINGTEXTURE
//==============================================================================
///	
///	File: ScriptingTexture2D.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class TextureResource2D;

//==============================================================================
/// Loader for textures.
//==============================================================================

class ScriptingTexture2D: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingTexture2D,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingTexture2D  (void);	
									ScriptingTexture2D  (const ScriptingTexture2D &rhs);
        ScriptingTexture2D &		operator =          (const ScriptingTexture2D &rhs);
        virtual                     ~ScriptingTexture2D (void);
    
        virtual void                archive             (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void                                initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean                                   compute					(const PlugBase *plug);


		/// Set the texture
		/// \param attr texture 
        void                                        set_texture_property    (const std::shared_ptr<TextureResource2D> &texture);

		/// Get the texture
		/// \return texture 
        const std::shared_ptr<TextureResource2D>&   texture_property        (void) const    {   return _texture;	}

	private:		
		std::shared_ptr<TextureResource2D>          _texture;
		Plug<std::shared_ptr<TextureResource2D>>   _out_texture;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
