#ifndef DT3_GUIANIMKEY
#define DT3_GUIANIMKEY
//==============================================================================
///	
///	File: GUIAnimKey.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/LatentCall.hpp"
#include "DT3Core/Types/Math/Vector3.hpp"
#include "DT3Core/Types/Math/Matrix3.hpp"
#include "DT3Core/Types/Math/Color4f.hpp"
#include <string>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward Declarations
//==============================================================================

class GUIObject;

//==============================================================================
/// Class
//==============================================================================

class GUIAnimKey {
    public:
        DEFINE_TYPE_BASE(GUIAnimKey)
    
                                    GUIAnimKey              (void);
                                    GUIAnimKey              (const GUIAnimKey &rhs);
        GUIAnimKey &				operator =              (const GUIAnimKey &rhs);
		virtual                     ~GUIAnimKey             (void);
  
	public:
		/// Sets repeating animation
		/// \return this object
		GUIAnimKey&					set_repeat              (DTboolean s = true);

		/// Description
		/// \param param description
		/// \return description
		DTboolean					is_repeat               (void) const	{	return _repeat_flag;	}

		/// Sets spline interpolation
		/// \return this object
		GUIAnimKey&					set_spline              (DTboolean s = true);

		/// Sets latent call to be called
		/// \return this object
		GUIAnimKey&					set_call                (std::shared_ptr<LatentCall> latent_call);

		/// Sets delay before starting
		/// \return this object
		GUIAnimKey&					set_delay               (DTfloat delay);

		/// Sets frame duration
		/// \return this object
		GUIAnimKey&					set_duration            (DTfloat duration);

		/// Sets local translation
		/// \return this object
		GUIAnimKey&					set_translation_local   (const Vector3 &translation);
		
		/// Sets orientation
		/// \return this object
		GUIAnimKey&					set_orientation         (const Matrix3 &orientation);

		/// Sets color
		/// \return this object
		GUIAnimKey&					set_color               (const Color4f &color);

		/// Sets scale
		/// \return this object
		GUIAnimKey&					set_scale               (const Vector3 &scale);

		/// Sets label
		/// \return this object
		GUIAnimKey&					set_label               (const std::string &label);

		/// Sets width
		/// \return this object
		GUIAnimKey&					set_width               (DTfloat width);

		/// Sets height
		/// \return this object
		GUIAnimKey&					set_height              (DTfloat height);

		/// Sets bounds
		/// \return this object
		GUIAnimKey&					set_bounds              (DTfloat xpos, DTfloat ypos, DTfloat width, DTfloat height);

		/// Remembers state of object before animating the frame
		/// \param object to remember state of
		void						cache_frame_state       (GUIObject *frame);
		
		/// Pass time in the animation frame
		/// \param dt delta time
		/// \return Still animating
		DTboolean					animate                 (DTfloat dt, GUIObject *frame);
        
		/// Returns wether this frame is used in an active animation
		/// \return is used in animation
        DTboolean                   is_busy                 (void) const    {   return _busy;  }
		
	private:		

		
		DTubyte                         _repeat_flag:1;
		DTubyte                         _color_flag:1;
		DTubyte                         _scale_flag:1;
		DTubyte                         _translation_flag:1;
		DTubyte                         _orientation_flag:1;
		DTubyte                         _spline_flag:1;
		DTubyte                         _label_flag:1;
		DTubyte                         _width_flag:1;
		DTubyte                         _height_flag:1;
        
		DTubyte                         _busy:1;
        
        std::shared_ptr<LatentCall>     _latent_call;
		
		DTfloat                         _time;
		DTfloat                         _delay;
		DTfloat                         _duration;
		
		Matrix3                         _orientation;
		Matrix3                         _cache_orientation;

		Color4f                         _color;
		Color4f                         _cache_color;
	
		Vector3                         _scale;
		Vector3                         _cache_scale;
		
		DTfloat                         _width;
		DTfloat                         _cache_width;
		
		DTfloat                         _height;
		DTfloat                         _cache_height;
		
		Vector3                         _translation;
		Vector3                         _cache_translation;

		std::string                     _label;
		std::string                     _cache_label;
		
		DTfloat                         interp(DTfloat v0, DTfloat v1, DTfloat t);
        Vector3                         interp(Vector3 v0, Vector3 v1, DTfloat t);
};

//==============================================================================
//==============================================================================

} // DT3

#endif
