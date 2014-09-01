#ifndef DT3_CALLBACK
#define DT3_CALLBACK
//==============================================================================
///	
///	File: Callback.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/CheckedCast.hpp"
#include "DT3Core/Types/Utility/TypeTraits.hpp"
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

template <typename... T>
class Callback {
	public:
		DEFINE_TYPE_BASE(Callback)
		virtual ~Callback(void)    {}
		virtual void operator ()(T... t) = 0;
		virtual DTboolean	operator ==	(const Callback& rhs) const = 0;
};

template <typename CLASS,typename... T>
class CallbackImpl: public Callback<T...> {
	public:
        typedef typename TypeTraits<CLASS>::NonPtrType CLASS_BASE;
		typedef Callback<T...> CBT;
    
		DEFINE_TYPE(CallbackImpl,CBT)

		CallbackImpl (CLASS c, void (CLASS_BASE::*fn)(T... t))
		{
			_obj = c;
			_fn = fn;
		}

		virtual void operator ()(T... t)
		{
            (_obj->*_fn)(t...);
		}

		virtual DTboolean operator == (const CBT& rhs) const
		{
			const CallbackImpl<CLASS,T...> *rhs_type = checked_cast<const CallbackImpl<CLASS,T...> *>(&rhs);
			return rhs_type && (_obj == rhs_type->_obj) && (_fn == rhs_type->_fn);
		}

	public:
		CLASS _obj;
		void (CLASS_BASE::*_fn)(T... t);
};

template <typename... T>
class CallbackStaticImpl: public Callback<T...> {
	public:
		typedef Callback<T...> CBT;
		DEFINE_TYPE(CallbackStaticImpl,CBT)

		CallbackStaticImpl (void (*fn)(T... t))
		{
			_fn = fn;
		}

		virtual void operator ()(T... t)
		{
			if (_fn)
				(*_fn)(t...);
		}

		virtual DTboolean operator == (const CBT& rhs) const
		{
			const CallbackStaticImpl<T...> *rhs_type = checked_cast<const CallbackStaticImpl<T...> *>(&rhs);
			return rhs_type && (_fn == rhs_type->_fn);
		}

	public:
		void (*_fn)(T... t);
};

//==============================================================================
//==============================================================================

template<typename CLASS, typename... T>
inline std::shared_ptr<Callback<T...>> make_callback(CLASS c, void (TypeTraits<CLASS>::NonPtrType::*fn)(T...))
{
	return std::shared_ptr<Callback<T...>>(new ("Callback") CallbackImpl<CLASS, T...>(c, fn));
}

template<typename... T>
inline std::shared_ptr<Callback<T...>> make_callback(void (*fn)(T...))
{
	return std::shared_ptr<Callback<T...>>(new ("Callback") CallbackStaticImpl<T...>(fn));
}

//==============================================================================
//==============================================================================

} // DT3

#endif
