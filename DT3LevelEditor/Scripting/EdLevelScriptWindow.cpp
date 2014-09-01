//==============================================================================
///	
///	File: EdLevelScriptWindow.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelScriptWindow.hpp"
#include "EdLevelDocument.hpp"
#include "EdLevelGroup.hpp"
#include "EdLevelScriptNodeStandard.hpp"
#include "EdLevelScriptPlugConnection.hpp"
#include "EdLevelScriptEventConnection.hpp"
#include "EdLevelScriptConnectionRubberBand.hpp"
#include "EdLevelScriptSelectionRubberBand.hpp"
#include "EdLevelLibraryDialog.hpp"

// Qt include
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QScrollBar>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>
#include <QtCore/QMimeData>

// Engine includes
#include "DT3Core/System/Globals.hpp"
#include "DT3Core/Objects/ObjectBase.hpp"
#include "DT3Core/Components/ComponentBase.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"
#include "DT3Core/Types/Node/Group.hpp"
#include "DT3Core/Types/FileBuffer/FilePath.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveBinaryBufferReader.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveObjectQueue.hpp"

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

const int EdLevelScriptWindow::AUTO_SCROLL_TIMER = 30;
const int EdLevelScriptWindow::AUTO_SCROLL_MARIGN = 30;

//==============================================================================
//==============================================================================

EdLevelScriptWindow::EdLevelScriptWindow(QWidget *parent, QToolBar *toolbar, EdLevelDocument *document)
    :   QGraphicsView   (parent),
		_mode           (MODE_NONE),
		_scale          (1.0F),
		_z              (0)
{
	_document = document;
    _toolbar = toolbar;
    
    //
    // Actions and toolbar
    //
    
    _script_align = new QAction(tr("&Align"), this);
    _script_align->setIcon(QIcon(":/images/align.png"));
    _script_align->setStatusTip(tr("Align Nodes"));
    connect(_script_align, SIGNAL(triggered()), this, SLOT(onScriptAlign()));

    _script_values = new QAction(tr("&Values"), this);
    _script_values->setIcon(QIcon(":/images/togglevals.png"));
    _script_values->setStatusTip(tr("Toggle Values"));
    connect(_script_values, SIGNAL(triggered()), this, SLOT(onScriptToggleValues()));
    
    _script_add_to_library = new QAction(tr("&Add To Library"), this);
    _script_add_to_library->setIcon(QIcon(":/images/add_to_library.png"));
    _script_add_to_library->setStatusTip(tr("Add To Library"));
    connect(_script_add_to_library, SIGNAL(triggered()), this, SLOT(onScriptAddToLibrary()));

    _script_color = new QAction(tr("&Color"), this);
    _script_color->setIcon(QIcon(":/images/color.png"));
    _script_color->setStatusTip(tr("Change Color"));
    connect(_script_color, SIGNAL(triggered()), this, SLOT(onScriptColor()));

    toolbar->addAction(_script_align);
    toolbar->addAction(_script_values);
    toolbar->addAction(_script_add_to_library);
    toolbar->addAction(_script_color);

    // 
    // Set up window
    //
    
    
    _scene.setItemIndexMethod(QGraphicsScene::NoIndex);

	setScene(&_scene);
    
    connect(&_scene,    SIGNAL(selectionChanged()),
            this,       SLOT(selectionChanged()));

    connect(&_auto_scroll_timer,    SIGNAL(timeout()),
            this,                   SLOT(onAutoScroll()));
            
    _auto_scroll_timer.setSingleShot(false);
            
            
    setContextMenuPolicy(Qt::PreventContextMenu);
    
	//
	// Build rubber bands
	//

	_connection_rubber_band = new EdLevelScriptConnectionRubberBand();
	_connection_rubber_band->setEndpoints(QPoint(400.0F,500.0F),QPoint(700.0F,300.0F), false);
	_connection_rubber_band->setParent(this);
	_connection_rubber_band->hide();

	_selection_rubber_band = new EdLevelScriptSelectionRubberBand();
	_selection_rubber_band->setEndpoints(QPoint(100.0F,300.0F),QPoint(300.0F,600.0F));
	_selection_rubber_band->setParent(this);
	_selection_rubber_band->hide();

	// Drag mode
	setDragMode(NoDrag);
    setAcceptDrops(true);
    
	//setDragMode(ScrollHandDrag);
    
    setAttribute(Qt::WA_TranslucentBackground, false);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

	readjustSceneRect();
}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::onScriptAlign (void)
{
    emit doCommand("AlignNodes");
}

void EdLevelScriptWindow::onScriptToggleValues (void)
{
    if (MoreStrings::cast_from_string<DTboolean>(Globals::global("ED_SHOW_VALUES")))
        emit doCommand("SetGlobal ED_SHOW_VALUES 0 PERSISTENT");
    else
        emit doCommand("SetGlobal ED_SHOW_VALUES 1 PERSISTENT");
}

