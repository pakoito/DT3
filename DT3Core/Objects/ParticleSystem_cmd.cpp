//==============================================================================
///	
///	File: ParticleSystem_cmd.cpp
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
#include "DT3Core/System/FileManager.hpp"
#include "DT3Core/Types/Utility/CommandRegistry.hpp"
#include "DT3Core/Types/Utility/CommandResult.hpp"
#include "DT3Core/Types/Utility/CommandContext.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/FileBuffer/TextFileStream.hpp"
#include "DT3Core/Objects/ParticleSystem.hpp"
#include "DT3Core/World/World.hpp"
#include "DT3Core/Scripting/ScriptingParticleEmitterBurst.hpp"
#include "DT3Core/Scripting/ScriptingParticleEmitterPeriodic.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class ParticleSystem_cmd: public Command {
    public:
        DEFINE_TYPE(ParticleSystem_cmd,Command);
        DEFINE_CREATE
        
        void register_commands (void) {
#ifdef DT3_EDITOR
            CommandRegistry::register_command("ParticlesToCode", &ParticleSystem_cmd::do_particles_to_code);
#endif
        }

        static CommandResult do_particles_to_code (CommandContext &ctx, const CommandParams &p);
};

//==============================================================================
//==============================================================================

IMPLEMENT_FACTORY_COMMAND(ParticleSystem_cmd)

//==============================================================================
//==============================================================================

#ifdef DT3_EDITOR

CommandResult ParticleSystem_cmd::do_particles_to_code (CommandContext &ctx, const CommandParams &p)
{
    if (p.count() != 2) {
        return CommandResult(false, "Usage: ParticlesToCode particleSystemNode", CommandResult::UPDATE_NONE);
    }

    std::shared_ptr<ParticleSystem> node = checked_cast<ParticleSystem>(ctx.world()->node_by_name(p[1]));
    if (!node) {
        return CommandResult(false, "Unable to find particle system " + p[1], CommandResult::UPDATE_NONE);
    }
    

    //
    // Dump all particle system nodes
    //
    
    DTboolean           burst = false;
    DTfloat             burst_time = 0.0F;
    
    std::list<std::shared_ptr<WorldNode>>    nodes;
    nodes.push_back(node);

    // Walk up particle nodes until we get to an emitter
    std::shared_ptr<PlugNode> n = node;
    PlugBase *plug = node->plug_by_name("Particles");
    
    while (plug && n) {
        // Check if connected
        if (!plug->incoming_connection())
            break;
            
        n = checked_static_cast<PlugNode>(plug->incoming_connection()->owner()->shared_from_this());
        plug = n->plug_by_name("In");

        // Add the node
        if (n->isa(WorldNode::kind())) {
            nodes.push_back( checked_static_cast<WorldNode>(n) );
        }
        
        if (n->isa(ScriptingParticleEmitterBurst::kind())) {
            std::shared_ptr<ScriptingParticleEmitterBurst> emitter = checked_static_cast<ScriptingParticleEmitterBurst>(n);

            burst = true;
            burst_time = emitter->max_lifetime();
            
            break;
        }

        if (n->isa(ScriptingParticleEmitterPeriodic::kind())) {
            burst = true;
            break;
        }

    }
    
    // Dump all of the nodes
    TextFileStream s;
    
    // open the file
	DTerr error = FileManager::open(s, FilePath("Particles.cpp"), false);
	if (error != DT3_ERR_NONE)
        return CommandResult(false, "Unable to open file", CommandResult::UPDATE_NONE);

    // Dump Object setup
    for (auto i = nodes.begin(); i != nodes.end(); ++i) {
        std::string name = MoreStrings::camel_caps((**i).name());
    
        s << "std::shared_ptr<" << std::string((**i).class_id_child()) << "> " << name << " = " << std::string((**i).class_id_child()) << "::create();\n";
        s << "world->add_node(" << name << ");\n";

        (**i).dump_code(name, s);
            
        s << "\n";
        
        // Add a UtilExpire component
        if ( (**i).isa(ParticleSystem::kind()) && burst) {
            s <<    "std::shared_ptr<ComponentUtilExpire> expire = ComponentUtilExpire::create();\n";
            s <<    "expire->set_expire_time(" << burst_time << ");\n";
            s <<    name << "->add_component(expire);\n\n";
        }
    }
    
    // Dump Connections
    for (auto i = nodes.begin(); i != nodes.end(); ++i) {
        std::shared_ptr<PlugNode> node_in = *i;
        
        std::string plug_in_name = node_in->isa(ParticleSystem::kind()) ? "Particles" : "In";

        PlugBase *plug_in = node_in->plug_by_name(plug_in_name);
        if (!plug_in) continue;
        
        PlugBase *plug_out = plug_in->incoming_connection();
        if (!plug_in) continue;
       
        PlugNode *node_out = plug_out->owner();
        if (!node_out) continue;
        
        s   << MoreStrings::camel_caps(node_in->name()) << "->plug_by_name(\"" << plug_in->name() << "\")->set_incoming_connection("
            << MoreStrings::camel_caps(node_out->name()) << "->plug_by_name(\"" << plug_out->name() << "\"));\n";
                
    }

    s << "\n";
    
    CommandResult cr;
    cr.set_message("Dumped " + p[1]);
    cr.set_result(true);
    cr.set_update_hint(CommandResult::UPDATE_NONE);

    return cr;
}

#endif

//==============================================================================
//==============================================================================

} // DT3

#endif // DT3_COMMANDS

