//==============================================================================
///	
///	File: ObjectBase.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/ObjectBase.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/SystemCallbacks.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveProcessPointers.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_PLACEABLE(ObjectBase,"Object","EdPlaceableObjectAdapter")

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ObjectBase::ObjectBase (void)
{  

}
		
ObjectBase::ObjectBase (const ObjectBase &rhs)
    :   WorldNode				(rhs)
{   
    for (DTuint i = 0; i < ComponentBase::NUM_COMPONENT_TYPES; ++i) {
        if (rhs._components[i]) {
            _components[i] = checked_static_cast<ComponentBase>(rhs._components[i]->clone());
        }
    }
}

ObjectBase & ObjectBase::operator = (const ObjectBase &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		WorldNode::operator = (rhs);
        
        for (DTuint i = 0; i < ComponentBase::NUM_COMPONENT_TYPES; ++i) {
            _components[i].reset();
            
            if (rhs._components[i]) {
                _components[i] = checked_static_cast<ComponentBase>(rhs._components[i]->clone());
                SystemCallbacks::add_component_cb().fire(world(),_components[i].get());
            }
        }
    }
    return (*this);
}
			
ObjectBase::~ObjectBase (void)
{

}

//==============================================================================
//==============================================================================

void ObjectBase::initialize (void)
{
	WorldNode::initialize();
}

//==============================================================================
//==============================================================================

void ObjectBase::archive (const std::shared_ptr<Archive> &archive)
{
    WorldNode::archive(archive);

    archive->push_domain (class_id ());

    DTuint num_components = ComponentBase::NUM_COMPONENT_TYPES;
    *archive << ARCHIVE_DATA(num_components,DATA_PERSISTENT);
	
    for (DTuint i = 0; i < ComponentBase::NUM_COMPONENT_TYPES; ++i)
        archive->add_post_process(ARCHIVE_PROCESS_POINTERS(archive,_components[i]));

    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ObjectBase::add_to_world(World *world)
{
    WorldNode::add_to_world(world);

    // Tell the components that they were added to this object
    for (DTint i = 0; i < ComponentBase::NUM_COMPONENT_TYPES; ++i) {
        if (_components[i]) {
        
            if (_components[i]->owner() == this) {
                LOG_MESSAGE << "Component already has an owner... so skipping";
                continue;
            }

            if (_components[i]->owner() && _components[i]->owner() != this) {
                LOG_ERROR << "Component already has ANOTHER owner... so skipping";
                continue;
            }
        
            _components[i]->add_to_owner(this);

            SystemCallbacks::add_component_cb().fire(world,_components[i].get());
        }
    }
}

void ObjectBase::remove_from_world (void)			
{	
    // Tell the components that they were removed from this object
    for (DTint i = ComponentBase::NUM_COMPONENT_TYPES-1; i >= 0; --i) {
        if (_components[i]) {
            SystemCallbacks::remove_component_cb().fire(world(),_components[i].get());
                
            _components[i]->remove_from_owner();
            _components[i].reset();
        }
    }
    
    WorldNode::remove_from_world();
}

//==============================================================================
//==============================================================================

void ObjectBase::add_component (const std::shared_ptr<ComponentBase> &component)
{
    if (_components[component->component_type()])
        remove_component(_components[component->component_type()]);
    
    _components[component->component_type()] = component;
    
    if (world()) {
        component->add_to_owner(this);
        
        SystemCallbacks::add_component_cb().fire(world(),component.get());
    }
    

}
        
void ObjectBase::remove_component (const std::shared_ptr<ComponentBase> &component)
{
    if (!component)
        return;
    
    if (_components[component->component_type()] == component)
        _components[component->component_type()].reset();  // Release happens after callback
        
    if (world()) {
        SystemCallbacks::remove_component_cb().fire (world(),component.get());

        component->remove_from_owner();
    }
}

void ObjectBase::remove_component_by_type (ComponentBase::ComponentType type)
{
    remove_component(_components[type]);
}

std::list<std::shared_ptr<ComponentBase>> ObjectBase::all_components (void) const
{   
    std::list<std::shared_ptr<ComponentBase>> components;
    
    for (DTuint i = 0; i < ComponentBase::NUM_COMPONENT_TYPES; ++i) {
        if (_components[i]) {
            components.push_back(_components[i]);
        }
    }
    
    return components;
}

std::shared_ptr<ComponentBase> ObjectBase::component_by_name (const std::string &name)
{
    for (DTuint i = 0; i < ComponentBase::NUM_COMPONENT_TYPES; ++i) {
        if (_components[i] && (_components[i]->name() == name)) {
            return _components[i];
        }
    }
    
    return std::shared_ptr<ComponentBase>();
}

const std::shared_ptr<ComponentBase>& ObjectBase::component_by_type	(ComponentBase::ComponentType type)
{
    return _components[type];
}

//==============================================================================
//==============================================================================

} // DT3