bool EdLevelScriptWindow::onScriptAddToLibrary (void)
{
    std::string name, description;

    // Library dialog
    EdLevelLibraryDialog dialog(this, name, description);
    int result = dialog.exec();
    if (result != QDialog::Accepted) 
        return false;
    
    QFileDialog fileDialog(this, "Choose object file to save");
    fileDialog.setDirectory(FilePath("{ED_LIBRARY_PATH}").full_path().c_str());
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setNameFilter("Object files (*.lobj)");
    fileDialog.setDefaultSuffix("lobj");
    result = fileDialog.exec();
    if (!result)
        return false;
        
    QString filename = fileDialog.selectedFiles().first();							   
    if (filename.isEmpty())
        return false;
        
    emit doCommand( QString("Export \"") + filename + "\" \"" + name.c_str() + "\" \"" + description.c_str() + "\"" );
        
    return true;
}

void EdLevelScriptWindow::onScriptColor (void)
{
    QColor color = QColorDialog::getColor(QColor(150,150,150,255), this);
    if (color.isValid()) {	
        emit doCommand( QString("ColorNodes ")  + MoreStrings::cast_to_string(color.redF()).c_str() + " "
                                                + MoreStrings::cast_to_string(color.greenF()).c_str() + " "
                                                + MoreStrings::cast_to_string(color.blueF()).c_str() );
    }
}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::dragEnterEvent (QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("DT3/Group"))
        event->acceptProposedAction();
}

void EdLevelScriptWindow::dragMoveEvent (QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("DT3/Group"))
        event->acceptProposedAction();
}

void EdLevelScriptWindow::dropEvent (QDropEvent *event)
{
    emit doUndoBlock();

    QByteArray data = event->mimeData()->data("DT3/Group");
    
    std::shared_ptr<ArchiveBinaryBufferReader> reader = ArchiveBinaryBufferReader::create();
    reader->stream().set_buffer( data.data(), data.length() );
    
    std::shared_ptr<Group> group = checked_cast<Group>(ArchiveObjectQueue::queue_in_tree(reader));
    if (!group)
        return;
        
    QPointF mouse = mapToScene (event->pos());
    group->set_nodes_center(Vector2(mouse.x(), mouse.y()));
        
    const std::list<std::shared_ptr<WorldNode>> &items_world = group->nodes();
        
    // Add nodes to world with an offset
    FOR_EACH (i,items_world) {
        _document->world()->add_node_unique_name(*i);
    }
            
    _document->world()->add_group(group);
    
    event->acceptProposedAction();
    
    onRefreshScript();
}

//==============================================================================
//==============================================================================

EdLevelScriptNodeStandard* EdLevelScriptWindow::getSceneObjectForNode (std::shared_ptr<PlugNode> node)
{
    // Check if node is a world node
    std::shared_ptr<WorldNode> world_node = checked_cast<WorldNode>(node);
    if (!world_node) {
    
        // Check if it is a component of a world node
        std::shared_ptr<ComponentBase> component = checked_cast<ComponentBase>(node);
        if (!component)
            return NULL;

        if (!component->owner())
            return NULL;
            
        world_node = checked_cast<WorldNode>(component->owner()->shared_from_this());
        if (!world_node)
            return NULL;
    }
        
    auto i = std::find(_node_cache.begin(), _node_cache.end(), NodeCache(world_node));
    if (i == _node_cache.end())
        return NULL;
        
    return i->_scene_object;
}

EdLevelGroup* EdLevelScriptWindow::getSceneObjectForGroup (std::shared_ptr<Group> group)
{
    auto i = std::find(_group_cache.begin(), _group_cache.end(), GroupCache(group));
    if (i == _group_cache.end())
        return NULL;
        
    return i->_scene_object;
}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::onRefreshScript()
{
    // Sync node parameters
    FOR_EACH (i2, _node_cache) {
    
        // Positions
        Vector3 pos = i2->_node->node_position();
        i2->_scene_object->setPos(QPoint(pos.x,pos.y));
    }

    // Sync plug value
    FOR_EACH (i3, _plug_cache) {
        i3->_scene_object->update();
    }
    
    // Sync groups
    FOR_EACH (i4, _group_cache) {
        adjustGroupBounds(i4->_group);
    }
    
    readjustSceneRect();
    update();
}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::onSelectionChanged (const std::list<std::shared_ptr<PlugNode>> &selection_list)
{
    // Don't clear the selection if it was sent from this window because node connections can be selected too
    // and aren't in the selection_list
    if (sender() == this)
        return;
        
    blockSignals(true);
    _scene.blockSignals(true);
    
    QList<QGraphicsItem *> scene_items = _scene.items();

    // Reselect all items
    _scene.clearSelection();
    
    FOR_EACH (i,scene_items) {
        EdLevelScriptNodeStandard *n = qgraphicsitem_cast<EdLevelScriptNodeStandard*>(*i);
        if (n && std::find(selection_list.begin(), selection_list.end(), n->getNode()) != selection_list.end()) {
            (**i).setSelected(true);
            
            // Recenter
            ensureVisible(*i, 0, 0);
        }
        
        EdLevelGroup *g = qgraphicsitem_cast<EdLevelGroup*>(*i);
        if (g && std::find(selection_list.begin(), selection_list.end(), g->getGroup()) != selection_list.end()) {
            (**i).setSelected(true);
            
            // Recenter
            ensureVisible(*i, 0, 0);
        }
        
    }

    blockSignals(false);
    _scene.blockSignals(false);
}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::onNodeContextMenu (std::shared_ptr<WorldNode> node, const QPointF &global_pos)
{
    emit doNodeContextMenu(node, global_pos);
}

