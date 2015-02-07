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
 * include/asm-sh/cpu-sh3/timer.h
 *
 * Copyright (C) 2004 Lineo Solutions, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#ifndef __ASM_CPU_SH3_TIMER_H
#define __ASM_CPU_SH3_TIMER_H

/*
 * ---------------------------------------------------------------------------
 * TMU Common definitions for SH3 processors
 *	SH7706
 *	SH7709S
 *	SH7727
 *	SH7729R
 *	SH7710
 *	SH7720
 *	SH7710
 * ---------------------------------------------------------------------------
 */

#if  !defined(CONFIG_CPU_SUBTYPE_SH7720) && !defined(CONFIG_CPU_SUBTYPE_SH7721)
#define TMU_TOCR	0xfffffe90	/* Byte access */
#endif

#if defined(CONFIG_CPU_SUBTYPE_SH7710) || \
    defined(CONFIG_CPU_SUBTYPE_SH7720) || \
    defined(CONFIG_CPU_SUBTYPE_SH7721)
#define TMU_012_TSTR	0xa412fe92	/* Byte access */

#define TMU0_TCOR	0xa412fe94	/* Long access */
#define TMU0_TCNT	0xa412fe98	/* Long access */
#define TMU0_TCR	0xa412fe9c	/* Word access */

#define TMU1_TCOR	0xa412fea0	/* Long access */
#define TMU1_TCNT	0xa412fea4	/* Long access */
#define TMU1_TCR	0xa412fea8	/* Word access */

#define TMU2_TCOR	0xa412feac	/* Long access */
#define TMU2_TCNT	0xa412feb0	/* Long access */
#define TMU2_TCR	0xa412feb4	/* Word access */

#else
#define TMU_012_TSTR	0xfffffe92	/* Byte access */

#define TMU0_TCOR	0xfffffe94	/* Long access */
#define TMU0_TCNT	0xfffffe98	/* Long access */
#define TMU0_TCR	0xfffffe9c	/* Word access */

#define TMU1_TCOR	0xfffffea0	/* Long access */
#define TMU1_TCNT	0xfffffea4	/* Long access */
#define TMU1_TCR	0xfffffea8	/* Word access */

#define TMU2_TCOR	0xfffffeac	/* Long access */
#define TMU2_TCNT	0xfffffeb0	/* Long access */
#define TMU2_TCR	0xfffffeb4	/* Word access */
#if !defined(CONFIG_CPU_SUBTYPE_SH7720) && !defined(CONFIG_CPU_SUBTYPE_SH7721)
#define TMU2_TCPR2	0xfffffeb8	/* Long access */
#endif
#endif

#endif /* __ASM_CPU_SH3_TIMER_H */

