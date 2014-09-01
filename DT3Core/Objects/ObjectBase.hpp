#ifndef DT3_BASEOBJECT
#define DT3_BASEOBJECT
//==============================================================================
///	
///	File: ObjectBase.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/World/WorldNode.hpp"
#include "DT3Core/Components/ComponentBase.hpp"

namespace DT3 {

//==============================================================================
/// Base object for the different component based objects in the engine.
//==============================================================================

class ObjectBase: public WorldNode {
    public:
        DEFINE_TYPE(ObjectBase,WorldNode)
		DEFINE_CREATE_AND_CLONE
        
                                                    ObjectBase              (void);
                                                    ObjectBase              (const ObjectBase &rhs);
        ObjectBase &                                operator =              (const ObjectBase &rhs);
        virtual                                     ~ObjectBase             (void);
    
        virtual void                                archive                 (const std::shared_ptr<Archive> &archive);

		/// Object was added to a world
		/// world world that object was added to
        virtual void                                add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                                remove_from_world       (void);

	public:
		/// Called to initialize the object
		virtual void                                initialize              (void);
        
		/// Adds a component to the object
		/// \param component component
        void                                        add_component           (const std::shared_ptr<ComponentBase> &component);
        
		/// Removes a component from the object
		/// \param component component
        void                                        remove_component        (const std::shared_ptr<ComponentBase> &component);

		/// Removes a component from the object based on type
		/// \param type component type
        void                                        remove_component_by_type(ComponentBase::ComponentType type);

		/// Retrieves a list of all of the components attached to the object
		/// \return components list to be modified
		std::list<std::shared_ptr<ComponentBase>>   all_components          (void) const;
        
		/// Retrieves a component by name
		/// \param name name of component
		/// \return component
        std::shared_ptr<ComponentBase>              component_by_name       (const std::string &name);

		/// Retrieves a component by type
		/// \param type componenet type
		/// \return component
        const std::shared_ptr<ComponentBase>&       component_by_type       (ComponentBase::ComponentType type);

    private:
        std::shared_ptr<ComponentBase>              _components[ComponentBase::NUM_COMPONENT_TYPES];
};

//==============================================================================
//==============================================================================

} // DT3

#endif
