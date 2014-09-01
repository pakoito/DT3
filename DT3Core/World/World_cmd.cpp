//==============================================================================
///	
///	File: World_cmd.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/Command.hpp"

#ifdef DT3_COMMANDS

#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/System/Console.hpp"
#include "DT3Core/Types/Utility/CommandResult.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandContext.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/Math/Color4f.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveTextBufferReader.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveTextBufferWriter.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveTextReader.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveTextWriter.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveObjectQueue.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/Node/PlugNodeUtils.hpp"
#include "DT3Core/Types/Node/Group.hpp"
#include "DT3Core/Objects/PlaceableObject.hpp"
#include "DT3Core/World/World.hpp"
#include "DT3Core/World/WorldNode.hpp"
#include DT3_HAL_INCLUDE_PATH
#include <algorithm>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class World_cmd: public Command {
    public:
        DEFINE_TYPE(World_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("Add", &World_cmd::doAdd);
            CommandRegistry::register_command("AddArchive", &World_cmd::doAddArchive);
            CommandRegistry::register_command("Remove", &World_cmd::doRemove);
            CommandRegistry::register_command("Duplicate", &World_cmd::doDuplicate);
            CommandRegistry::register_command("DuplicateHierarchy", &World_cmd::doDuplicateHierarchy);
            CommandRegistry::register_command("Rename", &World_cmd::doRename);
            CommandRegistry::register_command("Select", &World_cmd::doSelect);
            CommandRegistry::register_command("SelectAll", &World_cmd::doSelectAll);
            CommandRegistry::register_command("SelectNone", &World_cmd::doSelectNone);
            CommandRegistry::register_command("ListSelected", &World_cmd::doListSelected);

            CommandRegistry::register_command("New", &World_cmd::doNew);
            CommandRegistry::register_command("Save", &World_cmd::doSave);
            CommandRegistry::register_command("Load", &World_cmd::doLoad);
            CommandRegistry::register_command("Export", &World_cmd::doExport);
            CommandRegistry::register_command("Import", &World_cmd::doImport);
            CommandRegistry::register_command("RunScript", &World_cmd::doRunScript);
        }

        static CommandResult doAdd (CommandContext &ctx, const CommandParams &p);
        static CommandResult doAddArchive (CommandContext &ctx, const CommandParams &p);
        static CommandResult doRemove (CommandContext &ctx, const CommandParams &p);
        static CommandResult doDuplicate (CommandContext &ctx, const CommandParams &p);
        static CommandResult doDuplicateHierarchy (CommandContext &ctx, const CommandParams &p);
        static CommandResult doRename (CommandContext &ctx, const CommandParams &p);
        static CommandResult doSelect (CommandContext &ctx, const CommandParams &p);
        static CommandResult doSelectAll (CommandContext &ctx, const CommandParams &p);
        static CommandResult doSelectNone (CommandContext &ctx, const CommandParams &p);
        static CommandResult doListSelected (CommandContext &ctx, const CommandParams &p);
        
        static CommandResult doNew (CommandContext &ctx, const CommandParams &p);
        static CommandResult doSave (CommandContext &ctx, const CommandParams &p);
        static CommandResult doLoad (CommandContext &ctx, const CommandParams &p);
        static CommandResult doExport (CommandContext &ctx, const CommandParams &p);
        static CommandResult doImport (CommandContext &ctx, const CommandParams &p);
        static CommandResult doRunScript (CommandContext &ctx, const CommandParams &p);

};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(World_cmd)

//==============================================================================
//==============================================================================

