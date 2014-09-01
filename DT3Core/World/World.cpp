//==============================================================================
///	
///	File: World.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/World/World.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/System.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Devices/DeviceAudio.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveProcessPointers.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Utility/Session.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Node/Group.hpp"
#include "DT3Core/Objects/GameController.hpp"
#include "DT3Core/Objects/CameraObject.hpp"
#include <algorithm>

//#ifdef DT3_DEBUG
#if 0
    #define VALIDATE validate();
#else
    #define VALIDATE
#endif

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with node factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_WORLD(World)
		
//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

World::World (void)	
    :   _default_camera ("CameraObject")
{
	set_name("World");
}	

World::~World (void)
{    

}

//==============================================================================
//==============================================================================

void World::initialize (void)	
{
	PlugNode::initialize();
}

void World::uninitialize (void)
{
	PlugNode::uninitialize();
    
    remove_all_groups();
    remove_all_nodes();
    
    clean();
}

//==============================================================================
//==============================================================================

void World::archive (const std::shared_ptr<Archive> &archive)
{
	PlugNode::archive(archive);

	archive->push_domain (class_id ());
    
    // Nodes
    DTsize node_count = _nodes.size();
    *archive << ARCHIVE_DATA(node_count,DATA_PERSISTENT);
    _nodes.resize(node_count, NULL);
    
    LOG_MESSAGE << node_count;
    
    FOR_EACH (i,_nodes) {
        archive->add_post_process(ARCHIVE_PROCESS_POINTERS(archive,*i));
    }

    // Groups
    DTsize group_count = _groups.size();
    *archive << ARCHIVE_DATA(group_count,DATA_PERSISTENT);
    _groups.resize(group_count, NULL);
    
    FOR_EACH (j,_groups) {
        archive->add_post_process(ARCHIVE_PROCESS_POINTERS(archive,*j));
    }
 
    // Default camera
    *archive << ARCHIVE_DATA(_default_camera,DATA_PERSISTENT | DATA_SETTABLE);
    
    // Session
    archive->add_post_process(ARCHIVE_PROCESS_POINTERS(archive,_session));
	
	archive->pop_domain();
}

void World::archive_done (const std::shared_ptr<Archive> &archive)
{		
	PlugNode::archive_done(archive);
		
    if (archive->is_writing())
        return;
    
    //
    // Remove NULL nodes and groups
    //
    
    auto nn = std::remove(_nodes.begin(), _nodes.end(), nullptr);
    _nodes.erase(nn, _nodes.end());

    auto gg = std::remove(_groups.begin(), _groups.end(), nullptr);
    _groups.erase(gg, _groups.end());

    VALIDATE

    //
    // Add objects
    //

	// Register special nodes.
    for (auto &i : _nodes) {
    			
        if (i->world() == NULL)
            i->add_to_world(this);
        
		// Special case game controller
		if (i->isa(GameController::kind())) {
			_game_controller = checked_static_cast<GameController>(i);
		}
				
		// Special case camera
		if (i->isa(CameraObject::kind())) {
			_camera = checked_static_cast<CameraObject>(i);
		}
        
	}
    
    VALIDATE
    
    //
    // Add groups
    //
    
    // Add groups to world
    for (auto &i : _groups) {
        if (!i->world())
            i->add_to_world(this);
    }
    
    VALIDATE

    //
    // Default camera
    //
    
    // Find the default camera if it exists
    std::shared_ptr<CameraObject> default_camera = checked_cast<CameraObject>(node_by_name(_default_camera));
    if (default_camera)
        _camera = default_camera;
    
    //
    // Ready to Go
    //
    
    // Signal ready to go
    ready_to_go();
    
    // Ready to Go!!
    for (auto &i : _nodes) {
        i->ready_to_go();
    }

    clean();
    
    VALIDATE
}

//==============================================================================
//==============================================================================

void World::set_session (const std::shared_ptr<Session> &s)
{
	_session = s;
}

const std::shared_ptr<Session>& World::session (void) const
{
    return _session;
}

