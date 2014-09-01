#ifndef DT3_COMMANDCONTEXT
#define DT3_COMMANDCONTEXT
//==============================================================================
///	
///	File: CommandContext.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Math/Rectangle.hpp"
#include <list>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class World;
class PlugNode;

//==============================================================================
/// Class
//==============================================================================

class CommandContext {
    public:
                                                        CommandContext          (void);
                                                        CommandContext          (const CommandContext &rhs);
        CommandContext &                                operator =              (const CommandContext &rhs);
        virtual                                         ~CommandContext         (void);
  
	public:
		
		/// Set the current world
		/// \param world current world
        void                                            set_world               (std::shared_ptr<World> world);
        
		/// Returns the current world
		/// \return current world
        std::shared_ptr<World>                          world                   (void)          {   return _world;      }
        
		/// Set the current selection
		/// \param selection current selection
        void                                            set_selection           (const std::list<std::shared_ptr<PlugNode>> &selection);
        
		/// Returns the current selection
		/// \return current selection
        const std::list<std::shared_ptr<PlugNode>>&     selection               (void) const    {   return _selection;  }
        
		/// Clears the current selection
        void                                            clear_selection         (void);

        /// Set the current selection rectangle
		/// \param rectangle selection rectangle
        void                                            set_selection_rectangle (const Rectangle &rectangle);

        /// Returns the current selection rectangle
		/// \return current selection rectangle
        const Rectangle &                               selection_rectangle     (void) const    {   return _rectangle;  }

	private:
        std::shared_ptr<World>                          _world;
        std::list<std::shared_ptr<PlugNode>>            _selection;
        Rectangle                                       _rectangle;
        
};

//==============================================================================
//==============================================================================

} // DT3

#endif
