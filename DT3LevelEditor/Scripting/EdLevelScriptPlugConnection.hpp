#ifndef EDLEVELSCRIPTPLUGCONNECTION
#define EDLEVELSCRIPTPLUGCONNECTION
//==============================================================================
///	
///	File: EdLevelScriptPlugConnection.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelScriptConnection.hpp"

// Qt include
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>

// Engine includes

//==============================================================================
/// Forward declarations
//==============================================================================

namespace DT3 {
    class PlugBase;
}

class EdLevelScriptNodeStandard;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelScriptPlugConnection: public EdLevelScriptConnection
{
    //Q_OBJECT

	public:
        enum { Type = UserType + 2 };
        int type() const {  return Type;    }

	public:
                                    EdLevelScriptPlugConnection (void)  
                                        :   _font("Arial", 9),
                                            _fm(_font)
                                    {}
                                    
									EdLevelScriptPlugConnection	(   EdLevelScriptNodeStandard *node1, PlugBase *plug1,
                                                                    EdLevelScriptNodeStandard *node2, PlugBase *plug2);
                                                                
                                    ~EdLevelScriptPlugConnection()  {}

	public:
        virtual void                readjustBounds  (void);

		QRectF						boundingRect	(void) const;
		virtual void				paint			(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        
        void                        disconnect      (void)          {   _plug1 = _plug2 = NULL; }

        PlugBase*                   plug1           (void) const    {   return _plug1;  }
        PlugBase*                   plug2           (void) const    {   return _plug2;  }

    private:
        static const float SHADOW_OFFSET_X;
        static const float SHADOW_OFFSET_Y;
        static const float BEZIER_SIZE;
        static const float PAD;
        static const float OFFSET;

        std::string                 getValue        (void) const;

        QRectF                      getTextRect     (void) const;
        
        PlugBase                    *_plug1;
        PlugBase                    *_plug2;

        QFont                       _font;
        QFontMetrics                _fm;

};

//==============================================================================
//==============================================================================

#endif