void EdLevelScriptWindow::onComponentContextMenu (std::shared_ptr<ComponentBase> component, const QPointF &global_pos)
{
    emit doComponentContextMenu (component, global_pos);
}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::drawBackground(QPainter *painter, const QRectF &rect)
{
	painter->setRenderHint(QPainter::Antialiasing, false);
	
    painter->setPen(Qt::NoPen);
	painter->setBrush(QBrush(QColor(100,100,100,255)));
	painter->drawRect(rect);

	QRectF intersected = rect.intersected(sceneRect());

	const float X_GRID_SIZE = 10.0F;
	const float Y_GRID_SIZE = 10.0F;

	int x_start_num = std::floor(intersected.left() / X_GRID_SIZE);
	int y_start_num = std::floor(intersected.top() / Y_GRID_SIZE);

	int x_start = x_start_num * (int) X_GRID_SIZE;
	int y_start = y_start_num * (int) Y_GRID_SIZE;

    painter->setPen(QPen(QColor(90,90,90,255)));
	painter->setBrush(Qt::NoBrush);

	while (x_start <= intersected.right()) {
		if (x_start_num % 10)	painter->setPen(QPen(QColor(90,90,90,255)));
		else					painter->setPen(QPen(QColor(75,75,75,255)));

		painter->drawLine(  x_start, 
                            (int) intersected.top(), 
                            x_start, 
                            (int) intersected.bottom());
                            
		x_start += (int) X_GRID_SIZE;
		x_start_num += 1;
	}

	while (y_start <= intersected.bottom()) {
		if (y_start_num % 10)	painter->setPen(QPen(QColor(90,90,90,255)));
		else					painter->setPen(QPen(QColor(75,75,75,255)));

		painter->drawLine(  (int) intersected.left(), 
                            y_start, 
                            (int) intersected.right(), 
                            y_start);
                            
		y_start += (int) Y_GRID_SIZE;
		y_start_num += 1;
	}
}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::readjustSceneRect	(void)
{
	//setSceneRect(QRectF(-1000.0F,-1000.0F,1000.0F,1000.0F));

	QRectF scene_rect = _scene.itemsBoundingRect();
	scene_rect = scene_rect.adjusted (-1000.0F, -1000.0F, 1000.0F, 1000.0F);
    scene_rect = scene_rect.united( QRect(-2000,-2000,4000,4000) );

	setSceneRect(scene_rect);
}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::setMode (Mode mode)
{
	switch (_mode) {
		case MODE_NONE:													break;
		case MODE_DRAG_SELECTING:	_selection_rubber_band->hide();		break;
		case MODE_CLICKING:												break;
		case MODE_DRAGGING:												break;
		case MODE_CONNECTING:		_connection_rubber_band->hide();	break;
		case MODE_PANNING:												break;
        default:                                                        break;
	};

	switch (mode) {
		case MODE_NONE:													break;
		case MODE_DRAG_SELECTING:	_selection_rubber_band->show();		break;
		case MODE_CLICKING:												break;
		case MODE_DRAGGING:												break;
		case MODE_CONNECTING:		_connection_rubber_band->show();	break;
		case MODE_PANNING:												break;
        default:                                                        break;
	};

	_mode = mode;
}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::mouseDoubleClickEvent   (QMouseEvent *event)
{
    event->accept();
}

void EdLevelScriptWindow::mousePressEvent (QMouseEvent *event)
{
    emit doUndoBlock();

	_start_point = _last_point = _end_point = event->pos();
    _start_point_scene = _last_point_scene = _end_point_scene = mapToScene(event->pos());

    _auto_scroll_x = 0;
    _auto_scroll_y = 0;
    _auto_scroll_timer.start(AUTO_SCROLL_TIMER);

	// Left click. Either select an item or start drawing a rectangle
	if (event->buttons() == Qt::LeftButton && !(event->modifiers() & Qt::ALT)) {
        
        // Check for internal node actions
        QGraphicsItem *item = itemAt (_start_point);

        // Check for node click
        EdLevelScriptNodeStandard *node = qgraphicsitem_cast<EdLevelScriptNodeStandard*>(item);
        EdLevelGroup *group = qgraphicsitem_cast<EdLevelGroup*>(item);

        if (node) {
        
            // Immediately select if node will handle click
            if (node->checkClick( _start_point_scene, QPointF(event->globalPos()) )) {
                _scene.clearSelection ();
                item->setSelected(true);     
                
                node->handleClick( _start_point_scene, QPointF(event->globalPos()));
                setMode(MODE_NONE);
            } else {
                setMode(MODE_CLICKING);
            }
           
            item->setZValue(++_z);
        } else if (group) {

            // Immediately select if node will handle click
            if (group->checkClick( _start_point_scene, QPointF(event->globalPos()) )) {
                _scene.clearSelection ();
                item->setSelected(true);     
                
                group->handleClick( _start_point_scene, QPointF(event->globalPos()));
                setMode(MODE_NONE);
            }

        } else {
            setMode(MODE_CLICKING);
        }

	// Middle click
	} else if (event->buttons() == Qt::MidButton) {
		setMode(MODE_PANNING);

	// Left and Middle click
	} else if ( (event->buttons() & Qt::LeftButton) && (event->modifiers() & Qt::ALT) ) {
		setMode(MODE_ZOOMING);
	} else if (event->buttons() == Qt::RightButton) {

	}
    
    event->accept();
}

