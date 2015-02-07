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
 * include/asm-sh/cpu-sh4/timer.h
 *
 * Copyright (C) 2004 Lineo Solutions, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#ifndef __ASM_CPU_SH4_TIMER_H
#define __ASM_CPU_SH4_TIMER_H

/*
 * ---------------------------------------------------------------------------
 * TMU Common definitions for SH4 processors
 *	SH7750S/SH7750R
 *	SH7751/SH7751R
 *	SH7760
 *	SH-X3
 * ---------------------------------------------------------------------------
 */
#ifdef CONFIG_CPU_SUBTYPE_SHX3
#define TMU_012_BASE	0xffc10000
#define TMU_345_BASE	0xffc20000
#else
#define TMU_012_BASE	0xffd80000
#define TMU_345_BASE	0xfe100000
#endif

#define TMU_TOCR	TMU_012_BASE	/* Not supported on all CPUs */

#define TMU_012_TSTR	(TMU_012_BASE + 0x04)
#define TMU_345_TSTR	(TMU_345_BASE + 0x04)

#define TMU0_TCOR	(TMU_012_BASE + 0x08)
#define TMU0_TCNT	(TMU_012_BASE + 0x0c)
#define TMU0_TCR	(TMU_012_BASE + 0x10)

#define TMU1_TCOR       (TMU_012_BASE + 0x14)
#define TMU1_TCNT       (TMU_012_BASE + 0x18)
#define TMU1_TCR        (TMU_012_BASE + 0x1c)

#define TMU2_TCOR       (TMU_012_BASE + 0x20)
#define TMU2_TCNT       (TMU_012_BASE + 0x24)
#define TMU2_TCR	(TMU_012_BASE + 0x28)
#define TMU2_TCPR	(TMU_012_BASE + 0x2c)

#define TMU3_TCOR	(TMU_345_BASE + 0x08)
#define TMU3_TCNT	(TMU_345_BASE + 0x0c)
#define TMU3_TCR	(TMU_345_BASE + 0x10)

#define TMU4_TCOR	(TMU_345_BASE + 0x14)
#define TMU4_TCNT	(TMU_345_BASE + 0x18)
#define TMU4_TCR	(TMU_345_BASE + 0x1c)

#define TMU5_TCOR	(TMU_345_BASE + 0x20)
#define TMU5_TCNT	(TMU_345_BASE + 0x24)
#define TMU5_TCR	(TMU_345_BASE + 0x28)

#endif /* __ASM_CPU_SH4_TIMER_H */
