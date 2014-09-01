//==============================================================================
///	
///	File: EdLevelPropertyBase.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

// Editor include
#include "EdLevelPropertyBase.hpp"
#include "EdLevelLineEdit.hpp"

// Qt include
#include <QtCore/QEvent>
#include <QtGui/QKeyEvent>

// Engine includes
#include "DT3Core/Types/Math/ExpressionParser.hpp"
#include "DT3Core/Types/Utility/MoreStrings.hpp"

//==============================================================================
//==============================================================================

using namespace DT3;

//==============================================================================
//==============================================================================

std::string  EdLevelPropertyBase::parseParam(const std::string &s)
{
    if (s.size() == 0)
        return s;
        
    if (s[0] != '=')
        return s;
        
    ExpressionParser expr;
    
    // Try to parse expression
    if (!expr.parse(s))
        return s;
        
    DTfloat result;
    
    // Try to evaluate expression
    if (!expr.eval(result))
        return s;
        
    return MoreStrings::cast_to_string(result);
}

void EdLevelPropertyBase::buildStatusButtons (void)
{
    _key = new QToolButton(this);
    _key->setMaximumWidth(17);
    _key->setMinimumWidth(17);
    _key->setFocusPolicy(Qt::NoFocus);
    _key->setStyleSheet("QToolButton { border: none }");

    _has_input = new QToolButton(this);
    _has_input->setMaximumWidth(17);
    _has_input->setMinimumWidth(17);
    _has_input->setFocusPolicy(Qt::NoFocus);
    _has_input->setStyleSheet("QToolButton { border: none }");

    _has_output = new QToolButton(this);
    _has_output->setMaximumWidth(17);
    _has_output->setMinimumWidth(17);
    _has_output->setFocusPolicy(Qt::NoFocus);
    _has_output->setStyleSheet("QToolButton { border: none }");
}

//==============================================================================
//==============================================================================

#include "moc_EdLevelPropertyBase.cpp"