CommandResult World_cmd::doAdd (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() < 2 || p.count() > 3) {
        return CommandResult(false, "Usage: Add kind [name]", CommandResult::UPDATE_NONE);
    }

    std::shared_ptr<BaseClass> new_object_base = Factory::create_object(p[1]);
    
    // Check that an object was created
    if (!new_object_base) {
        return CommandResult(false, "Unable to create object: " + p[1], CommandResult::UPDATE_NONE);
    }

    std::shared_ptr<WorldNode> new_object = checked_cast<WorldNode>(new_object_base);
    
    // Verify node can be added to the world
    if (!new_object) {
        return CommandResult(false, "Unable to add object: " + p[1], CommandResult::UPDATE_NONE);
    }
    
    // Position
    new_object->set_node_position(Vector3(ctx.selection_rectangle().center().x, ctx.selection_rectangle().center().y, 0.0F));
    
    // Set the name of the object
    std::string default_name;
    if (p.count() == 3) {
        default_name = p[2];
    } else {
        default_name = new_object->preferred_name();
    }
    new_object->set_name(default_name);
    
    // Colorize the node
    std::string color_global = "SYS_" + p[1] + "_COLOR";
    if (Globals::has_global(color_global)) {
        Color4f c = MoreStrings::cast_from_string<Color4f>(Globals::global(color_global));
        new_object->set_node_color(c);
    }

    // Add the object
    ctx.world()->add_node_unique_name(new_object);
    
    CommandResult cr;

    //
    // Add Undo Command
    //
    
    cr.append_undo_command(CommandParams("Remove \"" + new_object->name() + "\"" ));
    
    // Change the selection
    std::list<std::shared_ptr<PlugNode>> new_selection;
    new_selection.push_back(new_object);
    
    ctx.clear_selection();
    ctx.set_selection(new_selection);
    
    
    cr.set_message(new_object->name());
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_ANIMATION);

    return cr;
}

CommandResult World_cmd::doAddArchive (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: AddArchive archive", CommandResult::UPDATE_NONE);
    }

    LOG_MESSAGE << p[1];
    
    std::shared_ptr<ArchiveTextBufferReader> reader = ArchiveTextBufferReader::create();
    reader->stream().set_ignore_whitespace(false);   // need to set this when reading objects
    reader->stream().set_buffer(p[1]);
    
    std::shared_ptr<BaseClass> new_object_base = ArchiveObjectQueue::queue_in_tree(reader);

    // Check that an object was created
    if (!new_object_base) {
        return CommandResult(false, "Unable to create object: " + p[1], CommandResult::UPDATE_NONE);
    }

    std::shared_ptr<WorldNode> new_object = checked_cast<WorldNode>(new_object_base);
    
    // Verify node can be added to the world
    if (!new_object) {
        return CommandResult(false, "Unable to add object: " + p[1], CommandResult::UPDATE_NONE);
    }
        
    // Add the object
    ctx.world()->add_node_unique_name(new_object);
    
    CommandResult cr;

    //
    // Add Undo Command
    //
    
    cr.append_undo_command(CommandParams("Remove \"" + new_object->name() + "\"" ));

    // Change the selection
    std::list<std::shared_ptr<PlugNode>> new_selection;
    new_selection.push_back(new_object);
    
    ctx.clear_selection();
    ctx.set_selection(new_selection);
    
    
    cr.set_message(new_object->name());
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_ANIMATION);

    return cr;
}

