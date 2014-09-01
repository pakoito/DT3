//==============================================================================
///	
///	File: PlaceableObject_cmd.cpp
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
#include "DT3Core/Objects/PlaceableObject.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandResult.hpp"
#include "DT3Core/Types/Utility/CommandContext.hpp"
#include "DT3Core/Types/FileBuffer/TextBufferStream.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class PlaceableObject_cmd: public Command {
    public:
        DEFINE_TYPE(PlaceableObject_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("Parent", &PlaceableObject_cmd::do_parent);
            CommandRegistry::register_command("Unparent", &PlaceableObject_cmd::do_unparent);
            CommandRegistry::register_command("setTransform", &PlaceableObject_cmd::do_set_transform);
            CommandRegistry::register_command("setScale", &PlaceableObject_cmd::do_set_scale);
        }

        static CommandResult do_parent (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_unparent (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_set_transform (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_set_scale (CommandContext &ctx, const CommandParams &p);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(PlaceableObject_cmd)

//==============================================================================
//==============================================================================

CommandResult PlaceableObject_cmd::do_parent (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 3) {
        return CommandResult(false, "Usage: Parent object newparent", CommandResult::UPDATE_NONE);
    }
      
    std::shared_ptr<PlaceableObject> obj = checked_cast<PlaceableObject>(ctx.world()->node_by_name(p[1]));
    if (!obj) {
        return CommandResult(false, "Unable to find placeable object: " + p[1], CommandResult::UPDATE_NONE);
    }

    std::shared_ptr<PlaceableObject> parent = checked_cast<PlaceableObject>(ctx.world()->node_by_name(p[2]));
    if (!parent) {
        return CommandResult(false, "Unable to find parent placeable object: " + p[2], CommandResult::UPDATE_NONE);
    }

    if (obj == parent) {
        return CommandResult(false, "Cannot parent object to itself", CommandResult::UPDATE_NONE);
    }
    
    PlaceableObject *test = parent.get();
    while (test) {
        
        if (test == obj.get())
            return CommandResult(false, "Object: " + p[2] + " is already a parent of " + p[1], CommandResult::UPDATE_NONE);
            
        test = test->parent();
    }
    
    CommandResult cr;

    //
    // Add Undo Command
    //
    
    // Get old parent
    PlaceableObject *old_parent = obj->parent();
    if (old_parent)
        cr.append_undo_command(CommandParams("Parent \"" + obj->name() + "\" \"" + old_parent->name() + "\"" ));
    else
        cr.append_undo_command(CommandParams("Unparent \"" + obj->name() + "\"" ));

    Matrix4 old_transform = obj->transform();
    
    // Add the child
    parent->add_child(obj);
    
    obj->set_transform(old_transform);
    
    cr.set_message(p[1]);
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY);
    
    return cr;
}

CommandResult PlaceableObject_cmd::do_unparent (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: Unparent object", CommandResult::UPDATE_NONE);
    }
      
    std::shared_ptr<PlaceableObject> obj = checked_cast<PlaceableObject>(ctx.world()->node_by_name(p[1]));
    if (!obj) {
        return CommandResult(false, "Unable to find placeable object: " + p[1], CommandResult::UPDATE_NONE);
    }
    
    CommandResult cr;

    //
    // Add Undo Command
    //
    
    // Get old parent
    PlaceableObject *old_parent = obj->parent();
    if (old_parent)
        cr.append_undo_command(CommandParams("Parent \"" + obj->name() + "\" \"" + old_parent->name() + "\"" ));
    
    // Add the child
    if (obj->parent()) {
        Matrix4 old_transform = obj->transform();
        obj->parent()->unparent_child(obj);
        obj->set_transform(old_transform);
    }
    
    cr.set_message(p[1]);
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_WORLD | CommandResult::UPDATE_HIERARCHY);
    
    return cr;
}

//==============================================================================
//==============================================================================

CommandResult PlaceableObject_cmd::do_set_transform (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 3) {
        return CommandResult(false, "Usage: SetTransform nodename transform", CommandResult::UPDATE_NONE);
    }

    std::shared_ptr<PlaceableObject> node = checked_cast<PlaceableObject>(ctx.world()->node_by_name(p[1]));
    if (!node) {
        return CommandResult(false, "Unable to find placeable object " + p[1], CommandResult::UPDATE_NONE);
    }
    
    CommandResult cr;
    
    //
    // Add Undo Command
    //
    
    TextBufferStream stream1;
    stream1 << node->transform();
    cr.append_undo_command(CommandParams("SetTransform \"" + p[1] + "\" (" + stream1.buffer() + ")" ));

    //
    // Do Command
    //

    TextBufferStream stream2(p[2]);
    Matrix4 pos;
    stream2 >> pos;
    
    node->set_transform(pos);
    
    cr.set_message("Transformed " + p[1]);
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_WORLD | CommandResult::UPDATE_PROPERTIES);
    return cr;
}

//==============================================================================
//==============================================================================

CommandResult PlaceableObject_cmd::do_set_scale (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 3) {
        return CommandResult(false, "Usage: SetScale nodename scale", CommandResult::UPDATE_NONE);
    }

    std::shared_ptr<PlaceableObject> node = checked_cast<PlaceableObject>(ctx.world()->node_by_name(p[1]));
    if (!node) {
        return CommandResult(false, "Unable to find placeable object " + p[1], CommandResult::UPDATE_NONE);
    }
    
    CommandResult cr;
    
    //
    // Add Undo Command
    //
    
    TextBufferStream stream1;
    stream1 << node->scale();
    cr.append_undo_command(CommandParams("SetScale \"" + p[1] + "\" (" + stream1.buffer() + ")" ));

    //
    // Do Command
    //

    TextBufferStream stream2(p[2]);
    Vector3 scale;
    stream2 >> scale;
    
    node->set_scale(scale);
    
    cr.set_message("Scaled " + p[1]);
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_WORLD | CommandResult::UPDATE_PROPERTIES);

    return cr;
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_COMMANDS

