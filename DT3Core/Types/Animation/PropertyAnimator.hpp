#ifndef DT3_PROPERTYANIMATOR
#define DT3_PROPERTYANIMATOR
//==============================================================================
///	
///	File: PropertyAnimator.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include "DT3Core/Types/Utility/TypeTraits.hpp"
#include "DT3Core/Types/Utility/LatentCall.hpp"
#include "DT3Core/Types/Math/Matrix3.hpp"
#include "DT3Core/Types/Math/Matrix4.hpp"
#include "DT3Core/Types/Math/Quaternion.hpp"
#include <list>
#include <cmath>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
// Forward declarations
//==============================================================================


//==============================================================================
/// Class
//==============================================================================

template<typename T>
class PropertyAnimatorInterpolatorBase {
    public:
        virtual T interp (const T &src,const T &dst, DTfloat t) = 0;
};

template<typename T>
class PropertyAnimatorInterpolatorLinear: public PropertyAnimatorInterpolatorBase<T> {
    public:
        virtual T interp (const T &src,const T &dst, DTfloat t) {
            return src + (dst-src) * t;
        }
};

template<typename T>
class PropertyAnimatorInterpolatorSpline: public PropertyAnimatorInterpolatorBase<T> {
    public:
        virtual T interp (const T &src,const T &dst, DTfloat t) {
            DTfloat tt = (3.0F - 2.0F * t) * (t * t);
            return src + (dst-src) * tt;
        }
};

template<typename T>
class PropertyAnimatorInterpolatorAccel: public PropertyAnimatorInterpolatorBase<T> {
    public:
        virtual T interp (const T &src,const T &dst, DTfloat t) {
            DTfloat tt = t*t;
            return src + (dst-src) * tt;
        }
};

template<typename T>
class PropertyAnimatorInterpolatorDecel: public PropertyAnimatorInterpolatorBase<T> {
    public:
        virtual T interp (const T &src,const T &dst, DTfloat t) {
            DTfloat tt = std::sqrt(t);
            return src + (dst-src) * tt;
        }
};


// Specialized interpolators

class PropertyAnimatorInterpolatorSlerp3: public PropertyAnimatorInterpolatorBase<Matrix3> {
    public:
        virtual Matrix3 interp (const Matrix3 &src,const Matrix3 &dst, DTfloat t) {
            return Matrix3(Quaternion::slerp(Quaternion(src), Quaternion(dst), t));
        }
};

class PropertyAnimatorInterpolatorSlerpSpline3: public PropertyAnimatorInterpolatorBase<Matrix3> {
    public:
        virtual Matrix3 interp (const Matrix3 &src,const Matrix3 &dst, DTfloat t) {
            DTfloat tt = (3.0F - 2.0F * t) * (t * t);
            return Matrix3(Quaternion::slerp(Quaternion(src), Quaternion(dst), tt));
        }
};


class PropertyAnimatorInterpolatorSlerp4: public PropertyAnimatorInterpolatorBase<Matrix4> {
    public:
        virtual Matrix4 interp (const Matrix4 &src,const Matrix4 &dst, DTfloat t) {
            return Matrix4( Matrix3(Quaternion::slerp(Quaternion(src.orientation()), Quaternion(dst.orientation()), t)),
                            src.translation() + (dst.translation()-src.translation()) * t);
        }
};

class PropertyAnimatorInterpolatorSlerpSpline4: public PropertyAnimatorInterpolatorBase<Matrix4> {
    public:
        virtual Matrix4 interp (const Matrix4 &src,const Matrix4 &dst, DTfloat t) {
            DTfloat tt = (3.0F - 2.0F * t) * (t * t);
            return Matrix4( Matrix3(Quaternion::slerp(Quaternion(src.orientation()), Quaternion(dst.orientation()), tt)),
                            src.translation() + (dst.translation()-src.translation()) * tt);
        }
};

//==============================================================================
/// Class
//==============================================================================

class PropertyAnimatorHandleBase: public BaseClass {
    public:
                                PropertyAnimatorHandleBase  (void)  {}
    
    private:
                                PropertyAnimatorHandleBase  (const PropertyAnimatorHandleBase &rhs);
        PropertyAnimatorHandleBase& operator =              (const PropertyAnimatorHandleBase &rhs);

    public:
        virtual					~PropertyAnimatorHandleBase (void)  {}

    public:
    
        /// Tick animator
		/// \param dt timestep for this animation
        virtual DTboolean       tick                        (const DTfloat dt) = 0;
};

template<typename CLASS, typename U, typename V>
class PropertyAnimatorHandle: public PropertyAnimatorHandleBase {
    private:
		typedef U (CLASS::*GetFN)(void) const;
		typedef void (CLASS::*SetFN)(V);

    public:
        PropertyAnimatorHandle (const std::shared_ptr<CLASS> &object, GetFN get, SetFN set)
            :   _object (object),
                _get    (get),
                _set    (set)
        {
            _accum_time = 0.0F;
            _last_v = (object.get()->*_get)();
        }
    
