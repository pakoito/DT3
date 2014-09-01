//==============================================================================
///	
///	File: ScriptingSpline.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingSpline.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Math/Vector4.hpp"
#include "DT3Core/Types/Math/Quaternion.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingSpline,"Animation",EdManipScriptingSpline)
IMPLEMENT_PLUG_NODE(ScriptingSpline)

IMPLEMENT_PLUG_INFO_INDEX(_d)
IMPLEMENT_PLUG_INFO_INDEX(_transform)
IMPLEMENT_PLUG_INFO_INDEX(_orientation)
IMPLEMENT_PLUG_INFO_INDEX(_translation)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingSpline)
      
	PLUG_INIT(_d,"d")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_transform))
		.affects(PLUG_INFO_INDEX(_orientation))
		.affects(PLUG_INFO_INDEX(_translation));

	PLUG_INIT(_transform,"Transform")
		.set_output(true);

	PLUG_INIT(_translation,"Translation")
		.set_output(true);

	PLUG_INIT(_orientation,"Orientation")
		.set_output(true);

END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingSpline::ScriptingSpline (void)
	:	_constant_speed (true),
        _d				(PLUG_INFO_INDEX(_d), 0.0F),
		_transform      (PLUG_INFO_INDEX(_transform), Matrix4::identity()),
		_orientation    (PLUG_INFO_INDEX(_orientation), Matrix3::identity()),
		_translation    (PLUG_INFO_INDEX(_translation), Vector3(0.0F,0.0F,0.0F)),
        _dirty          (true),
		_keyframe_cache	(0)
{  

}
		
ScriptingSpline::ScriptingSpline (const ScriptingSpline &rhs)
    :   ScriptingBase       (rhs),
        _constant_speed     (rhs._constant_speed),
        _d                  (rhs._d),
        _transform          (rhs._transform),
        _orientation        (rhs._orientation),
        _translation        (rhs._translation),
        _dirty              (true),
		_keyframe_cache		(0)
{   

}

ScriptingSpline & ScriptingSpline::operator = (const ScriptingSpline &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);	

        _constant_speed = rhs._constant_speed;
        _d = rhs._d;
        _transform = rhs._transform;
        _orientation = rhs._orientation;
        _translation = rhs._translation;
        
		_keyframe_cache = rhs._keyframe_cache;
        _dirty = true;

    }
    return (*this);
}
			
