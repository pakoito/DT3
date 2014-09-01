#ifndef DT3_SCRIPTINGSPLINE
#define DT3_SCRIPTINGSPLINE
//==============================================================================
///	
///	File: ScriptingSpline.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"
#include <vector>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Constant speed spline.
//==============================================================================

class ScriptingSpline: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingSpline,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingSpline         (void);	
									ScriptingSpline         (const ScriptingSpline &rhs);
        ScriptingSpline &           operator =              (const ScriptingSpline &rhs);	
        virtual						~ScriptingSpline        (void);

        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
        virtual void				archive_done            (const std::shared_ptr<Archive> &archive);

	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);
        
		/// Sets number of points along the path
		/// \param s number of points
        void                        set_num_points          (DTsize num);

		/// Gets number of points along the path
		/// \return number of points
        DTsize                      num_points              (void) const;

		/// Sets the transform of a point along the path
		/// \param k index of point
		/// \param transform transform
		void						set_point_transform     (DTuint k, const Matrix4 &transform);
        
		/// Gets the transform of a point along the path
		/// \param k index of point
		/// \return transform
		const Matrix4 &				point_transform         (DTuint k) const;

		/// Sets the tangent of a point along the path
		/// \param k index of point
		/// \param tangent tangent
		void						set_point_tangent       (DTuint k, const Vector3 &tangent);

		/// Gets the tangent of a point along the path
		/// \param k index of point
		/// \return tangent
		const Vector3 &				point_tangent           (DTuint k) const;

		/// Length of spline
		/// \return length
        DTfloat                     max_distance            (void);

		/// Interpolate the spline
		/// \param d distance
		/// \param transform interpolated transform
		void						interpolate				(DTfloat d, Matrix4 &transform);

	private:	
        
        void                        interpolate             (DTint i0, DTint i1, DTfloat d, Matrix4 &transform);
        void                        process_distances       (void);
    
        DTboolean                   _constant_speed;
		Plug<DTfloat>				_d;
        
        Plug<Matrix4>               _transform;
        Plug<Matrix3>				_orientation;
		Plug<Vector3>				_translation;

        struct Pt {
            Pt (void)
                :   _transform  (Matrix4::identity()),
                    _tangent    (0.0F,0.0F,0.0F),
                    _length     (0.0F),
                    _distance   (0.0F),
                    //_a0         (0.0F),
                    _a1         (0.0F),
                    _a2         (0.0F),
                    _a3         (0.0F)
            {}
        
            Matrix4                 _transform;
            Vector3                 _tangent;
            DTfloat                 _length;
            DTfloat                 _distance;

            //DTfloat                 _a0;  // a0 is always 0
            DTfloat                 _a1;
            DTfloat                 _a2;
            DTfloat                 _a3;
        };
            
        std::vector<Pt>             _transforms;
        
		mutable DTint				_keyframe_cache;

        DTboolean                   _dirty;
                    	
};

//==============================================================================
//==============================================================================

} // DT3

#endif