CommandResult World_cmd::doRemove (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: Remove object", CommandResult::UPDATE_NONE);
    }
      
    std::shared_ptr<WorldNode> obj = ctx.world()->node_by_name(p[1]);
    if (!obj) {
        return CommandResult(false, "Unable to find object: " + p[1], CommandResult::UPDATE_NONE);
    }
    
    CommandResult cr;

    //
    // Add Undo Commands
    //
    
    // Holy crap this is a biggie. This has to build the undo queue for an object taking into account its
    // children, components, plugs and events.
    
    // Build a list of all of the node and children nodes
    std::list<std::shared_ptr<WorldNode>> nodes;
    nodes.push_back(obj);
    
    FOR_EACH (node,nodes) {
        std::shared_ptr<PlaceableObject> placeable = checked_cast<PlaceableObject>(*node);
        
        if (placeable) {
            std::list<PlaceableObject*> children = placeable->children ();
            
            for (auto &j : children) {
                nodes.push_back(checked_cast<PlaceableObject>(j->shared_from_this()));
            }
        }
    
    }
	
    // Add to undo queue, all nodes from the built list
    FOR_EACH (node,nodes) {
    
        std::shared_ptr<ArchiveTextBufferWriter> writer = ArchiveTextBufferWriter::create();
        
        writer->set_ignore_streamable_flag(true);
        ArchiveObjectQueue::queue_out_tree(writer, *node);
        
        cr.append_undo_command(CommandParams( "AddArchive \"" + writer->stream().buffer() + "\"" ));
        
        // Set the parent
        if ((*node)->isa(PlaceableObject::kind())) {
            PlaceableObject* parent = checked_cast<PlaceableObject*>(node->get())->parent();
            if (parent)
                cr.append_undo_command(CommandParams("Parent \"" + (*node)->name() + "\" \"" + parent->name() + "\"" ));
        }
    }    
        
    FOR_EACH (node,nodes) {

        // Adding components
        std::list<std::shared_ptr<PlugNode>> node_and_components;
        PlugNodeUtils::node_and_components(*node, node_and_components);
        
        // Build nodes and components
        FOR_EACH (node_and_components_iter,node_and_components) {
            // Scan connections
            for (PlugIter p_iter(node_and_components_iter->get()); p_iter; ++p_iter) {
                if (p_iter()->is_no_draw())   continue;
                
                // Incoming
                if (p_iter()->has_incoming_connection()) {
                    if (p_iter()->incoming_connection()->is_no_draw())   continue;
                    cr.append_undo_command(CommandParams("ConnectPlugs \"" + p_iter()->incoming_connection()->full_name() + "\" \"" + p_iter()->full_name() + "\"" ));
                }
                
                // Outgoing
                const std::vector<PlugBase*> &connections = p_iter()->outgoing_connections();
                for (DTuint k = 0; k < connections.size(); ++k) {
                    if (connections[k]->is_no_draw())   continue;
                    cr.append_undo_command(CommandParams("ConnectPlugs \"" + p_iter()->full_name() + "\" \"" + connections[k]->full_name() + "\"" ));
                }

            }
            
            // Events
            for (EventIter e_iter(node_and_components_iter->get()); e_iter; ++e_iter) {
                if (e_iter()->is_no_draw())   continue;

                // Incoming
                const std::vector<Event*> &connections_in = e_iter()->incoming_connections();
                for (DTuint k = 0; k < connections_in.size(); ++k) {
                    if (connections_in[k]->is_no_draw())   continue;
                    cr.append_undo_command(CommandParams("ConnectEvents \"" + connections_in[k]->full_name() + "\" \"" + e_iter()->full_name() + "\"" ));
                }
                
                // Outgoing
                const std::vector<Event*> &connections_out = e_iter()->outgoing_connections();
                for (DTuint k = 0; k < connections_out.size(); ++k) {
                    if (connections_out[k]->is_no_draw())   continue;
                    cr.append_undo_command(CommandParams("ConnectEvents \"" + e_iter()->full_name() + "\" \"" + connections_out[k]->full_name() + "\"" ));
                }

            }
                        
        }
        
    }
    
    cr.set_message(obj->name());
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_ANIMATION);

    //obj->disconnectAllPlugs();
    //obj->disconnectAllEvents();
    ctx.world()->remove_node(obj);
    ctx.world()->clean();
    
    // Change the selection
    std::list<std::shared_ptr<PlugNode>> new_selection = ctx.selection();
    
    // Remove deleted nodes from the selection
    FOR_EACH (node,nodes) {
        auto i = std::find(new_selection.begin(), new_selection.end(), *node);
        if (i != new_selection.end())
            new_selection.erase(i);
    }

    ctx.clear_selection();
    ctx.set_selection(new_selection);

    return cr;
}

CommandResult World_cmd::doDuplicate(CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 1) {
        return CommandResult(false, "Usage: Duplicate", CommandResult::UPDATE_NONE);
    }
        
    std::map<std::shared_ptr<PlugNode>, std::shared_ptr<PlugNode>>  orig_to_new_node;

    const std::list<std::shared_ptr<PlugNode>>& selection = ctx.selection();
    std::list<std::shared_ptr<PlugNode>> new_selection;

    PlugNodeUtils::copy_nodes (selection, new_selection, orig_to_new_node);
    
    // Release new nodes because copy increases ref counts in the new list
    FOR_EACH (i,new_selection) {
        std::shared_ptr<WorldNode> world_node = checked_static_cast<WorldNode>(*i);
        
        world_node->set_node_position(world_node->node_position() + Vector3(10.0F,10.0F,0.0F));
        
        ctx.world()->add_node_unique_name(world_node);
    }

    //
    // Copy to same place in hierarchy
    //
    
    FOR_EACH (mi,orig_to_new_node) {
        std::shared_ptr<PlugNode> orig_node = mi->first;
        std::shared_ptr<PlugNode> new_node = mi->second;

        // Place into the same place in the hierarchy
        if ( orig_node->isa(PlaceableObject::kind()) && new_node->isa(PlaceableObject::kind()) ) {
            std::shared_ptr<PlaceableObject> orig_placeable = checked_static_cast<PlaceableObject>(orig_node);
            std::shared_ptr<PlaceableObject> new_placeable = checked_static_cast<PlaceableObject>(new_node);
        
            if (orig_placeable->parent())
                orig_placeable->parent()->add_child(new_placeable);
        }
    }
    
    
    CommandResult cr;

    //
    // Add Undo Command
    //
    
    FOR_EACH (mi,orig_to_new_node) {
        std::shared_ptr<PlugNode> new_node = mi->second;

        cr.append_undo_command(CommandParams("Remove \"" + new_node->name() + "\"" ));
    }
        
    // Change the selection
    ctx.clear_selection();
    ctx.set_selection(new_selection);
    
    
    cr.set_message("Duplicated selection");
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_ANIMATION);

    return cr;

}

