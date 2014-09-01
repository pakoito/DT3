//==============================================================================
///	
///	File: WorldNode_cmd.cpp
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
#include "DT3Core/Types/Utility/CommandResult.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandContext.hpp"
#include "DT3Core/Types/Utility/CheckedCast.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/FileBuffer/TextBufferStream.hpp"
#include "DT3Core/Types/Node/PlugNode.hpp"
#include "DT3Core/Types/Node/Group.hpp"
#include "DT3Core/World/World.hpp"
#include "DT3Core/World/WorldNode.hpp"
#include "DT3Core/Components/ComponentBase.hpp"
#include <algorithm>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class WorldNode_cmd: public Command {
    public:
        DEFINE_TYPE(WorldNode_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("MoveNode", &WorldNode_cmd::do_move_node);
            CommandRegistry::register_command("AlignNodes", &WorldNode_cmd::do_align_nodes);
            CommandRegistry::register_command("ColorNodes", &WorldNode_cmd::do_color_nodes);
            CommandRegistry::register_command("Group", &WorldNode_cmd::do_group);
            CommandRegistry::register_command("Ungroup", &WorldNode_cmd::do_ungroup);
            CommandRegistry::register_command("SetName", &WorldNode_cmd::do_set_name);
        }

        static CommandResult do_move_node (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_align_nodes (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_color_nodes (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_group (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_ungroup (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_set_name (CommandContext &ctx, const CommandParams &p);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(WorldNode_cmd)

//==============================================================================
//==============================================================================

CommandResult WorldNode_cmd::do_move_node (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 3) {
        return CommandResult(false, "Usage: MoveNode nodename position", CommandResult::UPDATE_NONE);
    }

    std::shared_ptr<WorldNode> node = ctx.world()->node_by_name(p[1]);
    if (!node) {
        return CommandResult(false, "Unable to find " + p[1], CommandResult::UPDATE_NONE);
    }
    
    CommandResult cr;
    
    //
    // Add Undo Command
    //
    
    TextBufferStream stream1;
    stream1 << node->node_position();
    cr.append_undo_command(CommandParams("_move_node \"" + p[1] + "\" (" + stream1.buffer() + ")" ));

    //
    // Do Command
    //

    TextBufferStream stream2(p[2]);
    Vector3 pos;
    stream2 >> pos;
    
    node->set_node_position(pos);
    
    cr.set_message("Moved " + p[1] + " to " + p[2]);
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT);

    return cr;
}

//==============================================================================
//==============================================================================

namespace {
    
    struct AlignComparison {
        AlignComparison (DTboolean sort_horz)
            :   _sort_horz(sort_horz)
        {}
    
        DTint operator() (const std::shared_ptr<WorldNode> &a, const std::shared_ptr<WorldNode> &b) const {
            if (_sort_horz) {
                
                if (a->node_position().x > b->node_position().x) return 1;
                if (a->node_position().x < b->node_position().x) return -1;
                return 0;
            
            } else {
            
                if (a->node_position().y > b->node_position().y) return 1;
                if (a->node_position().y < b->node_position().y) return -1;
                return 0;
                
            }
        }
        
        DTboolean _sort_horz;
    };

};

CommandResult WorldNode_cmd::do_align_nodes (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 1) {
        return CommandResult(false, "Usage: AlignNodes", CommandResult::UPDATE_NONE);
    }
    
    // Change the selection
    const std::list<std::shared_ptr<PlugNode>> &selection = ctx.selection();
    
    if (selection.size() == 0) {
        // Set nodes
        CommandResult cr;
        cr.set_message("No nodes selected");
        cr.set_result(true);
        cr.set_update_hint(CommandResult::UPDATE_NONE);
    }
    
    // Set nodes
    CommandResult cr;

    // Build a list of nodes
    std::vector<std::shared_ptr<WorldNode>> nodes;
    
    FOR_EACH (i,selection) {
        std::shared_ptr<WorldNode> node = checked_cast<WorldNode>(*i);
        if (!node)
            continue;

        nodes.push_back(node);
        
        // Add undo
        TextBufferStream stream1;
        stream1 << node->node_position();
        cr.append_undo_command(CommandParams("MoveNode \"" + node->name() + "\" (" + stream1.buffer() + ")" ));
    }

    
    // Check the orientation of the existing nodes
    DTuint horz_count = 0;
    DTuint vert_count = 0;
    
    FOR_EACH (i,nodes) {
        FOR_EACH (j,nodes) {            
            Vector3 diff = (**i).node_position() - (**j).node_position();
            if ( std::abs(diff.x) > std::abs(diff.y) )      ++horz_count;
            else if ( std::abs(diff.x) < std::abs(diff.y) ) ++vert_count;
        }
    }
    
    
    DTboolean sort_horz = horz_count > vert_count;
    AlignComparison comp(sort_horz);
    
    std::sort(nodes.begin(), nodes.end(), comp);
    
    // Layout nodes
    const DTfloat HORZ_SPACING = 300.0F;
    const DTfloat VERT_SPACING = 400.0F;
    
    Vector3 origin = nodes[0]->node_position();
    origin.x = std::floor((origin.x / HORZ_SPACING) + 0.5F) * HORZ_SPACING;
    origin.y = std::floor((origin.y / VERT_SPACING) + 0.5F) * VERT_SPACING;
    
    if (sort_horz) {
        
        for (DTuint i = 0; i < nodes.size(); ++i) {
            nodes[i]->set_node_position(origin);
            origin.x += HORZ_SPACING;
        }
    
    } else {
    
        for (DTuint i = 0; i < nodes.size(); ++i) {
            nodes[i]->set_node_position(origin);
            origin.y += VERT_SPACING;
        }
        
    }
    
    // Result
    cr.set_message("Aligned");
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT);

    return cr;
}

//==============================================================================
//==============================================================================

CommandResult WorldNode_cmd::do_color_nodes (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 4) {
        return CommandResult(false, "Usage: ColorNodes r g b", CommandResult::UPDATE_NONE);
    }    
    
    Color4f c(  MoreStrings::cast_from_string<DTfloat>(p[1]),
                MoreStrings::cast_from_string<DTfloat>(p[2]),
                MoreStrings::cast_from_string<DTfloat>(p[3]),
                1.0F);
    
    // Change the selection
    const std::list<std::shared_ptr<PlugNode>> &selection = ctx.selection();
              
    FOR_EACH (i,selection) {
        std::shared_ptr<WorldNode> node = checked_cast<WorldNode>(*i);
        if (node)      
            node->set_node_color(c);

        std::shared_ptr<Group> group = checked_cast<Group>(*i);
        if (group)      
            group->set_group_color(c);
    }

    CommandResult cr;
    cr.set_message("Done!");
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT);

    return cr;
}

