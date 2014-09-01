//==============================================================================
///	
///	File: ContextSwitcher.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================
///
/// Based heavily on the cross platform Context routines from libtask
/// Portions copyright Copyright (c) 2005-2006 Russ Cox, MIT
///
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/ContextSwitcher.hpp"
#include "DT3Core/Types/Utility/Assert.hpp"
#include <cstdarg>
#include <cstring>

//==============================================================================
//==============================================================================

extern "C" {
    extern	int     m_context(DT3::mcontext*);
    extern	void    set_m_context(const DT3::mcontext*);
}

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

DTint ContextSwitcher::swap_context (uContext *oucp, const uContext *ucp)
{
	if(context(oucp) == 0)
		set_context(ucp);
	return 0;
}

void ContextSwitcher::make_context(uContext *ucp, void (*func)(void*), void *data)
{
#if DT3_CPU == DT3_INTEL && DT3_POINTER_SIZE == DT3_32_BIT
	::memset(&ucp->uc_mcontext, 0, sizeof(ucp->uc_mcontext));

	DTuint *sp = (DTuint*)ucp->uc_stack_sp + ucp->uc_stack_ss/sizeof(void*);
	sp -= 1;
	sp = (DTuint*)((DTuint)sp - (DTuint)sp%16);	/* 16-align for OS X */
    
    *sp = (DTuint64) data;
    ucp->uc_mcontext.gregs[1] = *sp;

	*--sp = 0;		// return address
	ucp->uc_mcontext.gregs[15] = (DTuint)func;    // eip
	ucp->uc_mcontext.gregs[18] = (DTuint)sp;      // esp
    
#elif DT3_CPU == DT3_INTEL && DT3_POINTER_SIZE == DT3_64_BIT
	::memset(&ucp->uc_mcontext, 0, sizeof(ucp->uc_mcontext));

	DTuint64 *sp = (DTuint64*)ucp->uc_stack_sp+ucp->uc_stack_ss/sizeof(void*);
	sp -= 1;
	sp = (DTuint64*)((DTuint64)sp - (DTuint64)sp%16);	/* 16-align for OS X */
    
    *sp = (DTuint64) data;
    ucp->uc_mcontext.gregs[1] = *sp;

	*--sp = 0;	// return address
	ucp->uc_mcontext.gregs[20] = (DTuint64)func;
	ucp->uc_mcontext.gregs[23] = (DTuint64)sp;

#elif DT3_CPU == DT3_ARM
	DTuint *sp;
	
	sp = (DTuint*)ucp->uc_stack_sp+ucp->uc_stack_ss/sizeof(void*);
    ucp->uc_mcontext.gregs[0] = (DTuint) data;

	ucp->uc_mcontext.gregs[13] = (DTuint)sp;
	ucp->uc_mcontext.gregs[14] = (DTuint)func;
    
#else
    //#error Not available on this architecture yet!!
#endif

}

DTint ContextSwitcher::context(uContext*ucp)
{
    return m_context( &(ucp->uc_mcontext) );
}

void ContextSwitcher::set_context(const uContext*ucp)
{
    set_m_context( &(ucp->uc_mcontext) );
}

//==============================================================================
//==============================================================================

} // DT3

