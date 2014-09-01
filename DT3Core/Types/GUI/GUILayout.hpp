#ifndef DT3_GUILAYOUT
#define DT3_GUILAYOUT
//==============================================================================
///	
///	File: GUILayout.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include <memory>
#include <list>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward Declarations
//==============================================================================

class Rectangle;
class GUIAnimKey;
class GUIObject;

//==============================================================================
/// Class
//==============================================================================

class GUILayout: public BaseClass {
    public:
        DEFINE_TYPE_BASE(GUILayout)
        		
                                                        GUILayout           (void);
                                                        GUILayout           (const GUILayout &rhs);
        GUILayout &                                     operator =          (const GUILayout &rhs);
		virtual                                         ~GUILayout          (void);
  
	public:
		/// Perform layout
		/// \param size_target Rectangle to fit widgets into
        virtual void                                    layout              (const Rectangle &size_target) = 0;

		/// Animate layout
		/// \param size_target Rectangle to fit widgets into
		/// \param time Animation time
		/// \param spline Use spline interpolation
        virtual void                                    animate             (const Rectangle &size_target, DTfloat time, DTboolean spline = false) = 0;
        
		/// Add an animation key
		/// \param key animation key to add
        virtual void                                    add_anim_key        (const GUIAnimKey &key) = 0;

		/// Clear all animations
        virtual void                                    clear_anims         (void) = 0;
    
        /// Content rectangle
        /// \return Content rectangle
        virtual Rectangle                               get_content_rect    (void) const = 0;

    
		/// Returns all objects managed by the layout
		/// \return all objects
        virtual std::list<std::shared_ptr<GUIObject>>   all_objects         (void) = 0;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