void EdLevelScriptWindow::mouseMoveEvent (QMouseEvent *event)
{
	_end_point = event->pos();
    _end_point_scene = mapToScene(event->pos());

	QPoint dist = _end_point - _start_point;
	QPoint delta = _end_point - _last_point;
    
    // Autoscroll
    if(_end_point.x() < AUTO_SCROLL_MARIGN) {
        _auto_scroll_x = AUTO_SCROLL_MARIGN - _end_point.x();
    } else if(width()-_end_point.x() < AUTO_SCROLL_MARIGN) {
        _auto_scroll_x = -(AUTO_SCROLL_MARIGN - (width()-_end_point.x()));
    } else {
        _auto_scroll_x = 0;
    }
    
    if(_end_point.y() < AUTO_SCROLL_MARIGN) {
        _auto_scroll_y = AUTO_SCROLL_MARIGN - _end_point.y();
    } else if(height()-_end_point.y() < AUTO_SCROLL_MARIGN) {
        _auto_scroll_y = -(AUTO_SCROLL_MARIGN - (height()-_end_point.y()));
    } else {
        _auto_scroll_y = 0;
    }
    
    
    // Manipulators
	switch (getMode()) {
		case MODE_NONE:
			break;

		case MODE_CLICKING:	
			if (dist.manhattanLength() > 3) {
				
				// Mode switch to dragging
				QGraphicsItem *item = itemAt (_start_point);
                if (item) {
					item->setZValue(++_z);
                }

                // Check for connection click
                _connection_start_node = qgraphicsitem_cast<EdLevelScriptNodeStandard*>(item);
                if (_connection_start_node) {
                
                    QPoint click_location = _connection_start_node->mapFromScene(_start_point_scene).toPoint();

                    bool hit_plug = _connection_start_node->getPlugAtLocation (click_location, _connection_start_location, _connection_start_location_is_input, _connection_start_plug);
                    if (hit_plug && _connection_start_plug) {
                        QPoint connection_start_location = mapFromScene(_connection_start_node->mapToScene(_connection_start_location));
                        _connection_rubber_band->setEndpoints(connection_start_location, _end_point, _connection_start_location_is_input);
                        
                        _connection_rubber_band->show();
                        setMode(MODE_CONNECTING);
                   }
                    
                    bool hit_event = _connection_start_node->getEventAtLocation (click_location, _connection_start_location, _connection_start_location_is_input, _connection_start_event);
                    if (hit_event && _connection_start_event) {
                        QPoint connection_start_location = mapFromScene(_connection_start_node->mapToScene(_connection_start_location));
                        _connection_rubber_band->setEndpoints(connection_start_location, _end_point, _connection_start_location_is_input);
                            
                        _connection_rubber_band->show();
                        setMode(MODE_CONNECTING);
                    }
                    
                    
                }
    
                // Normal manipulation
                if (getMode() != MODE_CONNECTING) {
                    if (item) {
                        if (!item->isSelected()) {
                            if (!(event->modifiers() & Qt::CTRL) && !(event->modifiers() & Qt::SHIFT)) {
                                _scene.clearSelection ();
                            }
                            item->setSelected(true);
                        }

                        setMode(MODE_DRAGGING);

                    } else {
                        setMode(MODE_DRAG_SELECTING);
                        _selection_rubber_band->setEndpoints(mapFromScene(_start_point_scene), _end_point);
                    }
                }
                
            }

			break;

		case MODE_DRAG_SELECTING:	
			_selection_rubber_band->setEndpoints(mapFromScene(_start_point_scene), _end_point);
			break;

		case MODE_DRAGGING:	{
				QPointF delta_scene = mapToScene(_end_point - _last_point) - mapToScene(QPoint(0, 0));
                                
                QList<QGraphicsItem*> selected_items = _scene.selectedItems();

				FOR_EACH (i,selected_items) {
                    EdLevelScriptNodeStandard *node = qgraphicsitem_cast<EdLevelScriptNodeStandard*>(*i);
                    if (node) {
                        // Build a command to move the node to this position
                        Vector3 node_pos = node->getNode()->node_position();
                        QPointF new_position = QPointF(node_pos.x, node_pos.y) + delta_scene;
                        std::string cmd = "MoveNode \"" + node->getNode()->name() + "\" (" + MoreStrings::cast_to_string(new_position.x()) + " " + MoreStrings::cast_to_string(new_position.y()) + " 0.0)";
                                                
                        emit doCommand(cmd.c_str());
                    }
                }

				readjustSceneRect();
			} break;

		case MODE_CONNECTING: {		            
                QPoint connection_start_location = mapFromScene(_connection_start_node->mapToScene(_connection_start_location));
                _connection_rubber_band->setEndpoints(connection_start_location, _end_point, _connection_start_location_is_input);

			} break;

		case MODE_PANNING: {
				QScrollBar *hBar = horizontalScrollBar();
				QScrollBar *vBar = verticalScrollBar();
				hBar->setValue(hBar->value() - delta.x());
				vBar->setValue(vBar->value() - delta.y());
			} break;

		case MODE_ZOOMING: {
				_scale *= 1.0F + (delta.x() + delta.y()) / -100.0F;
				if (_scale < 0.2F)		_scale = 0.2F;
				else if (_scale > 1.5F)	_scale = 1.5F;

				resetMatrix();
				scale(_scale,_scale);
			} break;
	};

	_last_point = _end_point;
	_last_point_scene = _end_point_scene;

    event->accept();
}

