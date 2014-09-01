//==============================================================================
///	
///	File: GUIGridLayout.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/GUI/GUIGridLayout.hpp"
#include "DT3Core/Types/GUI/GUILayoutPolicy.hpp"
#include "DT3Core/Types/GUI/GUIAnimKey.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"
#include "DT3Core/Types/Math/Rectangle.hpp"
#include "DT3Core/Objects/GUIObject.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

GUIGridLayoutItem::GUIGridLayoutItem (void)
{

}
	
GUIGridLayoutItem::GUIGridLayoutItem (const GUIGridLayoutItem &rhs)
{
    _objects = rhs._objects;
    _layouts = rhs._layouts;
}

GUIGridLayoutItem& GUIGridLayoutItem::operator = (const GUIGridLayoutItem &rhs)
{
    _objects = rhs._objects;
    _layouts = rhs._layouts;

	return *this;
}
		
GUIGridLayoutItem::~GUIGridLayoutItem (void)
{

}

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

GUIGridLayout::GUIGridLayout (void)
    :   _num_rows       (0),
        _num_columns    (0),
        _border_pad_h   (0.01F),
        _border_pad_v   (0.01F),
        _border_rect    (0.0F,1.0F,0.0F,1.0F)
{  

}
	
GUIGridLayout::GUIGridLayout (const GUIGridLayout &rhs)
    :   GUILayout       (rhs),
        _widths         (rhs._widths),
        _heights        (rhs._heights),
        _row_policies   (rhs._row_policies),
        _column_policies(rhs._column_policies),
        _items          (rhs._items),
        _num_rows       (rhs._num_rows),
        _num_columns    (rhs._num_columns),
        _border_pad_h   (rhs._border_pad_h),
        _border_pad_v   (rhs._border_pad_v),
        _border_rect    (rhs._border_rect)
{
    
}

GUIGridLayout& GUIGridLayout::operator = (const GUIGridLayout &rhs)
{
	GUILayout::operator = (rhs);

    _widths = rhs._widths;
    _heights = rhs._heights;
    _row_policies = rhs._row_policies;
    _column_policies = rhs._column_policies;
    _items = rhs._items;
    _num_rows = rhs._num_rows;
    _num_columns = rhs._num_columns;
    _border_pad_h = rhs._border_pad_h;
    _border_pad_v = rhs._border_pad_v;
    _border_rect = rhs._border_rect;

	return *this;
}
		
GUIGridLayout::~GUIGridLayout (void)
{
    
}

//==============================================================================
//==============================================================================

void GUIGridLayout::set_rows_and_columns (DTint num_rows, DTint num_cols)
{
    _num_rows = num_rows;
    _num_columns = num_cols;

    _row_policies.resize(num_rows);
    _column_policies.resize(num_cols);
    _items.resize(num_rows * num_cols);
}

void GUIGridLayout::set_column_policy (DTint column, const GUILayoutPolicy &p)
{
    ASSERT(column >= 0);
    ASSERT(column < _num_columns);
    _column_policies[column] = p;
}
        
void GUIGridLayout::set_row_policy (DTint row, const GUILayoutPolicy &p)
{
    ASSERT(row >= 0);
    ASSERT(row < _num_rows);
    _row_policies[row] = p;
}

void GUIGridLayout::add_item (DTint row, DTint column, const std::shared_ptr<GUIObject> &object, GUIGridLayoutItem::ResizeMode resize_mode)
{
    ASSERT(column >= 0);
    ASSERT(column < _num_columns);
    ASSERT(row >= 0);
    ASSERT(row < _num_rows);

    DTuint i = row * _num_columns + column;
    
    GUIGridLayoutItem &item = _items[i];
    
    GUIGridLayoutItem::Object obj_entry;
    obj_entry._object = object;
    obj_entry._resize_mode = resize_mode;
    
    item._objects.push_back(obj_entry);
}

void GUIGridLayout::add_item (DTint row, DTint column, const std::shared_ptr<GUILayout> &layout)
{
    ASSERT(column >= 0);
    ASSERT(column < _num_columns);
    ASSERT(row >= 0);
    ASSERT(row < _num_rows);

    DTuint i = row * _num_columns + column;
    
    GUIGridLayoutItem &item = _items[i];
    item._layouts.push_back(layout);
}

void GUIGridLayout::set_border_item (const std::shared_ptr<GUIObject> &object, DTfloat pad_h, DTfloat pad_v)
{
    _border_item = object;
    _border_pad_h = pad_h;
    _border_pad_v = pad_v;
}

//==============================================================================
//==============================================================================

void GUIGridLayout::layout (const Rectangle &size_target)
{
    animate(size_target, 0.0F);
}

//==============================================================================
//==============================================================================

