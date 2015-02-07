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
 * include/asm-mips/txx9tmr.h
 * TX39/TX49 timer controller definitions.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#ifndef __ASM_TXX9TMR_H
#define __ASM_TXX9TMR_H

#include <linux/types.h>

struct txx9_tmr_reg {
	u32 tcr;
	u32 tisr;
	u32 cpra;
	u32 cprb;
	u32 itmr;
	u32 unused0[3];
	u32 ccdr;
	u32 unused1[3];
	u32 pgmr;
	u32 unused2[3];
	u32 wtmr;
	u32 unused3[43];
	u32 trr;
};

/* TMTCR : Timer Control */
#define TXx9_TMTCR_TCE		0x00000080
#define TXx9_TMTCR_CCDE		0x00000040
#define TXx9_TMTCR_CRE		0x00000020
#define TXx9_TMTCR_ECES		0x00000008
#define TXx9_TMTCR_CCS		0x00000004
#define TXx9_TMTCR_TMODE_MASK	0x00000003
#define TXx9_TMTCR_TMODE_ITVL	0x00000000
#define TXx9_TMTCR_TMODE_PGEN	0x00000001
#define TXx9_TMTCR_TMODE_WDOG	0x00000002

/* TMTISR : Timer Int. Status */
#define TXx9_TMTISR_TPIBS	0x00000004
#define TXx9_TMTISR_TPIAS	0x00000002
#define TXx9_TMTISR_TIIS	0x00000001

/* TMITMR : Interval Timer Mode */
#define TXx9_TMITMR_TIIE	0x00008000
#define TXx9_TMITMR_TZCE	0x00000001

/* TMWTMR : Watchdog Timer Mode */
#define TXx9_TMWTMR_TWIE	0x00008000
#define TXx9_TMWTMR_WDIS	0x00000080
#define TXx9_TMWTMR_TWC		0x00000001

void txx9_clocksource_init(unsigned long baseaddr,
			   unsigned int imbusclk);
void txx9_clockevent_init(unsigned long baseaddr, int irq,
			  unsigned int imbusclk);
void txx9_tmr_init(unsigned long baseaddr);

#ifdef CONFIG_CPU_TX39XX
#define TXX9_TIMER_BITS	24
#else
#define TXX9_TIMER_BITS	32
#endif

#endif /* __ASM_TXX9TMR_H */