CommandResult World_cmd::doDuplicateHierarchy(CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 1) {
        return CommandResult(false, "Usage: DuplicateHierarchy", CommandResult::UPDATE_NONE);
    }
        
    std::map<std::shared_ptr<PlugNode>, std::shared_ptr<PlugNode>>  orig_to_new_node;

    std::list<std::shared_ptr<PlugNode>> selection = ctx.selection();
    
    // Expand the selection to include children
    FOR_EACH(i,selection) {
        std::shared_ptr<PlaceableObject> placeable = checked_cast<PlaceableObject>(*i);
        if (!placeable)
            continue;
    
        std::list<PlaceableObject*> children = placeable->children();
        FOR_EACH(j,children) {
            
            // Check if already in selection
            std::shared_ptr<PlaceableObject> child = checked_cast<PlaceableObject>( (**j).shared_from_this() );

            if (std::find(selection.begin(), selection.end(), child) != selection.end())
                continue;
            
            selection.push_back(checked_cast<PlugNode>(child));
        }
    }

    
    std::list<std::shared_ptr<PlugNode>> new_selection;

    PlugNodeUtils::copy_nodes (selection, new_selection, orig_to_new_node);
    
    // Release new nodes because copy increases ref counts in the new list
    FOR_EACH (i,new_selection) {
        std::shared_ptr<WorldNode> world_node = checked_static_cast<WorldNode>(*i);
        
        world_node->set_node_position(world_node->node_position() + Vector3(10.0F,10.0F,0.0F));
        
        ctx.world()->add_node_unique_name(world_node);
    }

    //
    // Copy to same relative place in hierarchy
    //
    
    new_selection.clear();
    
    FOR_EACH (mi,orig_to_new_node) {
        std::shared_ptr<PlugNode> orig_node = mi->first;
        std::shared_ptr<PlugNode> new_node = mi->second;
                
        // Place into the same place in the hierarchy
        if ( orig_node->isa(PlaceableObject::kind()) && new_node->isa(PlaceableObject::kind()) ) {
            std::shared_ptr<PlaceableObject> orig_placeable = checked_static_cast<PlaceableObject>(orig_node);
            std::shared_ptr<PlaceableObject> new_placeable = checked_static_cast<PlaceableObject>(new_node);
        
            std::shared_ptr<PlaceableObject> new_parent = NULL;
            
            // Check parent to see if it was also duplicated
            std::shared_ptr<PlugNode> parent = checked_cast<PlugNode>(orig_placeable->parent()->shared_from_this());
            std::map<std::shared_ptr<PlugNode>, std::shared_ptr<PlugNode>>::iterator new_parent_pair = orig_to_new_node.find(parent);
            if (new_parent_pair != orig_to_new_node.end()) {
                
                // Get the new parent node
                new_parent = checked_cast<PlaceableObject>(new_parent_pair->second);
            } else {
                new_selection.push_back(new_placeable);
                new_parent = checked_cast<PlaceableObject>(orig_placeable->parent()->shared_from_this());
            }
            
            if (new_parent) {
                new_parent->add_child(new_placeable);
            }

        }
    }
    
    
    CommandResult cr;

    //
    // Add Undo Command
    //
    
    FOR_EACH (mi,orig_to_new_node) {
        std::shared_ptr<PlugNode> new_node = mi->second;

        cr.append_undo_command(CommandParams("Remove \"" + new_node->name() + "\"" ));
    }
        
    // Change the selection
    ctx.clear_selection();
    ctx.set_selection(new_selection);
    
    
    cr.set_message("Duplicated selection");
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_ANIMATION);

    return cr;

}

