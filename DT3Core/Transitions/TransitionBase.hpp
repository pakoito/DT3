#ifndef DT3_TRANSITIONBASE
#define DT3_TRANSITIONBASE
//==============================================================================
///	
///	File: TransitionBase.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseClass.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Class
//==============================================================================

class TransitionBase: public BaseClass {
    public:
        DEFINE_TYPE(TransitionBase,BaseClass)
		DEFINE_CREATE
         
							TransitionBase			(void);	
	private:
							TransitionBase			(const TransitionBase &rhs);
        TransitionBase &	operator =				(const TransitionBase &rhs);	
	public:
        virtual				~TransitionBase			(void);

	public:
		
		/// Description
		/// \param param description
		/// \return description
		virtual DTboolean	begin_new_world         (void)	{	return false;	};

		/// Description
		/// \param param description
		/// \return description
		virtual void		end_new_world           (void)	{};

		/// Description
		/// \param param description
		/// \return description
		virtual DTboolean	begin_old_world         (void)	{	return false;	};

		/// Description
		/// \param param description
		/// \return description
		virtual void		end_old_world           (void)	{};
		
		/// Description
		/// \param param description
		/// \return description
		virtual void		transition				(const DTfloat t)	{};

		/// Description
		/// \param param description
		/// \return description
		virtual void		draw					(void)	{};
		
		/// Description
		/// \param param description
		/// \return description
		DTfloat				remap_zero_one			(DTfloat value, DTfloat begin, DTfloat end);
};

//==============================================================================
//==============================================================================

} // DT3

#endif
