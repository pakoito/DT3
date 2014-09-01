#ifndef DT3_ENDIAN
#define DT3_ENDIAN
//==============================================================================
///	
///	File: Endian.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

namespace EndianInfo {

// No "swap" so we get an error
template <int S> struct swapHelper {};


template <> struct swapHelper<1> {
    static void    swap    (DTubyte *v)
    {
        // Do nothing
    }
};

template <> struct swapHelper<2> {
    static void    swap    (DTubyte *v)
    {
        DTubyte temp = v[0];
        v[0] = v[1];
        v[1] = temp;
    }
};

template <> struct swapHelper<4> {
    static void    swap    (DTubyte *v)
    {
        DTubyte temp = v[0];
        v[0] = v[3];
        v[3] = temp;

        temp = v[1];
        v[1] = v[2];
        v[2] = temp;
    }
};

template <> struct swapHelper<8> {
    static void    swap    (DTubyte *v)
    {
        DTubyte temp = v[0];
        v[0] = v[7];
        v[7] = temp;

        temp = v[1];
        v[1] = v[6];
        v[6] = temp;

        temp = v[2];
        v[2] = v[5];
        v[5] = temp;

        temp = v[3];
        v[3] = v[4];
        v[4] = temp;
    }
};

} // EndianInfo

//==============================================================================
//==============================================================================

class Endian {
    private:
								Endian                  (void);	
								Endian                  (const Endian &rhs);
		Endian &				operator =              (const Endian &rhs);		
								~Endian                 (void);
            
    public:
		/// Swap Endianness of data
		/// \param v value to swap
        template <typename T>
        static void swap   (T &v)
        {
			EndianInfo::swapHelper<sizeof(T)>::swap((DTubyte*)&v);
        }
		

		/// Swap Endianness of data to network byte order if necessary
		/// \param v value to swap
        template <typename T>
        static void to_network_byte_order   (T &v)
        {
#if DT3_BYTEORDER == DT3_LIL_ENDIAN
			EndianInfo::swapHelper<sizeof(T)>::swap((DTubyte*)&v);
#endif
        }

		/// Swap Endianness of data from network byte order if necessary
		/// \param v value to swap
        template <typename T>
        static void from_network_byte_order (T &v)
        {
#if DT3_BYTEORDER == DT3_LIL_ENDIAN
			EndianInfo::swapHelper<sizeof(T)>::swap((DTubyte*)&v);
#endif
        }
        
        
		/// Convert to big endian if necessary
		/// \param v value to swap
        template <typename T>
        static void to_big_endian   (T &v) {
#if DT3_BYTEORDER == DT3_LIL_ENDIAN
			EndianInfo::swapHelper<sizeof(T)>::swap((DTubyte*)&v);
#endif
		}

		/// Convert from big endian if necessary
		/// \param v value to swap
        template <typename T>
		static void from_big_endian   (T &v) {
#if DT3_BYTEORDER == DT3_LIL_ENDIAN
			EndianInfo::swapHelper<sizeof(T)>::swap((DTubyte*)&v);
#endif
		}


		/// Convert to little endian if necessary
		/// \param v value to swap
       template <typename T>
       static void to_little_endian   (T &v) {
#if DT3_BYTEORDER == DT3_BIG_ENDIAN
			EndianInfo::swapHelper<sizeof(T)>::swap((DTubyte*)&v);
#endif
		}

		/// Convert from little endian if necessary
		/// \param v value to swap
        template <typename T>
		static void from_little_endian   (T &v) {
#if DT3_BYTEORDER == DT3_BIG_ENDIAN
			EndianInfo::swapHelper<sizeof(T)>::swap((DTubyte*)&v);
#endif
		}
        
        
};

//==============================================================================
//==============================================================================

} // DT3

#endif
