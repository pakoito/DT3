//==============================================================================
///	
///	File: WorldNode.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/World/WorldNode.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveProcessPointers.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Node/Group.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(WorldNode)

IMPLEMENT_PLUG_INFO_INDEX(_self)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(WorldNode)

	PLUG_INIT(_self,"Self")
		.set_output(true);
      
END_IMPLEMENT_PLUGS


//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

WorldNode::WorldNode (void)
    :   _self                   (PLUG_INFO_INDEX(_self), this),
        _node_collapsed         (false),
        _node_position			(0.0F,0.0F,0.0F),
        _node_color             ((DTubyte)150,(DTubyte)150,(DTubyte)150,(DTubyte)255),
        _group                  (NULL),
        _world                  (NULL)
{
	set_name(class_id_child());
}
		
WorldNode::WorldNode (const WorldNode &rhs)
    :   PlugNode				(rhs),
        _self                   (PLUG_INFO_INDEX(_self), this),
        _node_collapsed         (rhs._node_collapsed),
		_node_position			(rhs._node_position),
        _node_color             (rhs._node_color),
        _group                  (NULL),
        _world                  (NULL)
{

}

WorldNode & WorldNode::operator = (const WorldNode &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		PlugNode::operator = (rhs);

        _node_collapsed = rhs._node_collapsed;
        _node_position = rhs._node_position;
        _node_color = rhs._node_color;
	}
    return (*this);
}
			
WorldNode::~WorldNode (void)
{

}

//==============================================================================
//==============================================================================

void WorldNode::initialize (void)
{
	PlugNode::initialize();
}

//==============================================================================
//==============================================================================

void WorldNode::archive (const std::shared_ptr<Archive> &archive)
{
    PlugNode::archive(archive);

	archive->push_domain (class_id ());
        
    *archive << ARCHIVE_DATA_ACCESSORS("Node_Collapsed", WorldNode::node_collapsed, WorldNode::set_node_collapsed, DATA_PERSISTENT);
    *archive << ARCHIVE_DATA_ACCESSORS("Node_Position", WorldNode::node_position, WorldNode::set_node_position, DATA_PERSISTENT);
    *archive << ARCHIVE_DATA_ACCESSORS("Node_Color", WorldNode::node_color, WorldNode::set_node_color, DATA_PERSISTENT);

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void WorldNode::add_to_world (World *world)
{	
	_world = world;		
    SystemCallbacks::add_node_cb().fire(_world,this);
}

//==============================================================================
//==============================================================================

void WorldNode::remove_from_world (void)
{	
    disconnect_all_plugs();
    disconnect_all_events();

    SystemCallbacks::remove_node_cb().fire(_world,this);
	_world = NULL;
}

//==============================================================================
//==============================================================================

void WorldNode::remove_from_group(void)
{	
	_group = NULL;
}

//==============================================================================
//==============================================================================

#ifdef DT3_EDITOR
void WorldNode::dump_code (const std::string &object_name, Stream &s)
{
    LOG_ERROR << "dump_code not implemented for " << PlugNode::class_id_child();
    s << "// dump_code not implemented for this node\n";
}
#endif

//==============================================================================
//==============================================================================
} // DT3