//==============================================================================
//==============================================================================

void World::add_node (const std::shared_ptr<WorldNode> &node)
{	
	ASSERT(node);

	// Special case game controller
	if (node->isa(GameController::kind())) {
		_game_controller = checked_static_cast<GameController>(node);
	}
    
    // Special case camera
    if (node->isa(CameraObject::kind())) {
        _camera = checked_static_cast<CameraObject>(node);
    }
		
	// Add the node to the world list
	node->add_to_world(this);

	_nodes.push_back(node);
    
    VALIDATE
}


void World::add_node_unique_name (const std::shared_ptr<WorldNode> &node)
{
    // Check for existing node
    if (node_by_name(node->name())) {
    
        std::string default_name = MoreStrings::trim_end_digits(MoreStrings::trim(node->name()));

        // Check existing object names
        DTuint index = 1;
        std::string test_name = default_name + MoreStrings::cast_to_string(index);
        
        while (node_by_name(test_name)) {
            ++index;
            test_name = default_name + MoreStrings::cast_to_string(index);
        }
        
        node->set_name(test_name);
    }
    
    add_node(node);
    
    VALIDATE
}

void World::remove_node	(const std::shared_ptr<WorldNode> node)
{

	// Special case game controller
	if (_game_controller == node) {
		_game_controller.reset();
	}
    
    // Special case camera
    if (_camera == node) {
        _camera.reset();
    }
    
    // Build a list of all of the children nodes too
    std::list<WorldNode*> nodes;
    nodes.push_back(node.get());
    
    FOR_EACH (i,nodes) {
        PlaceableObject* placeable = checked_cast<PlaceableObject*>(*i);
        if (placeable) {
            std::list<PlaceableObject*> children = placeable->children();
            std::copy(children.begin(), children.end(), std::back_inserter(nodes));
        }
    
    }
	
    // Delete all nodes in reverse order from the built list
    for (auto i = nodes.rbegin(); i != nodes.rend(); ++i) {
        std::shared_ptr<WorldNode> node_to_delete = checked_cast<WorldNode>((*i)->shared_from_this());
    
        _nodes_to_delete.push_back(node_to_delete);

        auto j = std::find(_nodes.begin(), _nodes.end(), node_to_delete);
        if (j != _nodes.end())
            (**j).remove_from_world();
        
        _nodes.erase(j);
    }
    
    VALIDATE
}

void World::remove_all_nodes (void)
{
    while (_nodes.size() > 0) {
        remove_node(_nodes.front());
    }
    
    VALIDATE
}

//==============================================================================
//==============================================================================

void World::add_group (const std::shared_ptr<Group> &group)
{
	_groups.push_back(group);
    
    // Add the group to the world
    group->add_to_world(this);
    
    VALIDATE
}

void World::add_group_unique_name (const std::shared_ptr<Group> &group)
{
    if (group_by_name(group->name())) {
    
        std::string default_name = MoreStrings::trim_end_digits(MoreStrings::trim(group->name()));

        // Check existing object names
        DTuint index = 1;
        std::string test_name = default_name + MoreStrings::cast_to_string(index);
        
        while (group_by_name(test_name)) {
            ++index;
            test_name = default_name + MoreStrings::cast_to_string(index);
        }
        
        group->set_name(test_name);
    }
    
    add_group(group);
    
    VALIDATE
}

void World::remove_group (const std::shared_ptr<Group> group)
{
    // Remove the group from the world
    group->remove_from_world();

    auto i = std::remove(_groups.begin(), _groups.end(), group);
    _groups.erase(i,_groups.end());
    
    VALIDATE
}
			
void World::remove_all_groups (void)
{
	// delete all groups
	while (_groups.size() > 0) {
		remove_group(_groups.front());
	}
    
    VALIDATE
}

//==============================================================================
//==============================================================================

const std::list<std::shared_ptr<WorldNode>>& World::nodes (void) const
{    
    return _nodes;
}

