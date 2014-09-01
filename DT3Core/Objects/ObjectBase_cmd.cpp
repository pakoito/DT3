//==============================================================================
///	
///	File: ObjectBase_cmd.cpp
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
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandResult.hpp"
#include "DT3Core/Types/Utility/CommandContext.hpp"
#include "DT3Core/Components/ComponentBase.hpp"
#include "DT3Core/Objects/ObjectBase.hpp"
#include "DT3Core/World/World.hpp"
#include "DT3Core/World/WorldNode.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/ArchivePropertyReaderWriter.hpp"
#include "DT3Core/Types/FileBuffer/TextBufferStream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class ObjectBase_cmd: public Command {
    public:
        DEFINE_TYPE(ObjectBase_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("AddComponent", &ObjectBase_cmd::do_add_component);
            CommandRegistry::register_command("RemoveComponent", &ObjectBase_cmd::do_remove_component);
        }

        static CommandResult do_add_component (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_remove_component (CommandContext &ctx, const CommandParams &p);

};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(ObjectBase_cmd)

//==============================================================================
//==============================================================================

CommandResult ObjectBase_cmd::do_add_component (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() < 3 || p.count() > 4) {
        return CommandResult(false, "Usage: AddComponent object kind [name]", CommandResult::UPDATE_NONE);
    }
    
    std::shared_ptr<WorldNode> obj = ctx.world()->node_by_name(p[1]);
    if (!obj) {
        return CommandResult(false, "Unable to find object: " + p[1], CommandResult::UPDATE_NONE);
    }

    std::shared_ptr<ObjectBase> obj_base = checked_cast<ObjectBase>(obj);
    if (!obj_base) {
        return CommandResult(false, "Unable to find object: " + p[1], CommandResult::UPDATE_NONE);
    }

    std::shared_ptr<BaseClass> new_component_base = Factory::create_object(p[2]);
    
    // Check that an object was created
    if (!new_component_base) {
        return CommandResult(false, "Unable to create component: " + p[2], CommandResult::UPDATE_NONE);
    }
    
    std::shared_ptr<ComponentBase> new_component = checked_cast<ComponentBase>(new_component_base);
    
    // Check that an component was created
    if (!new_component) {
        return CommandResult(false, "Unable to create component: " + p[2], CommandResult::UPDATE_NONE);
    }

    std::string default_name;

    // Set the name of the object
    if (p.count() == 4) {
        default_name = p[3];
    } else {
        default_name = new_component->class_id_child();
    }

    new_component->set_name(default_name);
    
    obj_base->add_component(new_component);
    
    CommandResult cr;

    //
    // Add Undo Commands
    //

    cr.append_undo_command(CommandParams( "RemoveComponent \"" + obj->name() + "\" \"" + new_component->name() + "\"" ));
        
    cr.set_message(new_component->name());
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_WORLD);

    return cr;
}

CommandResult ObjectBase_cmd::do_remove_component (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 3) {
        return CommandResult(false, "Usage: RemoveComponent object name", CommandResult::UPDATE_NONE);
    }
      
    std::shared_ptr<WorldNode> obj = ctx.world()->node_by_name(p[1]);
    if (!obj) {
        return CommandResult(false, "Unable to find object: " + p[1], CommandResult::UPDATE_NONE);
    }

    std::shared_ptr<ObjectBase> obj_base = checked_cast<ObjectBase>(obj);
    if (!obj_base) {
        return CommandResult(false, "Unable to find object: " + p[1], CommandResult::UPDATE_NONE);
    }
    
    std::shared_ptr<ComponentBase> component = obj_base->component_by_name(p[2]);
    if (!component) {
        return CommandResult(false, "Unable to find component: " + p[2], CommandResult::UPDATE_NONE);
    }
    
    CommandResult cr;

    //
    // Add Undo Commands
    //
    
    // Build component
    cr.append_undo_command(CommandParams( "AddComponent \"" + obj_base->full_name() + "\" \"" + component->class_id_child() + "\" \"" + component->name() + "\"" ));
    
    // Scan connections
    for (PlugIter p_iter(component.get()); p_iter; ++p_iter) {
        
        // Incoming
        if (p_iter()->has_incoming_connection()) {
            cr.append_undo_command(CommandParams("ConnectPlugs \"" + p_iter()->full_name() + "\" \"" + p_iter()->incoming_connection()->full_name() + "\"" ));
        }
        
        // Outgoing
        const std::vector<PlugBase*> &connections = p_iter()->outgoing_connections();
        for (DTuint k = 0; k < connections.size(); ++k) {
            cr.append_undo_command(CommandParams("ConnectPlugs \"" + p_iter()->full_name() + "\" \"" + connections[k]->full_name() + "\"" ));
        }

    }
    
    // Events
    for (EventIter e_iter(component.get()); e_iter; ++e_iter) {
        
        // Incoming
        const std::vector<Event*> &connections_in = e_iter()->incoming_connections();
        for (DTuint k = 0; k < connections_in.size(); ++k) {
            cr.append_undo_command(CommandParams("ConnectEvents \"" + e_iter()->full_name() + "\" \"" + connections_in[k]->full_name() + "\"" ));
        }
        
        // Outgoing
        const std::vector<Event*> &connections_out = e_iter()->outgoing_connections();
        for (DTuint k = 0; k < connections_out.size(); ++k) {
            cr.append_undo_command(CommandParams("ConnectEvents \"" + e_iter()->full_name() + "\" \"" + connections_out[k]->full_name() + "\"" ));
        }

    }
    
    // Parameters
    std::shared_ptr<ArchivePropertyReaderWriter> properties = ArchivePropertyReaderWriter::create();
    component->archive(properties);
    
    // Get the properties
    std::vector<std::shared_ptr<ArchiveData>> properties_list = properties->properties();
    
    std::string result;
    for (DTuint i = 0; i < properties_list.size(); ++i) {
        TextBufferStream s;
        properties_list[i]->value(s);
    
        cr.append_undo_command(CommandParams("SetProp \"" + component->full_name() + "." + properties_list[i]->title() + "\" \"" + s.buffer() + "\"" ));
    }
        
    obj_base->remove_component(component);      

    cr.set_message(p[1]);
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND | CommandResult::UPDATE_PROPERTIES | CommandResult::UPDATE_WORLD);

    return cr;
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_COMMANDS