void GUIGridLayout::animate (const Rectangle &size_target, DTfloat time, DTboolean spline)
{
    // Total size
    DTfloat total_width = size_target.plus_x() - size_target.minus_x();
    DTfloat total_height = size_target.plus_y() - size_target.minus_y();

    //
    // Columns first
    //

    _widths.clear();
    _widths.resize(_num_columns, 0.0F);
    
    std::vector<DTuint>   variable_column_indices;
    DTfloat         total_column_width = 0.0F;
    DTfloat         total_column_weights = 0.0F;
    
    // Scan for variable columns, total weights, and current size
    for (DTuint i = 0; i < _widths.size(); ++i) {
        GUILayoutPolicy &p = _column_policies[i];
        
        if (p.policy() == GUILayoutPolicy::POLICY_FIXED_SIZE) {
            _widths[i] = p.value();
        } else {
            variable_column_indices.push_back(i);
            total_column_weights += p.value();
        }
            
        total_column_width += _widths[i];
    }
    
    // Update variable columns
    total_column_width = total_width - total_column_width;  // Adjust to remaining width
    
    for (DTuint i = 0; i < variable_column_indices.size(); ++i) {
        DTuint c = variable_column_indices[i];
        
        _widths[c] = (_column_policies[c].value() / total_column_weights) * total_column_width;
    }
    
    //
    // Rows second
    //
    
    _heights.clear();
    _heights.resize(_num_rows, 0.0F);
    
    std::vector<DTuint>   variable_row_indices;
    DTfloat         total_row_height = 0.0F;
    DTfloat         total_row_weights = 0.0F;
    
    // Scan for variable rows, total weights, and current size
    for (DTuint i = 0; i < _heights.size(); ++i) {
        GUILayoutPolicy &p = _row_policies[i];
        
        if (p.policy() == GUILayoutPolicy::POLICY_FIXED_SIZE) {
            _heights[i] = p.value();
        } else {
            variable_row_indices.push_back(i);
            total_row_weights += p.value();
        }
            
        total_row_height += _heights[i];
    }

    // Update variable rows
    total_row_height = total_height - total_row_height;  // Adjust to remaining height
    
    for (DTuint i = 0; i < variable_row_indices.size(); ++i) {
        DTuint c = variable_row_indices[i];
        
        _heights[c] = (_row_policies[c].value() / total_row_weights) * total_row_height;
    }


    //
    // Finally do layout
    //
    
    _border_rect.clear();

    DTfloat row_pos = size_target.minus_y();
    for (DTuint row = 0; row < _num_rows; ++row) {
        DTfloat row_height = _heights[row];
        
        DTfloat column_pos = size_target.minus_x();
        for (DTuint column = 0; column < _num_columns; ++column) {
            DTfloat column_width = _widths[column];
            
            DTuint i = row * _num_columns + column;
            GUIGridLayoutItem &item = _items[i];
            
            // Update border rect
            if (item._objects.size() > 0)
                _border_rect = Rectangle::calc_union(_border_rect, Rectangle(column_pos, column_pos + column_width, row_pos, row_pos + row_height));

            // Objects
            for (auto &obj_entry : item._objects) {
                
                std::shared_ptr<GUIObject> &object = obj_entry._object;
                GUIGridLayoutItem::ResizeMode resize_mode = obj_entry._resize_mode;
                
                
                if (time <= 0.0F) {
            
                    object->set_translation_local(Vector3(column_pos + column_width*0.5F, row_pos + row_height*0.5F, 0.0F));
                    
                    if (resize_mode == GUIGridLayoutItem::RESIZE_MODE_RESIZE) {
                        object->set_width(column_width);
                        object->set_height(row_height);
                    } else if (resize_mode == GUIGridLayoutItem::RESIZE_MODE_SCALE) {
                        
                        DTfloat cell_aspect = column_width / row_height;
                        DTfloat item_aspect = object->width() / object->height();
                    
                        if (cell_aspect > item_aspect) {
                            // Vertically constrained
                            DTfloat scale = row_height / object->height();
                            object->set_width(object->width() * scale);
                            object->set_height(object->height() * scale);
                        } else {
                            // Vertically constrained
                            DTfloat scale = column_width / object->width();
                            object->set_width(object->width() * scale);
                            object->set_height(object->height() * scale);
                        }
                        
                    } else if (resize_mode == GUIGridLayoutItem::RESIZE_MODE_SCALE_FIT_HEIGHT) {
                        DTfloat scale = row_height / object->height();
                        object->set_width(object->width() * scale);
                        object->set_height(object->height() * scale);
                    }
                    
                } else {
                
                    if (resize_mode == GUIGridLayoutItem::RESIZE_MODE_RESIZE) {
                        object->add_anim_key()
                            .set_spline(spline)
                            .set_duration(time)
                            .set_translation_local(Vector3(column_pos + column_width*0.5F, row_pos + row_height*0.5F, 0.0F))
                            .set_width(column_width)
                            .set_height(row_height);
                        
                    } else if (resize_mode == GUIGridLayoutItem::RESIZE_MODE_SCALE) {

                        DTfloat cell_aspect = column_width / row_height;
                        DTfloat item_aspect = object->width() / object->height();
                    
                        if (cell_aspect > item_aspect) {
                            // Vertically constrained
                            DTfloat scale = object->height() / row_height;
                            
                            object->add_anim_key()
                                .set_spline(spline)
                                .set_duration(time)
                                .set_translation_local(Vector3(column_pos + column_width*0.5F, row_pos + row_height*0.5F, 0.0F))
                                .set_width(object->width() * scale)
                                .set_height(object->height() * scale);

                        } else {
                            // Vertically constrained
                            DTfloat scale = object->width() / column_width;

                            object->add_anim_key()
                                .set_spline(spline)
                                .set_duration(time)
                                .set_translation_local(Vector3(column_pos + column_width*0.5F, row_pos + row_height*0.5F, 0.0F))
                                .set_width(object->width() * scale)
                                .set_height(object->height() * scale);
                        }
                    
                    } else if (resize_mode == GUIGridLayoutItem::RESIZE_MODE_SCALE_FIT_HEIGHT) {
                        // Vertically constrained
                        DTfloat scale = object->height() / row_height;
                        
                        object->add_anim_key()
                            .set_spline(spline)
                            .set_duration(time)
                            .set_translation_local(Vector3(column_pos + column_width*0.5F, row_pos + row_height*0.5F, 0.0F))
                            .set_width(object->width() * scale)
                            .set_height(object->height() * scale);

                    } else {
                        object->add_anim_key()
                            .set_spline(spline)
                            .set_duration(time)
                            .set_translation_local(Vector3(column_pos + column_width*0.5F, row_pos + row_height*0.5F, 0.0F));
                    }
                }
                
            }

            // Child layouts
            for (DTuint j = 0; j < item._layouts.size(); ++j) {
                std::shared_ptr<GUILayout> &layout = item._layouts[j];
                
                Rectangle r;
                r.set_minus_x(column_pos);
                r.set_plus_x(column_pos + column_width);
                r.set_minus_y(row_pos);
                r.set_plus_y(row_pos + row_height);
                
                if (time <= 0) {
                    layout->layout(r);
                } else {
                    layout->animate(r,time);
                }
            }
            
            column_pos += column_width;
        }
        
        row_pos += row_height;
    }
    
    // Update border item
    if (_border_item) {
    
        // Calculate bounds
        Rectangle border = get_content_rect();
        
        FOR_EACH(i, _items) {
            FOR_EACH(j, i->_layouts) {
                
                if (*j)
                    border = Rectangle::calc_union(border, (**j).get_content_rect());
                
            }
        }
    
        // Expand bounds
        border.set_minus_x(border.minus_x() - _border_pad_h);
        border.set_plus_x(border.plus_x() + _border_pad_h);
        border.set_minus_y(border.minus_y() - _border_pad_v);
        border.set_plus_y(border.plus_y() + _border_pad_v);
    
        if (time <= 0.0F) {
            _border_item->set_translation_local(Vector3(border.minus_x() + border.width()*0.5F, border.minus_y() + border.height()*0.5F, 0.0F));
            _border_item->set_width(border.width());
            _border_item->set_height(border.height());
        } else {
            _border_item->add_anim_key()
                .set_spline(spline)
                .set_duration(time)
                .set_translation_local(Vector3(border.minus_x() + border.width()*0.5F, border.minus_y() + border.height()*0.5F, 0.0F))
                .set_width(border.width())
                .set_height(border.height());
        }
    }

}

