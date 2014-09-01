#ifndef DT3_CAMERAOBJECT
#define DT3_CAMERAOBJECT
//==============================================================================
///	
///	File: CameraObject.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/PlaceableObject.hpp"
#include "DT3Core/Types/Math/Plane.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"
#include "DT3Core/Types/Math/Rectangle.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Box;

//==============================================================================
/// Class
//==============================================================================

class CameraObject: public PlaceableObject {
    public:
        DEFINE_TYPE(CameraObject,PlaceableObject)
		DEFINE_CREATE_AND_CLONE
         
									CameraObject				(void);
									CameraObject                (const CameraObject &rhs);
        CameraObject &				operator =                  (const CameraObject &rhs);	
        virtual                     ~CameraObject               (void);
     
        virtual void                archive                     (const std::shared_ptr<Archive> &archive);

    public:
        /// Accessors for the far plane of the camera (Perspective Camera)
		DEFINE_ACCESSORS		(far_plane, set_far_plane, DTfloat, _far_plane);

        /// Accessors for the near plane of the camera (Perspective Camera)
		DEFINE_ACCESSORS		(near_plane, set_near_plane, DTfloat, _near_plane);

        /// Accessors for the angle of the view frustum (Perspective Camera)
		DEFINE_ACCESSORS		(angle, set_angle, DTfloat, _angle);
    
        /// Accessors for the auxilary aspect ratio multiplier (Perspective Camera)
		DEFINE_ACCESSORS		(aspect_ratio_mul, set_aspect_ratio_mul, DTfloat, _aspect_ratio_mul);

        /// Accessors for the auxilary aspect ratio multiplier (Perspective Camera)
		DEFINE_ACCESSORS		(perspective_fill_width, set_perspective_fill_width, DTboolean, _perspective_fill_width);

		/// Sets the perspective camera frustum
		/// \param near_plane Near plane of the camera
		/// \param far_plane Far plane of the camera
		/// \param angle Angle of the camera frustum
		/// \param aspect_ratio_mul Aspect ratio multiplier for the frustum
        void						set_frustum                 (   const DTfloat near_plane,
                                                                    const DTfloat far_plane,
                                                                    const DTfloat angle,
                                                                    const DTfloat aspect_ratio_mul);

		/// Get Far plane (Ortho camera)
		DEFINE_ACCESSORS		(far, set_far, DTfloat, _far);

		/// Get Near plane (Ortho camera)
		DEFINE_ACCESSORS		(near, set_near, DTfloat, _near);

		/// Get Left plane (Ortho camera)
		DEFINE_ACCESSORS		(left, set_left, DTfloat, _left);

		/// Get Right plane (Ortho camera)
		DEFINE_ACCESSORS		(right, set_right, DTfloat, _right);

		/// Get Bottom plane (Ortho camera)
		DEFINE_ACCESSORS		(bottom, set_bottom, DTfloat, _bottom);

		/// Get Top plane (Ortho camera)
		DEFINE_ACCESSORS		(top, set_top, DTfloat, _top);


		/// Sets the orthographic camera frustum
		/// \param left left plane
		/// \param right right plane
		/// \param bottom bottom plane
		/// \param top top plane
		/// \param near near plane
		/// \param far far plane
        void						set_ortho                   (   const DTfloat left,
                                                                    const DTfloat right,
                                                                    const DTfloat bottom,
                                                                    const DTfloat top,
                                                                    const DTfloat near,
                                                                    const DTfloat far);


		/// Is the camera a perspective camera (as opposed to ortho)
		/// \return is perspective
        DTboolean                   is_perspective              (void)  {   return _perspective;    }


		/// Set a picking camera (used in the editor for picking)
		/// \param x x position
		/// \param y y position
		/// \param width width of window
		/// \param height height of window
		/// \param viewport viewport of window
        void						set_picking                 (const DTfloat x, const DTfloat y, const DTfloat width, const DTfloat height, DTint viewport[4]);
        
		/// End picking and return the camera back to normal
        void						end_picking					(void)							{	_picking = false;		}

		/// Is the camera a picking camera
		/// \return is picking
        DTboolean                   is_picking                  (void) const                    {   return _picking;        }


		/// Checks if a point is in the frustum
		/// \param point point to check
		/// \return is in frustum
		DTboolean					point_in_frustum            (const Vector3 &point) const;

		/// Checks the distance of a point to the frustum
		/// \param point point to check
		/// \return distance to frustum
		DTfloat						distance_to_frustum         (const Vector3 &point) const;


