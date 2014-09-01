//==============================================================================
///	
///	File: ComponentGUIScrollerLayout.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Components/ComponentGUIScrollerLayout.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveObjectQueue.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/GUI/GUITouchEvent.hpp"
#include "DT3Core/Objects/GUIObject.hpp"
#include "DT3Core/System/Factory.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_COMPONENT(ComponentGUIScrollerLayout,"GUIBehaviour","ComponentAdapter")
IMPLEMENT_PLUG_NODE(ComponentGUIScrollerLayout)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ComponentGUIScrollerLayout)
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ComponentGUIScrollerLayout::ComponentGUIScrollerLayout (void)
    :   _content_width  (0.0F),
        _content_height (0.0F)
{

}
		
ComponentGUIScrollerLayout::ComponentGUIScrollerLayout (const ComponentGUIScrollerLayout &rhs)
    :   ComponentGUIScroller    (rhs),
        _content_width          (rhs._content_width),
        _content_height         (rhs._content_height)
{

}

ComponentGUIScrollerLayout & ComponentGUIScrollerLayout::operator = (const ComponentGUIScrollerLayout &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ComponentGUIScroller::operator = (rhs);
        
        _content_width = rhs._content_width;
        _content_height = rhs._content_height;
    }
    return (*this);
}
			
ComponentGUIScrollerLayout::~ComponentGUIScrollerLayout (void)
{

}

//==============================================================================
//==============================================================================

void ComponentGUIScrollerLayout::initialize (void)
{
	ComponentGUIScroller::initialize();
}

//==============================================================================
//==============================================================================

void ComponentGUIScrollerLayout::archive (const std::shared_ptr<Archive> &archive)
{
    ComponentGUIScroller::archive(archive);

	archive->push_domain (class_id ());
    
	*archive << ARCHIVE_DATA_ACCESSORS("Content_Width", ComponentGUIScrollerLayout::content_width, ComponentGUIScrollerLayout::set_content_width, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA_ACCESSORS("Content_Height", ComponentGUIScrollerLayout::content_height, ComponentGUIScrollerLayout::set_content_height, DATA_PERSISTENT | DATA_SETTABLE);
    
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ComponentGUIScrollerLayout::arrange_items (DTfloat time)
{
    GUIObject *gui = checked_cast<GUIObject*>(owner());
    if (!gui)
        return;
    
    if (!_layout)
        return;
    
    DTfloat width = scroll_horz() ? _content_width : gui->rectangle().width();
    DTfloat height = scroll_vert() ? _content_height : gui->rectangle().height();
    
    _layout->animate(Rectangle(-width*0.5F, width*0.5F, -height*0.5F, height*0.5F), time);

    calculate_contect_rect();
}

//==============================================================================
//==============================================================================

void ComponentGUIScrollerLayout::set_layout (std::shared_ptr<GUILayout> layout)
{
    _layout = layout;
}

std::shared_ptr<GUILayout> ComponentGUIScrollerLayout::layout (void) const
{
    return _layout;
}

void ComponentGUIScrollerLayout::set_content_width (DTfloat width)
{
    _content_width = width;
}

DTfloat ComponentGUIScrollerLayout::content_width (void) const
{
    return _content_width;
}

void ComponentGUIScrollerLayout::set_content_height (DTfloat height)
{
    _content_height = height;
}

DTfloat ComponentGUIScrollerLayout::content_height (void) const
{
    return _content_height;
}

//==============================================================================
//==============================================================================

} // DT3

