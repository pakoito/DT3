#ifndef DT3_SCRIPTINGKEYFRAMESMATERIALRESOURCE
#define DT3_SCRIPTINGKEYFRAMESMATERIALRESOURCE
//==============================================================================
///	
///	File: ScriptingKeyframesMaterialResource.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingKeyframes.hpp"
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Keyframes for Material Resource type.
//==============================================================================

class ScriptingKeyframesMaterialResource: public ScriptingKeyframes {
    public:
        DEFINE_TYPE(ScriptingKeyframesMaterialResource,ScriptingKeyframes)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
												ScriptingKeyframesMaterialResource	(void);	
												ScriptingKeyframesMaterialResource	(const ScriptingKeyframesMaterialResource &rhs);
        ScriptingKeyframesMaterialResource &	operator =							(const ScriptingKeyframesMaterialResource &rhs);	
        virtual									~ScriptingKeyframesMaterialResource	(void);

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
		Plug<DTfloat>                               _t;
		Plug<std::shared_ptr<MaterialResource>>    _out;
		DTint                                       _id;
		
		struct keyframe {
			int operator < (const keyframe& rhs) const	{	return _time < rhs._time;	}
		
			DTfloat                                 _time;
			std::shared_ptr<MaterialResource>       _value;
			DTint                                   _id;
		};
		
		std::vector<keyframe>		_keyframes;
		mutable DTint				_keyframe_cache;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