void EdLevelScriptWindow::mouseReleaseEvent	(QMouseEvent *event)
{
	_end_point = event->pos();
    _end_point_scene = mapToScene(event->pos());
    
    _auto_scroll_timer.stop();

	switch (getMode()) {
		case MODE_NONE:
			break;

		case MODE_CLICKING:	{
				
				QGraphicsItem *item = itemAt (_start_point);
				if (item) {
					item->setZValue(++_z);

					if ((event->modifiers() & Qt::CTRL) || (event->modifiers() & Qt::SHIFT)) {
						if (item->isSelected())	item->setSelected(false);
						else					item->setSelected(true);
					} else {
						_scene.clearSelection ();
						item->setSelected(true);
					}
				} else {
					_scene.clearSelection ();
				}
			} break;

		case MODE_DRAG_SELECTING: {	
				// Build a selection rectangle
				QRect selection_rect = QRect(   QPoint( (int) _start_point_scene.x(), (int) _start_point_scene.y()), 
                                                QPoint( (int) _end_point_scene.x(), (int) _end_point_scene.y()) 
                                            ).normalized();

				QList<QGraphicsItem*> selected_items = _scene.selectedItems();

				QPainterPath pp;
				pp.addRect(selection_rect);
				_scene.setSelectionArea(pp);

				// Reselect all previous items if adding to the selection
				if ((event->modifiers() & Qt::CTRL) || (event->modifiers() & Qt::SHIFT)) {
                    FOR_EACH (i,selected_items) {
						(**i).setSelected(true);
					}
				}

			} break;

		case MODE_DRAGGING: {
                readjustSceneRect();
			} break;

		case MODE_CONNECTING: {
        
				// Mode switch to connecting
				QGraphicsItem *item = itemAt (_end_point);

                // Check for connection click
                EdLevelScriptNodeStandard *connection_end_node = qgraphicsitem_cast<EdLevelScriptNodeStandard*>(item);
                if (connection_end_node) {
                
                    QPoint click_location = connection_end_node->mapFromScene(_end_point_scene).toPoint();
                    
                    QPoint      connection_end_location;
                    bool        connection_end_location_is_input;
                    PlugBase    *connection_end_plug;
                    Event   *connection_end_event;

                    bool hit_plug = connection_end_node->getPlugAtLocation (click_location, connection_end_location, connection_end_location_is_input, connection_end_plug);
                    if (hit_plug && _connection_start_plug) {
                                                
                        std::string cmd;                        
                        if (!_connection_start_location_is_input && connection_end_location_is_input)
                            cmd = "ConnectPlugs \"" + _connection_start_plug->full_name() + "\" \"" + connection_end_plug->full_name() + "\"";
                        else if (_connection_start_location_is_input && !connection_end_location_is_input)
                            cmd = "ConnectPlugs \"" + connection_end_plug->full_name() + "\" \"" + _connection_start_plug->full_name() + "\"";
                            
                        emit doCommand(cmd.c_str());
                   }
                    
                    bool hit_event = connection_end_node->getEventAtLocation (click_location, connection_end_location, connection_end_location_is_input, connection_end_event);
                    if (hit_event && _connection_start_event) {
                        
                        std::string cmd;
                        if (!_connection_start_location_is_input && connection_end_location_is_input)
                            cmd = "ConnectEvents \"" + _connection_start_event->full_name() + "\" \"" + connection_end_event->full_name() + "\"";
                        else if (_connection_start_location_is_input && !connection_end_location_is_input)
                            cmd = "ConnectEvents \"" + connection_end_event->full_name() + "\" \"" + _connection_start_event->full_name() + "\"";

                        emit doCommand(cmd.c_str());
                    }
                    
                    
                }

			} break;

		case MODE_PANNING:												
			break;
            
        default:
            break;
	}

	setMode(MODE_NONE);

    event->accept();
}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::onAutoScroll (void)
{
    if (_auto_scroll_x == 0 && _auto_scroll_y == 0)
        return;
        
    QScrollBar *hBar = horizontalScrollBar();
    QScrollBar *vBar = verticalScrollBar();
    hBar->setValue(hBar->value() - _auto_scroll_x);
    vBar->setValue(vBar->value() - _auto_scroll_y);
    
    
	switch (getMode()) {
		case MODE_DRAG_SELECTING:	
			_selection_rubber_band->setEndpoints(mapFromScene(_start_point_scene), _end_point);
			break;

		case MODE_DRAGGING:	{
				QPointF delta_scene = mapToScene(QPoint(-_auto_scroll_x, -_auto_scroll_y)) - mapToScene(QPoint(0, 0));
                                
                QList<QGraphicsItem*> selected_items = _scene.selectedItems();

                FOR_EACH (i,selected_items) {
                    EdLevelScriptNodeStandard *node = qgraphicsitem_cast<EdLevelScriptNodeStandard*>(*i);
                    if (node) {
                        // Build a command to move the node to this position
                        Vector3 node_pos = node->getNode()->node_position();
                        QPointF new_position = QPointF(node_pos.x, node_pos.y) + delta_scene;
                        std::string cmd = "MoveNode \"" + node->getNode()->name() + "\" (" + MoreStrings::cast_to_string(new_position.x()) + " " + MoreStrings::cast_to_string(new_position.y()) + " 0.0)";
                                                
                        emit doCommand(cmd.c_str());
                    }
                }

				readjustSceneRect();
			} break;

		case MODE_CONNECTING: {		            
                QPoint connection_start_location = mapFromScene(_connection_start_node->mapToScene(_connection_start_location));
                _connection_rubber_band->setEndpoints(connection_start_location, _end_point, _connection_start_location_is_input);

			} break;
        
        default:
            break;
    }
    
    
}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::keyPressEvent (QKeyEvent *event)
{
    emit doUndoBlock();

    int key = event->key();

    if (event->matches(QKeySequence::Delete) || key == 0x1000003) {
        QList<QGraphicsItem *> scene_items = _scene.items();
        
        // Build a list of selected items and then clear the selection because the items will be deleted
        QList<QGraphicsItem *> selected_items = _scene.selectedItems();
        
        // Delete connections first
        for (auto i = selected_items.begin(); i != selected_items.end(); ) {
            if (*i == NULL)
                continue;

            EdLevelScriptPlugConnection *connection_plugs = qgraphicsitem_cast<EdLevelScriptPlugConnection*>(*i);
            if (connection_plugs) {                
                std::string cmd = "DisconnectPlugs \"" + connection_plugs->plug1()->full_name() + "\" \"" + connection_plugs->plug2()->full_name() + "\"";
                emit doCommand(cmd.c_str());
            
                i = selected_items.erase(i);
                continue;
            }
            
            EdLevelScriptEventConnection *connection_events = qgraphicsitem_cast<EdLevelScriptEventConnection*>(*i);
            if (connection_events) {
                std::string cmd = "DisconnectEvents \"" + connection_events->event1()->full_name() + "\" \"" + connection_events->event2()->full_name() + "\"";
                emit doCommand(cmd.c_str());
            
                i = selected_items.erase(i);
                continue;
            }
            
            ++i;
        }
        
        // Delete nodes. Items can disappear if they are parented which screws up our list.
        while (_scene.selectedItems().size() > 0) {
            EdLevelScriptNodeStandard *n = qgraphicsitem_cast<EdLevelScriptNodeStandard*>(_scene.selectedItems().front());
            EdLevelGroup *g = qgraphicsitem_cast<EdLevelGroup*>(_scene.selectedItems().front());
            
            if (n) {
                std::string nodename = n->getNode()->name();
                n->setSelected(false);

                std::string cmd = "Remove \"" + nodename + "\"";
                emit doCommand(cmd.c_str());
            }
            
            if (g) {
                std::string groupname = g->getGroup()->name();
                g->setSelected(false);

                std::string cmd = "Ungroup \"" + groupname + "\"";
                emit doCommand(cmd.c_str());
                
            }
        }

        _scene.clearSelection();

        event->accept();
    }

}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::selectionChanged ()
{
    std::list<std::shared_ptr<PlugNode>> selection_list;
    QList<QGraphicsItem *> selected_items = _scene.selectedItems();

    FOR_EACH (i,selected_items) {
        EdLevelScriptNodeStandard *node = qgraphicsitem_cast<EdLevelScriptNodeStandard*>(*i);
        EdLevelGroup *group = qgraphicsitem_cast<EdLevelGroup*>(*i);
        
        if (node) {
            std::shared_ptr<PlugNode> n = checked_static_cast<PlugNode>(node->getNode());
            ASSERT(n);
            selection_list.push_back(n);
        }

        if (group) {
            std::shared_ptr<Group> g = group->getGroup();
            ASSERT(g);
            selection_list.push_back(g);
        }
        
    }
    
    emit doSelectionChanged(selection_list);
}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::adjustGroupBounds (std::shared_ptr<Group> group)
{
    if (!group)
        return;

    EdLevelGroup *g = getSceneObjectForGroup(group);
    if (!g)
        return;
    
    // Find bounds of grouped items
    const std::list<std::shared_ptr<WorldNode>>& nodes = group->nodes();
    
    QRectF bounds;
    
    for (auto i : nodes) {
        auto j = std::find(_node_cache.begin(), _node_cache.end(), NodeCache(i));
        if (j != _node_cache.end()) {
            bounds = bounds.united( j->_scene_object->sceneBoundingRect() );
        }
    }

    bounds.adjust(-30,-30,30,30);
    
    g->setBoundingRect(bounds);
}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::syncConnections (std::shared_ptr<PlugNode> node)
{
    // Since a node can be added that already has a bunch of plug connections, we have to iterate
    // through them and be sure that they are added.
    
    // Plugs first
    for (PlugIter p_iter(node.get()); p_iter; ++p_iter) {
        PlugBase* plug = p_iter();
    
        onConnectPlug (plug->incoming_connection(), plug);

        const std::vector<PlugBase*> &connections = plug->outgoing_connections();
        for (DTuint k = 0; k < connections.size(); ++k) {
            onConnectPlug (plug, connections[k]);
        }
	}
    
    // Events second
    for (EventIter e_iter(node.get()); e_iter; ++e_iter) {
        Event* event = e_iter();
        
        const std::vector<Event*> &connections_in = event->incoming_connections();
        for (DTuint k = 0; k < connections_in.size(); ++k) {
            onConnectEvent (connections_in[k], event);
        }
    
        const std::vector<Event*> &connections_out = event->outgoing_connections();
        for (DTuint k = 0; k < connections_out.size(); ++k) {
            onConnectEvent (event, connections_out[k]);
        }
	}
}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::onAddNode (WorldNode *node_raw)
{
    std::shared_ptr<WorldNode> node = checked_cast<WorldNode>(node_raw->shared_from_this());

    // Build an item
    EdLevelScriptNodeStandard *item = new EdLevelScriptNodeStandard(node);
    
    Vector3 position = node->node_position();
    item->setPos(position.x, position.y);
    
    _scene.addItem(item);
    
    // Cache the item
    NodeCache c(node);
    c._scene_object = item;
    _node_cache.push_back(c);
    
    item->setZValue(++_z);
        
    // Set position to center of window
    QPointF center = mapToScene(rect().center()) - item->boundingRect().center();
    item->setPos(center);
    
    connect(    item,           SIGNAL(doNodeContextMenu(std::shared_ptr<WorldNode>, const QPointF &)),
                this,           SLOT(onNodeContextMenu(std::shared_ptr<WorldNode>, const QPointF &))  );

    connect(    item,           SIGNAL(doComponentContextMenu(std::shared_ptr<ComponentBase>, const QPointF &)),
                this,           SLOT(onComponentContextMenu(std::shared_ptr<ComponentBase>, const QPointF &))  );
                

    item->layoutNode();

    syncConnections(node);
    
    // Do components if necessary
    std::shared_ptr<ObjectBase> base_object = checked_cast<ObjectBase>(node);
    if (base_object) {
        std::list<std::shared_ptr<ComponentBase>> c = base_object->all_components();
        
        FOR_EACH (i,c) {
            syncConnections(*i);
        }
    }
    
    if (node->group()) {
        std::shared_ptr<Group> group = checked_cast<Group>(node->group()->shared_from_this());
        adjustGroupBounds (group);
    }
}

