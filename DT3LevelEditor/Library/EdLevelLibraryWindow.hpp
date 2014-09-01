#ifndef EDLEVELLIBRARYWINDOW
#define EDLEVELLIBRARYWINDOW
//==============================================================================
///	
///	File: EdLevelLibraryWindow.hpp
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
#include <QtWidgets/QToolBar>

// Engine includes
#include "DT3Core/Types/FileBuffer/FilePath.hpp"

//==============================================================================
/// Forward declarations
//==============================================================================

namespace DT3 {
    class Group;
}

class EdLevelDocument;
class QGLWidget;

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
/// Class
//==============================================================================

class EdLevelLibraryWindow: public QTreeWidget
{
    Q_OBJECT

	public:
										EdLevelLibraryWindow		(QWidget *parent, QToolBar *toolbar, EdLevelDocument *document);

        QMimeData*                      mimeData                    (const QList<QTreeWidgetItem *> items) const;

	private:
        void                            buildTree                   (QTreeWidgetItem *parent, const FilePath &dir);
        std::shared_ptr<Group>          importGroup                 (const FilePath &path);

        EdLevelDocument                 *_document;
        QToolBar                        *_toolbar;
        QTreeWidgetItem                 *_top_level;
        
        // Internal representation of scene
        struct LibraryCache {            
            DTboolean   operator == (const LibraryCache& rhs) const	{	return _path == rhs._path;		}

            FilePath                    _path;
        };
        
        std::list<LibraryCache>              _resource_cache;
        	
    public slots:
        void                            onRefreshLibrary            (void);
};

//==============================================================================
//==============================================================================

#endif
