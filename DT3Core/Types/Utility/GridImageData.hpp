#ifndef DT3_GRIDIMAGEDATA
#define DT3_GRIDIMAGEDATA
//==============================================================================
///	
///	File: GridImageData.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Math/Vector2.hpp"
#include <vector>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Stream;
class Archive;

//==============================================================================
/// Class
//==============================================================================

class GridImageData {
    public:
        DEFINE_TYPE_SIMPLE_BASE(GridImageData)

                                    GridImageData           (void);
                                    GridImageData           (const GridImageData &rhs);
        GridImageData &             operator =              (const GridImageData &rhs);
        virtual                     ~GridImageData          (void);
    	
	public:
    
		inline DTboolean	operator ==                     (const GridImageData& rhs) const  {	return false;		}
		inline DTboolean	operator !=                     (const GridImageData& rhs) const  {	return true;		}


		/// Set number of control points in X direction
		/// \param num_x number of control points
   		void                        set_num_x               (const DTuint num_x);

		/// Returns number of control points in X direction
		/// \return number of control points
   		const DTuint				num_x                   (void) const;

		/// Set number of control points in Y direction
		/// \param num_y number of control points
   		void                        set_num_y               (const DTuint num_y);

		/// Returns number of control points in Y direction
		/// \return number of control points
   		const DTuint				num_y                   (void) const;
        
		/// Returns the control point at x,y location
		/// \param x x index
		/// \param y y index
		/// \return Control Point
        const Vector2 &             point                   (DTuint x, DTuint y);
        
		/// Sets the control point at x,y location
		/// \param x x index
		/// \param y y index
		/// \param p new control point value
        void                        set_point               (DTuint x, DTuint y, const Vector2 &p);
    
    private:
		friend Stream& operator << (Stream &s, const GridImageData&v);
		friend Stream& operator >> (Stream &s, GridImageData&v);

		friend GridImageData		operator +              (const GridImageData &a, const GridImageData &b);
		friend GridImageData		operator -              (const GridImageData &a, const GridImageData &b);
		friend GridImageData		operator *              (const DTfloat b, const GridImageData &a);

        void                        clear                   (void);
    
        DTuint                      _num_x;
        DTuint                      _num_y;
        std::vector<Vector2>        _grid;

};

//==============================================================================
//==============================================================================

inline GridImageData operator - (const GridImageData &a, const GridImageData &b)
{
    GridImageData r;
    
    if (a.num_x() != b.num_x() || b.num_y() != b.num_y())
        return r;
        
    r._num_x = a._num_x;
    r._num_y = a._num_y;
    
    r._grid.resize(r._num_x * r._num_y);
    
    for (DTuint i = 0; i < r._grid.size(); ++i) {
        r._grid[i] = a._grid[i] - b._grid[i];
    }

    return r;
}

inline GridImageData operator + (const GridImageData &a, const GridImageData &b)
{
    GridImageData r;
    
    if (a.num_x() != b.num_x() || b.num_y() != b.num_y())
        return r;
    
    r._num_x = a._num_x;
    r._num_y = a._num_y;
    
    r._grid.resize(r._num_x * r._num_y);
    
    for (DTuint i = 0; i < r._grid.size(); ++i) {
        r._grid[i] = a._grid[i] + b._grid[i];
    }

    return r;
}

inline GridImageData operator * (const DTfloat b, const GridImageData &a)
{
    GridImageData r;
            
    r._num_x = a._num_x;
    r._num_y = a._num_y;
    
    r._grid.resize(r._num_x * r._num_y);
    
    for (DTuint i = 0; i < r._grid.size(); ++i) {
        r._grid[i] = a._grid[i] * b;
    }

    return r;
}

//==============================================================================
/// Streaming operators
//==============================================================================

Stream& operator << (Stream &s, const GridImageData&v);
Stream& operator >> (Stream &s, GridImageData&v);

//==============================================================================
//==============================================================================

} // DT3

#endif