void EdLevelScriptWindow::onRemoveNode (WorldNode *node_raw)
{
    std::shared_ptr<WorldNode> node = checked_cast<WorldNode>(node_raw->shared_from_this());

    NodeCache c(node);

    auto i = std::find(_node_cache.begin(), _node_cache.end(), c);
    if (i != _node_cache.end()) {
        _scene.removeItem(i->_scene_object);
        delete i->_scene_object;  
              
        _node_cache.erase(i);
    }
}

void EdLevelScriptWindow::onReparentNode (WorldNode *node, WorldNode *old_parent, WorldNode *new_parent)
{

}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::onAddGroup (Group *group_raw)
{
    std::shared_ptr<Group> group = checked_cast<Group>(group_raw->shared_from_this());

    EdLevelGroup *item = new EdLevelGroup(group);

    _scene.addItem(item);
    item->setZValue(-1);

    // Cache the item
    GroupCache c(group);
    c._scene_object = item;
    _group_cache.push_back(c);

    adjustGroupBounds (group);
}

void EdLevelScriptWindow::onRemoveGroup (Group *group_raw)
{
    std::shared_ptr<Group> group = checked_cast<Group>(group_raw->shared_from_this());

    GroupCache c(group);

    auto i = std::find(_group_cache.begin(), _group_cache.end(), c);
    if (i != _group_cache.end()) {
        _scene.removeItem(i->_scene_object);
        delete i->_scene_object;  
              
        _group_cache.erase(i);
    }
}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::onAddComponent (ComponentBase *component_raw)
{
    std::shared_ptr<ComponentBase> component = checked_cast<ComponentBase>(component_raw->shared_from_this());

    std::shared_ptr<ObjectBase> node = checked_cast<ObjectBase>(component->owner()->shared_from_this());

    NodeCache c(node);

    auto i = std::find(_node_cache.begin(), _node_cache.end(), c);
    if (i != _node_cache.end()) {
        i->_scene_object->layoutNode();
    }

    syncConnections(component);
}

