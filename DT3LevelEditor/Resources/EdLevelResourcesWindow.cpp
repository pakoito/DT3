//==============================================================================
///	
///	File: EdLevelResourcesWindow.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelResourcesWindow.hpp"
#include "EdLevelDocument.hpp"
#include "EdLevelResourceImage.hpp"
#include "EdLevelResourceSound.hpp"

// Qt include
#include <QtWidgets/QHeaderView>
#include <QtCore/QFileInfo>
#include <QtCore/QDateTime>

// Engine includes
#include "DT3Core/Types/Utility/DirectoryLister.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Resources/Importers/ImporterImage.hpp"
#include "DT3Core/Resources/Importers/ImporterSound.hpp"
#include "DT3Core/Resources/Importers/ImporterMaterial.hpp"
#include "DT3Core/System/Factory.hpp"

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelResourcesWindow::EdLevelResourcesWindow(QWidget *parent, QToolBar *toolbar, EdLevelDocument *document, QGLWidget *gl)
    :   QTreeWidget   (parent)
{
	_document = document;
    _toolbar = toolbar;

    _gl = gl;
    
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    
    setDragEnabled(true);
    setAcceptDrops(false);
    //setDropIndicatorShown(true);
    
    setColumnCount(3);
     
    setAlternatingRowColors(true);

    QTreeWidgetItem *header_item = new QTreeWidgetItem();
	header_item->setText(0, "Name");
	header_item->setText(1, "Size");
	header_item->setText(2, "Modification Date");
    header_item->setFlags (0);

    setHeaderItem (header_item);
    header()->resizeSection(0, 220);


    onRefreshResources();
}

//==============================================================================
//==============================================================================

QMimeData* EdLevelResourcesWindow::mimeData (const QList<QTreeWidgetItem *> items) const
{
    if (items.size() == 0)
        return NULL;

    QMimeData *mime = new QMimeData();
    
    FOR_EACH (i,items) {
        QString data = (**i).data(0,Qt::UserRole).toString();
        mime->setText(data);
    }
    
    return mime;
}

//==============================================================================
//==============================================================================

void EdLevelResourcesWindow::buildTree (QTreeWidgetItem *parent, const FilePath &dir)
{

	DirectoryLister lister;
	lister.scan_path(dir, false);    // Include dirs, not recursive

	for (DTuint i = 0; i < lister.num_files(); ++i) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setData(0, Qt::UserRole, QString("{") + QString(lister.file_name(i).c_str()) + QString("}") );
        parent->addChild(item);

        // A little heavy - see if there's an importer by trying to create one
        std::shared_ptr<BaseClass> importer = Factory::create_importer(lister.file_path(i).file_ext());
        
        // Figure out which widget to use based on the file kind
        if (importer && importer->isa(ImporterImage::kind())) {
            EdLevelResourceImage *itemWidget = new EdLevelResourceImage(lister.file_path(i));
        
            setItemWidget(item,0,itemWidget);
            item->setSizeHint(0,itemWidget->minimumSize());
            
        } else if (importer && importer->isa(ImporterSound::kind())) {
            EdLevelResourceSound *itemWidget = new EdLevelResourceSound(lister.file_path(i));
        
            setItemWidget(item,0,itemWidget);
            item->setSizeHint(0,itemWidget->minimumSize());
            
        } else {
            item->setText(0, lister.file_path(i).file_name().c_str() );
        }
        
        // Add meta info
        QFileInfo file_info(lister.file_path(i).full_path().c_str());
        
        // Size
        if (file_info.size() < 1024)
            item->setText(1, (MoreStrings::cast_to_string(file_info.size()) + " bytes").c_str() );
        else
            item->setText(1, (MoreStrings::cast_to_string(file_info.size()/1024.0F) + " kb").c_str() );
            
        // Modification
        item->setText(2, file_info.lastModified().toString("ddd MMMM d, yyyy") );
            
        // Recurse
        if (lister.is_directory(i))
            buildTree( item, lister.file_path(i) );

        /*ResourceCache c;
        c._path = lister.getFilePath(i);
        _resource_cache.pushBack(c);*/

	}

}

//==============================================================================
//==============================================================================

void EdLevelResourcesWindow::onRefreshResources (void)
{
    clear();

    QTreeWidgetItem *root_item = invisibleRootItem();
    root_item->setFlags (0);

	_top_level = new QTreeWidgetItem();
	_top_level->setText(0, "Resources");
    //_top_level->setFlags (Qt::ItemIsDropEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    addTopLevelItem(_top_level);
	
    buildTree(_top_level, FilePath("{ED_DATA_PATH}"));	
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelResourcesWindow.cpp"

