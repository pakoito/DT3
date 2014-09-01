#ifndef DT3_STATICINITIALIZER
#define DT3_STATICINITIALIZER
//==============================================================================
///	
///	File: StaticInitializer.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include <list>

namespace DT3 {

//==============================================================================
// Forward declarations
//==============================================================================

class StaticInitializerCallback;

//==============================================================================
//==============================================================================

class StaticInitializer {
    private:
									StaticInitializer					(void);	
									StaticInitializer					(const StaticInitializer &rhs);
		StaticInitializer &			operator =							(const StaticInitializer &rhs);		
									~StaticInitializer					(void);
            
    public:
	
		/// Description
		/// \param param description
		/// \return description
		static void					register_initializer                (StaticInitializerCallback *initializer);

		/// Description
		/// \param param description
		/// \return description
		static void					initialize							(void);

		/// Description
		/// \param param description
		/// \return description
		static void					destroy                             (void);
};
		
//==============================================================================
//==============================================================================

class StaticInitializerCallback {
	public:
									StaticInitializerCallback			(void)				{	_priority = 0;	StaticInitializer::register_initializer(this);	}
	private:
									StaticInitializerCallback			(const StaticInitializerCallback &rhs);
		StaticInitializerCallback&	operator =							(const StaticInitializerCallback &rhs);		
	public:
		virtual 					~StaticInitializerCallback			(void)				{}

		/// Description
		/// \param param description
		/// \return description
		void						set_priority                        (DTuint priority)	{	_priority = priority;	}
	
		/// Description
		/// \param param description
		/// \return description
		DTuint						priority                            (void) const		{	return _priority;		}

		/// Description
		/// \param param description
		/// \return description
		virtual void				initialize							(void)              {}

		/// Description
		/// \param param description
		/// \return description
		virtual void				destroy                             (void)              {}
		
	private:
		DTuint						_priority;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
