//==============================================================================
///	
///	File: ScriptingKeyframes_cmd.cpp
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
#include "DT3Core/Types/Utility/CommandResult.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandContext.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Scripting/ScriptingKeyframes.hpp"
#include "DT3Core/Scripting/ScriptingKeyframesEvent.hpp"
#include "DT3Core/Components/ComponentBase.hpp"
#include "DT3Core/Objects/ObjectBase.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class ScriptingKeyframes_cmd: public Command {
    public:
        DEFINE_TYPE(ScriptingKeyframes_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
            CommandRegistry::register_command("SetKeyframe", &ScriptingKeyframes_cmd::do_set_keyframe);
            CommandRegistry::register_command("ClearAllKeyframes", &ScriptingKeyframes_cmd::do_clear_all_keyframes);
            CommandRegistry::register_command("GetNumKeyframes", &ScriptingKeyframes_cmd::do_get_num_keyframes);
            CommandRegistry::register_command("KeyframeIndexToID", &ScriptingKeyframes_cmd::do_keyframe_index_to_id);
            CommandRegistry::register_command("SetKeyframeTimeByID", &ScriptingKeyframes_cmd::do_set_keyframe_time_by_id);
            CommandRegistry::register_command("ClearKeyframeByID", &ScriptingKeyframes_cmd::do_clear_keyframe_by_id);
            
        }

        static CommandResult do_set_keyframe (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_clear_all_keyframes (CommandContext &ctx, const CommandParams &p);

        static CommandResult do_get_num_keyframes (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_keyframe_index_to_id (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_set_keyframe_time_by_id (CommandContext &ctx, const CommandParams &p);
        static CommandResult do_clear_keyframe_by_id (CommandContext &ctx, const CommandParams &p);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(ScriptingKeyframes_cmd)

//==============================================================================
//==============================================================================

CommandResult ScriptingKeyframes_cmd::do_set_keyframe (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: SetKeyframe param", CommandResult::UPDATE_NONE);
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

    CommandResult cr;

    PlugBase *plug = n->plug_by_name(param);
    if (plug) {
                    
        if (!plug->incoming_connection() || !plug->incoming_connection()->owner()->isa(ScriptingKeyframes::kind())) {
            std::string pt = plug->plug_type();
            
            // Make first char uppercase
            pt[0] = static_cast<DTcharacter>(::toupper(pt[0]));
            
            CommandResult result = Console::do_command(ctx, CommandParams("Add ScriptingKeyframes" + pt) );
            if (!result.result())
                return CommandResult(false, "Cannot keyframe " + p[1], CommandResult::UPDATE_NONE);
                
            // Transfer undo commands from result so the can be propagated
            cr.append_undo_command(result);
        
            // Find the node again
            std::shared_ptr<ScriptingKeyframes> key_node = checked_cast<ScriptingKeyframes>(node_or_group(ctx,result.message()));
            if (!key_node) {
                return CommandResult(false, "Cannot keyframe " + p[1], CommandResult::UPDATE_NONE);
            }
            
            plug->set_incoming_connection(key_node->plug_by_name("Out"));

            // Position the node
            std::shared_ptr<PlugNode> owner = checked_static_cast<PlugNode>(plug->owner()->shared_from_this());
            if (owner->isa(ComponentBase::kind())) {
                key_node->set_node_position( checked_static_cast<ComponentBase>(owner)->owner()->node_position() + Vector3(-200.0F,0.0F,0.0F));
            } else {
                key_node->set_node_position( checked_static_cast<WorldNode>(owner)->node_position() + Vector3(-200.0F,0.0F,0.0F));
            }
            
            /*// Connect up selected timer
            if (_timer) {
                node->Get_Plug("t")->Set_Incoming_Connection(_timer->Get_Plug("TimeOut"));
            }*/
        }
        
        if (plug->incoming_connection() && plug->incoming_connection()->owner()->isa(ScriptingKeyframes::kind())) {
            checked_static_cast<ScriptingKeyframes>(plug->incoming_connection()->owner()->shared_from_this())->set_key();
        } else {
            return CommandResult(false, "Cannot keyframe " + p[1], CommandResult::UPDATE_NONE);
        }
    }
    
    Event *event = n->event_by_name(param);
    if (event) {
    
        // Search for keyframes node        
        const std::vector<Event*>& events = event->incoming_connections();
                
        std::shared_ptr<ScriptingKeyframesEvent> key_node;

        FOR_EACH (i,events) {
            key_node = checked_cast<ScriptingKeyframesEvent>( (*i)->owner()->shared_from_this() );
            if (key_node)
                break;
        }
        
        if (!key_node) {
            CommandResult result = Console::do_command(ctx, CommandParams("Add ScriptingKeyframesEvent") );
            if (!result.result())
                return CommandResult(false, "Cannot keyframe " + p[1], CommandResult::UPDATE_NONE);
                
            // Transfer undo commands from result so the can be propagated
            cr.append_undo_command(result);

            // Find the node again
            key_node = checked_cast<ScriptingKeyframesEvent>(node_or_group(ctx,result.message()));
            if (!key_node) {
                return CommandResult(false, "Cannot keyframe " + p[1], CommandResult::UPDATE_NONE);
            }
            
            event->add_incoming_connection(key_node->event_by_name("Out"));
            
            // Position the node
            std::shared_ptr<PlugNode> o = checked_static_cast<PlugNode>(event->owner()->shared_from_this());
            if (o->isa(ComponentBase::kind())) {
                key_node->set_node_position( checked_static_cast<ComponentBase>(o)->owner()->node_position() + Vector3(-200.0F,0.0F,0.0F));
            } else {
                key_node->set_node_position( checked_static_cast<WorldNode>(o)->node_position() + Vector3(-200.0F,0.0F,0.0F));
            }

        }
        
        key_node->set_key();
    }
    

    if (!plug && !event)
        return CommandResult(false, "Cannot find plug/event for " + p[1], CommandResult::UPDATE_NONE);


    cr.set_message("Set Keyframe");
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_ANIMATION | CommandResult::UPDATE_SCRIPT | CommandResult::UPDATE_SOUND);

    return cr;
}

CommandResult ScriptingKeyframes_cmd::do_clear_all_keyframes (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: ClearAllKeyframes param", CommandResult::UPDATE_NONE);
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
 
    PlugBase *plug = n->plug_by_name(param);
    if (!plug)
        return CommandResult(false, "Cannot find plug for " + p[1], CommandResult::UPDATE_NONE);


	if (!plug->incoming_connection() || !plug->incoming_connection()->owner()->isa(ScriptingKeyframes::kind())) {
        return CommandResult(false, "Cannot find keyframes for " + p[1], CommandResult::UPDATE_NONE);
	}

    CommandResult result = Console::do_command(ctx, CommandParams("Remove \"" + plug->incoming_connection()->owner()->full_name() + "\"" ) );

    return result;
}

//==============================================================================
//==============================================================================

CommandResult ScriptingKeyframes_cmd::do_get_num_keyframes (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: GetNumKeyframes object", CommandResult::UPDATE_NONE);
    }
      
    std::shared_ptr<WorldNode> obj = ctx.world()->node_by_name(p[1]);
    if (!obj) {
        return CommandResult(false, "Unable to find object: " + p[1], CommandResult::UPDATE_NONE);
    }
    
    std::shared_ptr<ScriptingKeyframes> keyframes = checked_cast<ScriptingKeyframes>(obj);
    if (!keyframes) {
        return CommandResult(false, "Object not keyframes: " + p[1], CommandResult::UPDATE_NONE);
    }

    std::string s = MoreStrings::cast_to_string(keyframes->num_keys());

    return CommandResult(true, s, CommandResult::UPDATE_NONE);
}

CommandResult ScriptingKeyframes_cmd::do_keyframe_index_to_id (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 3) {
        return CommandResult(false, "Usage: KeyframeIndexToID object index", CommandResult::UPDATE_NONE);
    }
      
    std::shared_ptr<WorldNode> obj = ctx.world()->node_by_name(p[1]);
    if (!obj) {
        return CommandResult(false, "Unable to find object: " + p[1], CommandResult::UPDATE_NONE);
    }
    
    std::shared_ptr<ScriptingKeyframes> keyframes = checked_cast<ScriptingKeyframes>(obj);
    if (!keyframes) {
        return CommandResult(false, "Object not keyframes: " + p[1], CommandResult::UPDATE_NONE);
    }

    DTuint index = MoreStrings::cast_from_string<DTuint>(p[2]);

    std::string s = MoreStrings::cast_to_string(keyframes->key_id(index));

    return CommandResult(true, s, CommandResult::UPDATE_NONE);
}

