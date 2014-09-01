#ifndef DT3_LATENTCALL
#define DT3_LATENTCALL
//==============================================================================
///	
///	File: LatentCall.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/Callback.hpp"
#include <memory>
#include <functional>
#include <utility>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class LatentCall {
    public:
        DEFINE_TYPE_BASE(LatentCall)

                        LatentCall          (void)                      {}
                        LatentCall          (const LatentCall &rhs)     {}
        LatentCall&     operator =          (const LatentCall& rhs)     {	return (*this);     }
                        ~LatentCall         (void)                      {}

    public:
        virtual void    fire            (void) = 0;
};

//==============================================================================
// http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
//==============================================================================

template <typename... T>
class LatentCallImpl: public LatentCall {
	public:
		DEFINE_TYPE(LatentCallImpl,LatentCall)

		LatentCallImpl (std::shared_ptr<Callback<T...>> cb, T... t)
			:   _cb (cb),
                _t  (t...)
		{}

		LatentCallImpl (const LatentCallImpl<T...>& rhs)
			:	LatentCall(rhs),
                _cb (rhs._cb),
				_t  (rhs._t)
		{}

		LatentCallImpl<T...>& operator = (const LatentCallImpl<T...>& rhs)
		{
			LatentCall::operator = (rhs);
            _cb = rhs._cb;
			_t = rhs.t;
			return (*this);
		}

		~LatentCallImpl (void)
		{
		}

		void fire (void) {
            call_func(typename gens<sizeof...(T)>::type());
		}

	private:
        template<int ...> struct seq {};
        template<int N, int ...S> struct gens : gens<N-1, N-1, S...> {};
        template<int ...S> struct gens<0, S...>{ typedef seq<S...> type; };

        template<int ...S>
        void call_func(seq<S...>)
        {
            (*_cb)(std::get<S>(_t) ...);
        }

        std::shared_ptr<Callback<T...>> _cb;
		std::tuple<T...>                _t;
    
};

//==============================================================================
//==============================================================================

template<typename CLASS, typename... T>
inline std::shared_ptr<LatentCall> make_latent_call(CLASS c, void (TypeTraits<CLASS>::NonPtrType::*fn)(T...), T... t)
{
	return std::shared_ptr<LatentCall>(
				new LatentCallImpl<T...>(
					std::shared_ptr<Callback<T...>>(new ("Callback") CallbackImpl<CLASS,T...>(c, fn)),
					t...
				)
			);
}

template<typename... T>
inline std::shared_ptr<LatentCall> make_latent_call(void (*fn)(T...), T... t)
{
	return std::shared_ptr<LatentCall>(
				new LatentCallImpl<T...>(
					std::shared_ptr<Callback<T...>>(new ("Callback") CallbackStaticImpl<T...>(fn)),
					t...
				)
			);
}

//==============================================================================
//==============================================================================

} // DT3

#endif
