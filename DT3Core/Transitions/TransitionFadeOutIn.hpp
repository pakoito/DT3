#ifndef DT3_TRANSITIONFADEOUTIN
#define DT3_TRANSITIONFADEOUTIN
//==============================================================================
///	
///	File: TransitionFadeOutIn.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Transitions/TransitionBase.hpp"
#include "DT3Core/Types/Graphics/DrawBatcher.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class MaterialResource;
class ShaderResource;

//==============================================================================
/// Class
//==============================================================================

class TransitionFadeOutIn: public TransitionBase {
    public:
        DEFINE_TYPE(TransitionFadeOutIn,TransitionBase)
		DEFINE_CREATE
         
								TransitionFadeOutIn			(void);	
	private:
								TransitionFadeOutIn			(const TransitionFadeOutIn &rhs);
        TransitionFadeOutIn &	operator =					(const TransitionFadeOutIn &rhs);	
	public:
        virtual					~TransitionFadeOutIn		(void);

	public:
	
		/// Description
		/// \param param description
		/// \return description
		virtual DTboolean		begin_new_world             (void);

		/// Description
		/// \param param description
		/// \return description
		virtual void			end_new_world               (void);

		/// Description
		/// \param param description
		/// \return description
		virtual DTboolean		begin_old_world             (void);

		/// Description
		/// \param param description
		/// \return description
		virtual void			end_old_world				(void);
		
		/// Description
		/// \param param description
		/// \return description
		virtual void			transition				(const DTfloat transition);

		/// Description
		/// \param param description
		/// \return description
		virtual void			draw					(void);
		
	private:
		std::shared_ptr<MaterialResource>   _material;
		std::shared_ptr<ShaderResource>     _shader;

		DTfloat                             _transition;
    
        DrawBatcher                         _b;

};

//==============================================================================
//==============================================================================

} // DT3

#endif
