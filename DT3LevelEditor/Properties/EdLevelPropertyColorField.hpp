#ifndef EDLEVELPROPERTYCOLORFIELD
#define EDLEVELPROPERTYCOLORFIELD
//==============================================================================
///	
///	File: EdLevelPropertyColorField.hpp
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
class EdLevelColorSwatch;
class QSlider;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

class EdLevelPropertyColorField : public EdLevelPropertyBase
{
    Q_OBJECT

	public:
                        EdLevelPropertyColorField       (EdLevelPropertiesWindow *parent, std::shared_ptr<PlugNode> node, std::shared_ptr<ArchiveData> data);
                        ~EdLevelPropertyColorField      (void);
    
        /// Gets the value of the field
		/// \return value of field
        virtual std::string                  getValueOfField (void);
    
        /// Sets the value of the field
		/// \param value value to set
        virtual void                    setValueOfField (const std::string &value);

    private:
        std::shared_ptr<PlugNode>       _node;
        std::shared_ptr<ArchiveData>    _data;
        EdLevelColorSwatch              *_fill;
        
        QSlider                         *_r;
        QSlider                         *_g;
        QSlider                         *_b;
        QSlider                         *_a;    

        EdLevelLineEdit                 *_r2;
        EdLevelLineEdit                 *_g2;
        EdLevelLineEdit                 *_b2;
        EdLevelLineEdit                 *_a2;

    private slots:
        void                            doColorChange                   (int);
        void                            doReadParams                    (void);
        void                            doWriteParams                   (void);
        void                            doWriteParams2                  (void);
        void                            doKeyframePressed               (void);

    signals:
        void                            doCommand                       (QString command, bool recache);
        void                            doKeyMenu                       (QString property, const QPointF &global_pos);
};

//==============================================================================
//==============================================================================

#endif
