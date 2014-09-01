#ifndef DT3_SCRIPTINGMATERIAL
#define DT3_SCRIPTINGMATERIAL
//==============================================================================
///	
///	File: ScriptingMaterial.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class MaterialResource;

//==============================================================================
/// Loader for materials.
//==============================================================================

class ScriptingMaterial: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingMaterial,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingMaterial		(void);	
									ScriptingMaterial		(const ScriptingMaterial &rhs);
        ScriptingMaterial &			operator =				(const ScriptingMaterial &rhs);	
        virtual                     ~ScriptingMaterial		(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void                                initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean                                   compute					(const PlugBase *plug);


		/// Sets the material property
		/// \param attr material
        void                                        set_material_property   (const std::shared_ptr<MaterialResource> &material);

		/// Gets the material property
		/// \return material
        const std::shared_ptr<MaterialResource>&    material_property       (void) const        {   return _material;	}

	private:		
		std::shared_ptr<MaterialResource>           _material;
		Plug<std::shared_ptr<MaterialResource>>    _out_material;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
