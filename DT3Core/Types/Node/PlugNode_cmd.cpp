 //==============================================================================
///	
///	File: PlugNode_cmd.cpp
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
#include "DT3Core/System/Console.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandResult.hpp"
#include "DT3Core/Types/Node/Plug.hpp"
#include "DT3Core/Types/Node/Event.hpp"
#include "DT3Core/Types/Node/PlugNode.hpp"
#include "DT3Core/Types/FileBuffer/ArchivePropertyReaderWriter.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/TextBufferStream.hpp"
#include "DT3Core/Components/ComponentBase.hpp"
#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/World/WorldNode.hpp"
#include "DT3Core/Objects/ObjectBase.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class PlugNode_cmd: public Command {
    public:
        DEFINE_TYPE(PlugNode_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("ListPlugs", &PlugNode_cmd::do_list_plugs);
            CommandRegistry::register_command("ListEvents", &PlugNode_cmd::do_list_events);
            CommandRegistry::register_command("ConnectPlugs", &PlugNode_cmd::do_connect_plugs);
            CommandRegistry::register_command("ConnectEvents", &PlugNode_cmd::do_connect_events);
            CommandRegistry::register_command("DisconnectPlugs", &PlugNode_cmd::do_disconnect_plugs);
            CommandRegistry::register_command("DisconnectEvents", &PlugNode_cmd::do_disconnect_events);
            CommandRegistry::register_command("ListProp", &PlugNode_cmd::do_list_prop);
            CommandRegistry::register_command("GetProp", &PlugNode_cmd::do_get_prop);
            CommandRegistry::register_command("SetProp", &PlugNode_cmd::do_set_prop);
            CommandRegistry::register_command("TriggerEvent", &PlugNode_cmd::do_trigger_event);
        }

        static CommandResult do_list_plugs (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_list_events (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_list_attributes (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_connect_plugs (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_connect_events (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_disconnect_plugs (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_disconnect_events (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_list_prop (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_get_prop (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_set_prop (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_trigger_event (CommandContext &ctx, const CommandParams &p);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(PlugNode_cmd)

//==============================================================================
//==============================================================================

CommandResult PlugNode_cmd::do_list_plugs (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: ListPlugs nodename", CommandResult::UPDATE_NONE);
    }

    // Get node
    std::string node1, component1;

    node_name (p[1], node1);
    component_name (p[1], component1);
    
    std::shared_ptr<PlugNode> node;
    
    if (component1.size() > 0) {
        node = component (ctx, node1, component1);
    } else {
        node = node_or_group (ctx, node1);
    }
    
    std::string result;
    
    for (PlugIter iter(node.get()); iter; ++iter) {
        result = iter->name() + " " + result;
    }
    
    return CommandResult(true, result, CommandResult::UPDATE_NONE);
}

CommandResult PlugNode_cmd::do_list_events (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: ListPlugs nodename", CommandResult::UPDATE_NONE);
    }

    // Get node
    std::string node1, component1;

    node_name (p[1], node1);
    component_name (p[1], component1);
    
    std::shared_ptr<PlugNode> node;
    
    if (component1.size() > 0) {
        node = component (ctx, node1, component1);
    } else {
        node = node_or_group (ctx, node1);
    }

    std::string result;
    
    for (EventIter iter(node.get()); iter; ++iter) {
        result = iter->name() + " " + result;
    }
    
    return CommandResult(true, result, CommandResult::UPDATE_NONE);
}

CommandResult PlugNode_cmd::do_connect_plugs (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 3) {
        return CommandResult(false, "Usage: ConnectPlugs plug1 plug2", CommandResult::UPDATE_NONE);
    }
    
    // Get plug 1
    std::string node1, component1, param1;

    node_name (p[1], node1);
    component_name (p[1], component1);
    param_name (p[1], param1);

    PlugBase* plug1 = plug (ctx, node1, component1, param1);
    if (!plug1) {
        return CommandResult(false, "Unable to find " + p[1], CommandResult::UPDATE_NONE);
    }
        
    // Get plug 2
    std::string node2, component2, param2;

    node_name (p[2], node2);
    component_name (p[2], component2);
    param_name (p[2], param2);

    PlugBase* plug2 = plug (ctx, node2, component2, param2);
    if (!plug2) {
        return CommandResult(false, "Unable to find " + p[2], CommandResult::UPDATE_NONE);
    }
    
    // Make sure plugs have input and output flags set correctly
    if (!plug1->is_output() || !plug2->is_input()) {
        return CommandResult(false, "Unable to connect " + p[1] + " and " + p[2], CommandResult::UPDATE_NONE);
    }
        
    
    
    CommandResult cr;

    //
    // Do Command
    //
        
    DTboolean r = plug2->set_incoming_connection(plug1);
    if (!r) {
        
        // Try to build conversion plug
        std::string conversion = std::string("Scripting") + plug1->plug_type() + "To" + plug2->plug_type();
        
        CommandResult result = Console::do_command(ctx, CommandParams("Add " + conversion));
        if (!result.result())
            return CommandResult(false, "Cannot connect " + p[1] + " to " + p[2], CommandResult::UPDATE_NONE);

        // Transfer undo commands from result so the can be propagated
        cr.append_undo_command(result);

        // Find the node again
        std::shared_ptr<ScriptingBase> conversion_node = checked_cast<ScriptingBase>(node_or_group(ctx,result.message()));
        if (!conversion_node) {
            return CommandResult(false, "Cannot connect " + p[1] + " to " + p[2], CommandResult::UPDATE_NONE);
        }
        
        std::shared_ptr<PlugNode> plug_owner1 = checked_static_cast<PlugNode>(plug1->owner()->shared_from_this());
        std::shared_ptr<PlugNode> plug_owner2 = checked_static_cast<PlugNode>(plug2->owner()->shared_from_this());
        
        Vector3 pos1(0.0F,0.0F,0.0F);
        Vector3 pos2(0.0F,0.0F,0.0F);
        
        if (plug_owner1->isa(WorldNode::kind()))
            pos1 += checked_static_cast<WorldNode>(plug_owner1)->node_position();
        
        if (plug_owner1->isa(ComponentBase::kind()))
            pos1 += checked_static_cast<ComponentBase>(plug_owner1)->owner()->node_position();

        if (plug_owner2->isa(WorldNode::kind()))
            pos2 += checked_static_cast<WorldNode>(plug_owner2)->node_position();
        
        if (plug_owner2->isa(ComponentBase::kind()))
            pos2 += checked_static_cast<ComponentBase>(plug_owner2)->owner()->node_position();
        

        // Set position halfway
        Vector3 pos = 0.5F * ( pos1 + pos2 );
        conversion_node->set_node_position(pos);
        
        // Connect it up
        plug1->add_outgoing_connection(conversion_node->plug_by_name("In"));
        plug2->set_incoming_connection(conversion_node->plug_by_name("Out"));
    }
    
    //
    // Add Undo Command
    //

    cr.append_undo_command(CommandParams("DisconnectPlugs \"" + p[1] + "\" \"" + p[2] + "\""));

    cr.set_message("Connected " + p[1] + " to " + p[2]);
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_PROPERTIES);

    return cr;
}

CommandResult PlugNode_cmd::do_connect_events (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 3) {
        return CommandResult(false, "Usage: ConnectEvents event1 event2", CommandResult::UPDATE_NONE);
    }

    // Get plug 1
    std::string node1, component1, param1;

    node_name (p[1], node1);
    component_name (p[1], component1);
    param_name (p[1], param1);

    Event* event1 = event (ctx, node1, component1, param1);
    if (!event1) {
        return CommandResult(false, "Unable to find " + p[1], CommandResult::UPDATE_NONE);
    }
        
    // Get plug 2
    std::string node2, component2, param2;

    node_name (p[2], node2);
    component_name (p[2], component2);
    param_name (p[2], param2);

    Event* event2 = event (ctx, node2, component2, param2);
    if (!event2) {
        return CommandResult(false, "Unable to find " + p[2], CommandResult::UPDATE_NONE);
    }
    
    //
    // Do Command
    //

    event2->add_incoming_connection(event1);
    
    //
    // Add Undo Command
    //

    CommandResult cr;
    cr.append_undo_command(CommandParams("DisconnectEvents \"" + p[1] + "\" \"" + p[2] + "\""));

    cr.set_message("Connected " + p[1] + " to " + p[2]);
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_PROPERTIES);

    return cr;
}

//==============================================================================
//==============================================================================

CommandResult PlugNode_cmd::do_disconnect_plugs (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 3) {
        return CommandResult(false, "Usage: DisconnectPlugs plug1 plug2", CommandResult::UPDATE_NONE);
    }

    // Get plug 1
    std::string node1, component1, param1;

    node_name (p[1], node1);
    component_name (p[1], component1);
    param_name (p[1], param1);

    PlugBase* plug1 = plug (ctx, node1, component1, param1);
    if (!plug1) {
        return CommandResult(false, "Unable to find " + p[1], CommandResult::UPDATE_NONE);
    }
        
    // Get plug 2
    std::string node2, component2, param2;

    node_name (p[2], node2);
    component_name (p[2], component2);
    param_name (p[2], param2);

    PlugBase* plug2 = plug (ctx, node2, component2, param2);
    if (!plug2) {
        return CommandResult(false, "Unable to find " + p[2], CommandResult::UPDATE_NONE);
    }
    
    //
    // Do Command
    //

    plug1->remove_outgoing_connection(plug2);
    
    //
    // Add Undo Command
    //

    CommandResult cr;
    cr.append_undo_command(CommandParams("ConnectPlugs \"" + p[1] + "\" \"" + p[2] + "\""));

    cr.set_message("Disconnected " + p[2] + " from " + p[1]);
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_PROPERTIES);

    return cr;
}

CommandResult PlugNode_cmd::do_disconnect_events (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 3) {
        return CommandResult(false, "Usage: DisconnectEvents nodename1 eventname1 nodename2 eventname2", CommandResult::UPDATE_NONE);
    }

    // Get plug 1
    std::string node1, component1, param1;

    node_name (p[1], node1);
    component_name (p[1], component1);
    param_name (p[1], param1);

    Event* event1 = event (ctx, node1, component1, param1);
    if (!event1) {
        return CommandResult(false, "Unable to find " + p[1], CommandResult::UPDATE_NONE);
    }
        
    // Get plug 2
    std::string node2, component2, param2;

    node_name (p[2], node2);
    component_name (p[2], component2);
    param_name (p[2], param2);

    Event* event2 = event (ctx, node2, component2, param2);
    if (!event2) {
        return CommandResult(false, "Unable to find " + p[2], CommandResult::UPDATE_NONE);
    }
    
    //
    // Do Command
    //

    event1->remove_outgoing_connection(event2);
    
    //
    // Add Undo Command
    //

    CommandResult cr;
    cr.append_undo_command(CommandParams("ConnectEvents \"" + p[1] + "\" \"" + p[2] + "\""));

    cr.set_message("Disconnected " + p[2] + " from " + p[1]);
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_PROPERTIES);

    return cr;
}

