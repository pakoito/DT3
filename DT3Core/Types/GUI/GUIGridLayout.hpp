#ifndef DT3_GUIGRIDLAYOUT
#define DT3_GUIGRIDLAYOUT
//==============================================================================
///	
///	File: GUIGridLayout.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/GUI/GUILayout.hpp"
#include "DT3Core/Types/Math/Rectangle.hpp"
#include <vector>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward Declarations
//==============================================================================

class GUIObject;
class GUILayoutPolicy;

//==============================================================================
/// Class
//==============================================================================

class GUIGridLayoutItem { 
    public:
                                    GUIGridLayoutItem       (void);
                                    GUIGridLayoutItem       (const GUIGridLayoutItem &rhs);
        GUIGridLayoutItem&          operator =              (const GUIGridLayoutItem &rhs);
		virtual                     ~GUIGridLayoutItem      (void);
       
	public:
        enum ResizeMode {
            RESIZE_MODE_RESIZE,
            RESIZE_MODE_SCALE,
            RESIZE_MODE_SCALE_FIT_HEIGHT,
            RESIZE_MODE_DO_NOTHING
        };
    
        struct Object {
            std::shared_ptr<GUIObject>              _object;
            ResizeMode                              _resize_mode;
        };
    
        std::vector<Object>                         _objects;
        std::vector<std::shared_ptr<GUILayout>>     _layouts;
};

//==============================================================================
/// Class
//==============================================================================

class GUIGridLayout: public GUILayout {
    public:
        DEFINE_TYPE(GUIGridLayout,GUILayout)
        DEFINE_CREATE_AND_CLONE

                                    GUIGridLayout           (void);
                                    GUIGridLayout           (const GUIGridLayout &rhs);
        GUIGridLayout &				operator =              (const GUIGridLayout &rhs);
		virtual                     ~GUIGridLayout          (void);
  
	public:
		/// Sets the number of rows in columns in the sizer
		/// \param num_rows number of rows
		/// \param num_cols number of columns
        void                        set_rows_and_columns    (DTint num_rows, DTint num_cols);
        
		/// Sets the policy for resizing a column
		/// \param column column index
        /// \param p Policy
        void                        set_column_policy       (DTint column, const GUILayoutPolicy &p);
        
		/// Sets the policy for resizing a row
		/// \param row row index
        /// \param p Policy
        void                        set_row_policy          (DTint row, const GUILayoutPolicy &p);
        
		/// Add an item to the layout
		/// \param row row index
		/// \param column column index
		/// \param object object to add
		/// \param resize_mode how to resize the item
        void                        add_item                (DTint row, DTint column, const std::shared_ptr<GUIObject> &object, GUIGridLayoutItem::ResizeMode resize_mode = GUIGridLayoutItem::RESIZE_MODE_RESIZE);
    
		/// Add sub layout to the layout
		/// \param row row index
		/// \param column column index
		/// \param layout layout to add
        void                        add_item                (DTint row, DTint column, const std::shared_ptr<GUILayout> &layout);

		/// Set the border item
		/// \param object object to add
		/// \param pad_h horizontal padding
		/// \param pad_v horizontal padding
        void                        set_border_item         (const std::shared_ptr<GUIObject> &object, DTfloat pad_h, DTfloat pad_v);
    
		/// Perform layout
		/// \param size_target Rectangle to fit widgets into
        virtual void                layout                  (const Rectangle &size_target);

		/// Animate layout
		/// \param size_target Rectangle to fit widgets into
		/// \param time Animation time
		/// \param spline Use spline interpolation
        virtual void                animate                 (const Rectangle &size_target, DTfloat time, DTboolean spline = false);

		/// Add an animation key
		/// \param key animation key to add
        virtual void                add_anim_key            (const GUIAnimKey &key);

		/// Clear all animations
        virtual void                clear_anims             (void);

        /// Content rectangle
        /// \return Content rectangle
        virtual Rectangle           get_content_rect        (void) const;

		/// Returns the height of the row
		/// \param i row index
		/// \return row height
        DTfloat                     row_height              (DTuint i)      {   return _heights[i];     }

		/// Returns the width of the column
		/// \param i column index
		/// \return column width
        DTfloat                     column_width            (DTuint i)      {   return _widths[i];      }

		/// Returns all objects managed by the layout
		/// \return all objects
        virtual std::list<std::shared_ptr<GUIObject>>  all_objects (void);

    private:
    
        std::vector<DTfloat>                _widths;
        std::vector<DTfloat>                _heights;

        std::vector<GUILayoutPolicy>        _row_policies;
        std::vector<GUILayoutPolicy>        _column_policies;
        
        std::vector<GUIGridLayoutItem>      _items;
        DTint                               _num_rows;
        DTint                               _num_columns;
    
        Rectangle                           _border_rect;
        std::shared_ptr<GUIObject>          _border_item;
        DTfloat                             _border_pad_h;
        DTfloat                             _border_pad_v;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
