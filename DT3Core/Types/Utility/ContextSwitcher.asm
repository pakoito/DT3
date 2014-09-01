;==============================================================================
;	
;	File: 			ContextSwitcher.hpp
;	
;   Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
;
;   This file is subject to the terms and conditions defined in
;   file 'LICENSE.txt', which is part of this source code package.
;	
;==============================================================================
;
;   Based heavily on the cross platform Context routines from libtask
;   Portions copyright Copyright (c) 2005-2006 Russ Cox, MIT
;
;==============================================================================

.586              ;Target processor.  Use instructions for Pentium class machines

.MODEL FLAT, C    ;Use the flat memory model. Use C calling conventions

.STACK            ;Define a stack segment of 1KB

.DATA             ;Create a near data segment.

.CODE             ;Indicates the start of a code segment.

setMContext PROC
	mov	eax,[esp+4]

	mov	fs, [eax+8]
	mov	es, [eax+12]
	mov	ds, [eax+16]
	mov	ss, [eax+76]
	mov	edi, [eax+20]
	mov	esi, [eax+24]
	mov	ebp, [eax+28]
	mov	ebx, [eax+36]
	mov	edx, [eax+40]
	mov	ecx, [eax+44]

	mov	esp, [eax+72]
	push	[eax+60]	; new eip
	mov	eax, [eax+48]
	ret
setMContext ENDP

getMContext PROC
	mov	eax, [esp+4]

	mov	[eax+8], fs
	mov	[eax+12], es
	mov	[eax+16], ds 
	mov	[eax+76], ss
	mov	[eax+20], edi
	mov	[eax+24], esi
	mov	[eax+28], ebp
	mov	[eax+36], ebx
	mov	[eax+40], edx		 
	mov	[eax+44], ecx

	mov	DWORD PTR [eax+48], 1 	; eax
	mov	ecx, [esp]	; eip
	mov	[eax+60], ecx
	lea	ecx,[esp+4]	; esp
	mov	[eax+72], ecx 

	mov	ecx,[eax+44]	; restore ecx
	mov	eax, 0
	ret
getMContext ENDP

END