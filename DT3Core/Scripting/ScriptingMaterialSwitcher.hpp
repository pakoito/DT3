#ifndef DT3_SCRIPTINGMATERIALSWITCHER
#define DT3_SCRIPTINGMATERIALSWITCHER
//==============================================================================
///	
///	File: ScriptingMaterialSwitcher.hpp
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
/// Switcher for materials.
//==============================================================================

class ScriptingMaterialSwitcher: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingMaterialSwitcher,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingMaterialSwitcher	(void);	
									ScriptingMaterialSwitcher	(const ScriptingMaterialSwitcher &rhs);
        ScriptingMaterialSwitcher &	operator =					(const ScriptingMaterialSwitcher &rhs);	
        virtual                     ~ScriptingMaterialSwitcher	(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

	private:		
		Plug<DTboolean>                             _in1;
		Plug<std::shared_ptr<MaterialResource>>    _in_material1;
		Plug<DTboolean>                             _in2;
		Plug<std::shared_ptr<MaterialResource>>    _in_material2;
		Plug<DTboolean>                             _in3;
		Plug<std::shared_ptr<MaterialResource>>    _in_material3;
		Plug<DTboolean>                             _in4;
		Plug<std::shared_ptr<MaterialResource>>    _in_material4;

		Plug<std::shared_ptr<MaterialResource>>    _out;
        DTboolean                                   _buffer_output;
                
};

//==============================================================================
//==============================================================================

} // DT3

#endif