CommandResult ScriptingKeyframes_cmd::do_set_keyframe_time_by_id (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 4) {
        return CommandResult(false, "Usage: SetKeyframeTimeByID object index time", CommandResult::UPDATE_NONE);
    }
      
    std::shared_ptr<WorldNode> obj = ctx.world()->node_by_name(p[1]);
    if (!obj) {
        return CommandResult(false, "Unable to find object: " + p[1], CommandResult::UPDATE_NONE);
    }
    
    std::shared_ptr<ScriptingKeyframes> keyframes = checked_cast<ScriptingKeyframes>(obj);
    if (!keyframes) {
        return CommandResult(false, "Object not keyframes: " + p[1], CommandResult::UPDATE_NONE);
    }

    DTuint index = MoreStrings::cast_from_string<DTuint>(p[2]);
    DTfloat time = MoreStrings::cast_from_string<DTfloat>(p[3]);

    DTint k = keyframes->key_index(index);
    keyframes->set_key_time(k, time);

    return CommandResult(true, p[3], CommandResult::UPDATE_ANIMATION);
}

CommandResult ScriptingKeyframes_cmd::do_clear_keyframe_by_id (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 3) {
        return CommandResult(false, "Usage: ClearKeyframeByID object index", CommandResult::UPDATE_NONE);
    }
      
    std::shared_ptr<WorldNode> obj = ctx.world()->node_by_name(p[1]);
    if (!obj) {
        return CommandResult(false, "Unable to find object: " + p[1], CommandResult::UPDATE_NONE);
    }
    
    std::shared_ptr<ScriptingKeyframes> keyframes = checked_cast<ScriptingKeyframes>(obj);
    if (!keyframes) {
        return CommandResult(false, "Object not keyframes: " + p[1], CommandResult::UPDATE_NONE);
    }

    DTuint index = MoreStrings::cast_from_string<DTuint>(p[2]);

    DTint k = keyframes->key_index(index);
    keyframes->clear_key(k);

    return CommandResult(true, p[3], CommandResult::UPDATE_ANIMATION);
}

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_COMMANDS

