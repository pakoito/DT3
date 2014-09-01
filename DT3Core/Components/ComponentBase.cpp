//==============================================================================
///	
///	File: ComponentBase.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Components/ComponentBase.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveObjectQueue.hpp"
#include "DT3Core/Types/Node/PlugNode.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Objects/ObjectBase.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ComponentBase::ComponentBase (void)
    :   _owner      (NULL)
{

}
		
ComponentBase::ComponentBase (const ComponentBase &rhs)
    :   PlugNode	(rhs),
        _owner      (NULL)
{

}

ComponentBase & ComponentBase::operator = (const ComponentBase &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		PlugNode::operator = (rhs);
    }
    return (*this);
}
			
ComponentBase::~ComponentBase (void)
{
    
}

//==============================================================================
//==============================================================================

void ComponentBase::initialize (void)
{
	PlugNode::initialize();
    set_name(class_id_child());
}

//==============================================================================
//==============================================================================

void ComponentBase::archive (const std::shared_ptr<Archive> &archive)
{
    PlugNode::archive(archive);

	archive->push_domain (class_id());
        		
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

std::string ComponentBase::full_name (void) const
{
    return _owner->full_name() + ":" + name();
}

//==============================================================================
//==============================================================================

void ComponentBase::add_to_owner (ObjectBase *owner)
{
    _owner = owner;
}

void ComponentBase::remove_from_owner (void)
{
    disconnect_all_plugs();
    disconnect_all_events();

    _owner = NULL;
}

//==============================================================================
//==============================================================================

} // DT3

