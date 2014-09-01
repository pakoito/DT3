//==============================================================================
///	
///	File: CameraObject.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
///
/// NOTE: Plane normals point inwards
///
//==============================================================================

#include "DT3Core/Objects/CameraObject.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Math/Quaternion.hpp"
#include "DT3Core/Devices/DeviceGraphics.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_PLACEABLE(CameraObject,"Camera","EdCameraObjectAdapter")

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

CameraObject::CameraObject (void)
    :	_picking				(false),
		_perspective			(true),
        _perspective_fill_width (false),
		_near_plane				(1.0F),
        _far_plane				(10000.0F),
        _angle					(60.0F),
		_aspect_ratio_mul       (1.0F),
		_left					(-1.0F),
		_right					(1.0F),
		_bottom					(-1.0F),
		_top					(1.0F),
		_near					(-100.0F),
		_far					(100.0F),
		_x						(0),
		_y						(0),
		_deltax					(10),
		_deltay					(10),
		_modelview				(Matrix4::identity()),
		_projection				(Matrix4::identity()),
        _post_projection        (Matrix4::identity())
										
{
	_viewport[0] = 0;
	_viewport[1] = 0;
	_viewport[2] = 1;
	_viewport[3] = 1;
}

CameraObject::CameraObject (const CameraObject &rhs)
    :	PlaceableObject         (rhs),
		_picking                (rhs._picking),
		_perspective            (rhs._perspective),
        _perspective_fill_width (rhs._perspective_fill_width),
		_near_plane             (rhs._near_plane),
		_far_plane              (rhs._far_plane),
		_angle                  (rhs._angle),
		_aspect_ratio_mul       (rhs._aspect_ratio_mul),
		_left                   (rhs._left),
		_right                  (rhs._right),
		_bottom                 (rhs._bottom),
		_top                    (rhs._top),
		_near                   (rhs._near),
		_far                    (rhs._far),
		_x                      (rhs._x),
		_y                      (rhs._y),
		_deltax                 (rhs._deltax),
		_deltay                 (rhs._deltay),
        _modelview              (rhs._modelview),
        _projection             (rhs._projection),
        _post_projection        (rhs._post_projection)
		
{
    for (DTint i = 0; i < 6; ++i)	
        _frustum[i] = rhs._frustum[i];
    for (DTint i = 0; i < 4; ++i)	
        _viewport[i] = rhs._viewport[i];

}
            
CameraObject & CameraObject::operator = (const CameraObject& rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
        PlaceableObject::operator = (rhs);
		
		_picking = rhs._picking;
		_perspective = rhs._perspective;
		_perspective_fill_width = rhs._perspective_fill_width;
		
		_near_plane = rhs._near_plane;
		_far_plane = rhs._far_plane;
		_angle = rhs._angle;
		_aspect_ratio_mul = rhs._aspect_ratio_mul;

		_left = rhs._left;
		_right = rhs._right;
		_bottom = rhs._bottom;
		_top = rhs._top;
		_near = rhs._near;
		_far = rhs._far;

		_x = rhs._x;
		_y = rhs._y;
		_deltax = rhs._deltax;
		_deltay = rhs._deltay;
        
        _modelview = rhs._modelview;
        _projection = rhs._projection;
        _post_projection = rhs._post_projection;
	
		for (DTint i = 0; i < 6; ++i)	
			_frustum[i] = rhs._frustum[i];
		for (DTint i = 0; i < 4; ++i)	
			_viewport[i] = rhs._viewport[i];
        

    }
    return (*this);
}	

CameraObject::~CameraObject (void)
{

}

//==============================================================================
//==============================================================================

