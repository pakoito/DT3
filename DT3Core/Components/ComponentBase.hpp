#ifndef DT3_COMPONENTBASE
#define DT3_COMPONENTBASE
//==============================================================================
///	
///	File: ComponentBase.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Node/PlugNode.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class ObjectBase;

//==============================================================================
/// Base object for the different component types in the engine. Components
/// are a good alternative to subclassing. Components can define different
/// behaviours for the same type of object. For example, a game object
/// can represent an object lika an enemy and different "drawing" components
/// can be attached to different instances of the object to draw different
/// types of enemies.
//==============================================================================

class ComponentBase: public PlugNode {
    public:
        DEFINE_TYPE(ComponentBase,PlugNode)
        
                                    ComponentBase		(void);	
									ComponentBase		(const ComponentBase &rhs);
        ComponentBase &				operator =			(const ComponentBase &rhs);	
        virtual                     ~ComponentBase		(void);
    
        virtual void                archive             (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize			(void);
        
        /// Returns the full name of the component that uniquely identifies it
		/// \return full name of the component
        virtual std::string         full_name           (void) const;
        
		/// Returns the component type. This defines which slot the component is
        /// put into on the object.
		/// \return Component type
        
        enum ComponentType {
            COMPONENT_DRAW = 0,
            COMPONENT_PHYSICS = 1,
            COMPONENT_TOUCH = 2,
            COMPONENT_COLLISION = 3,
            COMPONENT_CONSTRAINT = 4,
            COMPONENT_UTILITY = 5,
            COMPONENT_USER1 = 6,
            COMPONENT_USER2 = 7,
            COMPONENT_USER3 = 8,
            NUM_COMPONENT_TYPES
        };
        virtual ComponentType       component_type      (void) = 0;
                
		/// Returns the object that owns this component.
		/// \return Pointer to object
        ObjectBase*                 owner               (void) const        {   return _owner;  }
                
		/// Called when this component is added to the owner. Note that this will
        /// only be called if the owner is added to a world already. If not it 
        /// will be called when it is added to the World.
		/// \param owner Pointer to the owner
		virtual void                add_to_owner        (ObjectBase *owner);

		/// Called when this component is removed from its owner.
		virtual void                remove_from_owner   (void);

    private:
        ObjectBase                  *_owner;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
