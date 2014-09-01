//==============================================================================
///	
///	File: Group.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Node/Group.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveProcessPointers.hpp"
#include "DT3Core/World/WorldNode.hpp"
#include <algorithm>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(Group)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Group::Group (void)
    :   _description("Description"),
        _group_color((DTubyte)80,(DTubyte)80,(DTubyte)80,(DTubyte)255)
{  

}
		
Group::Group (const Group &rhs)
    :   PlugNode    (rhs),
        _description(rhs._description),
        _group_color(rhs._group_color)
{   

}

Group & Group::operator = (const Group &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		PlugNode::operator = (rhs);
        
        _description = rhs._description;
        _group_color = rhs._group_color;
        
        remove_all_nodes();
    }
    return (*this);
}
			
Group::~Group (void)
{
    remove_all_nodes();
}

//==============================================================================
//==============================================================================

void Group::initialize (void)
{
	PlugNode::initialize();
}

//==============================================================================
//==============================================================================

void Group::archive (const std::shared_ptr<Archive> &archive)
{
    PlugNode::archive(archive);

	archive->push_domain (class_id ());
    
    *archive << ARCHIVE_DATA(_description, DATA_PERSISTENT | DATA_SETTABLE);
    *archive << ARCHIVE_DATA_ACCESSORS("Group_Color", Group::group_color, Group::set_group_color, DATA_PERSISTENT);

    DTsize node_count = _nodes.size();;
    *archive << ARCHIVE_DATA(node_count,DATA_PERSISTENT);
    _nodes.resize(node_count, NULL);
    
    FOR_EACH (i,_nodes) {
        archive->add_post_process(ARCHIVE_PROCESS_POINTERS(archive,*i));
    }
    
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void Group::add_node (const std::shared_ptr<WorldNode> &node)
{
    auto i = std::find(_nodes.begin(), _nodes.end(), node);
    if (i == _nodes.end()) {
        _nodes.push_back(node);
        node->add_to_group(this);
    }
}

void Group::add_nodes (const std::list<std::shared_ptr<WorldNode>> &nodes)
{
    FOR_EACH (i,nodes) {
        add_node(*i);
    }
}
        
void Group::remove_node (const std::shared_ptr<WorldNode> &node)
{    
    auto i = std::find(_nodes.begin(), _nodes.end(), node);
    if (i != _nodes.end()) {
        _nodes.erase(i);
        
        node->add_to_group(NULL);
    }
}

void Group::remove_all_nodes (void)
{
    FOR_EACH(i, _nodes) {
        (**i).add_to_group(NULL);
    }
    _nodes.clear();
}

//==============================================================================
//==============================================================================

void Group::add_to_world(World *world)
{
    _world = world;
    SystemCallbacks::add_group_cb().fire(world,this);
}

void Group::remove_from_world (void)
{
    SystemCallbacks::remove_group_cb().fire(_world,this);
}

//==============================================================================
//==============================================================================

void Group::set_nodes_center (const Vector2 &center)
{
    // Find center
    Vector3 avg(0.0F,0.0F,0.0F);
    
    FOR_EACH (i,_nodes) {
        avg += (*i)->node_position();
    }
    
    avg /= _nodes.size();
    
    Vector2 delta = center - Vector2(avg);
    
    // Add an offset
    FOR_EACH (i,_nodes) {
        (*i)->set_node_position( (*i)->node_position() + Vector3(delta) );
    }

}

//==============================================================================
//==============================================================================

} // DT3

