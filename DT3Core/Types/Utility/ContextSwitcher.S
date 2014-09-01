//==============================================================================
///	
///	File: ContextSwitcher.hpp
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

#if (defined(__i386__) || defined(__i386))
	#define DT3_NEED_INTEL32  1
    #define DT3_NEED_INTEL64  0
    #define DT3_NEED_ARM    0
#elif (defined(__x86_64__) || defined(__ia64__))
	#define DT3_NEED_INTEL32  0
    #define DT3_NEED_INTEL64  1
    #define DT3_NEED_ARM    0
#elif defined(__arm__) || defined(__arm)
	#define DT3_NEED_INTEL32  0
    #define DT3_NEED_INTEL64  0
    #define DT3_NEED_ARM    1
#endif


#if DT3_NEED_INTEL32
.globl _set_m_context
_set_m_context:
	movl	4(%esp), %eax

	movl	8(%eax), %fs
	movl	12(%eax), %es
	movl	16(%eax), %ds
	movl	76(%eax), %ss
	movl	20(%eax), %edi
	movl	24(%eax), %esi
	movl	28(%eax), %ebp
	movl	36(%eax), %ebx
	movl	40(%eax), %edx
	movl	44(%eax), %ecx

	movl	72(%eax), %esp
	pushl	60(%eax)	/* new %eip */
	movl	48(%eax), %eax
	ret

.globl _m_context
_m_context:
	movl	4(%esp), %eax

	movl	%fs, 8(%eax)
	movl	%es, 12(%eax)
	movl	%ds, 16(%eax)
	movl	%ss, 76(%eax)
	movl	%edi, 20(%eax)
	movl	%esi, 24(%eax)
	movl	%ebp, 28(%eax)
	movl	%ebx, 36(%eax)
	movl	%edx, 40(%eax)		 
	movl	%ecx, 44(%eax)

	movl	$1, 48(%eax)	/* %eax */
	movl	(%esp), %ecx	/* %eip */
	movl	%ecx, 60(%eax)
	leal	4(%esp), %ecx	/* %esp */
	movl	%ecx, 72(%eax)

	movl	44(%eax), %ecx	/* restore %ecx */
	movl	$0, %eax
	ret
#endif

#if DT3_NEED_INTEL64
.globl _set_m_context
_set_m_context:
	movq	16(%rdi), %rsi
	movq	24(%rdi), %rdx
	movq	32(%rdi), %rcx
	movq	40(%rdi), %r8
	movq	48(%rdi), %r9
	movq	56(%rdi), %rax
	movq	64(%rdi), %rbx
	movq	72(%rdi), %rbp
	movq	80(%rdi), %r10
	movq	88(%rdi), %r11
	movq	96(%rdi), %r12
	movq	104(%rdi), %r13
	movq	112(%rdi), %r14
	movq	120(%rdi), %r15
	movq	184(%rdi), %rsp
	pushq	160(%rdi)	/* new %eip */
	movq	8(%rdi), %rdi
	ret

.globl _m_context
_m_context:
	movq	%rdi, 8(%rdi)
	movq	%rsi, 16(%rdi)
	movq	%rdx, 24(%rdi)
	movq	%rcx, 32(%rdi)
	movq	%r8, 40(%rdi)
	movq	%r9, 48(%rdi)
	movq	$1, 56(%rdi)	/* %rax */
	movq	%rbx, 64(%rdi)
	movq	%rbp, 72(%rdi)
	movq	%r10, 80(%rdi)
	movq	%r11, 88(%rdi)
	movq	%r12, 96(%rdi)
	movq	%r13, 104(%rdi)
	movq	%r14, 112(%rdi)
	movq	%r15, 120(%rdi)

	movq	(%rsp), %rcx	/* %rip */
	movq	%rcx, 160(%rdi)
	leaq	8(%rsp), %rcx	/* %rsp */
	movq	%rcx, 184(%rdi)
	
	movq	32(%rdi), %rcx	/* restore %rcx */
	movq	$0, %rax
	ret
#endif