std::shared_ptr<WorldNode> World::node_by_name (std::string name)
{
    auto i = std::find_if(_nodes.begin(), _nodes.end(),
        [&name](std::shared_ptr<WorldNode> &n) {
            return name == n->name();
        }
    );

    VALIDATE

    return i == _nodes.end() ? NULL : *i;
}

std::shared_ptr<WorldNode> World::node_by_id (DTuint unique_id)
{
    auto i = std::find_if(_nodes.begin(), _nodes.end(),
        [&unique_id](std::shared_ptr<WorldNode> &n) {
            return unique_id == n->unique_id();
        }
    );

    VALIDATE
    
    return i == _nodes.end() ? NULL : *i;
}

//==============================================================================
//==============================================================================

std::shared_ptr<Group> World::group_by_name (std::string name)
{
    auto i = std::find_if(_groups.begin(), _groups.end(),
        [&name](std::shared_ptr<Group> &g) {
            return name == g->name();
        }
    );
    
    VALIDATE

    return i == _groups.end() ? NULL : *i;
}

//==============================================================================
//==============================================================================

void World::set_camera (const std::shared_ptr<CameraObject> &camera)
{
    _camera = camera;
}

const std::shared_ptr<CameraObject>& World::camera (void) const
{
    return _camera;
}

//==============================================================================
//==============================================================================

void World::draw (const DTfloat lag)
{
    VALIDATE
    
    PROFILER(DRAW);

    clean();
    
    if (!_camera)
        return;
    
    for (auto draw_iterator = _draw_callbacks.begin(); draw_iterator != _draw_callbacks.end(); ++draw_iterator) {

//#ifdef DT3_EDITOR
//        if (_camera->is_picking()) {
//            PlaceableObject *placeable = checked_cast<PlaceableObject*>(draw_iterator->_node );
//            if (placeable && placeable->is_pickable() ) {
//                ::glLoadName(draw_iterator->_node->unique_id());
//                (*(draw_iterator->_callback))(_camera);
//            }
//        } else {
//#endif

            (*draw_iterator)(_camera, lag);
            
//#ifdef DT3_EDITOR
//        }
//#endif
        
    }
    
    VALIDATE
}

void World::draw_diagnostics (const DTfloat lag)
{
    VALIDATE
    
    PROFILER(DRAW_DIAGNOSTICS);

    clean();

    if (!_camera)
        return;

    for (auto draw_iterator = _draw_diagnostics_callbacks.begin(); draw_iterator != _draw_diagnostics_callbacks.end(); ++draw_iterator) {
        (*draw_iterator)(_camera, lag);
    }
    
    VALIDATE
}

//==============================================================================
//==============================================================================

void World::tick (const DTfloat dt)
{
    VALIDATE
    
    PROFILER(TICK);

    clean();

    for (auto tick_iterator = _tick_callbacks.begin(); tick_iterator != _tick_callbacks.end(); ++tick_iterator) {
        (*tick_iterator)(dt);
    }
    
    VALIDATE
}

//==============================================================================
//==============================================================================

void World::clean (void)
{
    VALIDATE
    
    //
    // Clean up nodes
    //
    
    
    for(auto &d : _nodes_to_delete) {
        auto dd = std::remove(_nodes.begin(), _nodes.end(), d);
        _nodes.erase(dd,_nodes.end());
    }                         
    _nodes_to_delete.clear();
    
    //
    // Clean up callbacks
    //
    
    for(auto &d : _draw_callbacks_to_delete) {
        auto dd = std::remove(_draw_callbacks.begin(), _draw_callbacks.end(), d);
        _draw_callbacks.erase(dd,_draw_callbacks.end());
    }

    for(auto &d : _draw_diagnostics_callbacks_to_delete) {
        auto dd = std::remove(_draw_diagnostics_callbacks.begin(), _draw_diagnostics_callbacks.end(), d);
        _draw_diagnostics_callbacks.erase(dd,_draw_diagnostics_callbacks.end());
    }

    for(auto &d : _tick_callbacks_to_delete) {
        auto dd = std::remove(_tick_callbacks.begin(), _tick_callbacks.end(), d);
        _tick_callbacks.erase(dd,_tick_callbacks.end());
    }

    for(auto &d : _touch_callbacks_to_delete) {
        auto dd = std::remove(_touch_callbacks.begin(), _touch_callbacks.end(), d);
        _touch_callbacks.erase(dd,_touch_callbacks.end());
    }

    _draw_callbacks_to_delete.clear();
    _draw_diagnostics_callbacks_to_delete.clear();
    _tick_callbacks_to_delete.clear();
    _touch_callbacks_to_delete.clear();
    
    //
    // Clean up groups
    //

    for (auto i = _groups.begin(); i != _groups.end();) {
        
        if ( (*i)->nodes().size() == 0) {
            (*i)->remove_from_world();
            i = _groups.erase(i);
        } else {
            ++i;
        }
    }

    VALIDATE
}

