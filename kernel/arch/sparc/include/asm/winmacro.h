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
 * winmacro.h: Window loading-unloading macros.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 */

#ifndef _SPARC_WINMACRO_H
#define _SPARC_WINMACRO_H

#include <asm/ptrace.h>

/* Store the register window onto the 8-byte aligned area starting
 * at %reg.  It might be %sp, it might not, we don't care.
 */
#define STORE_WINDOW(reg) \
	std	%l0, [%reg + RW_L0]; \
	std	%l2, [%reg + RW_L2]; \
	std	%l4, [%reg + RW_L4]; \
	std	%l6, [%reg + RW_L6]; \
	std	%i0, [%reg + RW_I0]; \
	std	%i2, [%reg + RW_I2]; \
	std	%i4, [%reg + RW_I4]; \
	std	%i6, [%reg + RW_I6];

/* Load a register window from the area beginning at %reg. */
#define LOAD_WINDOW(reg) \
	ldd	[%reg + RW_L0], %l0; \
	ldd	[%reg + RW_L2], %l2; \
	ldd	[%reg + RW_L4], %l4; \
	ldd	[%reg + RW_L6], %l6; \
	ldd	[%reg + RW_I0], %i0; \
	ldd	[%reg + RW_I2], %i2; \
	ldd	[%reg + RW_I4], %i4; \
	ldd	[%reg + RW_I6], %i6;

/* Loading and storing struct pt_reg trap frames. */
#define LOAD_PT_INS(base_reg) \
        ldd     [%base_reg + STACKFRAME_SZ + PT_I0], %i0; \
        ldd     [%base_reg + STACKFRAME_SZ + PT_I2], %i2; \
        ldd     [%base_reg + STACKFRAME_SZ + PT_I4], %i4; \
        ldd     [%base_reg + STACKFRAME_SZ + PT_I6], %i6;

#define LOAD_PT_GLOBALS(base_reg) \
        ld      [%base_reg + STACKFRAME_SZ + PT_G1], %g1; \
        ldd     [%base_reg + STACKFRAME_SZ + PT_G2], %g2; \
        ldd     [%base_reg + STACKFRAME_SZ + PT_G4], %g4; \
        ldd     [%base_reg + STACKFRAME_SZ + PT_G6], %g6;

#define LOAD_PT_YREG(base_reg, scratch) \
        ld      [%base_reg + STACKFRAME_SZ + PT_Y], %scratch; \
        wr      %scratch, 0x0, %y;

#define LOAD_PT_PRIV(base_reg, pt_psr, pt_pc, pt_npc) \
        ld      [%base_reg + STACKFRAME_SZ + PT_PSR], %pt_psr; \
        ld      [%base_reg + STACKFRAME_SZ + PT_PC], %pt_pc; \
        ld      [%base_reg + STACKFRAME_SZ + PT_NPC], %pt_npc;

#define LOAD_PT_ALL(base_reg, pt_psr, pt_pc, pt_npc, scratch) \
        LOAD_PT_YREG(base_reg, scratch) \
        LOAD_PT_INS(base_reg) \
        LOAD_PT_GLOBALS(base_reg) \
        LOAD_PT_PRIV(base_reg, pt_psr, pt_pc, pt_npc)

#define STORE_PT_INS(base_reg) \
        std     %i0, [%base_reg + STACKFRAME_SZ + PT_I0]; \
        std     %i2, [%base_reg + STACKFRAME_SZ + PT_I2]; \
        std     %i4, [%base_reg + STACKFRAME_SZ + PT_I4]; \
        std     %i6, [%base_reg + STACKFRAME_SZ + PT_I6];

#define STORE_PT_GLOBALS(base_reg) \
        st      %g1, [%base_reg + STACKFRAME_SZ + PT_G1]; \
        std     %g2, [%base_reg + STACKFRAME_SZ + PT_G2]; \
        std     %g4, [%base_reg + STACKFRAME_SZ + PT_G4]; \
        std     %g6, [%base_reg + STACKFRAME_SZ + PT_G6];

#define STORE_PT_YREG(base_reg, scratch) \
        rd      %y, %scratch; \
        st      %scratch, [%base_reg + STACKFRAME_SZ + PT_Y];

#define STORE_PT_PRIV(base_reg, pt_psr, pt_pc, pt_npc) \
        st      %pt_psr, [%base_reg + STACKFRAME_SZ + PT_PSR]; \
        st      %pt_pc,  [%base_reg + STACKFRAME_SZ + PT_PC]; \
        st      %pt_npc, [%base_reg + STACKFRAME_SZ + PT_NPC];

#define STORE_PT_ALL(base_reg, reg_psr, reg_pc, reg_npc, g_scratch) \
        STORE_PT_PRIV(base_reg, reg_psr, reg_pc, reg_npc) \
        STORE_PT_GLOBALS(base_reg) \
        STORE_PT_YREG(base_reg, g_scratch) \
        STORE_PT_INS(base_reg)

#define SAVE_BOLIXED_USER_STACK(cur_reg, scratch) \
        ld       [%cur_reg + TI_W_SAVED], %scratch; \
        sll      %scratch, 2, %scratch; \
        add      %scratch, %cur_reg, %scratch; \
        st       %sp, [%scratch + TI_RWIN_SPTRS]; \
        sub      %scratch, %cur_reg, %scratch; \
        sll      %scratch, 4, %scratch; \
        add      %scratch, %cur_reg, %scratch; \
        STORE_WINDOW(scratch + TI_REG_WINDOW); \
        sub      %scratch, %cur_reg, %scratch; \
        srl      %scratch, 6, %scratch; \
        add      %scratch, 1, %scratch; \
        st       %scratch, [%cur_reg + TI_W_SAVED];

#ifdef CONFIG_SMP
#define LOAD_CURRENT4M(dest_reg, idreg) \
        rd       %tbr, %idreg; \
	sethi    %hi(current_set), %dest_reg; \
        srl      %idreg, 10, %idreg; \
	or       %dest_reg, %lo(current_set), %dest_reg; \
	and      %idreg, 0xc, %idreg; \
	ld       [%idreg + %dest_reg], %dest_reg;

#define LOAD_CURRENT4D(dest_reg, idreg) \
	lda	 [%g0] ASI_M_VIKING_TMP1, %idreg; \
	sethi	%hi(C_LABEL(current_set)), %dest_reg; \
	sll	%idreg, 2, %idreg; \
	or	%dest_reg, %lo(C_LABEL(current_set)), %dest_reg; \
	ld	[%idreg + %dest_reg], %dest_reg;

/* Blackbox - take care with this... - check smp4m and smp4d before changing this. */
#define LOAD_CURRENT(dest_reg, idreg) 					\
	sethi	 %hi(___b_load_current), %idreg;			\
	sethi    %hi(current_set), %dest_reg; 			\
	sethi    %hi(boot_cpu_id4), %idreg; 			\
	or       %dest_reg, %lo(current_set), %dest_reg; 	\
	ldub	 [%idreg + %lo(boot_cpu_id4)], %idreg;		\
	ld       [%idreg + %dest_reg], %dest_reg;
#else
#define LOAD_CURRENT(dest_reg, idreg) \
        sethi    %hi(current_set), %idreg; \
        ld       [%idreg + %lo(current_set)], %dest_reg;
#endif

#endif /* !(_SPARC_WINMACRO_H) */