void CameraObject::archive (const std::shared_ptr<Archive> &archive)
{
    PlaceableObject::archive(archive);

    archive->push_domain (class_id ());

    *archive << ARCHIVE_DATA(_perspective, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_perspective_fill_width, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_left, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_right, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_top, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_bottom, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_near, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_far, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_post_projection, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_DATA_ACCESSORS("Far_Plane", CameraObject::far_plane, CameraObject::set_far_plane, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA_ACCESSORS("Near_Plane", CameraObject::near_plane, CameraObject::set_near_plane, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA_ACCESSORS("Angle", CameraObject::angle, CameraObject::set_angle, DATA_PERSISTENT | DATA_SETTABLE)
        .add_range(0.0F,180.0F);
    *archive << ARCHIVE_DATA_ACCESSORS("Aspect_Ratio_Mul", CameraObject::aspect_ratio_mul, CameraObject::set_aspect_ratio_mul, DATA_PERSISTENT | DATA_SETTABLE);

    archive->pop_domain ();
}

//==============================================================================
/// Checks to see if the point is in the camera frustum.
//==============================================================================

DTboolean CameraObject::point_in_frustum(const Vector3  &point) const
{
    for (DTint plane = 0; plane < 6; ++plane)
        if (_frustum[plane].distance_to_point(point) < 0.0F)
            return false;
                            
    return true;
}

DTfloat CameraObject::distance_to_frustum (const Vector3 &point) const
{
	DTfloat dist = 0.0F;
	
    for (DTint plane = 0; plane < 6; ++plane) {
		DTfloat distance = _frustum[plane].distance_to_point(point);
		dist = MoreMath::min(distance, dist);
	}
		
	return -dist;
}

//==============================================================================
/// Checks to see if the sphere is in the camera frustum.
//==============================================================================

DTboolean CameraObject::sphere_in_frustum(const Vector3 &translation, const Sphere &sphere) const
{

    for (DTint plane = 0; plane < 6; ++plane) {            
        if (_frustum[plane].is_sphere_completely_in_back(translation, sphere))
            return false;
    }
                    
    return true;
}

DTboolean CameraObject::sphere_in_frustum_no_front_back	(const Vector3 &translation, const Sphere &sphere) const
{
    for (DTint plane = 2; plane < 6; ++plane) {            
        if (_frustum[plane].is_sphere_completely_in_back(translation, sphere))
            return false;
    }
                    
    return true;
}

//==============================================================================
/// Checks to see if the box is in the frustum.
//==============================================================================

DTboolean CameraObject::box_in_frustum(const Box &box) const {
    for (DTint plane = 0; plane < 6; ++plane) {
		DTfloat dist = _frustum[plane].distance_to_box(box);
        if (dist < 0.0F)	return false;
		else if (dist == 0.0F)  return true;
    }
                    
    return true;
}

//==============================================================================
/// Points the camera to a point with the up vector.
//==============================================================================

void CameraObject::look_at (const Vector3 &to, const Vector3 &up)
{
    Matrix3 m;	
    Vector3 delta;
    
    delta = translation() - to;
    
    m = Matrix3::set_orientation(delta, up);
    set_orientation(m);
}

//==============================================================================
//==============================================================================

void CameraObject::focus_at (const Vector3 &to, const Vector3 &up, const DTfloat radius)
{
    look_at (to, up);
    
    Vector3 diff;
    DTfloat dist;
    
    diff = to - translation();
    dist = diff.abs();
    
    _angle = RAD_TO_DEG * 2.0F * std::atan(radius / dist);
    
    // Keep from getting too small
    if (_angle < 0.1F)  _angle = 0.1F;
}

//==============================================================================
/// Sets up the viewing frustum
//==============================================================================

void CameraObject::set_frustum (const DTfloat near_plane, const DTfloat far_plane, const DTfloat angle, const DTfloat aspect_ratio_mul)
{
    _near_plane = near_plane;
    _far_plane = far_plane;
    _angle = angle;
    _aspect_ratio_mul = aspect_ratio_mul;

	_perspective = true;
}


void CameraObject::set_ortho (const DTfloat left, const DTfloat right, const DTfloat bottom, const DTfloat top, const DTfloat near, const DTfloat far)
{
	_left = left;
	_right = right;
	_bottom = bottom;
	_top = top;
	_near = near;
	_far = far;
	
	_perspective = false;
}

void CameraObject::set_picking (const DTfloat x, const DTfloat y, const DTfloat deltax, const DTfloat deltay, DTint viewport[4])
{
	_x = x;
	_y = y;
	_deltax = deltax;
	_deltay = deltay;
	
	_viewport[0] = viewport[0];
	_viewport[1] = viewport[1];
	_viewport[2] = viewport[2];
	_viewport[3] = viewport[3];

	_picking = true;
}

//==============================================================================
/// PRIVATE: Calculates and stores the frustum information for culling against the
/// camera frustum. Must be called before any of the test routines (*_In_Frustum)
//==============================================================================

void CameraObject::calculate_frustum()
{
    // Build modelview matrix
    _modelview = transform().inversed();
	
	if (_perspective) {

		DTfloat radians = _angle * 0.5F * PI / 180.0F;
		DTfloat d = _near_plane - _far_plane;
		DTfloat f = std::cos(radians) / std::sin(radians);
        
        DTfloat aspect_ratio = System::renderer()->screen_aspect() * _aspect_ratio_mul;
        
		if (_perspective_fill_width && aspect_ratio > 1.0F) {
            _projection._m11 = f*aspect_ratio;         _projection._m12 = 0.0F;                            _projection._m13 = 0.0F;						_projection._m14 = 0.0F;
            _projection._m21 = 0.0F;					_projection._m22 = f*aspect_ratio*aspect_ratio;   _projection._m23 = 0.0F;						_projection._m24 = 0.0F;
            _projection._m31 = 0.0F;					_projection._m32 = 0.0F;                            _projection._m33 = (_far_plane+_near_plane)/d;	_projection._m34 = 2.0F*_far_plane*_near_plane/d;
            _projection._m41 = 0.0F;					_projection._m42 = 0.0F;                            _projection._m43 = -1.0F;                       _projection._m44 = 0.0F;
        } else {
            _projection._m11 = f/aspect_ratio;			_projection._m12 = 0.0F;				_projection._m13 = 0.0F;						_projection._m14 = 0.0F;
            _projection._m21 = 0.0F;					_projection._m22 = f;					_projection._m23 = 0.0F;						_projection._m24 = 0.0F;
            _projection._m31 = 0.0F;					_projection._m32 = 0.0F;				_projection._m33 = (_far_plane+_near_plane)/d;	_projection._m34 = 2.0F*_far_plane*_near_plane/d;
            _projection._m41 = 0.0F;					_projection._m42 = 0.0F;				_projection._m43 = -1.0F;                       _projection._m44 = 0.0F;
        }
    
    } else {
		_projection._m11 = 2.0F/(_right-_left);		_projection._m12 = 0.0F;				_projection._m13 = 0.0F;				_projection._m14 = -(_right+_left)/(_right-_left);
		_projection._m21 = 0.0F;					_projection._m22 = 2.0F/(_top-_bottom);	_projection._m23 = 0.0F;				_projection._m24 = -(_top+_bottom)/(_top-_bottom);
		_projection._m31 = 0.0F;					_projection._m32 = 0.0F;				_projection._m33 = -2.0F/(_far-_near);	_projection._m34 = -(_far+_near)/(_far-_near);
		_projection._m41 = 0.0F;					_projection._m42 = 0.0F;				_projection._m43 = 0.0F;				_projection._m44 = 1.0F;
	}

	// If picking then multiply the matrix
	if (_picking) {
		Matrix4 picking;

		picking._m11 = _viewport[2] / _deltax;	picking._m12 = 0.0F;					picking._m13 = 0.0F;		picking._m14 = (_viewport[2] - 2 * (_x - _viewport[0])) / _deltax;
		picking._m21 = 0.0F;					picking._m22 = _viewport[3] / _deltay;	picking._m23 = 0.0F;		picking._m24 = (_viewport[3] - 2 * (_y - _viewport[1])) / _deltay;
		picking._m31 = 0.0F;					picking._m32 = 0.0F;					picking._m33 = 1.0F;		picking._m34 = 0.0F;
		picking._m41 = 0.0F;					picking._m42 = 0.0F;					picking._m43 = 0.0F;		picking._m44 = 1.0F;
		
		_projection = picking * _projection;
	}
	
    // Apply post projection matrix
    _projection = _post_projection * _projection;
		
    Matrix4 clip;
    DTfloat mag;
    Vector3 vec;
	
	clip = _projection * _modelview;
    
    // Far plane
    _frustum[0].set_normal(Vector3 ( clip._mi[3] - clip._mi[2], clip._mi[7] - clip._mi[6], clip._mi[11] - clip._mi[10] ));
    _frustum[0].set_d(clip._mi[15] - clip._mi[14]);
    
    mag = _frustum[0].normal().abs();
    vec = _frustum[0].normal() / mag;
    _frustum[0].set_normal(vec);
    _frustum[0].set_d(_frustum[0].D() / mag);
    
    
    // Near plane
    _frustum[1].set_normal(Vector3 (clip._mi[3] + clip._mi[2],clip._mi[7] + clip._mi[6],clip._mi[11] + clip._mi[10]));
    _frustum[1].set_d(clip._mi[15] + clip._mi[14]);
    
    mag = _frustum[1].normal().abs();
    vec = _frustum[1].normal() / mag;
    _frustum[1].set_normal(vec);
    _frustum[1].set_d(_frustum[1].D() / mag);
    
    
    // Right plane
    _frustum[2].set_normal(Vector3 (clip._mi[3] - clip._mi[0],clip._mi[7] - clip._mi[4],clip._mi[11] - clip._mi[8]));
    _frustum[2].set_d(clip._mi[15] - clip._mi[12]);
    
    mag = _frustum[2].normal().abs();
    vec = _frustum[2].normal() / mag;
    _frustum[2].set_normal(vec);
    _frustum[2].set_d(_frustum[2].D() / mag);
    
    
    // Left plane
    _frustum[3].set_normal(Vector3 (clip._mi[3] + clip._mi[0],clip._mi[7] + clip._mi[4],clip._mi[11] + clip._mi[8]));
    _frustum[3].set_d(clip._mi[15] + clip._mi[12]);
    
    mag = _frustum[3].normal().abs();
    vec = _frustum[3].normal() / mag;
    _frustum[3].set_normal(vec);
    _frustum[3].set_d(_frustum[3].D() / mag);
            
            
    // Bottom plane
    _frustum[4].set_normal(Vector3 (clip._mi[3] + clip._mi[1],clip._mi[7] + clip._mi[5],clip._mi[11] + clip._mi[9]));
    _frustum[4].set_d(clip._mi[15] + clip._mi[13]);
    
    mag = _frustum[4].normal().abs();
    vec = _frustum[4].normal() / mag;
    _frustum[4].set_normal(vec);
    _frustum[4].set_d(_frustum[4].D() / mag);
    
    
    // top plane
    _frustum[5].set_normal(Vector3 (clip._mi[3] - clip._mi[1],clip._mi[7] - clip._mi[5],clip._mi[11] - clip._mi[9]));
    _frustum[5].set_d(clip._mi[15] - clip._mi[13]);
    
    mag = _frustum[5].normal().abs();
    vec = _frustum[5].normal() / mag;
    _frustum[5].set_normal(vec);
    _frustum[5].set_d(_frustum[5].D() / mag);
    
}

//==============================================================================
//==============================================================================

DTfloat CameraObject::fraction_screen_size (const DTfloat distance)
{
	if (_perspective) {
		if (distance == 0.0F) return 1.0F;
		return 1.0F / (distance * std::tan(_angle * DEG_TO_RAD * 0.5F) * 2.0F);
	} else {
		return 1.0F / (_right - _left);
	}
}

//==============================================================================
/// Projects/Unprojects a point
//==============================================================================

Vector3 CameraObject::project_point(const Vector3 &point) const
{
	Vector3 result = _modelview * point;
    return MoreMath::transform_4h(_projection, result);
}

Vector3 CameraObject::project_point_to_pixel(const Vector3 &point) const
{
    Vector3 p = project_point(point);
    
    DTfloat screen_width = System::renderer()->screen_width();
    DTfloat screen_height = System::renderer()->screen_height();
    
    Matrix4 to_raster   (   screen_width*0.5F,  0.0F,               0.0F,   screen_width*0.5F,
                            0.0F,               screen_height*0.5F, 0.0F,   screen_height*0.5F,
                            0.0F,               0.0F,               1.0F,   0.0F,
                            0.0F,               0.0F,               0.0F,   1.0F);
    
    return to_raster * p;
}

Vector3 CameraObject::unproject_point(const Vector3 &point) const
{
    Matrix4 m = _projection * _modelview;
    
	Vector3 result; 
		
    return MoreMath::transform_4h(m.inversed(), point);
}

//==============================================================================
//==============================================================================

void CameraObject::rotate (DTfloat angle_x, DTfloat angle_y)
{
    Matrix3 roty, rotx;
    roty = Matrix3::set_rotation_y(angle_y);
    rotx = Matrix3::set_rotation_x(angle_x);

    Matrix3 o = orientation() * roty * rotx;
    set_orientation(o);
}

void CameraObject::orbit (DTfloat angle_x, DTfloat angle_y, const Vector3 &around_pt)
{
    Matrix3 roty, rotx;
    roty = Matrix3::set_rotation_y(angle_y);
    rotx = Matrix3::set_rotation_x(angle_x);
        
    Matrix3 o = orientation();
    Vector3 offset = o.inversed() * (translation() - around_pt);
    o = o * roty * rotx;
    
    set_translation(o * offset + around_pt);
    set_orientation(o);
}

//==============================================================================
//==============================================================================

} // DT3