void EdLevelScriptWindow::onRemoveComponent (ComponentBase *component_raw)
{
    std::shared_ptr<ComponentBase> component = checked_cast<ComponentBase>(component_raw->shared_from_this());

    std::shared_ptr<ObjectBase> node = checked_cast<ObjectBase>(component->owner()->shared_from_this());

    NodeCache c(node);

    auto i = std::find(_node_cache.begin(), _node_cache.end(), c);
    if (i != _node_cache.end()) {
        i->_scene_object->layoutNode();
    }

}

//==============================================================================
//==============================================================================

void EdLevelScriptWindow::onConnectPlug (PlugBase *outgoing, PlugBase *incoming)
{
    if (outgoing == NULL || incoming == NULL)
        return;
        
    if (outgoing->is_no_draw() || incoming->is_no_draw())
        return;

    ConnectionPlugCache c(outgoing, incoming);
    
    // Make sure we aren't duplicating connections
    auto i = std::find(_plug_cache.begin(), _plug_cache.end(), c);
    if (i != _plug_cache.end())
        return;

    ASSERT(outgoing->owner());
    ASSERT(incoming->owner());

    EdLevelScriptNodeStandard* outgoing_scene_item = getSceneObjectForNode (checked_static_cast<PlugNode>(outgoing->owner()->shared_from_this()));
    EdLevelScriptNodeStandard* incoming_scene_item = getSceneObjectForNode (checked_static_cast<PlugNode>(incoming->owner()->shared_from_this()));

    if (!outgoing_scene_item || !incoming_scene_item)
        return;

    EdLevelScriptPlugConnection *item = new EdLevelScriptPlugConnection(    outgoing_scene_item, outgoing,
                                                                            incoming_scene_item, incoming );
    item->readjustBounds();
    _scene.addItem(item);
    
    c._scene_object = item;
    _plug_cache.push_back(c);
    
}

