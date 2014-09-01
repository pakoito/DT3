//==============================================================================
///	
///	File: EdSpriteAnimations.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "EdSpriteAnimations.hpp"
#include "KeyedSpriteAnimation.hpp"

#include "ArchiveData.hpp"
#include "ConsoleStream.hpp"

using namespace DT2;

//==============================================================================
//==============================================================================

EdSpriteAnimations::EdSpriteAnimations(EdSpriteMainWindow *main_window)
    :   QListWidget     (main_window)
{
	_main_window = main_window;

    setMinimumWidth(200);
    setMaximumWidth(200);
    
    connect(	this,                           SIGNAL(itemChanged(QListWidgetItem *)),
				this,                           SLOT(onItemChanged(QListWidgetItem *))	);

    connect(	this,                           SIGNAL(itemClicked(QListWidgetItem *)),
				this,                           SLOT(onItemClicked(QListWidgetItem *))	);

}

//==============================================================================
//==============================================================================

void EdSpriteAnimations::buildList (void)
{
    clearList();

    KeyedSpriteAnimationResource* animation = _main_window->getAnimationResource ();
    if (!animation)
        return;
        
    KeyedSpriteAnimation* selected = _main_window->getSelectedAnimation();

        
    List<KeyedSpriteAnimation*>& animations = animation->getAnimations ();
    
    FOR_EACH(i, animations) {
        QListWidgetItem *list_item = new QListWidgetItem();
        list_item->setText( (**i).getName().cStr());
        list_item->setFlags (list_item->flags () | Qt::ItemIsEditable);
    
        list_item->setData(Qt::UserRole, qVariantFromValue( (void*) (*i) ));
                
        addItem(list_item);
        
        DTboolean is_selected = (selected == (*i));
        if (is_selected)
            setCurrentItem(list_item);

    }
        
}

//==============================================================================
//==============================================================================

void EdSpriteAnimations::clearList (void)
{
    clear();
}

//==============================================================================
//==============================================================================

void EdSpriteAnimations::onSpriteChanged (void)
{

}

void EdSpriteAnimations::onSelectionChanged (void)
{

}

void EdSpriteAnimations::onAnimationsChanged (void)
{
    buildList();
}

//==============================================================================
//==============================================================================

void EdSpriteAnimations::onItemChanged ( QListWidgetItem * item )
{
    _main_window->pushUndo();

    KeyedSpriteAnimation *animation = (KeyedSpriteAnimation*) item->data(Qt::UserRole).value<void*>();
    animation->setName(item->text().toUtf8().data());
    
    
}

void EdSpriteAnimations::onItemClicked ( QListWidgetItem * item )
{
    KeyedSpriteAnimation *animation = (KeyedSpriteAnimation*) item->data(Qt::UserRole).value<void*>();
    _main_window->setSelectedAnimation(animation);
}

//==============================================================================
//==============================================================================


#include "moc_EdSpriteAnimations.cpp"