ScriptingSpline::~ScriptingSpline (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingSpline::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingSpline::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
    
    *archive << ARCHIVE_DATA_ACCESSORS("Num_Points", ScriptingSpline::num_points, ScriptingSpline::set_num_points, DATA_PERSISTENT | DATA_SETTABLE);
    
    for (std::size_t i = 0; i < _transforms.size(); ++i) {
        *archive << ARCHIVE_DATA_RAW(_transforms[i]._transform, DATA_PERSISTENT);
    }
    
    *archive << ARCHIVE_PLUG(_d, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA(_constant_speed, DATA_PERSISTENT | DATA_SETTABLE);

    *archive << ARCHIVE_PLUG(_transform, DATA_PERSISTENT);
    *archive << ARCHIVE_PLUG(_orientation, DATA_PERSISTENT);
    *archive << ARCHIVE_PLUG(_translation, DATA_PERSISTENT);

    archive->pop_domain ();
}

void ScriptingSpline::archive_done (const std::shared_ptr<Archive> &archive)
{
	ScriptingBase::archive_done (archive);

    if (archive->is_writing())
        return;

    process_distances();
}

//==============================================================================
//==============================================================================

DTfloat ScriptingSpline::max_distance (void)
{
	PROFILER(SCRIPTING);

    if (_transforms.size() > 0) {
        if (_dirty)
            process_distances();
        return _transforms.back()._distance;
    } else {
        return 0.0F;
    }
}

//==============================================================================
//==============================================================================

void ScriptingSpline::set_num_points (DTsize num)
{
	PROFILER(SCRIPTING);

    _transforms.resize(num);     
             
    _dirty = true;    
    _transform.set_dirty();
    _orientation.set_dirty();
    _translation.set_dirty();
}

DTsize ScriptingSpline::num_points (void) const
{
	PROFILER(SCRIPTING);

    return _transforms.size();              
}

void ScriptingSpline::set_point_transform (DTuint k, const Matrix4 &transform)
{
	PROFILER(SCRIPTING);

    _transforms[k]._transform = transform;  
    
    _dirty = true;    
    _transform.set_dirty();
    _orientation.set_dirty();
    _translation.set_dirty();
}

const Matrix4& ScriptingSpline::point_transform (DTuint k) const
{
	PROFILER(SCRIPTING);

    return _transforms[k]._transform;       
}

void ScriptingSpline::set_point_tangent (DTuint k, const Vector3 &tangent)
{
	PROFILER(SCRIPTING);

    _transforms[k]._tangent = tangent;      

    _dirty = true;    
    _transform.set_dirty();
    _orientation.set_dirty();
    _translation.set_dirty();
}

const Vector3& ScriptingSpline::point_tangent (DTuint k) const
{
	PROFILER(SCRIPTING);

    return _transforms[k]._tangent;         
}

//==============================================================================
// See http://mathworld.wolfram.com/LeastSquaresFittingPolynomial.html
//==============================================================================

void ScriptingSpline::process_distances (void)
{
	PROFILER(SCRIPTING);

    if (_transforms.size() == 0)
        return;

    DTfloat STEP = 1.0F/100.0F;
    
    DTfloat distance = 0.0F;
    DTfloat length = 0.0;

    DTuint i;
    for (i = 0; i < _transforms.size()-1; ++i) {
    
        _transforms[i]._distance = distance;

        Matrix4 transform;
    
        //
        // Get length
        //
      
        length = 0.0F;
        interpolate (i, i+1, 0.0F, transform);
        Vector3 last_point = transform.translation();

        // Sample curve to get length so we can normalize the
        // inputs for the regression analysis below
        for (DTfloat t = 0.0F; t <= 1.0F; t += STEP) {
            interpolate (i, i+1, t, transform);
            
            Vector3 this_point = transform.translation();
            
            DTfloat dp = (this_point-last_point).abs();

            // Accumulate lengths
            length += dp;
            distance += dp;
            
            last_point = this_point;
        }

        _transforms[i]._length = length;

        //
        // Perform regression
        //
                
        length = 0.0F;
        interpolate (i, i+1, 0.0F, transform);
        last_point = transform.translation();

        DTfloat s0=0.0F,s1=0.0F,s2=0.0F,s3=0.0F,s4=0.0F,s5=0.0F,s6=0.0F;
        DTfloat y0=0.0F,y1=0.0F,y2=0.0F,y3=0.0F;
        
        // Sample curve
        for (DTfloat t = 0.0F; t <= 1.0F; t += STEP) {
            interpolate (i, i+1, t, transform);
            
            Vector3 this_point = transform.translation();
            DTfloat dp = (this_point-last_point).abs();

            // Accumulate lengths
            length += dp;
            last_point = this_point;
            
            
            // Accumulate points for regression analysis.
            // t = f(length)  so x = length, y = t
                        
            DTfloat x1 = length / _transforms[i]._length;   // Normalized length
            DTfloat x2 = x1*x1;
            DTfloat x3 = x2*x1;
            DTfloat x4 = x2*x2;
            DTfloat x5 = x4*x1;
            DTfloat x6 = x4*x2;
            
            s0 += 1.0F;
            s1 += x1;
            s2 += x2;
            s3 += x3;
            s4 += x4;
            s5 += x5;
            s6 += x6;
            
            y0 += t;
            y1 += x1*t;
            y2 += x2*t;
            y3 += x3*t;
        }
        
        // Do regression
        Matrix4 X(  s0, s1, s2, s3,
                    s1, s2, s3, s4,
                    s2, s3, s4, s5,
                    s3, s4, s5, s6  );
        Vector4 Y(  y0, y1, y2, y3  );
        
        Vector4 a = X.inversed() * Y;
        
        DTfloat sum = a.y + a.z + a.w;  // To make sure the polynomial always returns 0-1
        
        //_transforms[i]._a0 = a.x;
        _transforms[i]._a1 = a.y / sum;
        _transforms[i]._a2 = a.z / sum;
        _transforms[i]._a3 = a.w / sum;
                        
    }
    
    _transforms[i]._distance = distance;
    _transforms[i]._length = 0.0F;
        
    _dirty = false;
}

//==============================================================================
//==============================================================================

void ScriptingSpline::interpolate (DTint i0, DTint i1, DTfloat d, Matrix4 &transform)
{
    DTfloat d2 = d * d;
    DTfloat d3 = d * d2;

    // interpolate via Hermite spline
    // See Real_time Rendering, 2nd Ed., Page 492
    Vector3 p1,p2,p3,p4;
    p1 = _transforms[i0]._transform.translation() * (2.0F * d3 - 3.0F * d2 + 1.0F);
    p2 = _transforms[i0]._tangent * (d3 - 2.0F * d2 + d);
    p3 = _transforms[i1]._tangent * (d3 - d2);
    p4 = _transforms[i1]._transform.translation() * (-2.0F * d3 + 3.0F * d2);
    
    Vector3 translation = p1 + p2 + p3 + p4;
    
    // Slerp orientations
    Quaternion q0 = Quaternion(_transforms[i0]._transform);
	Quaternion q1 = Quaternion(_transforms[i1]._transform);
	Quaternion rotation = Quaternion::slerp (q0, q1, d);
	
    transform = Matrix4( Matrix3(rotation), translation);
}

void ScriptingSpline::interpolate (DTfloat d, Matrix4 &transform)
{
    
	// Special cases
	if (_transforms.size() == 0)	{
        transform = Matrix4::identity();
		return;
	}
    
    if (_dirty)
        process_distances();
	
	if (_transforms.size() == 1)	{
		transform = _transforms[0]._transform;
		return;
	}
    
	// Scan for the best key
	if (_keyframe_cache < 0)									_keyframe_cache = 0;
	else if (_keyframe_cache > (DTint) _transforms.size() - 2)	_keyframe_cache = (DTint) _transforms.size() - 2;
			
	while (1) {
		if (d < _transforms[_keyframe_cache]._distance) {
			--_keyframe_cache;
			if (_keyframe_cache < 0) {
				_keyframe_cache = 0;
				transform = _transforms[_keyframe_cache]._transform;
				break;
			}
		} else if (d > _transforms[_keyframe_cache+1]._distance) {
			++_keyframe_cache;
			if (_keyframe_cache > (DTint) _transforms.size() - 2) {
				_keyframe_cache = (DTint) _transforms.size() - 2;
				transform = _transforms[_keyframe_cache+1]._transform;
				break;
			}
		} else {
            Pt &pt = _transforms[_keyframe_cache];
            
            DTfloat d1 = 0.0F;
            if (pt._length > 0.0F)
                d1 = (d - pt._distance) / pt._length;   // Normalized distance
                
            if (_constant_speed) {
                // Using polynomial approximation, convert distance to spline parametric value. Note a0 is always 0
                DTfloat t = (pt._a1 * d1 + pt._a2 * d1 * d1 + pt._a3 * d1 * d1 * d1); 
                                                
                interpolate (_keyframe_cache, _keyframe_cache+1, t, transform);
            } else {
                interpolate (_keyframe_cache, _keyframe_cache+1, d1, transform);
            }
			
			break;
		}
	
	}
    
}

//==============================================================================
//==============================================================================

DTboolean ScriptingSpline::compute (const PlugBase *plug)
{
	PROFILER(SCRIPTING);
    
    if (super_type::compute(plug))  return true;

	if (plug == &_transform || 
        plug == &_orientation || 
        plug == &_translation) {
        
        Matrix4 transform = Matrix4::identity();
        interpolate(_d,transform);
        
		_transform = transform;
		_orientation = transform.orientation();
		_translation = transform.translation();

		_transform.set_clean();
		_orientation.set_clean();
		_translation.set_clean();
		return true;
	}
	
	return false;

}
//==============================================================================
//==============================================================================

} // DT3

