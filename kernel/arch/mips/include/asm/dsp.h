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
 * Copyright (C) 2005 Mips Technologies
 * Author: Chris Dearman, chris@mips.com derived from fpu.h
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */
#ifndef _ASM_DSP_H
#define _ASM_DSP_H

#include <asm/cpu.h>
#include <asm/cpu-features.h>
#include <asm/hazards.h>
#include <asm/mipsregs.h>

#define DSP_DEFAULT	0x00000000
#define DSP_MASK	0x3ff

#define __enable_dsp_hazard()						\
do {									\
	asm("_ehb");							\
} while (0)

static inline void __init_dsp(void)
{
	mthi1(0);
	mtlo1(0);
	mthi2(0);
	mtlo2(0);
	mthi3(0);
	mtlo3(0);
	wrdsp(DSP_DEFAULT, DSP_MASK);
}

static inline void init_dsp(void)
{
	if (cpu_has_dsp)
		__init_dsp();
}

#define __save_dsp(tsk)							\
do {									\
	tsk->thread.dsp.dspr[0] = mfhi1();				\
	tsk->thread.dsp.dspr[1] = mflo1();				\
	tsk->thread.dsp.dspr[2] = mfhi2();				\
	tsk->thread.dsp.dspr[3] = mflo2();				\
	tsk->thread.dsp.dspr[4] = mfhi3();				\
	tsk->thread.dsp.dspr[5] = mflo3();				\
	tsk->thread.dsp.dspcontrol = rddsp(DSP_MASK);			\
} while (0)

#define save_dsp(tsk)							\
do {									\
	if (cpu_has_dsp)						\
		__save_dsp(tsk);					\
} while (0)

#define __restore_dsp(tsk)						\
do {									\
	mthi1(tsk->thread.dsp.dspr[0]);					\
	mtlo1(tsk->thread.dsp.dspr[1]);					\
	mthi2(tsk->thread.dsp.dspr[2]);					\
	mtlo2(tsk->thread.dsp.dspr[3]);					\
	mthi3(tsk->thread.dsp.dspr[4]);					\
	mtlo3(tsk->thread.dsp.dspr[5]);					\
	wrdsp(tsk->thread.dsp.dspcontrol, DSP_MASK);			\
} while (0)

#define restore_dsp(tsk)						\
do {									\
	if (cpu_has_dsp)						\
		__restore_dsp(tsk);					\
} while (0)

#define __get_dsp_regs(tsk)						\
({									\
	if (tsk == current)						\
		__save_dsp(current);					\
									\
	tsk->thread.dsp.dspr;						\
})

#endif /* _ASM_DSP_H */
