#ifndef DT3_BOX
#define DT3_BOX
//==============================================================================
///	
///	File: Box.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Math/Rectangle.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"

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

class Box: public Rectangle {
    public:		
        DEFINE_TYPE_SIMPLE(Box,Rectangle)

                                Box                 (void);
                                Box                 (const Box &rhs);
                                Box                 (   const DTfloat mx, const DTfloat px,
                                                        const DTfloat my, const DTfloat py,
                                                        const DTfloat mz, const DTfloat pz );
    
        Box &                   operator =          (const Box &rhs);
        
                                ~Box                (void);
           
	public:
    
        DTboolean               operator ==			(const Box& rhs) const;
		DTboolean               operator !=			(const Box& rhs) const;
    
		/// Sets the size of the box given the extents along all 3 axes.
		/// \param minus_x Minimum Coordinate along the X axis
		/// \param plus_x Maximum Coordinate along the X axis
		/// \param minus_y Minimum Coordinate along the Y axis
		/// \param plus_y Maximum Coordinate along the Y axis
		/// \param minus_z Minimum Coordinate along the Z axis
		/// \param plus_z Maximum Coordinate along the Z axis
        void                    set                 (   const DTfloat minus_x, const DTfloat plus_x, const DTfloat minus_y,
                                                        const DTfloat plus_y, const DTfloat minus_z, const DTfloat plus_z   );
    
		/// Sets the extents of the box along the Z axis.
		/// \param minus_z Minimum Coordinate along the Z axis
		/// \param plus_z Maximum Coordinate along the Z axis
        void                    set_Z_extents       (const DTfloat minus_z, const DTfloat plus_z)	{   _minus_z = minus_z; _plus_z = plus_z;	}
    
    
        DEFINE_ACCESSORS(minus_z, set_minus_z, DTfloat, _minus_z)
        DEFINE_ACCESSORS(plus_z, set_plus_z, DTfloat, _plus_z)
    
        /// Invalidates the box
        void                    clear               (void);

        /// Returns wether the box is still clear
		/// \return is box
        DTboolean               is_clear            (void)          {   return _minus_x >= _plus_x || _minus_y >= _plus_y || _minus_z >= _plus_z;  }

		/// Offset the rectangle
		/// \param offset offset for the rectangle
        void                    offset              (const Vector3 &offset);


        //
        // Info
        //
    
		/// Returns the area of the rectangle
		/// \return area
        DTfloat                 volume              (void) const    {   return area() * depth();    }

		/// Returns the width of the rectangle
		/// \return width
		DTfloat                 depth               (void) const    {   return _plus_z - _minus_z;  }
    
		/// Returns the center of the rectangle
		/// \return center
        Vector3                 center              (void) const    {   return Vector3( 0.5F*(_minus_x + _plus_x),
                                                                                        0.5F*(_minus_y + _plus_y),
                                                                                        0.5F*(_minus_z + _plus_z) );   }

        //
        // Computation
        //

		/// Calculate the union of two boxes
		/// \param a box 1
		/// \param b box 2
		/// \return union of boxes
        static Box              calc_union          (const Box &a, const Box &b);

		/// Calculate the union of two boxes
		/// \param a box 1
		/// \param b vector 2
		/// \return union of boxes
        static Box              calc_union          (const Box &a, const Vector3 &b);

		/// Calculate the intersection of two boxes
		/// \param a box 1
		/// \param b box 2
		/// \return intersection of boxes
        static Box              calc_intersection   (const Box &a, const Box &b);


        //
        // Query
        //
    
		/// Check for overlap beteen two rectangles
		/// \param other Other rectangle
		/// \return is touching
        DTboolean               is_touching         (const Box &other);

		/// Check for overlap beteen two rectangles
		/// \param pt point
		/// \return is touching
		DTboolean               is_touching			(const Vector3 &pt) const;
				
		/// Is this box touching an extruded sphere?
		/// \param start point
		/// \param end point
		/// \param radius radius
		/// \return is touching ray
		DTboolean               is_touching         (const Vector3 &from, const Vector3 &to, const DTfloat r);

		/// Is this box touching a ray?
		/// \param start point
		/// \param end point
		/// \return is touching ray
		DTboolean               is_touching         (const Vector3 &from, const Vector3 &to);

		/// Returns the distance from the rectangle to the point
		/// \param pt point
		/// \return distance
        DTfloat                 distance_to_point   (const Vector3 &pt) const;

		/// Returns the closest point in the rectangle
		/// \param pt point
		/// \return closest point
        const Vector3           closest_point       (const Vector3 &pt) const;

    private:
        DTfloat									_minus_z;	/// Minus position on Z axis
        DTfloat									_plus_z;	/// Plus position on Z axis
};

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator <<(Stream &s, const Box &v);
Stream& operator >>(Stream &s, Box &v);

//==============================================================================
//==============================================================================

} // DT3

#endif