#if DT3_NEED_ARM
#if defined(__APPLE__)
.globl _set_m_context
_set_m_context:
#else
.globl set_m_context
set_m_context:
#endif
	ldr	r1, [r0,#4]
	ldr	r2, [r0,#8]
	ldr	r3, [r0,#12]
	ldr	r4, [r0,#16]
	ldr	r5, [r0,#20]
	ldr	r6, [r0,#24]
	ldr	r7, [r0,#28]
	ldr	r8, [r0,#32]
	ldr	r9, [r0,#36]
	ldr	r10, [r0,#40]
	ldr	r11, [r0,#44]
	ldr	r12, [r0,#48]
	ldr	r13, [r0,#52]
	ldr	r14, [r0,#56]

    vldr.64 d0,[r0,#60]
    vldr.64 d1,[r0,#68]
    vldr.64 d2,[r0,#76]
    vldr.64 d3,[r0,#84]
    vldr.64 d4,[r0,#92]
    vldr.64 d5,[r0,#100]
    vldr.64 d6,[r0,#108]
    vldr.64 d7,[r0,#116]
    vldr.64 d8,[r0,#124]
    vldr.64 d9,[r0,#132]
    vldr.64 d10,[r0,#140]
    vldr.64 d11,[r0,#148]
    vldr.64 d12,[r0,#156]
    vldr.64 d13,[r0,#164]
    vldr.64 d14,[r0,#172]
    vldr.64 d15,[r0,#180]

#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
    vldr.64 d16,[r0,#188]
    vldr.64 d17,[r0,#196]
    vldr.64 d18,[r0,#204]
    vldr.64 d19,[r0,#212]
    vldr.64 d20,[r0,#220]
    vldr.64 d21,[r0,#228]
    vldr.64 d22,[r0,#236]
    vldr.64 d23,[r0,#244]
    vldr.64 d24,[r0,#252]
    vldr.64 d25,[r0,#260]
    vldr.64 d26,[r0,#268]
    vldr.64 d27,[r0,#276]
    vldr.64 d28,[r0,#284]
    vldr.64 d29,[r0,#292]
    vldr.64 d30,[r0,#300]
    vldr.64 d31,[r0,#308]
#endif

	ldr	r0, [r0]
	mov	pc, lr

#if defined(__APPLE__)
.globl _m_context
_m_context:
#else
.globl m_context
m_context:
#endif
	str	r1, [r0,#4]
	str	r2, [r0,#8]
	str	r3, [r0,#12]
	str	r4, [r0,#16]
	str	r5, [r0,#20]
	str	r6, [r0,#24]
	str	r7, [r0,#28]
	str	r8, [r0,#32]
	str	r9, [r0,#36]
	str	r10, [r0,#40]
	str	r11, [r0,#44]
	str	r12, [r0,#48]
	str	r13, [r0,#52]
	str	r14, [r0,#56]

    vstr.64 d0,[r0,#60]
    vstr.64 d1,[r0,#68]
    vstr.64 d2,[r0,#76]
    vstr.64 d3,[r0,#84]
    vstr.64 d4,[r0,#92]
    vstr.64 d5,[r0,#100]
    vstr.64 d6,[r0,#108]
    vstr.64 d7,[r0,#116]
    vstr.64 d8,[r0,#124]
    vstr.64 d9,[r0,#132]
    vstr.64 d10,[r0,#140]
    vstr.64 d11,[r0,#148]
    vstr.64 d12,[r0,#156]
    vstr.64 d13,[r0,#164]
    vstr.64 d14,[r0,#172]
    vstr.64 d15,[r0,#180]

#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
    vstr.64 d16,[r0,#188]
    vstr.64 d17,[r0,#196]
    vstr.64 d18,[r0,#204]
    vstr.64 d19,[r0,#212]
    vstr.64 d20,[r0,#220]
    vstr.64 d21,[r0,#228]
    vstr.64 d22,[r0,#236]
    vstr.64 d23,[r0,#244]
    vstr.64 d24,[r0,#252]
    vstr.64 d25,[r0,#260]
    vstr.64 d26,[r0,#268]
    vstr.64 d27,[r0,#276]
    vstr.64 d28,[r0,#284]
    vstr.64 d29,[r0,#292]
    vstr.64 d30,[r0,#300]
    vstr.64 d31,[r0,#308]
#endif

	/* store 1 as r0-to-restore */
	mov	r1, #1
	str	r1, [r0]
	/* return 0 */
	mov	r0, #0
	mov	pc, lr

#endif

