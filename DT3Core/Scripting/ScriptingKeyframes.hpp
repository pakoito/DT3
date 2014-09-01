#ifndef DT3_PLUGKEYFRAMES
#define DT3_PLUGKEYFRAMES
//==============================================================================
///	
///	File: ScriptingKeyframes.hpp
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

//==============================================================================
/// Base object for the different keyframes.
//==============================================================================

class ScriptingKeyframes: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingKeyframes,ScriptingBase)
		//DEFINE_CREATE_AND_CLONE
         
									ScriptingKeyframes		(void);	
									ScriptingKeyframes		(const ScriptingKeyframes &rhs);
        ScriptingKeyframes &		operator =				(const ScriptingKeyframes &rhs);	
        virtual						~ScriptingKeyframes		(void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
	
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);

		/// Set a key at the current time
		virtual void				set_key					(void) = 0;

		/// Clear a key at the current time
		virtual void				clear_key				(void) = 0;

		/// Clear a key with index
		/// \param k key index
		virtual void				clear_key				(DTint k) = 0;

		/// Get the number of keys
		/// \return number of keys
		virtual DTsize				num_keys                (void) const = 0;

		/// Returns a unique ID for this key
		/// \param k key index
		/// \return ID
        virtual DTint               key_id                  (DTint k) const = 0;
        
		/// Returns an index for the key with the ID
		/// \param id key id
		/// \return index
        virtual DTint               key_index               (DTint id) const;

		/// Get the time for the key
		/// \param k key index
		/// \return time
		virtual DTfloat				key_time                (DTint k) const = 0;

		/// Set the time for the key
		/// \param k key index
		/// \param time key time
		/// \return new index
		virtual DTint				set_key_time            (DTint k, DTfloat time) = 0;
        
};

//==============================================================================
//==============================================================================

} // DT3

#endif
