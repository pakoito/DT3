#ifndef EDLEVELTOOL
#define EDLEVELTOOL
//==============================================================================
///	
///	File: EdLevelTool.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
// Qt include
#include <QtWidgets/QWidget>

// Engine includes
#include "DT3Core/Resources/ResourceTypes/MaterialResource.hpp"
#include <list>

//==============================================================================
/// Forward declarations
//==============================================================================

namespace DT3 {
    class PlugNode;
};

class EdLevelToolEvent;
class EdLevelToolWindow;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelTool: public BaseClass
{
	public:
        DEFINE_TYPE(EdLevelTool, BaseClass)
        //DEFINE_CREATE_AND_CLONE

                        EdLevelTool     (void);
        virtual         ~EdLevelTool    (void)  {}

    public:
		/// Description
		/// \param param description
		/// \return description
        virtual void                draw                    (EdLevelToolWindow *parent, const std::shared_ptr<CameraObject> &camera, DTfloat scale);
        
		/// Description
		/// \param param description
		/// \return description
        virtual void                doEvent                 (EdLevelToolWindow *parent, const EdLevelToolEvent &event);
        

		/// Description
		/// \param param description
		/// \return description
        Matrix4                     getCombinedTransform    (void) const;

		/// Description
		/// \param param description
		/// \return description
        void                        applyCombinedTransform  (EdLevelToolWindow *parent, const Matrix4 &transform, DTfloat grid = 0.0F);

		/// Description
		/// \param param description
		/// \return description
        void                        applyCombinedScale      (EdLevelToolWindow *parent, DTfloat scale);

		/// Description
		/// \param param description
		/// \return description
        virtual Matrix4             getManipulatorTransform (void) const = 0;


		/// Description
		/// \param param description
		/// \return description
        void                        snapToGrid              (Vector3 &translation, DTfloat grid);


		/// Description
		/// \param param description
		/// \return description
        void                        clearSelection          (void);

		/// Description
		/// \param param description
		/// \return description
        void                        setSelection            (const std::list<std::shared_ptr<PlugNode>> &s);
        
		/// Description
		/// \param param description
		/// \return description
        const std::list<std::shared_ptr<PlugNode>>&      getSelection            (void) const    {   return _selection;  }
    
    private:
        std::list<std::shared_ptr<PlugNode>>     _selection;

};

//==============================================================================
//==============================================================================

#endif
