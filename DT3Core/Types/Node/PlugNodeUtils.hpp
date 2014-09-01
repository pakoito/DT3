#ifndef DT3_PLUGNODEUTILS
#define DT3_PLUGNODEUTILS
//==============================================================================
///	
///	File: PlugNodeUtils.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Node/PlugNode.hpp"
#include <map>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class PlugNodeUtils {
    private:
								PlugNodeUtils		(void);	
								PlugNodeUtils       (const PlugNodeUtils &rhs);
        PlugNodeUtils &         operator =			(const PlugNodeUtils &rhs);
								~PlugNodeUtils		(void);

	public:
		/// Duplicate a list of nodes
		/// \param from list of source nodes
		/// \param to list of duplicate nodes
        static void             copy_nodes          (   const std::list<std::shared_ptr<PlugNode>> &from,
                                                        std::list<std::shared_ptr<PlugNode>> &to);

		/// Duplicate a list of nodes and report a mapping between them
		/// \param from list of source nodes
		/// \param to list of duplicate nodes
		/// \param orig_to_new_node mapping from old to new nodes
        static void             copy_nodes          (   const std::list<std::shared_ptr<PlugNode>> &from,
                                                        std::list<std::shared_ptr<PlugNode>> &to,
                                                        std::map<std::shared_ptr<PlugNode>, std::shared_ptr<PlugNode>> &orig_to_new_node);
        
		/// Copy connections between plugs
		/// \param src source plug
		/// \param dst destination plug
        static void             copy_connections    (PlugBase* src, PlugBase* dst);

		/// Recursively build a list of all of the nodes connected to a root node
		/// \param root root node
		/// \param connected all connected nodes
        static void             all_connected_nodes (std::shared_ptr<PlugNode> root, std::list<std::shared_ptr<PlugNode>> &connected);
        
		/// Given a node, build a list of it and it's components
		/// \param s node
		/// \param nodes nodes and components
        static void             node_and_components (std::shared_ptr<PlugNode> n, std::list<std::shared_ptr<PlugNode>> &nodes);
};

//==============================================================================
//==============================================================================

} // DT3

#endif
