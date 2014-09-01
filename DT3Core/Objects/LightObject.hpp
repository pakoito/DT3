#ifndef DT3_LIGHTOBJECT
#define DT3_LIGHTOBJECT
//==============================================================================
///	
///	File: LightObject.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Objects/PlaceableObject.hpp"
#include "DT3Core/Types/Math/Color4f.hpp"

namespace DT3 {


//==============================================================================
/// Forward declarations
//==============================================================================

class CameraObject;

//==============================================================================
/// Base class for building your own light sources. This does nothing to
/// the default renderer.
//==============================================================================

class LightObject: public PlaceableObject {    
    public:
        DEFINE_TYPE(LightObject,PlaceableObject)
		DEFINE_CREATE_AND_CLONE
   		DEFINE_PLUG_NODE
     
                                    LightObject						(void);	
                                    LightObject						(const LightObject &rhs);
        LightObject &				operator =						(const LightObject &rhs);	
        virtual                     ~LightObject					(void);
                
        virtual void                archive                         (const std::shared_ptr<Archive> &archive);
		
	public:
        /// Accessors for Red gain
		DEFINE_ACCESSORS(gain_r, set_gain_r, DTfloat, _gain_r);

        /// Accessors for Green gain
		DEFINE_ACCESSORS(gain_g, set_gain_g, DTfloat, _gain_g);

        /// Accessors for Blue gain
		DEFINE_ACCESSORS(gain_b, set_gain_b, DTfloat, _gain_b);

        /// Accessors for Alpha gain
		DEFINE_ACCESSORS(gain_a, set_gain_a, DTfloat, _gain_a);

        /// Accessors for Color
		DEFINE_ACCESSORS(color, set_color, Color4f, _color);

        /// Accessors for Specular Color
		DEFINE_ACCESSORS(specular_color, set_specular_color, Color4f, _specular_color);

        /// Accessors for Specular Exponent
		DEFINE_ACCESSORS(specular_exponent_gain, set_specular_exponent_gain, DTfloat, _specular_gain);
        
    private:
		Plug<DTfloat>				_gain_r;
		Plug<DTfloat>				_gain_g;
		Plug<DTfloat>				_gain_b;
		Plug<DTfloat>				_gain_a;
		
		Plug<Color4f>				_color;
		Plug<Color4f>				_specular_color;
		Plug<DTfloat>				_specular_gain;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