		/// Checks if a sphere is in the frustum
		/// \param translation translation of sphere
		/// \param sphere sphere
		/// \return is in frustum
		DTboolean					sphere_in_frustum           (const Vector3 &translation, const Sphere &sphere) const;

		/// Checks if a sphere is in the frustum, but doesn't check front and back planes
		/// \param translation translation of sphere
		/// \param sphere sphere
		/// \return is in frustum
		DTboolean					sphere_in_frustum_no_front_back (const Vector3 &translation, const Sphere &sphere) const;

		/// Checks if a box is in the frustum
		/// \param box box
		/// \return is in frustum
        DTboolean					box_in_frustum              (const Box &box) const;
    
    
		/// Orients the camera to look at a point in space
		/// \param to look to point
		/// \param up up vector
        void						look_at                     (const Vector3 &to, const Vector3 &up);

		/// Orients the camera to look at a point in space and also modifies the frustum angle to focus the camera
		/// \param to look to point
		/// \param up up vector
		/// \param radius radius of focal point
		void						focus_at                    (const Vector3 &to, const Vector3 &up, const DTfloat radius);
                    
		/// Calculates an apparent size of an oject at a distance given perspective
		/// \param distance distance to object
		/// \return multiplier
		DTfloat						fraction_screen_size        (const DTfloat distance);

		/// Projects a point to openGL clip space.
		/// \param point Point to project
		/// \return projected point
        Vector3						project_point               (const Vector3 &point) const;

		/// Projects a point to raster space.
		/// \param point Point to project
		/// \return projected point
        Vector3						project_point_to_pixel      (const Vector3 &point) const;

		/// Unprojects a point from clip space to world space
		/// \param point Point to unproject
		/// \return unprojected point
        Vector3						unproject_point             (const Vector3 &point) const;


		/// Returns the modelview matrix for this camera. Calculated by calculateFrustum.
		/// \return Modelview matrix
		const Matrix4 &				modelview                   (void) const					{	return _modelview;				}

		/// Returns the projection matrix for this camera. Calculated by calculateFrustum.
		/// \return projection matrix
		const Matrix4 &				projection                  (void) const					{	return _projection;				}


		/// Calculates all clip planes and matrices for the camera
		void						calculate_frustum           (void);
    
    
		/// Camera rotation around axes
		/// \param angle_x angle around x
		/// \param angle_y angle around y
        void                        rotate                      (DTfloat angle_x, DTfloat angle_y);

		/// Camera orbit calculation
		/// \param angle_x angle around x
		/// \param angle_y angle around y
		/// \param around_pt point to orbit
        void                        orbit                       (DTfloat angle_x, DTfloat angle_y, const Vector3 &around_pt);

		/// Called when the camera is activated
		virtual void				activated					(void)							{}
        		
		/// Called when the camera is done being activated
		virtual void				activated_done              (void)							{}
        		
		enum {
			PLANE_FAR = 0,
			PLANE_NEAR,
			PLANE_RIGHT,
			PLANE_LEFT,
			PLANE_BOTTOM,
			PLANE_TOP
		};
		
		/// Retrieves a clip plane from the camera
		/// \param which_plane plane index
		/// \return clip plane
		const Plane&				camera_plane                (DTint which_plane)				{	return _frustum[which_plane];	}
	

        /// Sets the post projection matrix. This is used to modify the projection matrix.
        /// This is useful for stereo rendering.
        /// \param pp Post projection matrix
        void                        set_post_projection_matrix  (const Matrix4 &pp)             {   _post_projection = pp;      }
    
        /// Returns the post projection matrix.
        /// \return Post projection matrix
        const Matrix4 &             post_projection_matrix      (void) const                    {   return _post_projection;    }
        
    private:    
        Plane				_frustum[6];
		
		DTboolean			_picking;
		DTboolean			_perspective;
        DTboolean           _perspective_fill_width;
              
		// Perspective
		DTfloat				_near_plane;
		DTfloat				_far_plane;
		DTfloat				_angle;
		DTfloat				_aspect_ratio_mul; 

		// Ortho
		DTfloat				_left;
		DTfloat				_right;
		DTfloat				_bottom;
		DTfloat				_top;
		DTfloat				_near;
		DTfloat				_far;

		// Picking (_mode = 2)
		DTfloat				_x;
		DTfloat				_y;
		DTfloat				_deltax;
		DTfloat				_deltay;
		DTint				_viewport[4];
		
		// Calculated matrices
		Matrix4				_modelview;
		Matrix4				_projection;
		Matrix4				_post_projection;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