//==============================================================================
//==============================================================================

CommandResult PlugNode_cmd::do_list_prop (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: ListProp param", CommandResult::UPDATE_NONE);
    }

    // Get param
    std::string node, comp;

    node_name (p[1], node);
    component_name (p[1], comp);
    
    std::shared_ptr<PlugNode> n;
    
    if (comp.size() > 0) {
        n = component(ctx, node, comp);
    } else {
        n = node_or_group(ctx, node);
    }

    if (!n)
        return CommandResult(false, "Cannot find object or component for " + p[1], CommandResult::UPDATE_NONE);

    std::shared_ptr<ArchivePropertyReaderWriter> props = ArchivePropertyReaderWriter::create();
    n->archive(props);
    
    // Get the properties
    std::vector<std::shared_ptr<ArchiveData>>& properties_list = props->properties();
    
    std::string result;
    for (DTuint i = 0; i < properties_list.size(); ++i) {
        result = properties_list[i]->title() + " " + result;
    }
    
    return CommandResult(true, result, CommandResult::UPDATE_NONE);
}

CommandResult PlugNode_cmd::do_get_prop (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: GetProp param", CommandResult::UPDATE_NONE);
    }

    // Get param
    std::string node, comp, param;

    node_name (p[1], node);
    component_name (p[1], comp);
    param_name (p[1], param);
    
    std::shared_ptr<PlugNode> n;
    
    if (comp.size() > 0) {
        n = component(ctx, node, comp);
    } else {
        n = node_or_group(ctx, node);
    }

    if (!n)
        return CommandResult(false, "Cannot find object or component for " + p[1], CommandResult::UPDATE_NONE);

    std::shared_ptr<ArchivePropertyReaderWriter> props = ArchivePropertyReaderWriter::create();
    n->archive(props);
    
    std::shared_ptr<ArchiveData> property = props->property(param);
    if (!property)
        return CommandResult(false, "Cannot find property for " + p[1], CommandResult::UPDATE_NONE);
        
    TextBufferStream stream;
	property->value(stream);

    return CommandResult(true, stream.buffer(), CommandResult::UPDATE_NONE);
}

