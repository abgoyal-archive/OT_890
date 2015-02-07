/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/*
 * kdebug.h:  Defines and definitions for debugging the Linux kernel
 *            under various kernel debuggers.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 */
#ifndef _SPARC_KDEBUG_H
#define _SPARC_KDEBUG_H

#include <asm/openprom.h>
#include <asm/vaddrs.h>

/* Breakpoints are enter through trap table entry 126.  So in sparc assembly
 * if you want to drop into the debugger you do:
 *
 * t DEBUG_BP_TRAP
 */

#define DEBUG_BP_TRAP     126

#ifndef __ASSEMBLY__
/* The debug vector is passed in %o1 at boot time.  It is a pointer to
 * a structure in the debuggers address space.  Here is its format.
 */

typedef unsigned int (*debugger_funct)(void);

struct kernel_debug {
	/* First the entry point into the debugger.  You jump here
	 * to give control over to the debugger.
	 */
	unsigned long kdebug_entry;
	unsigned long kdebug_trapme;   /* Figure out later... */
	/* The following is the number of pages that the debugger has
	 * taken from to total pool.
	 */
	unsigned long *kdebug_stolen_pages;
	/* Ok, after you remap yourself and/or change the trap table
	 * from what you were left with at boot time you have to call
	 * this synchronization function so the debugger can check out
	 * what you have done.
	 */
	debugger_funct teach_debugger;
}; /* I think that is it... */

extern struct kernel_debug *linux_dbvec;

/* Use this macro in C-code to enter the debugger. */
static inline void sp_enter_debugger(void)
{
	__asm__ __volatile__("jmpl %0, %%o7\n\t"
			     "nop\n\t" : :
			     "r" (linux_dbvec) : "o7", "memory");
}

#define SP_ENTER_DEBUGGER do { \
	     if((linux_dbvec!=0) && ((*(short *)linux_dbvec)!=-1)) \
	       sp_enter_debugger(); \
		       } while(0)

enum die_val {
	DIE_UNUSED,
	DIE_OOPS,
};

#endif /* !(__ASSEMBLY__) */

/* Some nice offset defines for assembler code. */
#define KDEBUG_ENTRY_OFF    0x0
#define KDEBUG_DUNNO_OFF    0x4
#define KDEBUG_DUNNO2_OFF   0x8
#define KDEBUG_TEACH_OFF    0xc

#endif /* !(_SPARC_KDEBUG_H) */
