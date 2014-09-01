#ifndef DT3_SCRIPTINGKEYFRAMESVECTOR2
#define DT3_SCRIPTINGKEYFRAMESVECTOR2
//==============================================================================
///	
///	File: ScriptingKeyframesVector2.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingKeyframes.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Keyframes for Vector2 type.
//==============================================================================

class ScriptingKeyframesVector2: public ScriptingKeyframes {
    public:
        DEFINE_TYPE(ScriptingKeyframesVector2,ScriptingKeyframes)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingKeyframesVector2	(void);	
									ScriptingKeyframesVector2	(const ScriptingKeyframesVector2 &rhs);
        ScriptingKeyframesVector2 &	operator =					(const ScriptingKeyframesVector2 &rhs);	
        virtual						~ScriptingKeyframesVector2	(void);

        virtual void				archive                (const std::shared_ptr<Archive> &archive);

	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

		/// Set a key at the current time
		virtual void				set_key					(void);

		/// Clear a key at the current time
		virtual void				clear_key				(void);

		/// Clear a key with index
		/// \param k key index
		virtual void				clear_key				(DTint k);

		/// Get the number of keys
		/// \return number of keys
		virtual DTsize				num_keys                (void) const            {	return _keyframes.size();		}

		/// Returns a unique ID for this key
		/// \param k key index
		/// \return ID
        virtual DTint               key_id                  (DTint k) const        {   return _keyframes[k]._id;		}

		/// Get the time for the key
		/// \param k key index
		/// \return time
		virtual DTfloat				key_time                (DTint k) const        {	return _keyframes[k]._time;		}

		/// Set the time for the key
		/// \param k key index
		/// \param time key time
		/// \return new index
		virtual DTint				set_key_time            (DTint k, DTfloat time);


		/// Set the point for the key
		/// \param k key index
		/// \param point point
		void						set_key_point           (DTint k, const Vector2 &point)	{	_keyframes[k]._value = point;		_out.set_dirty();	}

		/// Get the point for the key
		/// \param k key index
		/// \return point
		const Vector2 &				key_point               (DTint k)	const					{	return _keyframes[k]._value;			}

		/// Set the tangent for the key
		/// \param k key index
		/// \param tangent tangent
		void						set_key_point_tangent   (DTint k, const Vector2 &tangent)	{	_keyframes[k]._tangent = tangent;	_out.set_dirty();	}

		/// Get the tangent for the key
		/// \param k key index
		/// \return tangent
		const Vector2 &				key_point_tangent       (DTint k)	const					{	return _keyframes[k]._tangent;		}

		/// Interpolate along the keyframes
		/// \param t t (0.0-1.0)
		/// \param translation interpolated position
        void						interpolate				(DTfloat t, Vector2 &translation) const;

		/// Is interpolated flag
		/// \return is interpolated
		DTboolean					is_interpolated			(void)						{	return _interpolated;	}
		
	private:		
		DTboolean					_interpolated;
		Plug<DTfloat>				_t;
		Plug<Vector2>				_out;
		DTint						_id;

		struct keyframe {
			int operator < (const keyframe& rhs) const	{	return _time < rhs._time;	}
		
			DTfloat                 _time;
			Vector2                 _value;
			Vector2                 _tangent;
			DTint                   _id;
		};
		
		std::vector<keyframe>		_keyframes;
		mutable DTint				_keyframe_cache;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
