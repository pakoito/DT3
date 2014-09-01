#ifndef EDLEVELDOCUMENT
#define EDLEVELDOCUMENT
//==============================================================================
///	
///	File: EdLevelDocument.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include

// Qt include
#include <QtOpenGL/QGLWidget>

// Engine includes
#include "DT3Core/World/World.hpp"
#include "DT3Core/Types/Node/PlugNode.hpp"
#include <list>

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelDocument
{
	public:
                                        EdLevelDocument         (void);
                                        ~EdLevelDocument        (void);

		/// Description
		/// \param param description
		/// \return description
        void                                            setWorld        (const std::shared_ptr<World> &w);
        
		/// Description
		/// \param param description
		/// \return description
        const std::shared_ptr<World>&                   world           (void)          {   return _world;  }
    
		/// Description
		/// \param param description
		/// \return description
        void                                            clearSelection  (void);
        
		/// Description
		/// \param param description
		/// \return description
        void                                            setSelection    (const std::list<std::shared_ptr<PlugNode>> &s);
        
		/// Description
		/// \param param description
		/// \return description
        const std::list<std::shared_ptr<PlugNode>>&     selection       (void) const    {   return _selection;  }
        
		/// Description
		/// \param param description
		/// \return description
        DTboolean                                       isSelected      (const std::shared_ptr<PlugNode> &node) const;

    public:
        std::shared_ptr<World>                  _world;
        std::list<std::shared_ptr<PlugNode>>    _selection;

};

//==============================================================================
//==============================================================================

#endif