//==============================================================================
//==============================================================================

DTboolean World::touch_gui (const TouchEvent *event)
{
    WorldNode *handled = NULL;

    for (auto touch_iterator = _touch_callbacks.rbegin(); touch_iterator != _touch_callbacks.rend(); ++touch_iterator) {
        (*touch_iterator)(event, _camera, handled);
    }
    
    return (handled != NULL);
}

//==============================================================================
//==============================================================================

void World::register_for_draw (WorldNode *node, std::shared_ptr<DrawCallbackType> cb)
{
    ASSERT(std::find(_draw_callbacks.begin(), _draw_callbacks.end(), DrawCallbackEntry(node, cb)) == _draw_callbacks.end());
    _draw_callbacks.emplace_back(node, cb);
}

void World::unregister_for_draw (WorldNode *node, std::shared_ptr<DrawCallbackType> cb)
{
    _draw_callbacks_to_delete.emplace_back(node, cb);
}

//==============================================================================
//==============================================================================

void World::register_for_draw_diagnostics (WorldNode *node, std::shared_ptr<DrawCallbackType> cb)
{
    ASSERT(std::find(_draw_diagnostics_callbacks.begin(), _draw_diagnostics_callbacks.end(), DrawCallbackEntry(node, cb)) == _draw_diagnostics_callbacks.end());
    _draw_diagnostics_callbacks.emplace_back(node, cb);
}

void World::unregister_for_draw_diagnostics (WorldNode *node, std::shared_ptr<DrawCallbackType> cb)
{
    _draw_diagnostics_callbacks_to_delete.emplace_back(node, cb);
}

//==============================================================================
//==============================================================================

void World::register_for_tick (WorldNode *node, std::shared_ptr<TickCallbackType> cb)
{
    if (std::find(_tick_callbacks.begin(), _tick_callbacks.end(), TickCallbackEntry(node, cb)) == _tick_callbacks.end())
        _tick_callbacks.emplace_back(node, cb);
}

void World::unregister_for_tick (WorldNode *node, std::shared_ptr<TickCallbackType> cb)
{
    _tick_callbacks_to_delete.emplace_back(node, cb);
}

//==============================================================================
//==============================================================================

void World::register_for_touch (WorldNode *node, std::shared_ptr<TouchCallbackType> cb)
{
    ASSERT(std::find(_touch_callbacks.begin(), _touch_callbacks.end(), TouchCallbackEntry(node, cb)) == _touch_callbacks.end());
    _touch_callbacks.emplace_back(node, cb);
}

void World::unregister_for_touch (WorldNode *node, std::shared_ptr<TouchCallbackType> cb)
{
    _touch_callbacks_to_delete.emplace_back(node, cb);
}

//==============================================================================
//==============================================================================

void World::validate (void)
{
    const std::list<std::shared_ptr<WorldNode>> &objects = nodes();

    // Add the new cameras to the list
    FOR_EACH (i,objects) {
        ASSERT( *i != NULL );           // Check Null
        ASSERT( (**i).unique_id() );    // Just a query
    }
}

//==============================================================================
//==============================================================================

} // DT3

