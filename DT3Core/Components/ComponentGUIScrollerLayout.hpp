#ifndef DT3_COMPONENTGUISCROLLERLAYOUT
#define DT3_COMPONENTGUISCROLLERLAYOUT
//==============================================================================
///	
///	File: ComponentGUIScrollerLayout.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Components/ComponentGUIScroller.hpp"
#include "DT3Core/Types/GUI/GUILayout.hpp"

namespace DT3 {

//==============================================================================
/// GUI behaviour for a scroll zone. It scrolls its children objects.
/// This component arranges its children into a grid
//==============================================================================

class ComponentGUIScrollerLayout: public ComponentGUIScroller {
    public:
        DEFINE_TYPE(ComponentGUIScrollerLayout,ComponentGUIScroller)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE

                                    ComponentGUIScrollerLayout	(void);	
									ComponentGUIScrollerLayout	(const ComponentGUIScrollerLayout &rhs);
        ComponentGUIScrollerLayout& operator =                  (const ComponentGUIScrollerLayout &rhs);	
        virtual                     ~ComponentGUIScrollerLayout	(void);
    
        virtual void                archive                     (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize			(void);
            
        /// Set layout
		/// \param layout layout object
        void                        set_layout          (std::shared_ptr<GUILayout> layout);

        /// Returns the layout object
        std::shared_ptr<GUILayout>  layout              (void) const;
    
    
        /// Sets the width of the sizer (if horizontal scrolling enabled)
        /// \param width Content width
        void                        set_content_width   (DTfloat width);
    
        /// Returns the width of the content
        DTfloat                     content_width       (void) const;

        /// Sets the height of the sizer (if vertical scrolling enabled)
        /// \param width Content height
        void                        set_content_height  (DTfloat height);

        /// Returns the height of the content
        DTfloat                     content_height      (void) const;

        /// Causes layout of the items
        void                        arrange_items       (DTfloat time = 0.0F);

    private:        
        std::shared_ptr<GUILayout>  _layout;
    
        DTfloat                     _content_width;
        DTfloat                     _content_height;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
