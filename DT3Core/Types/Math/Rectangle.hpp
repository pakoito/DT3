#ifndef DT3_RECTANGLE
#define DT3_RECTANGLE
//==============================================================================
///	
///	File: Rectangle.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Stream;

//==============================================================================
/// Class
//==============================================================================

class Rectangle {
    public:		
        DEFINE_TYPE_SIMPLE_BASE(Rectangle)

         
                                Rectangle           (void);
        explicit                Rectangle           (const DTfloat mx, const DTfloat px, const DTfloat my, const DTfloat py);
        explicit                Rectangle           (const Vector2 &v);
                                Rectangle           (const Rectangle &rhs);
        Rectangle &				operator =          (const Rectangle &rhs);
                                ~Rectangle          (void);
                        
    public:

        DTboolean               operator ==			(const Rectangle& rhs) const;
		DTboolean               operator !=			(const Rectangle& rhs) const;

		/// Sets the extents of the rectangle
		/// \param minus_x Minimum Coordinate along the X axis
		/// \param plus_x Maximum Coordinate along the X axis
		/// \param minus_y Minimum Coordinate along the Y axis
		/// \param plus_y Maximum Coordinate along the Y axis
        void                    set                 (   const DTfloat minus_x, const DTfloat plus_x,
                                                        const DTfloat minus_y, const DTfloat plus_y );

		/// Set the extents of the rectangle along the X axis
		/// \param minus_x Set Minus X
		/// \param plus_x Set Plus X
        void                    set_X_extents       (const DTfloat minus_x, const DTfloat plus_x)	{       _minus_x = minus_x; _plus_x = plus_x;	}

		/// Set the extents of the rectangle along the Y axis
		/// \param minus_y Set Minus Y
		/// \param plus_y Set Plus Y
        void                    set_Y_extents       (const DTfloat minus_y, const DTfloat plus_y)	{       _minus_y = minus_y; _plus_y = plus_y;	}
    
    
        DEFINE_ACCESSORS(minus_x, set_minus_x, DTfloat, _minus_x)
        DEFINE_ACCESSORS(plus_x, set_plus_x, DTfloat, _plus_x)

        DEFINE_ACCESSORS(minus_y, set_minus_y, DTfloat, _minus_y)
        DEFINE_ACCESSORS(plus_y, set_plus_y, DTfloat, _plus_y)

    
        /// Reset the rectangle to empty
        void                    clear               (void);

        /// Returns wether the rectangle is still clear
		/// \return is clear
        DTboolean               is_clear            (void)          {   return _minus_x >= _plus_x || _minus_y >= _plus_y;  }

		/// Offset the rectangle
		/// \param offset offset for the rectangle
        void                    offset              (const Vector2 &offset);



        //
        // Info
        //
    
		/// Returns the area of the rectangle
		/// \return area
        DTfloat                 area                (void) const    {   return width() * height();    }

		/// Returns the width of the rectangle
		/// \return width
		DTfloat                 width               (void) const    {   return _plus_x - _minus_x;  }

		/// Returns the height of the rectangle
		/// \return height
		DTfloat					height              (void) const    {   return _plus_y - _minus_y;  }
        
		/// Returns the center of the rectangle
		/// \return center
        Vector2                 center              (void) const    {   return Vector2( 0.5F*(_minus_x + _plus_x), 0.5F*(_minus_y + _plus_y) );   }


        //
        // Computation
        //

		/// Calculate the union of two rectangles
		/// \param a rect 1
		/// \param b rect 2
		/// \return union of rectangles
        static Rectangle        calc_union          (const Rectangle &a, const Rectangle &b);

		/// Calculate the union of two rectangles
		/// \param a rect 1
		/// \param b vector 2
		/// \return union of rectangles
        static Rectangle        calc_union          (const Rectangle &a, const Vector2 &b);

		/// Calculate the intersection of two rectangles
		/// \param a rect 1
		/// \param b rect 2
		/// \return intersection of rectangles
        static Rectangle        calc_intersection   (const Rectangle &a, const Rectangle &b);


        //
        // Query
        //
    
		/// Check for overlap beteen two rectangles
		/// \param other Other rectangle
		/// \return is touching
        DTboolean               is_touching         (const Rectangle &other);

		/// Check for overlap beteen two rectangles
		/// \param pt point
		/// \return is touching
		DTboolean               is_touching			(const Vector2 &pt) const;
					
		/// Returns the distance from the rectangle to the point
		/// \param pt point
		/// \return distance
        DTfloat                 distance_to_point   (const Vector2 &pt) const;

		/// Returns the closest point in the rectangle
		/// \param pt point
		/// \return closest point
        const Vector2           closest_point       (const Vector2 &pt) const;
    
    protected:
        DTfloat                 _minus_x;	///< Minus position on X axis
        DTfloat                 _plus_x;	///< Plus position on X axis
        DTfloat                 _minus_y;	///< Minus position on Y axis
        DTfloat                 _plus_y;	///< Plus position on Y axis
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const Rectangle&v);
Stream& operator >>(Stream &s, Rectangle&v);

//==============================================================================
//==============================================================================

} // DT3
#endif
