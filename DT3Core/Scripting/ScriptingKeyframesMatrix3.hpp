#ifndef DT3_SCRIPTINGKEYFRAMESVECTOR3
#define DT3_SCRIPTINGKEYFRAMESVECTOR3
//==============================================================================
///	
///	File: ScriptingKeyframesMatrix3.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingKeyframes.hpp"
#include "DT3Core/Types/Math/Matrix3.hpp"
#include "DT3Core/Types/Math/Quaternion.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Keyframes for Matrix3 type.
//==============================================================================

class ScriptingKeyframesMatrix3: public ScriptingKeyframes {
    public:
        DEFINE_TYPE(ScriptingKeyframesMatrix3,ScriptingKeyframes)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingKeyframesMatrix3	(void);	
									ScriptingKeyframesMatrix3	(const ScriptingKeyframesMatrix3 &rhs);
        ScriptingKeyframesMatrix3 &	operator =					(const ScriptingKeyframesMatrix3 &rhs);	
        virtual						~ScriptingKeyframesMatrix3	(void);

        virtual void				archive                 (const std::shared_ptr<Archive> &archive);

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

	private:
		Plug<DTfloat>				_t;
		Plug<Matrix3>				_out;
		DTint						_id;

		struct keyframe {
			int operator < (const keyframe& rhs) const	{	return _time < rhs._time;	}
		
			DTfloat                 _time;
			Quaternion              _value;
			DTint                   _id;
		};
		
		std::vector<keyframe>		_keyframes;
		mutable DTint				_keyframe_cache;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
