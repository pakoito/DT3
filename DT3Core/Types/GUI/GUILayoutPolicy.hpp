#ifndef DT3_GUILAYOUTPOLICY
#define DT3_GUILAYOUTPOLICY
//==============================================================================
///	
///	File: GUILayoutPolicy.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Class
//==============================================================================

class GUILayoutPolicy {
    public:
        DEFINE_TYPE_BASE(GUILayoutPolicy)
    
        enum Policy {
            POLICY_FIXED_SIZE,
            POLICY_WEIGHTED_SIZE
        };

                                    GUILayoutPolicy     (void);
                                    GUILayoutPolicy		(Policy p, DTfloat v)   {   set_policy(p,v);            }
                                    GUILayoutPolicy		(const GUILayoutPolicy &rhs);
        GUILayoutPolicy&			operator =          (const GUILayoutPolicy &rhs);
		virtual                     ~GUILayoutPolicy	(void);
  
	public:
    
		/// Set the layout policy
		/// \param p policy
		/// \param v value
        void                        set_policy          (Policy p, DTfloat v)   {   _policy = p; _value = v;    }
        
        /// Returns the layout policy
		/// \return policy
        Policy                      policy              (void) const            {   return _policy;             }

        /// Returns the layout policy value or weight
		/// \return value or weight
        DTfloat                     value               (void) const            {   return _value;              }

    private:
        Policy                      _policy;
        DTfloat                     _value;
    
};

//==============================================================================
//==============================================================================

} // DT3

#endif