void EdLevelScriptWindow::onDisconnectPlug (PlugBase *outgoing, PlugBase *incoming)
{
    ConnectionPlugCache c(outgoing, incoming);

    auto i = std::find(_plug_cache.begin(), _plug_cache.end(), c);
    if (i != _plug_cache.end()) {
        i->_scene_object->disconnect();
        _scene.removeItem(i->_scene_object);
        delete i->_scene_object;

        _plug_cache.erase(i);
    }
}

void EdLevelScriptWindow::onConnectEvent (Event *outgoing, Event *incoming)
{
    if (outgoing == NULL || incoming == NULL)
        return;

    if (outgoing->is_no_draw() || incoming->is_no_draw())
        return;

    ConnectionEventCache c(outgoing, incoming);
    
    // Make sure we aren't duplicating connections
    auto i = std::find(_event_cache.begin(), _event_cache.end(), c);
    if (i != _event_cache.end())
        return;
        
    EdLevelScriptNodeStandard* outgoing_scene_item = getSceneObjectForNode (checked_static_cast<PlugNode>(outgoing->owner()->shared_from_this()));
    EdLevelScriptNodeStandard* incoming_scene_item = getSceneObjectForNode (checked_static_cast<PlugNode>(incoming->owner()->shared_from_this()));

    if (!outgoing_scene_item || !incoming_scene_item)
        return;

    EdLevelScriptEventConnection *item = new EdLevelScriptEventConnection(  outgoing_scene_item, outgoing,
                                                                            incoming_scene_item, incoming );
    item->readjustBounds();
    _scene.addItem(item);
    
    c._scene_object = item;
    _event_cache.push_back(c);
}

void EdLevelScriptWindow::onDisconnectEvent (Event *outgoing, Event *incoming)
{
    ConnectionEventCache c(outgoing, incoming);

    auto i = std::find(_event_cache.begin(), _event_cache.end(), c);
    if (i != _event_cache.end()) {
        i->_scene_object->disconnect();
        _scene.removeItem(i->_scene_object);
        delete i->_scene_object;

        _event_cache.erase(i);
    }
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelScriptWindow.cpp"

