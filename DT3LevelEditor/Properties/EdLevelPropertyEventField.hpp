#ifndef EDLEVELPROPERTYEVENTFIELD
#define EDLEVELPROPERTYEVENTFIELD
//==============================================================================
///	
///	File: EdLevelPropertyEventField.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelPropertyBase.hpp"

// Qt include
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QPushButton>

// Engine includes


//==============================================================================
/// Forward declarations
//==============================================================================

namespace DT3 {
    class ArchiveData;
    class PlugNode;
}

class EdLevelDocument;
class EdLevelPropertiesWindow;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

class EdLevelPropertyEventField : public EdLevelPropertyBase
{
    Q_OBJECT

	public:
										EdLevelPropertyEventField		(EdLevelPropertiesWindow *parent, std::shared_ptr<PlugNode> node, std::shared_ptr<ArchiveData> data);
										~EdLevelPropertyEventField      (void);
    
        /// Gets the value of the field
		/// \return value of field
        virtual std::string                  getValueOfField (void);
    
        /// Sets the value of the field
		/// \param value value to set
        virtual void                    setValueOfField (const std::string &value);

    private:
        std::shared_ptr<PlugNode>       _node;
        std::shared_ptr<ArchiveData>    _data;
        QPushButton                     *_value;

    public slots:
        void                            doReadParams                    (void);
        void                            doClicked                       (void);
        void                            doKeyframePressed               (void);
        
    signals:
        void                            doCommand                       (QString command, bool recache);
        void                            doKeyMenu                       (QString property, const QPointF &global_pos);

};

//==============================================================================
//==============================================================================

#endif
