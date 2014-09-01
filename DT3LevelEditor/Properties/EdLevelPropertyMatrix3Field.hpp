#ifndef EDLEVELPROPERTYMATRIX3FIELD
#define EDLEVELPROPERTYMATRIX3FIELD
//==============================================================================
///	
///	File: EdLevelPropertyMatrix3Field.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelPropertyBase.hpp"
#include "EdLevelLineEdit.hpp"

// Qt include
#include <QtWidgets/QToolButton>

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

class EdLevelPropertyMatrix3Field : public EdLevelPropertyBase
{
    Q_OBJECT

	public:
										EdLevelPropertyMatrix3Field		(EdLevelPropertiesWindow *parent, std::shared_ptr<PlugNode> node, std::shared_ptr<ArchiveData> data);
										~EdLevelPropertyMatrix3Field	(void);

        /// Gets the value of the field
		/// \return value of field
        virtual std::string             getValueOfField (void);
    
        /// Sets the value of the field
		/// \param value value to set
        virtual void                    setValueOfField (const std::string &value);

    private:
        std::shared_ptr<PlugNode>       _node;
        std::shared_ptr<ArchiveData>    _data;
    
        EdLevelLineEdit                      *_value_11;
        EdLevelLineEdit                      *_value_12;
        EdLevelLineEdit                      *_value_13;
        EdLevelLineEdit                      *_value_21;
        EdLevelLineEdit                      *_value_22;
        EdLevelLineEdit                      *_value_23;
        EdLevelLineEdit                      *_value_31;
        EdLevelLineEdit                      *_value_32;
        EdLevelLineEdit                      *_value_33;
            
    public slots:
        void                            doReadParams                    (void);
        void                            doWriteParams                   (void);
        void                            doKeyframePressed               (void);

    signals:
        void                            doCommand                       (QString command, bool recache);
        void                            doKeyMenu                       (QString property, const QPointF &global_pos);
};

//==============================================================================
//==============================================================================

#endif
