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
 * Hardware info common to DECstation 5000/1xx systems (otherwise
 * known as 3min or kn02ba) and Personal DECstations 5000/xx ones
 * (otherwise known as maxine or kn02ca).
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1995,1996 by Paul M. Antoine, some code and definitions
 * are by courtesy of Chris Fraser.
 * Copyright (C) 2000, 2002, 2003, 2005  Maciej W. Rozycki
 *
 * These are addresses which have to be known early in the boot process.
 * For other addresses refer to tc.h, ioasic_addrs.h and friends.
 */
#ifndef __ASM_MIPS_DEC_KN02XA_H
#define __ASM_MIPS_DEC_KN02XA_H

#include <asm/dec/ioasic_addrs.h>

#define KN02XA_SLOT_BASE	0x1c000000

/*
 * Memory control ASIC registers.
 */
#define KN02XA_MER		0x0c400000	/* memory error register */
#define KN02XA_MSR		0x0c800000	/* memory size register */

/*
 * CPU control ASIC registers.
 */
#define KN02XA_MEM_CONF		0x0e000000	/* write timeout config */
#define KN02XA_EAR		0x0e000004	/* error address register */
#define KN02XA_BOOT0		0x0e000008	/* boot 0 register */
#define KN02XA_MEM_INTR		0x0e00000c	/* write err IRQ stat & ack */

/*
 * Memory Error Register bits, common definitions.
 * The rest is defined in system-specific headers.
 */
#define KN02XA_MER_RES_28	(0xf<<28)	/* unused */
#define KN02XA_MER_RES_17	(0x3ff<<17)	/* unused */
#define KN02XA_MER_PAGERR	(1<<16)		/* 2k page boundary error */
#define KN02XA_MER_TRANSERR	(1<<15)		/* transfer length error */
#define KN02XA_MER_PARDIS	(1<<14)		/* parity error disable */
#define KN02XA_MER_SIZE		(1<<13)		/* r/o mirror of MSR_SIZE */
#define KN02XA_MER_RES_12	(1<<12)		/* unused */
#define KN02XA_MER_BYTERR	(0xf<<8)	/* byte lane error bitmask: */
#define KN02XA_MER_BYTERR_3	(0x8<<8)	/* byte lane #3 */
#define KN02XA_MER_BYTERR_2	(0x4<<8)	/* byte lane #2 */
#define KN02XA_MER_BYTERR_1	(0x2<<8)	/* byte lane #1 */
#define KN02XA_MER_BYTERR_0	(0x1<<8)	/* byte lane #0 */
#define KN02XA_MER_RES_0	(0xff<<0)	/* unused */

/*
 * Memory Size Register bits, common definitions.
 * The rest is defined in system-specific headers.
 */
#define KN02XA_MSR_RES_27	(0x1f<<27)	/* unused */
#define KN02XA_MSR_RES_14	(0x7<<14)	/* unused */
#define KN02XA_MSR_SIZE		(1<<13)		/* 16M/4M stride */
#define KN02XA_MSR_RES_0	(0x1fff<<0)	/* unused */

/*
 * Error Address Register bits.
 */
#define KN02XA_EAR_RES_29	(0x7<<29)	/* unused */
#define KN02XA_EAR_ADDRESS	(0x7ffffff<<2)	/* address involved */
#define KN02XA_EAR_RES_0	(0x3<<0)	/* unused */


#ifndef __ASSEMBLY__

#include <linux/interrupt.h>

struct pt_regs;

extern void dec_kn02xa_be_init(void);
extern int dec_kn02xa_be_handler(struct pt_regs *regs, int is_fixup);
extern irqreturn_t dec_kn02xa_be_interrupt(int irq, void *dev_id);
#endif

#endif /* __ASM_MIPS_DEC_KN02XA_H */
