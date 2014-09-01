#ifndef DT3_SCRIPTINGMATERIALANIMATOR
#define DT3_SCRIPTINGMATERIALANIMATOR
//==============================================================================
///	
///	File: ScriptingMaterialAnimator.hpp
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

class ScriptingMaterialAnimator: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingMaterialAnimator,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingMaterialAnimator	(void);	
									ScriptingMaterialAnimator	(const ScriptingMaterialAnimator &rhs);
        ScriptingMaterialAnimator &	operator =					(const ScriptingMaterialAnimator &rhs);	
        virtual                     ~ScriptingMaterialAnimator	(void);
    
        virtual void                archive                     (const std::shared_ptr<Archive> &archive);
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world                (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world           (void);

	public:
		/// Called to initialize the object
		virtual void				initialize                  (void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute                     (const PlugBase *plug);

		/// Sets the number of materials
		/// \param num_materials number of materials
        void                        set_num_materials           (DTsize num_materials)  {   _materials.resize(num_materials);   }
    
		/// Gets the number of materials
		/// \return number of materials
        DTsize                      num_materials               (void) const            {   return _materials.size();           }
    
        /// Registered with world to tick this node
		/// \param dt delta time
        void                        tick                        (const DTfloat dt);

	private:		
        std::vector<std::shared_ptr<MaterialResource>>      _materials;

		Plug<DTboolean>                             _active;
		Plug<DTfloat>                               _period;
    
        DTfloat                                     _out_time;
		Plug<std::shared_ptr<MaterialResource>>    _out;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