//==============================================================================
//==============================================================================

CommandResult WorldNode_cmd::do_group (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: Group name", CommandResult::UPDATE_NONE);
    }    
    
    std::shared_ptr<Group> group = Group::create();
    group->set_name(p[1]);

    ctx.world()->add_group_unique_name(group);
        
    // Change the selection
    const std::list<std::shared_ptr<PlugNode>> &selection = ctx.selection();
              
    FOR_EACH (i,selection) {
        std::shared_ptr<WorldNode> node = checked_cast<WorldNode>(*i);
        if (!node)  
            continue;
        
        group->add_node(node);
    }
    
    ctx.world()->clean();

    CommandResult cr;
    cr.set_message("Done!");
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT);

    return cr;
}

//==============================================================================
//==============================================================================

CommandResult WorldNode_cmd::do_ungroup (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() > 2) {
        return CommandResult(false, "Usage: Ungroup [group or node]", CommandResult::UPDATE_NONE);
    }    
    
    // Operate on selection
    if (p.count() == 1) {
        // Change the selection
        const std::list<std::shared_ptr<PlugNode>> &selection = ctx.selection();
                  
        FOR_EACH (i,selection) {
            std::shared_ptr<WorldNode> node = checked_cast<WorldNode>(*i);
            if (!node)  
                continue;
                    
            node->add_to_group(NULL);
            
            ctx.world()->clean();
        }
        
    } else {
        
        // If node, remove from group
        std::shared_ptr<WorldNode> node = ctx.world()->node_by_name(p[1]);
        if (node) {
            node->add_to_group(NULL);
        }

        // If group, remove all nodes
        std::shared_ptr<Group> group = ctx.world()->group_by_name(p[1]);
        if (group) {
            group->remove_all_nodes();
        }
        
        ctx.world()->clean();
    
    }
    
    
    
    CommandResult cr;
    cr.set_message("Done!");
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT);

    return cr;
}

//==============================================================================
//==============================================================================

CommandResult WorldNode_cmd::do_set_name (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 3) {
        return CommandResult(false, "Usage: SetName NodeOrComponent name", CommandResult::UPDATE_NONE);
    }

    // Get param 
    std::string node, comp, param;

    node_name (p[1], node);
    component_name (p[1], comp);
    
    std::shared_ptr<PlugNode> n;
    CommandResult cr;

    if (comp.size() > 0) {
        n = checked_cast<PlugNode>(component(ctx, node, comp));
        if (!n)
            return CommandResult(false, "Cannot find object or component for " + p[1], CommandResult::UPDATE_NONE);

        cr.append_undo_command(CommandParams("SetName \"" + node + ":" + p[2] + "\" \"" + n->name() + "\"" ));
        
        n->set_name(p[2]);
        cr.set_message("Set " + p[2]);

    } else {
        n = node_or_group(ctx, node);
        if (!n)
            return CommandResult(false, "Cannot find object or component for " + p[1], CommandResult::UPDATE_NONE);
            
        if (p[2] == n->name())
            return CommandResult(false, "Same name, not changed " + p[1], CommandResult::UPDATE_NONE);

        std::shared_ptr<WorldNode> check_node = ctx.world()->node_by_name(p[2]);
        if (check_node)
            return CommandResult(false, "Object with that name already exists " + p[2], CommandResult::UPDATE_NONE);

        n->set_name(p[2]);
        cr.set_message("Set " + p[2]);

    }

    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_ANIMATION);

    return cr;
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_COMMANDS

