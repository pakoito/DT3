#ifndef EDLEVELRESOURCESWINDOW
#define EDLEVELRESOURCESWINDOW
//==============================================================================
///	
///	File: EdLevelResourcesWindow.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
// Qt include
#include <QtWidgets/QTreeWidget>
#include <QtCore/QMimeData>
#include <QtWidgets/QToolBar>

// Engine includes
#include "DT3Core/Types/FileBuffer/FilePath.hpp"

//==============================================================================
/// Forward declarations
//==============================================================================

class EdLevelDocument;
class QGLWidget;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelResourcesWindow: public QTreeWidget
{
    Q_OBJECT

	public:
										EdLevelResourcesWindow		(QWidget *parent, QToolBar *toolbar, EdLevelDocument *document, QGLWidget *gl);

        Qt::DropActions                 supportedDropActions        (void) const    {   return Qt::CopyAction;  }
        
        QMimeData*                      mimeData                    (const QList<QTreeWidgetItem *> items) const;

	private:
        void                            buildTree                   (QTreeWidgetItem *parent, const FilePath &dir);

        EdLevelDocument                 *_document;
        QToolBar                        *_toolbar;

        QGLWidget                       *_gl;
        QTreeWidgetItem                 *_top_level;
        
        // Internal representation of scene
        struct ResourceCache {            
            DTboolean   operator == (const ResourceCache& rhs) const	{	return _path == rhs._path;		}

            FilePath                    _path;
        };
        
        std::list<ResourceCache>             _resource_cache;
        	
    public slots:
        void                            onRefreshResources          (void);
};

//==============================================================================
//==============================================================================

#endif