CommandResult PlugNode_cmd::do_set_prop (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 3) {
        return CommandResult(false, "Usage: SetProp param value", CommandResult::UPDATE_NONE);
    }

    // Get param
    std::string node, comp, param;

    node_name (p[1], node);
    component_name (p[1], comp);
    param_name (p[1], param);
    
    std::shared_ptr<PlugNode> n;
    
    if (comp.size() > 0) {
        n = component(ctx, node, comp);
    } else {
        n = node_or_group(ctx, node);
    }

    if (!n)
        return CommandResult(false, "Cannot find object or component for " + p[1], CommandResult::UPDATE_NONE);

    std::shared_ptr<ArchivePropertyReaderWriter> props = ArchivePropertyReaderWriter::create();
    n->archive(props);
    
    std::shared_ptr<ArchiveData> property = props->property(param);
    if (!property)
        return CommandResult(false, "Cannot find property for " + p[1], CommandResult::UPDATE_NONE);
        
    CommandResult cr;
        
    //
    // Add Undo Command
    //
    
    TextBufferStream stream3;
	property->value(stream3);
    cr.append_undo_command(CommandParams("SetProp \"" + p[1] + "\" \"" + stream3.buffer() + "\"" ));

    //
    // Do Command
    //

        
    TextBufferStream stream1;
    stream1.set_buffer(p[2]);
	property->set_value(stream1);

    TextBufferStream stream2;
	property->value(stream2);

    cr.set_message("Set " + stream2.buffer());
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_ANIMATION);

    return cr;
}

