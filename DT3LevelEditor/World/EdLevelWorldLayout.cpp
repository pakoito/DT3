//==============================================================================
///	
///	File: EdLevelWorldLayout.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
// Qt include
// Engine includes

#include "EdLevelWorldLayout.hpp"
#include <QtWidgets/QWidget>

//==============================================================================
//==============================================================================

//using namespace DT3;

//==============================================================================
//==============================================================================
EdLevelWorldLayout::EdLevelWorldLayout (QWidget *parent)
    :   QLayout(parent) 
{

}

EdLevelWorldLayout::EdLevelWorldLayout (void)
    : QLayout() 
{

}

EdLevelWorldLayout::~EdLevelWorldLayout (void)
{
    while (_list.count()) {
        delete takeAt(0);
    }
}

//==============================================================================
//==============================================================================

void EdLevelWorldLayout::addItem (QLayoutItem *item)
{
     _list.append(item);
}

QSize EdLevelWorldLayout::sizeHint (void) const
{
     QSize s(0,0);
     
     int n = _list.count();         
     int i = 0;
     while (i < n) {
         QLayoutItem *o = _list.at(i);
         s = s.expandedTo(o->sizeHint());
         ++i;
     }
     return s + n*QSize(spacing(), spacing());
}

QSize EdLevelWorldLayout::minimumSize (void) const
{
     QSize s(0,0);
     int n = _list.count();
     int i = 0;
     while (i < n) {
         QLayoutItem *o = _list.at(i);
         s = s.expandedTo(o->minimumSize());
         ++i;
     }
     return s + n*QSize(spacing(), spacing());
}

QLayoutItem* EdLevelWorldLayout::itemAt (int i) const
{
     return _list.value(i);
}

QLayoutItem* EdLevelWorldLayout::takeAt (int i)
{
     return i >= 0 && i < _list.size() ? _list.takeAt(i) : 0;
}

void EdLevelWorldLayout::setGeometry (const QRect &rect)
{
    if (_list.count() != 2)
        return;

    // Set the first item (toolbar) to the top
    QLayoutItem *tb = _list.at(0);
    QRect tb_rect = QRect(0,0,rect.width(), tb->minimumSize().height());
    
    if (tb_rect != tb->geometry())
        tb->setGeometry(tb_rect);
    
    // Window
    QLayoutItem *window = _list.at(1);
    QSize window_size = window->widget()->sizeHint();
    QRect sub_rect = QRect(0,tb->minimumSize().height(),rect.width(),rect.height()-tb->minimumSize().height());

    QRect window_rect;
    if (window_size.width() == 0 && window_size.height() == 0) {
        // Full screen
        window_rect = sub_rect;
    } else {
        float aspect = (float) window_size.width() / (float) window_size.height();
    
        // Move rectangle to center
        window_rect = QRect(0,0,window_size.width(),window_size.height());
        window_rect.moveCenter(sub_rect.center());
        
        // Scale so top fits within
        if (window_rect.top() < sub_rect.top()) {
            int adjustment = window_rect.top() - sub_rect.top();
            window_rect.adjust(-adjustment*aspect, -adjustment, adjustment*aspect, adjustment);
        }
        
        // Scale so left fits within
        if (window_rect.left() < sub_rect.left()) {
            int adjustment = window_rect.left() - sub_rect.left();
            window_rect.adjust(-adjustment, -adjustment/aspect, adjustment, adjustment/aspect);
        }
        
    }
    
    if (window_rect != window->geometry())
        window->setGeometry(window_rect);

}

//==============================================================================
//==============================================================================

//#include "moc_EdLevelWorldLayout.cpp"

