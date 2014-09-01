#ifndef DT3_COMMAND
#define DT3_COMMAND
//==============================================================================
///	
///	File: Command.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward decalrations
//==============================================================================

class PlugBase;
class PlugNode;
class Event;
class ComponentBase;

class CommandRegistry;
class CommandContext;

//==============================================================================
/// Defines a unit testing interface.
//==============================================================================

class Command: public BaseClass {
    public:
        DEFINE_TYPE(Command,BaseClass)
		
                                Command				(void);	
                                Command				(const Command &rhs);
        Command &               operator =			(const Command &rhs);	
        virtual                 ~Command			(void);
    
	public:

		/// Registers all of the commands defined by the subclass
		/// \registry registration node
        virtual void                            register_commands   (void) = 0;
  
		/// Gets the node defined by the node name passed in
		/// \ctx Command context
		/// \node Node name
		/// \return description
        static std::shared_ptr<PlugNode>        node_or_group       (CommandContext &ctx, const std::string &node);

		/// Description
		/// \param param description
		/// \return description
        static std::shared_ptr<ComponentBase>   component           (CommandContext &ctx, const std::string &node, const std::string &component);

		/// Description
		/// \param param description
		/// \return description
        static PlugBase*                        plug                (CommandContext &ctx, const std::string &node, const std::string &component, const std::string &plug);

		/// Description
		/// \param param description
		/// \return description
        static Event*                           event               (CommandContext &ctx, const std::string &node, const std::string &component, const std::string &event);

		/// Description
		/// \param param description
		/// \return description
        static DTboolean                        node_name           (const std::string &s, std::string &node);

		/// Description
		/// \param param description
		/// \return description
        static DTboolean                        component_name      (const std::string &s, std::string &component);

		/// Description
		/// \param param description
		/// \return description
       static  DTboolean                        param_name          (const std::string &s, std::string &param);


};

//==============================================================================
//==============================================================================
	
} // DT3

#endif