//==============================================================================
//==============================================================================

CommandResult PlugNode_cmd::do_trigger_event (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: TriggerEvent param", CommandResult::UPDATE_NONE);
    }

    // Get param
    std::string node, comp, param;

    node_name (p[1], node);
    component_name (p[1], comp);
    param_name (p[1], param);
    
    std::shared_ptr<PlugNode> n;
    
    if (comp.size() > 0) {
        n = component(ctx, node, comp);
    } else {
        n = node_or_group(ctx, node);
    }

    if (!n)
        return CommandResult(false, "Cannot find object or component for " + p[1], CommandResult::UPDATE_NONE);

    std::shared_ptr<ArchivePropertyReaderWriter> props = ArchivePropertyReaderWriter::create();
    n->archive(props);
    
    std::shared_ptr<ArchiveData> property = props->property(param);
    if (!property)
        return CommandResult(false, "Cannot event " + p[1], CommandResult::UPDATE_NONE);
        
    Event *event = property->event();
    if (!event) {
        return CommandResult(false, "Cannot event " + p[1], CommandResult::UPDATE_NONE);
    }
    
    //
    // Do Event
    //

    if (event->is_output())
        event->send(NULL);

    CommandResult cr;
    cr.set_message("Triggered " + p[1]);
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_ANIMATION);

    return cr;
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_COMMANDS

