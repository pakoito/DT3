//==============================================================================
///	
///	File: Command.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/System/Command.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/Types/Utility/CommandContext.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include "DT3Core/Types/Node/Group.hpp"
#include "DT3Core/World/World.hpp"
#include "DT3Core/World/WorldNode.hpp"
#include "DT3Core/Components/ComponentBase.hpp"
#include "DT3Core/Objects/ObjectBase.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with node factory
//==============================================================================

//IMPLEMENT_FACTORY_COMMAND(Command)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

Command::Command (void)
{  

}
		
Command::Command (const Command &rhs)
    :   BaseClass(rhs)
{   

}

Command & Command::operator = (const Command &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {
		BaseClass::operator = (rhs);
    }
    return (*this);
}
			
Command::~Command (void)
{

}

//==============================================================================
//==============================================================================

std::shared_ptr<PlugNode> Command::node_or_group (CommandContext &ctx, const std::string &node)
{
    if (ctx.world()->name() == node)
        return ctx.world();

    std::shared_ptr<PlugNode> o = ctx.world()->node_by_name(node);
    if (!o)
        o = ctx.world()->group_by_name(node);
    
    return o;
}


std::shared_ptr<ComponentBase> Command::component (CommandContext &ctx, const std::string &node, const std::string &comp)
{
    std::shared_ptr<PlugNode> o = node_or_group(ctx, node);
    if (!o) 
        return NULL;
        
    std::shared_ptr<ObjectBase> b = checked_cast<ObjectBase>(o);
    if (!b)
        return NULL;

    std::shared_ptr<ComponentBase> c = b->component_by_name(comp);
    return c;
}


PlugBase* Command::plug (CommandContext &ctx, const std::string &node, const std::string &comp, const std::string &plug)
{
    if (comp.size() > 0) {
        std::shared_ptr<ComponentBase> c = component(ctx, node, comp);
        if (!c) 
            return NULL;

        PlugBase *p = c->plug_by_name(plug);
        return p;
    } else {
        std::shared_ptr<PlugNode> o = node_or_group(ctx, node);
        if (!o) 
            return NULL;

        PlugBase *p = o->plug_by_name(plug);
        return p;
    }
}

Event* Command::event (CommandContext &ctx, const std::string &node, const std::string &comp, const std::string &event)
{
    if (comp.size() > 0) {
        std::shared_ptr<ComponentBase> c = component(ctx, node, comp);
        if (!c) 
            return NULL;

        Event *e = c->event_by_name(event);
        return e;
    } else {
        std::shared_ptr<PlugNode> o = checked_static_cast<PlugNode>(node_or_group(ctx, node));
        if (!o) 
            return NULL;

        Event *e = o->event_by_name(event);
        return e;
    }
}

//==============================================================================
//==============================================================================

DTboolean Command::node_name (const std::string &s, std::string &node)
{
    node.clear();

    std::string::size_type component_sep = s.find(":");
    std::string::size_type plug_sep = s.find_last_of(".");

    std::string::size_type sep = MoreMath::min(component_sep, plug_sep);

    node = s.substr(0,sep);

    if (node.size() > 0)    return true;
    else                    return false;
}

DTboolean Command::component_name        (const std::string &s, std::string &component)
{
    component.clear();

    std::string::size_type component_sep = s.find(":");
    std::string::size_type plug_sep = s.find_last_of(".");

    if (component_sep == std::string::npos)
    return false;

    component = s.substr(component_sep+1,plug_sep-component_sep-1);

    if (component.size() > 0)   return true;
    else                        return false;
}

DTboolean Command::param_name      (const std::string &s, std::string &param)
{
    param.clear();

    std::string::size_type param_sep = s.find_last_of(".");

    if (param_sep == std::string::npos)
        return false;

    param = s.substr(param_sep+1);

    if (param.size() > 0)   return true;
    else                    return false;
}

//==============================================================================
//==============================================================================

} // DT3