CommandResult World_cmd::doRename (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 3) {
        return CommandResult(false, "Usage: Rename object newname", CommandResult::UPDATE_NONE);
    }
      
    std::shared_ptr<WorldNode> obj = ctx.world()->node_by_name(p[1]);
    if (!obj) {
        return CommandResult(false, "Unable to find object: " + p[1], CommandResult::UPDATE_NONE);
    }
    
    if (ctx.world()->node_by_name(p[2])) {
        return CommandResult(false, "Object by that name already exits.", CommandResult::UPDATE_NONE);
    }
    
    CommandResult cr;

    //
    // Add Undo Command
    //
    
    cr.append_undo_command(CommandParams("Rename \"" + p[2] + "\" \"" + obj->name() + "\"" ));
    
    // Rename
    obj->set_name(p[2]);

    cr.set_message(p[2]);
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_ANIMATION);

    return cr;
}

//==============================================================================
//==============================================================================

CommandResult World_cmd::doSelect (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() <= 1) {
        return CommandResult(false, "Usage: Select object [object]...", CommandResult::UPDATE_NONE);
    }
    
    // Change the selection
    std::list<std::shared_ptr<PlugNode>> new_selection;
          
    for (DTuint i = 1; i != p.count(); ++i) {
        std::shared_ptr<PlugNode> obj = checked_cast<PlugNode>(ctx.world()->node_by_name(p[i]));
        if (obj) {
            new_selection.push_back(obj);
        }    
    }
      
    ctx.clear_selection();
    ctx.set_selection(new_selection);

    return CommandResult(true, p[1], CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_ANIMATION);    
}

CommandResult World_cmd::doSelectAll (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 1) {
        return CommandResult(false, "Usage: SelectAll", CommandResult::UPDATE_NONE);
    }
    
    // Change the selection
    std::list<std::shared_ptr<PlugNode>> new_selection;
          
    const std::list<std::shared_ptr<WorldNode>> &nodes = ctx.world()->nodes();

    FOR_EACH (i,nodes) {
        new_selection.push_back(*i);
    }
      
    ctx.clear_selection();
    ctx.set_selection(new_selection);

    return CommandResult(true, p[1], CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_ANIMATION);    
}

CommandResult World_cmd::doSelectNone (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 1) {
        return CommandResult(false, "Usage: SelectNone", CommandResult::UPDATE_NONE);
    }
    
    // Change the selection
    std::list<std::shared_ptr<PlugNode>> new_selection;
          
    ctx.clear_selection();
    ctx.set_selection(new_selection);

    return CommandResult(true, p[1], CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_ANIMATION);    
}

CommandResult World_cmd::doListSelected (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 1) {
        return CommandResult(false, "Usage: ListSelected", CommandResult::UPDATE_NONE);
    }
    
    // Change the selection
    const std::list<std::shared_ptr<PlugNode>> &selection = ctx.selection();
    
    std::string nodes;
          
    FOR_EACH (i,selection) {
        std::shared_ptr<WorldNode> node = checked_cast<WorldNode>(*i);
        if (!node)  
            continue;
    
        if (nodes.empty())
            nodes += node->name();
        else
            nodes += " " + node->name();
    }

    return CommandResult(true, nodes, CommandResult::UPDATE_NONE);    
}

//==============================================================================
//==============================================================================

CommandResult World_cmd::doNew (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: New worldtype", CommandResult::UPDATE_NONE);
    }

    std::shared_ptr<World> world = checked_cast<World>(Factory::create_world(p[1]));
    if (!world)
        return CommandResult(false, "Unable to create world " + p[1], CommandResult::UPDATE_NONE);

    // Change the selection
    std::list<std::shared_ptr<PlugNode>> new_selection;
    ctx.clear_selection();
    ctx.set_selection(new_selection);
    ctx.set_world(world);
    
    return CommandResult(true, "New " + p[1], CommandResult::UPDATE_ALL);
}

CommandResult World_cmd::doSave (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: Save file", CommandResult::UPDATE_NONE);
    }
    
    FilePath temp_file("{APPDIR}/temp.lvl");
    
    std::shared_ptr<ArchiveTextWriter> writer = ArchiveTextWriter::create();
	writer->set_ignore_streamable_flag(true);
	writer->open(FilePath(p[1]));
	ArchiveObjectQueue::queue_out_tree(writer, ctx.world() );
	writer->close();
    
    HAL::delete_file(FilePath(p[1]));
    HAL::move_file(temp_file,FilePath(p[1]));

    return CommandResult(true, "Saved " + p[1], CommandResult::UPDATE_NONE);    
}

