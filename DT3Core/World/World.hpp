#ifndef DT3_WORLD
#define DT3_WORLD
//==============================================================================
///	
///	File: World.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Node/PlugNode.hpp"
#include "DT3Core/World/WorldEntry.hpp"
#include <memory>
#include <list>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Group;
class WorldNode;
class CameraObject;
class Session;
class GameController;
class TouchEvent;

//==============================================================================
//==============================================================================

class World: public PlugNode {
    public:
        DEFINE_TYPE(World,PlugNode)
		DEFINE_CREATE

									World                   (void);
	private:
									World                   (const World &rhs);
        World &                     operator =              (const World &rhs);
	public:
        virtual						~World                  (void);
                                        
        virtual void				archive                 (const std::shared_ptr<Archive> &archive);
        virtual void				archive_done            (const std::shared_ptr<Archive> &archive);

    public:
		/// Called to initialize the object
		virtual void				initialize              (void);

		/// Called to uninitialize the object
		virtual void				uninitialize            (void);

		/// Called when everything is completely done loading
		virtual void                ready_to_go             (void)                      {}

		/// Process arguments
		virtual void                process_args            (const std::string &args)   {}

    
        //
        // Groups
        //
        
		/// Description
		/// \param param description
		/// \return description
        virtual void				add_group               (const std::shared_ptr<Group> &group);

		/// Description
		/// \param param description
		/// \return description
        void                        add_group_unique_name   (const std::shared_ptr<Group> &group);

		/// Description
		/// \param param description
		/// \return description
        void						remove_group            (const std::shared_ptr<Group> group);
			
		/// Description
		/// \param param description
		/// \return description
        void                        remove_all_groups       (void);
    
    
        //
        // Nodes
        //
		
		/// Description
		/// \param param description
		/// \return description
        virtual void				add_node                (const std::shared_ptr<WorldNode> &node);

		/// Description
		/// \param param description
		/// \return description
        void                        add_node_unique_name    (const std::shared_ptr<WorldNode> &node);
			
		/// Description
		/// \param param description
		/// \return description
        virtual void				remove_node             (const std::shared_ptr<WorldNode> node);
			
		/// Description
		/// \param param description
		/// \return description
        void                        remove_all_nodes        (void);


        //
        // Query Nodes
        //
        
		/// Description
		/// \param param description
		/// \return description
		const std::list<std::shared_ptr<WorldNode>>&    nodes           (void) const;
		
		/// Description
		/// \param param description
		/// \return description
		std::shared_ptr<WorldNode>                  node_by_name    (std::string name);

		/// Description
		/// \param param description
		/// \return description
		std::shared_ptr<WorldNode>                  node_by_id      (DTuint unique_id);
		
    
        //
        // Query Groups
        //
    
		/// Description
		/// \param param description
		/// \return description
		const std::list<std::shared_ptr<Group>>&    groups          (void)	{	return _groups;			}
		
		/// Description
		/// \param param description
		/// \return description
		std::shared_ptr<Group>                      group_by_name   (std::string name);


        //
        // Game Controller
        //
		
		/// Description
		/// \param param description
		/// \return description
		const std::shared_ptr<GameController>&      game_controller (void)  {	return _game_controller;	}
		
    
        //
        // Session
        //

		/// Description
		/// \param param description
		/// \return description
		void                                        set_session     (const std::shared_ptr<Session> &s);

		/// Description
		/// \param param description
		/// \return description
		const std::shared_ptr<Session>&             session         (void) const;


		//
		// Camera Management
		//
		
		/// Description
		/// \param param description
		/// \return description
		void                                        set_camera      (const std::shared_ptr<CameraObject> &camera);

		/// Description
		/// \param param description
		/// \return description
		const std::shared_ptr<CameraObject>&        camera          (void) const;


		//
		//	draw
		//
				
		/// Description
		/// \param param description
		/// \return description
		virtual void                                draw            (const DTfloat lag);

		/// Description
		/// \param param description
		/// \return description
		virtual void                                draw_diagnostics(const DTfloat lag);


		//
		// tick
		//
		
		/// Description
		/// \param param description
		/// \return description
		virtual void                                tick            (const DTfloat dt);
           
            
		/// Description
		/// \param param description
		/// \return description
        virtual void                                clean           (void);
                
        //
        // GUI
        //
            
        virtual DTboolean                           touch_gui       (const TouchEvent *event);
    
    
		//
		// Registration 
        //

        /// Description
		/// \param param description
		/// \return description
        virtual void                register_for_draw               (WorldNode *node, std::shared_ptr<DrawCallbackType> cb);
        virtual void                unregister_for_draw             (WorldNode *node, std::shared_ptr<DrawCallbackType> cb);

        /// Description
		/// \param param description
		/// \return description
        virtual void                register_for_draw_diagnostics   (WorldNode *node, std::shared_ptr<DrawCallbackType> cb);
        virtual void                unregister_for_draw_diagnostics (WorldNode *node, std::shared_ptr<DrawCallbackType> cb);

        /// Description
		/// \param param description
		/// \return description
        virtual void                register_for_tick               (WorldNode *node, std::shared_ptr<TickCallbackType> cb);
        virtual void                unregister_for_tick             (WorldNode *node, std::shared_ptr<TickCallbackType> cb);

        /// Description
		/// \param param description
		/// \return description
        virtual void                register_for_touch              (WorldNode *node, std::shared_ptr<TouchCallbackType> cb);
        virtual void                unregister_for_touch            (WorldNode *node, std::shared_ptr<TouchCallbackType> cb);

	protected:
    
        void                        validate                        (void);
    
    
		std::list<std::shared_ptr<WorldNode>>   _nodes;
		std::list<std::shared_ptr<WorldNode>>   _nodes_to_delete;       // Delayed delete of nodes
        
		std::list<std::shared_ptr<Group>>       _groups;
        
        std::list<DrawCallbackEntry>            _draw_callbacks;
        std::list<DrawCallbackEntry>            _draw_diagnostics_callbacks;
        std::list<TickCallbackEntry>            _tick_callbacks;
        std::list<TouchCallbackEntry>           _touch_callbacks;
    
        std::list<DrawCallbackEntry>            _draw_callbacks_to_delete;
        std::list<DrawCallbackEntry>            _draw_diagnostics_callbacks_to_delete;
        std::list<TickCallbackEntry>            _tick_callbacks_to_delete;
        std::list<TouchCallbackEntry>           _touch_callbacks_to_delete;
    
        std::string                             _default_camera;
    
        //
        // Current Camera
        //

		std::shared_ptr<CameraObject>           _camera;
						
		//
		// Special nodes
		//
		
		// Keep track of the game controller
		std::shared_ptr<Session>                _session;
		std::shared_ptr<GameController>         _game_controller;
        
};

//==============================================================================
//==============================================================================

} // DT3

#endif
