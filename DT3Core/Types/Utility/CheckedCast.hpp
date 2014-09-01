#ifndef DT3_CHECKEDCAST
#define DT3_CHECKEDCAST
//==============================================================================
///	
///	File: CheckedCast.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/Error.hpp"
#include "DT3Core/Types/Utility/TypeTraits.hpp"
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

/// Cast from one type to another without checking
/// \param o object to cast
/// \return casted object

template <class T, class U>
std::shared_ptr<T> checked_static_cast (const std::shared_ptr<U> &o)
{
#ifdef DT3_DEBUG
	if (o) {
		if (!o->isa(TypeTraits<T>::NonPtrType::kind())) {
			//DTcharacter* convert_to = TypeTraits<T>::NonPtrType::getClassIDStatic();
			//DTcharacter* convert_from = o->getClassIDChild();
                        
            ERRORMSG("Inavlid type conversion");
		}
		return std::static_pointer_cast<T>(o);
	} else {
		return NULL;
	}
#else
	return std::static_pointer_cast<T>(o);
#endif
}

/// Cast from one type to another with checking
/// \param o object to cast
/// \return casted object

template <class T, class U>
std::shared_ptr<T> checked_cast (const std::shared_ptr<U> &o)
{
	if (o) {
		if (!o->isa(TypeTraits<T>::NonPtrType::kind()))
            return NULL;
		return std::static_pointer_cast<T>(o);
	} else {
		return NULL;
	}
}

//==============================================================================
//==============================================================================

/// Cast from one type to another without checking
/// \param o object to cast
/// \return casted object

template <class T, class U>
T checked_static_cast (U *o)
{
#ifdef DT3_DEBUG
	if (o) {
		if (!o->isa(TypeTraits<T>::NonPtrType::kind())) {
			//DTcharacter* convert_to = TypeTraits<T>::NonPtrType::getClassIDStatic();
			//DTcharacter* convert_from = o->getClassIDChild();
                        
            ERRORMSG("Inavlid type conversion");
		}
		return static_cast<T>(o);
	} else {
		return NULL;
	}
#else
	return static_cast<T>(o);
#endif
}

/// Cast from one type to another with checking
/// \param o object to cast
/// \return casted object

template <class T, class U>
T checked_cast (U *o)
{
	if (o) {
		if (!o->isa(TypeTraits<T>::NonPtrType::kind()))
            return NULL;
		return static_cast<T>(o);
	} else {
		return NULL;
	}
}

//==============================================================================
//==============================================================================

} // DT3

#endif
