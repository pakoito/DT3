#ifndef DT3_SCRIPTINGVECTOR3SMOOTH
#define DT3_SCRIPTINGVECTOR3SMOOTH
//==============================================================================
///	
///	File: ScriptingVector3Smooth.hpp
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
/// Averages out previous values.
//==============================================================================

class ScriptingVector3Smooth: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingVector3Smooth,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
									ScriptingVector3Smooth	(void);	
									ScriptingVector3Smooth	(const ScriptingVector3Smooth &rhs);
        ScriptingVector3Smooth &    operator =				(const ScriptingVector3Smooth &rhs);
        virtual						~ScriptingVector3Smooth	(void);
    
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);

		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world       (void);
		
	public:
        /// Registered with world to tick this node
		/// \param dt delta time
        void                        tick                    (const DTfloat dt);

		/// Called to initialize the object
		virtual void				initialize				(void);
				
		/// Sets the size of the history
		/// \param size history size
 		void						setHistorySize          (DTsize size)		{	_history.resize(size);	_local_reset = true;	}

		/// Gets the size of the history
		/// \return size history size
 		DTsize						getHistorySize          (void) const		{	return _history.size();	}

	private:			
		Plug<Vector3>				_in;
		Plug<Vector3>				_out;
		Plug<DTboolean>				_reset;
		
		DTboolean					_local_reset;
		
		std::vector<Vector3>		_history;
		DTint						_history_index;
		Vector3						_sum;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