    private:
        PropertyAnimatorHandle (const PropertyAnimatorHandle &rhs);
        PropertyAnimatorHandle& operator = (const PropertyAnimatorHandle &rhs);
    
    public:
        virtual ~PropertyAnimatorHandle (void)
        {
        
        }
    
    public:
    
        /// Append a keyframe to the list of animations
		/// \param v value to interpolate to
		/// \param dt timestep for this animation
		/// \param delay delay
		/// \return this archiving object so you can chain them together
        void                    append ( V v, DTfloat time, DTfloat delay, std::shared_ptr<PropertyAnimatorInterpolatorBase<typename TypeTraits<V>::BaseType>> interpolator = nullptr)
        {
            Keyframe k;
            k._delay = delay;
            k._time = time;
            k._v = v;
            k._interpolator = interpolator;
            
            _keyframes.push_back(k);
        }

        /// Append a latent call to the list of animations
		/// \param v value to interpolate to
		/// \param dt timestep for this animation
		/// \param delay delay
		/// \return this archiving object so you can chain them together
        void                    append ( DTfloat delay, std::shared_ptr<LatentCall> lc)
        {
            Keyframe k;
            k._latent_call = lc;
            
            _keyframes.push_back(k);
        }
    
        /// Tick animator
		/// \param dt timestep for this animation
        virtual DTboolean       tick (const DTfloat dt)
        {
            if (_object.expired()) {
                return false;
            }
        
            const std::shared_ptr<CLASS> o(_object);

            if (_object.expired())
                return false;
            
            if (_keyframes.size() == 0)
                return false;

            Keyframe &k = _keyframes.front();
            
            _accum_time += dt;

            // Process delays
            if (k._delay > 0.0F) {
                if (_accum_time > k._delay) {
                    _accum_time -= k._delay;
                    k._delay = 0.0F;
                } else {
                    return true;    // Keep waiting
                }
            }
            
            // If we're just doing a call
            if (k._latent_call) {
                k._latent_call->fire();
                
                _keyframes.pop_front();
                
                if (_keyframes.size() > 0) {
                    return true;
                } else {
                    return false;
                }
            }
            
            // How far into the animation
            if (_accum_time > k._time) {
                _accum_time -= k._time;
                
                _last_v = k._v;
                (o.get()->*_set)(k._v); // Force to final position
                
                _keyframes.pop_front();
                
                if (_keyframes.size() > 0) {
                    return true;
                } else {
                    return false;
                }
            }
            
            DTfloat t = _accum_time / k._time;
            
            // Use interpolator if there is one
            if (k._interpolator) {
                (o.get()->*_set)(k._interpolator->interp(_last_v, k._v, t));
            } else {
                // Set the new value
                (o.get()->*_set)(t * (k._v - _last_v) + _last_v);
            }
            
            
            return true;
        }

    private:
        struct Keyframe {
            DTfloat                                                 _delay;
            DTfloat                                                 _time;
            typename TypeTraits<V>::BaseType                        _v;
            std::shared_ptr<PropertyAnimatorInterpolatorBase<typename TypeTraits<V>::BaseType>>    _interpolator;
            std::shared_ptr<LatentCall>                             _latent_call;
        };
    
        DTfloat                                 _accum_time;
        typename TypeTraits<V>::BaseType        _last_v;
    
        std::list<Keyframe>                     _keyframes;
    
		const std::weak_ptr<CLASS>              _object;
		GetFN                                   _get;
		SetFN                                   _set;
};


//==============================================================================
/// Class
//==============================================================================

class PropertyAnimator {
		/// PropertyAnimator implementation.
         
	private:
                                PropertyAnimator    (void);
                                PropertyAnimator    (const PropertyAnimator &rhs);
        PropertyAnimator &      operator =          (const PropertyAnimator &rhs);
        virtual					~PropertyAnimator   (void);
    
    public:
    
        /// Build a new animation handle for the property
        /// \param object objects that holds the property
        /// \param get get member funtion
        /// \param set set member function
		/// \return Property animator handle
        template<typename CLASS, typename U, typename V>
        static std::shared_ptr<PropertyAnimatorHandle<CLASS,U,V>> animate ( const std::shared_ptr<CLASS> &object,
                                                                            U (CLASS::*get)(void) const,
                                                                            void (CLASS::*set)(V))
        {
            std::shared_ptr<PropertyAnimatorHandle<CLASS,U,V>> p(new PropertyAnimatorHandle<CLASS,U,V>(object, get, set));
            _properties.push_back(p);
            
            return p;
        }
    
    
    
        /// Tick animator
		/// \param dt timestep for this animation
        static void             tick            (const DTfloat dt);

        /// Tick animator
		/// \param dt timestep for this animation
        static DTboolean        is_animating    (void)  {   return _properties.size() > 0;  }

    private:
        static std::list<std::shared_ptr<PropertyAnimatorHandleBase>>  _properties;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
