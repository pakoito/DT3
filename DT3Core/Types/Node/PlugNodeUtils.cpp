//==============================================================================
///	
///	File: PlugNodeUtils.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Node/PlugNodeUtils.hpp"
#include "DT3Core/Types/Node/Group.hpp"
#include "DT3Core/Objects/ObjectBase.hpp"
#include <algorithm>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

void PlugNodeUtils::copy_nodes (const std::list<std::shared_ptr<PlugNode>> &from, std::list<std::shared_ptr<PlugNode>> &to)
{
    std::map<std::shared_ptr<PlugNode>, std::shared_ptr<PlugNode>>  orig_to_new_node;
    
    copy_nodes (from, to, orig_to_new_node);
}

void PlugNodeUtils::copy_nodes (    const std::list<std::shared_ptr<PlugNode>> &from,
                                    std::list<std::shared_ptr<PlugNode>> &to,
                                    std::map<std::shared_ptr<PlugNode>, std::shared_ptr<PlugNode>> &orig_to_new_node)
{    
    FOR_EACH (si,from) {
        if ( (*si)->isa(Group::kind()))
            continue;
    
        std::shared_ptr<PlugNode> new_node = checked_static_cast<PlugNode>((*si)->clone());
        
        // Special case components too
        if (new_node->isa(ObjectBase::kind())) {
            std::shared_ptr<ObjectBase> orig_node_base = checked_static_cast<ObjectBase>(*si);
            std::shared_ptr<ObjectBase> new_node_base = checked_static_cast<ObjectBase>(new_node);
            
            for (DTuint i = 0; i < ComponentBase::NUM_COMPONENT_TYPES; ++i) {
                std::shared_ptr<ComponentBase> orig_component_base = orig_node_base->component_by_type	( (ComponentBase::ComponentType) i);
                std::shared_ptr<ComponentBase> new_component_base = new_node_base->component_by_type ( (ComponentBase::ComponentType) i);
                
                if (orig_component_base && new_component_base)
                    orig_to_new_node[orig_component_base] = new_component_base;
            }
        
        } 
        
        orig_to_new_node[*si] = new_node;
        
        
        // Update nodes
        to.push_back(new_node);
    }
    
    FOR_EACH (mi,orig_to_new_node) {
        std::shared_ptr<PlugNode> orig_node_src = mi->first;
        std::shared_ptr<PlugNode> new_node_src = mi->second;

        // Plugs
        for (PlugIter p_iter(orig_node_src.get()); p_iter; ++p_iter) {
            
            // Incoming
            if (p_iter()->has_incoming_connection()) {
                std::shared_ptr<PlugNode> orig_node_dest = checked_static_cast<PlugNode>(p_iter()->incoming_connection()->owner()->shared_from_this());

                auto ni = orig_to_new_node.find(orig_node_dest);
                if (ni != orig_to_new_node.end()) {
                    std::shared_ptr<PlugNode> new_node_dest = ni->second;
                    if (!new_node_dest)    continue;

                    // Connect up the nodes
                    new_node_src->plug_by_name(p_iter()->name())->set_incoming_connection(
                        new_node_dest->plug_by_name(p_iter()->incoming_connection()->name())
                    );
                    
                }
            
            }
            
            // Outgoing
            const std::vector<PlugBase*> &connections = p_iter()->outgoing_connections();
            for (DTuint k = 0; k < connections.size(); ++k) {
                std::shared_ptr<PlugNode> orig_node_dest = checked_static_cast<PlugNode>(connections[k]->owner()->shared_from_this());

                auto ni = orig_to_new_node.find(orig_node_dest);
                if (ni != orig_to_new_node.end()) {
                    std::shared_ptr<PlugNode> new_node_dest = ni->second;
                    if (!new_node_dest)    continue;
 
                    // Connect up the nodes
                    new_node_src->plug_by_name(p_iter()->name())->add_outgoing_connection(
                        new_node_dest->plug_by_name(connections[k]->name())
                    );
                    
                }

            }

        }
        
        
        // Events
        for (EventIter e_iter(orig_node_src.get()); e_iter; ++e_iter) {
            
            // Incoming
            const std::vector<Event*> &connections_in = e_iter()->incoming_connections();
            for (DTuint k = 0; k < connections_in.size(); ++k) {
                std::shared_ptr<PlugNode> orig_node_dest = checked_static_cast<PlugNode>(connections_in[k]->owner()->shared_from_this());
                if (!orig_node_dest)    continue;

                auto ni = orig_to_new_node.find(orig_node_dest);
                if (ni != orig_to_new_node.end()) {
                    std::shared_ptr<PlugNode> new_node_dest = ni->second;
                    
                    // Connect up the nodes
                    new_node_src->event_by_name(e_iter()->name())->add_incoming_connection(
                        new_node_dest->event_by_name(connections_in[k]->name())
                    );
                    
                }
            
            }
            
            // Outgoing
            const std::vector<Event*> &connections_out = e_iter()->outgoing_connections();
            for (DTuint k = 0; k < connections_out.size(); ++k) {
                std::shared_ptr<PlugNode> orig_node_dest = checked_static_cast<PlugNode>(connections_out[k]->owner()->shared_from_this());
                if (!orig_node_dest)    continue;

                auto ni = orig_to_new_node.find(orig_node_dest);
                if (ni != orig_to_new_node.end()) {
                    std::shared_ptr<PlugNode> new_node_dest = ni->second;
                    
                    // Connect up the nodes
                    new_node_src->event_by_name(e_iter()->name())->add_outgoing_connection(
                        new_node_dest->event_by_name(connections_out[k]->name())
                    );
                    
                }

            }

        }

    
    }
    
}

