#ifndef DT3_SCRIPTINGPARTICLEPATH
#define DT3_SCRIPTINGPARTICLEPATH
//==============================================================================
///	
///	File: ScriptingParticlePath.hpp
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

class Particles;

//==============================================================================
/// Path for Particle System.
//==============================================================================

class ScriptingParticlePath: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticlePath,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingParticlePath	(void);	
									ScriptingParticlePath	(const ScriptingParticlePath &rhs);
        ScriptingParticlePath &     operator =				(const ScriptingParticlePath &rhs);	
        virtual						~ScriptingParticlePath	(void);

        virtual void				archive                 (const std::shared_ptr<Archive> &archive);

	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);


		/// Sets number of points along the path
		/// \param s number of points
		virtual void				set_num_points			(const DTsize s)                    {	return _points.resize(s);	}

		/// Gets number of points along the path
		/// \return number of points
		virtual DTsize				num_points              (void) const                        {	return _points.size();		}



		/// Sets the position of a point along the path
		/// \param k index of point
		/// \param point point
		void						set_point               (DTint k, const Vector3 &point)    {	_points[k].value = point;		_out.set_dirty();	}

		/// Gets the position of a point along the path
		/// \param k index of point
		/// \return point
		const Vector3 &				point                   (DTint k)	const                   {	return _points[k].value;        }

		/// Sets the tangent of a point along the path
		/// \param k index of point
		/// \param tangent tangent
		void						set_point_tangent       (DTint k, const Vector3 &tangent)	{	_points[k].tangent = tangent;	_out.set_dirty();	}

		/// Gets the tangent of a point along the path
		/// \param k index of point
		/// \return tangent
		const Vector3 &				point_tangent           (DTint k)	const					{	return _points[k].tangent;		}



	private:
        friend class EdManipScriptingParticlePath;		
        
        void                        interpolate         (DTfloat t, Vector3 &translation) const;
        

		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;
		Plug<DTfloat>                                       _speed;
		Plug<DTfloat>                                       _bias;

		struct keyframe {
            keyframe(void)  {   value = tangent = Vector3(0.0F,0.0F,0.0F);  }
            
			Vector3	value;
			Vector3	tangent;
		};
		
		std::vector<keyframe>		_points;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