//==============================================================================
//==============================================================================

void GUIGridLayout::add_anim_key (const GUIAnimKey &key)
{
    for (auto &item : _items) {
        for (auto &object : item._objects)
            object._object->add_anim_key(key);
        
        for (auto &layout : item._layouts)
            layout->add_anim_key(key);
    }
    
    if (_border_item) {
        _border_item->add_anim_key(key);
    }
}

void GUIGridLayout::clear_anims (void)
{
    for (auto &item : _items) {
        for (auto &object : item._objects)
            object._object->clear_anims();
        
        for (auto &layout : item._layouts)
            layout->clear_anims();
    }

    if (_border_item) {
        _border_item->clear_anims();
    }
}

//==============================================================================
//==============================================================================

Rectangle GUIGridLayout::get_content_rect (void) const
{
    return _border_rect;
}

//==============================================================================
//==============================================================================

std::list<std::shared_ptr<GUIObject>> GUIGridLayout::all_objects ()
{
    std::list<std::shared_ptr<GUIObject>> objects;
    
    for (DTuint row = 0; row < _num_rows; ++row) {
        for (DTuint column = 0; column < _num_columns; ++column) {
            
            DTuint i = row * _num_columns + column;
            GUIGridLayoutItem &item = _items[i];
            
            // Objects
            for (auto &object : item._objects)
                objects.push_back(object._object);

            // Child layouts
            for (auto &layout : item._layouts) {
                std::list<std::shared_ptr<GUIObject>> sub_objects = layout->all_objects();
                objects.splice(objects.end(), sub_objects);
            }
            
        }
    }
    
    if (_border_item) {
        objects.push_back(_border_item);
    }

    return objects;
}

//==============================================================================
//==============================================================================

} // DT3

