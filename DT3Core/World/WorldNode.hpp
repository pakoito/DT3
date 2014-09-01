#ifndef DT3_WORLDNODE
#define DT3_WORLDNODE
//==============================================================================
///	
///	File: WorldNode.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Node/PlugNode.hpp"
#include "DT3Core/Types/Node/Plug.hpp"
#include "DT3Core/Types/Node/Event.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/Math/Color4f.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Group;
class World;

//==============================================================================
/// Base object for the different placeable types of objects in the engine.
//==============================================================================

class WorldNode: public PlugNode {
    public:
        DEFINE_TYPE(WorldNode,PlugNode)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
        
                                    WorldNode			(void);	
									WorldNode			(const WorldNode &rhs);
        WorldNode &                 operator =			(const WorldNode &rhs);	
        virtual                     ~WorldNode			(void);
    
        virtual void                archive             (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize			(void);


		// Setting the database

		/// Returns the database that the object belongs to.
		/// \param param description
		/// \return description
        World*                      world               (void) const                {   return _world;      }
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world        (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world   (void);
    
    
		/// Gets the group.
		/// \return group that this object belongs to
        Group*                      group               (void) const                {   return _group;      }

		/// Sets the group.
		/// \param group the group to belong to
        virtual void                add_to_group        (Group *group)              {   _group = group;     }
        
		/// Sets the group.
		/// \param group the group to belong to
        virtual void                remove_from_group   (void);



		/// Called when everything is completely done loading
		virtual void                ready_to_go         (void)                      {}

    
        DEFINE_ACCESSORS(node_position, set_node_position, Vector3, _node_position)
        DEFINE_ACCESSORS(node_collapsed, set_node_collapsed, DTboolean, _node_collapsed)
        DEFINE_ACCESSORS(node_color, set_node_color, Color4f, _node_color)


#ifdef DT3_EDITOR
		/// Dumps the C++ code that can be used to initialize an object of this type.
		/// \param param s
        virtual void                dump_code           (const std::string &object_name, Stream &s);
#endif
    
	private:		
    
		DTboolean					_node_collapsed;	// Editor node collapsed
		Vector3						_node_position;		// Editor node position
		Color4f						_node_color;		// Editor node color
    
        Plug<WorldNode*>            _self;
        Group                       *_group;            // weak reference - no ref count
        World                       *_world;            // weak reference - no ref count
};

//==============================================================================
//==============================================================================

} // DT3

#endif