//==============================================================================
//==============================================================================

void PlugNodeUtils::copy_connections     (PlugBase* src, PlugBase* dst)
{
    if (src->incoming_connection())
        dst->set_incoming_connection(src->incoming_connection());
    
    const std::vector<PlugBase*>& connections = src->outgoing_connections();
    for (DTuint i = 0; i < connections.size(); ++i) {
        dst->add_outgoing_connection(connections[i]);
    }
    
}

//==============================================================================
//==============================================================================

void PlugNodeUtils::all_connected_nodes(std::shared_ptr<PlugNode> root, std::list<std::shared_ptr<PlugNode>> &connected)
{
        
    //
    // Magic code to traverse backwards and forwards to get all nodes_and_components code
    //
    
    std::list<std::shared_ptr<PlugNode>> nodes_and_components;
    nodes_and_components.push_back(root);  // Prime the queue
    
    FOR_EACH (i,nodes_and_components) {
    
        std::shared_ptr<PlugNode> node = *i;
        
        if (!node->isa(ComponentBase::kind()))
            connected.push_back(node);
        
        std::list<PlugBase*> plugs;
        std::list<Event*> events;
                
        node->all_plugs (plugs);
        node->all_events (events);

        FOR_EACH (j,plugs) {
        
            if ((**j).has_incoming_connection()) {
                std::shared_ptr<PlugNode> remove = checked_static_cast<PlugNode>((**j).incoming_connection()->owner()->shared_from_this());
                if (std::find(nodes_and_components.begin(), nodes_and_components.end(), remove) == nodes_and_components.end())
                    nodes_and_components.push_back(remove);
            }

            const std::vector<PlugBase*>& outgoing = (**j).outgoing_connections();
            for (DTuint k = 0; k < outgoing.size(); ++k) {
                std::shared_ptr<PlugNode> remove = checked_static_cast<PlugNode>(outgoing[k]->owner()->shared_from_this());
                if (std::find(nodes_and_components.begin(), nodes_and_components.end(), remove) == nodes_and_components.end())
                    nodes_and_components.push_back(remove);
            }
                
            (**j).remove_incoming_connection();
            (**j).remove_outgoing_connections();
        }
        
        FOR_EACH (j,events) {
        
            const std::vector<Event*>& incoming = (**j).incoming_connections();
            for (DTuint k = 0; k < incoming.size(); ++k) {
                std::shared_ptr<PlugNode> remove = checked_static_cast<PlugNode>(incoming[k]->owner()->shared_from_this());
                if (std::find(nodes_and_components.begin(), nodes_and_components.end(), remove) == nodes_and_components.end())
                    nodes_and_components.push_back(remove);
            }


            const std::vector<Event*>& outgoing = (**j).outgoing_connections();
            for (DTuint k = 0; k < outgoing.size(); ++k) {
                std::shared_ptr<PlugNode> remove = checked_static_cast<PlugNode>(outgoing[k]->owner()->shared_from_this());
                if (std::find(nodes_and_components.begin(), nodes_and_components.end(), remove) == nodes_and_components.end())
                    nodes_and_components.push_back(remove);
            }

            (**j).remove_incoming_connections();
            (**j).remove_outgoing_connections();
        }
        
        // Special case components too
        if (node->isa(ObjectBase::kind())) {
            std::shared_ptr<ObjectBase> node_base = checked_static_cast<ObjectBase>(node);
            
            for (DTuint j = 0; j < ComponentBase::NUM_COMPONENT_TYPES; ++j) {
                std::shared_ptr<ComponentBase> component_base = node_base->component_by_type ( (ComponentBase::ComponentType) j);
                
                if (component_base) {
                    if (std::find(nodes_and_components.begin(), nodes_and_components.end(), component_base) == nodes_and_components.end())
                        nodes_and_components.push_back(component_base);
                }

            }
        
        } 

        
    }

}

//==============================================================================
//==============================================================================

void PlugNodeUtils::node_and_components(std::shared_ptr<PlugNode> n, std::list<std::shared_ptr<PlugNode>> &nodes)
{
    nodes.clear();
    nodes.push_back(n);
    
    // Special case components too
    if (n->isa(ObjectBase::kind())) {
        std::shared_ptr<ObjectBase> object_base = checked_static_cast<ObjectBase>(n);
        
        for (DTuint i = 0; i < ComponentBase::NUM_COMPONENT_TYPES; ++i) {
            std::shared_ptr<PlugNode> node = object_base->component_by_type ( (ComponentBase::ComponentType) i);
            if (node)
                nodes.push_back(node);
        }
    
    } 

}

//==============================================================================
//==============================================================================

} // DT3
