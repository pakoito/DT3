#ifndef DT3_SCRIPTINGMATERIAL
#define DT3_SCRIPTINGMATERIAL
//==============================================================================
///	
///	File: ScriptingGeometry.hpp
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

class GeometryResource;

//==============================================================================
/// Loader for geometry group.
//==============================================================================

class ScriptingGeometry: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingGeometry,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                                        ScriptingGeometry		(void);
                                                        ScriptingGeometry		(const ScriptingGeometry &rhs);
        ScriptingGeometry &                             operator =				(const ScriptingGeometry &rhs);
        virtual                                         ~ScriptingGeometry		(void);
    
        virtual void                                    archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void                                    initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean                                       compute					(const PlugBase *plug);


		/// Set the geometry
		/// \param attr geometry
        void                                            set_geometry_property   (const std::shared_ptr<GeometryResource> &geometry);

		/// Get the geometry
		/// \return geometry
        const std::shared_ptr<GeometryResource>&        geometry_property       (void) const				{   return _geometry;	}

	private:		
		std::shared_ptr<GeometryResource>               _geometry;
		Plug<std::shared_ptr<GeometryResource>>         _out_geometry;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