CommandResult World_cmd::doLoad (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: Load file", CommandResult::UPDATE_NONE);
    }
    
    CommandResult cr;
    
	std::shared_ptr<BaseClass> base_class = Factory::create_object_from_stream(FilePath(p[1]));
	if (base_class && base_class->isa(World::kind())) {
		std::shared_ptr<World> obj = checked_static_cast<World>(base_class);
		obj->set_streamable(true);
        
        ctx.set_world(obj);
        
        cr = CommandResult(true, "Loaded " + p[1], CommandResult::UPDATE_ALL);
	} else {
        cr = CommandResult(false, "unable to load " + p[1], CommandResult::UPDATE_NONE);
    }
    
    // Change the selection
    std::list<std::shared_ptr<PlugNode>> new_selection;
    ctx.clear_selection();
    ctx.set_selection(new_selection);

    return cr;    
}

//==============================================================================
//==============================================================================

CommandResult World_cmd::doExport (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() < 2 || p.count() > 4) {
        return CommandResult(false, "Usage: Export path [name] [description]", CommandResult::UPDATE_NONE);
    }
    
    const std::list<std::shared_ptr<PlugNode>>& selection = ctx.selection();
    std::list<std::shared_ptr<PlugNode>> items_plugs;
    std::list<std::shared_ptr<WorldNode>> items_world;
    
    PlugNodeUtils::copy_nodes(selection, items_plugs);
    
    // Build a list of world nodes
    FOR_EACH (i,items_plugs) {
        std::shared_ptr<WorldNode> item_world = checked_cast<WorldNode>(*i);
        if (item_world) {
            items_world.push_back(item_world);
        }
    }
    
    std::shared_ptr<Group> group = Group::create();
    
    if (p.count() >= 3)   group->set_name(p[2]);
    if (p.count() >= 4)   group->set_description(p[3]);
    
    group->add_nodes(items_world);
    
    std::shared_ptr<ArchiveTextWriter> writer = ArchiveTextWriter::create();
    DTerr error = writer->open(FilePath(p[1]));
    if (error != DT3_ERR_NONE)
		return CommandResult(false, "Unable to export " + p[1], CommandResult::UPDATE_NONE);
    
    writer->set_ignore_streamable_flag(true);
    ArchiveObjectQueue::queue_out_tree(writer, group );
    writer->close();

    return CommandResult(true, "Exported " + p[1], CommandResult::UPDATE_NONE);
}

CommandResult World_cmd::doImport (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: Import", CommandResult::UPDATE_NONE);
    }
        
    std::shared_ptr<ArchiveTextReader> reader = ArchiveTextReader::create();
    DTerr error = reader->open(FilePath(p[1]));
    if (error != DT3_ERR_NONE)
		return CommandResult(false, "Unable to import " + p[1], CommandResult::UPDATE_NONE);
    
    std::shared_ptr<Group> group = checked_cast<Group>(ArchiveObjectQueue::queue_in_tree(reader));
    if (!group)
        return CommandResult(true, "Not imported", CommandResult::UPDATE_NONE);
     
    // Center nodes to middle of viewport
    group->set_nodes_center(ctx.selection_rectangle().center());

    std::list<std::shared_ptr<WorldNode>> &items_world = group->nodes();
    
    // Add nodes to world
    FOR_EACH (i,items_world) {
        ctx.world()->add_node_unique_name(*i);
    }
        
    group->remove_all_nodes();

    return CommandResult(true, "Imported " + p[1], CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_ANIMATION);
}

//==============================================================================
//==============================================================================

CommandResult World_cmd::doRunScript (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: RunScript", CommandResult::UPDATE_NONE);
    }
    
    TextFileStream reader;

    // open the file
	DTerr error = FileManager::open(reader, FilePath(p[1]), true, NULL);
	if (error != DT3_ERR_NONE) {
		return CommandResult(false, "Unable to run " + p[1], CommandResult::UPDATE_NONE);
    }

    while (!reader.is_eof()) {
        DTcharacter buffer[1024];
        reader.line (buffer, ARRAY_SIZE(buffer));
            
        CommandResult result = Console::do_command(ctx, CommandParams(Globals::substitute_global(buffer)));
    }

    return CommandResult(true, "Run finished " + p[1], CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_ANIMATION);
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_COMMANDS

