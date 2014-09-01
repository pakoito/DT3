//==============================================================================
///	
///	File: EdLevelLibraryWindow.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelLibraryWindow.hpp"
#include "EdLevelDocument.hpp"

// Qt include
#include <QtWidgets/QHeaderView>
#include <QtCore/QFileInfo>
#include <QtCore/QDateTime>
#include <QtCore/QMimeData>

// Engine includes
#include "DT3Core/Types/Node/Group.hpp"
#include "DT3Core/Types/Utility/DirectoryLister.hpp"
#include "DT3Core/Types/Utility/ConsoleStream.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveTextReader.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveObjectQueue.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveBinaryBufferWriter.hpp"

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelLibraryWindow::EdLevelLibraryWindow(QWidget *parent, QToolBar *toolbar, EdLevelDocument *document)
    :   QTreeWidget   (parent)
{
	_document = document;
    _toolbar = toolbar;
    
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    
    setDragEnabled(true);
    //setAcceptDrops(false);
    //setDropIndicatorShown(true);
    
    setColumnCount(2);
     
    setAlternatingRowColors(true);

    QTreeWidgetItem *header_item = new QTreeWidgetItem();
	header_item->setText(0, "Name");
	header_item->setText(1, "Description");
    header_item->setFlags (0);

    setHeaderItem (header_item);
    header()->resizeSection(0, 220);
    
    onRefreshLibrary();
}

//==============================================================================
//==============================================================================

QMimeData* EdLevelLibraryWindow::mimeData (const QList<QTreeWidgetItem *> items) const
{
    if (items.size() != 1)
        return NULL;

    QMimeData *mime = new QMimeData();
    
    FOR_EACH (i,items) {
    
        QByteArray data = (**i).data(0,Qt::UserRole).toByteArray();
        mime->setData("DT3/Group", data);
    }
    
    return mime;
}

//==============================================================================
//==============================================================================

std::shared_ptr<Group> EdLevelLibraryWindow::importGroup(const FilePath &path)
{
    std::shared_ptr<ArchiveTextReader> reader = ArchiveTextReader::create();
    
    DTerr error = reader->open(path);
    if (error != DT3_ERR_NONE)
        return NULL;
    
    std::shared_ptr<Group> group = checked_cast<Group>(ArchiveObjectQueue::queue_in_tree(reader));
    if (!group)
        return NULL;
        
    return group;
}

//==============================================================================
//==============================================================================


void EdLevelLibraryWindow::buildTree (QTreeWidgetItem *parent, const FilePath &dir)
{
	DirectoryLister lister;
	lister.scan_path(dir, false);    // Include dirs, not recursive

	for (DTuint i = 0; i < lister.num_files(); ++i) {
		LOG_MESSAGE << lister.file_name(i);
        
        // Recurse
        if (lister.is_directory(i)) {
        
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setData(0, Qt::UserRole, QString(lister.file_name(i).c_str()) );
            item->setText(0, QString(lister.file_name(i).c_str()) );
            parent->addChild(item);

            buildTree( item, lister.file_path(i) );
            
        } else if (lister.file_path(i).file_ext() == "lobj") {
            
            std::shared_ptr<Group> group = importGroup(lister.file_path(i));
            if (group) {
            
                std::shared_ptr<ArchiveBinaryBufferWriter> writer = ArchiveBinaryBufferWriter::create();
                writer->set_ignore_streamable_flag(true);
                ArchiveObjectQueue::queue_out_tree(writer, group );
                            
                QTreeWidgetItem *item = new QTreeWidgetItem();
                item->setData(0, Qt::UserRole, QByteArray( (const char *) writer->stream().buffer(), (DTuint) writer->stream().size()) );
                item->setText(0, QString(group->name().c_str()) );
                item->setText(1, QString(group->description().c_str()) );
                parent->addChild(item);
                
                group->remove_all_nodes();
            }
            
        }


	}
}

//==============================================================================
//==============================================================================

void EdLevelLibraryWindow::onRefreshLibrary (void)
{
    clear();

    QTreeWidgetItem *root_item = invisibleRootItem();
    root_item->setFlags (0);

	_top_level = new QTreeWidgetItem();
	_top_level->setText(0, "Library");

    addTopLevelItem(_top_level);

	LOG_MESSAGE << "Scanning files";
    buildTree(_top_level, FilePath("{ED_LIBRARY_PATH}"));	
	LOG_MESSAGE << "Done scanning files";
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelLibraryWindow.cpp"

