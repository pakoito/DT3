#ifndef EDLEVELPROPERTYBASE
#define EDLEVELPROPERTYBASE
//==============================================================================
///	
///	File: EdLevelPropertyBase.hpp
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
#include <QtWidgets/QToolButton>

// Engine includes


//==============================================================================
//==============================================================================

//using namespace DT3;

//==============================================================================
//==============================================================================

class EdLevelPropertyBase : public QWidget
{
    Q_OBJECT

	public:
    
        /// Description
		/// \param param description
		/// \return description
        std::string  parseParam (const std::string &s);

        /// Description
		/// \param param description
		/// \return description
        void    buildStatusButtons (void);

        /// Description
		/// \param param description
		/// \return description
        QToolButton*    keyButton       (void)  {   return _key;            }
        QToolButton*    hasInputButton  (void)  {   return _has_input;      }
        QToolButton*    hasOutputButton (void)  {   return _has_output;     }

        /// Gets the value of the field
		/// \return value of field
        virtual std::string  getValueOfField (void) = 0;
    
        /// Sets the value of the field
		/// \param value value to set
        virtual void    setValueOfField (const std::string &value) = 0;
    
    
    private:
        QToolButton     *_key;
        QToolButton     *_has_input;
        QToolButton     *_has_output;
};

//==============================================================================
//==============================================================================

#endif
