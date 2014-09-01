//==============================================================================
///	
///	File: EdLevelConsoleWindow.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelConsoleWindow.hpp"
#include "EdLevelDocument.hpp"

// Qt include
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QScrollBar>
#include <QtGui/QFont>

// Engine includes
#include "DT3Core/System/SystemCallbacks.hpp"

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

EdLevelConsoleWindow::EdLevelConsoleWindow(QWidget *parent, QToolBar *toolbar, EdLevelDocument *document)
    :   QWidget                 (parent)
{
    _document = document;
    _toolbar = toolbar;
    
    _console = new QPlainTextEdit(this);
    _console->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    _console->setFont(QFont("Monaco", 10));
    _console->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    _console->setReadOnly(true);
    _console->setWordWrapMode(QTextOption::NoWrap);
    
    _command_widget = new EdLevelLineEdit(this);
    
    // Connect "Enter" pressed
    connect(_command_widget, SIGNAL(returnPressed()), this, SLOT(onCommandEntered()));
    connect(this, SIGNAL(doWriteLog(QString)), this, SLOT(onWriteLog(QString)));

    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->setHorizontalSpacing(1);
    layout->setVerticalSpacing(1);
    layout->addWidget(_console);
    layout->addWidget(_command_widget);
    
    setLayout(layout);

    SystemCallbacks::error_cb().add(make_callback(this, &EdLevelConsoleWindow::logError));
    SystemCallbacks::message_cb().add(make_callback(this, &EdLevelConsoleWindow::logMessage));
    SystemCallbacks::debug_cb().add(make_callback(this, &EdLevelConsoleWindow::logDebug));
}

EdLevelConsoleWindow::~EdLevelConsoleWindow(void)
{
    SystemCallbacks::error_cb().remove(make_callback(this, &EdLevelConsoleWindow::logError));
    SystemCallbacks::message_cb().remove(make_callback(this, &EdLevelConsoleWindow::logMessage));
    SystemCallbacks::debug_cb().remove(make_callback(this, &EdLevelConsoleWindow::logDebug));
}

//==============================================================================
//==============================================================================

void EdLevelConsoleWindow::onCommandEntered()
{
    emit doCommand(_command_widget->text());
    _command_widget->setText("");
}

//==============================================================================
//==============================================================================

void EdLevelConsoleWindow::append (const QString &s)
{
    emit doWriteLog(s);
}

//==============================================================================
//==============================================================================

void EdLevelConsoleWindow::logError(const std::string &error)
{
    emit doWriteLog((std::string("ERR: ") + error).c_str());
}

void EdLevelConsoleWindow::logMessage(const std::string &message)
{
    emit doWriteLog((std::string("MSG: ") + message).c_str());
}

void EdLevelConsoleWindow::logDebug(const std::string &debug)
{
    emit doWriteLog((std::string("DBG: ") + debug).c_str());
}

//==============================================================================
//==============================================================================

void EdLevelConsoleWindow::onWriteLog (QString msg)
{    
    _console->appendPlainText(msg);
    _console->verticalScrollBar()->setValue(_console->verticalScrollBar()->maximum());
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelConsoleWindow.cpp"

