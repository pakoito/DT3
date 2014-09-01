#ifndef EDLEVELWORLDLAYOUT
#define EDLEVELWORLDLAYOUT
//==============================================================================
///	
///	File: EdLevelWorldLayout.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
// Qt include
#include <QtWidgets/QLayout>
#include <QtCore/QList>

// Engine includes


//==============================================================================
//==============================================================================

//using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelWorldLayout: public QLayout
{
    public:
        EdLevelWorldLayout              (QWidget *parent);
        EdLevelWorldLayout              (void);
        ~EdLevelWorldLayout             (void);

    public:
    
        /// Description
		/// \param param description
		/// \return description
        void            addItem         (QLayoutItem *item);
        
        /// Description
		/// \param param description
		/// \return description
        QSize           sizeHint        (void) const;
        
        /// Description
		/// \param param description
		/// \return description
        QSize           minimumSize     (void) const;
        
        /// Description
		/// \param param description
		/// \return description
        QLayoutItem*    itemAt          (int i) const;
        
        /// Description
		/// \param param description
		/// \return description
        QLayoutItem*    takeAt          (int i);
        
        /// Description
		/// \param param description
		/// \return description
        void            setGeometry     (const QRect &rect);
        
        /// Description
		/// \param param description
		/// \return description
        int             count           (void) const    {   return _list.count();   }

     private:
         QList<QLayoutItem*>    _list;

};

//==============================================================================
//==============================================================================

#endif
