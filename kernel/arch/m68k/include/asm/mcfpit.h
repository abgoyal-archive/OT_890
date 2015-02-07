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

/****************************************************************************/

/*
 *	mcfpit.h -- ColdFire internal PIT timer support defines.
 *
 *	(C) Copyright 2003, Greg Ungerer (gerg@snapgear.com)
 */

/****************************************************************************/
#ifndef	mcfpit_h
#define	mcfpit_h
/****************************************************************************/


/*
 *	Get address specific defines for the 5270/5271, 5280/5282, and 5208.
 */
#if defined(CONFIG_M520x)
#define	MCFPIT_BASE1		0x00080000	/* Base address of TIMER1 */
#define	MCFPIT_BASE2		0x00084000	/* Base address of TIMER2 */
#else
#define	MCFPIT_BASE1		0x00150000	/* Base address of TIMER1 */
#define	MCFPIT_BASE2		0x00160000	/* Base address of TIMER2 */
#define	MCFPIT_BASE3		0x00170000	/* Base address of TIMER3 */
#define	MCFPIT_BASE4		0x00180000	/* Base address of TIMER4 */
#endif

/*
 *	Define the PIT timer register set addresses.
 */
#define	MCFPIT_PCSR		0x0		/* PIT control register */
#define	MCFPIT_PMR		0x2		/* PIT modulus register */
#define	MCFPIT_PCNTR		0x4		/* PIT count register */

/*
 *	Bit definitions for the PIT Control and Status register.
 */
#define	MCFPIT_PCSR_CLK1	0x0000		/* System clock divisor */
#define	MCFPIT_PCSR_CLK2	0x0100		/* System clock divisor */
#define	MCFPIT_PCSR_CLK4	0x0200		/* System clock divisor */
#define	MCFPIT_PCSR_CLK8	0x0300		/* System clock divisor */
#define	MCFPIT_PCSR_CLK16	0x0400		/* System clock divisor */
#define	MCFPIT_PCSR_CLK32	0x0500		/* System clock divisor */
#define	MCFPIT_PCSR_CLK64	0x0600		/* System clock divisor */
#define	MCFPIT_PCSR_CLK128	0x0700		/* System clock divisor */
#define	MCFPIT_PCSR_CLK256	0x0800		/* System clock divisor */
#define	MCFPIT_PCSR_CLK512	0x0900		/* System clock divisor */
#define	MCFPIT_PCSR_CLK1024	0x0a00		/* System clock divisor */
#define	MCFPIT_PCSR_CLK2048	0x0b00		/* System clock divisor */
#define	MCFPIT_PCSR_CLK4096	0x0c00		/* System clock divisor */
#define	MCFPIT_PCSR_CLK8192	0x0d00		/* System clock divisor */
#define	MCFPIT_PCSR_CLK16384	0x0e00		/* System clock divisor */
#define	MCFPIT_PCSR_CLK32768	0x0f00		/* System clock divisor */
#define	MCFPIT_PCSR_DOZE	0x0040		/* Clock run in doze mode */
#define	MCFPIT_PCSR_HALTED	0x0020		/* Clock run in halt mode */
#define	MCFPIT_PCSR_OVW		0x0010		/* Overwrite PIT counter now */
#define	MCFPIT_PCSR_PIE		0x0008		/* Enable PIT interrupt */
#define	MCFPIT_PCSR_PIF		0x0004		/* PIT interrupt flag */
#define	MCFPIT_PCSR_RLD		0x0002		/* Reload counter */
#define	MCFPIT_PCSR_EN		0x0001		/* Enable PIT */
#define	MCFPIT_PCSR_DISABLE	0x0000		/* Disable PIT */

/****************************************************************************/
#endif	/* mcfpit_h */
