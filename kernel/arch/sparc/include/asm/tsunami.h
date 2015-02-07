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
 * tsunami.h:  Module specific definitions for Tsunami V8 Sparcs
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 */

#ifndef _SPARC_TSUNAMI_H
#define _SPARC_TSUNAMI_H

#include <asm/asi.h>

/* The MMU control register on the Tsunami:
 *
 * -----------------------------------------------------------------------
 * | implvers |SW|AV|DV|MV| RSV |PC|ITD|ALC| RSV |PE| RC |IE|DE|RSV|NF|ME|
 * -----------------------------------------------------------------------
 *  31      24 23 22 21 20 19-18 17  16 14  13-12 11 10-9  8  7 6-2  1  0
 *
 * SW: Enable Software Table Walks  0=off 1=on
 * AV: Address View bit
 * DV: Data View bit
 * MV: Memory View bit
 * PC: Parity Control
 * ITD: ITBR disable
 * ALC: Alternate Cacheable
 * PE: Parity Enable   0=off 1=on
 * RC: Refresh Control
 * IE: Instruction cache Enable  0=off 1=on
 * DE: Data cache Enable  0=off 1=on
 * NF: No Fault, same as all other SRMMUs
 * ME: MMU Enable, same as all other SRMMUs
 */

#define TSUNAMI_SW        0x00800000
#define TSUNAMI_AV        0x00400000
#define TSUNAMI_DV        0x00200000
#define TSUNAMI_MV        0x00100000
#define TSUNAMI_PC        0x00020000
#define TSUNAMI_ITD       0x00010000
#define TSUNAMI_ALC       0x00008000
#define TSUNAMI_PE        0x00001000
#define TSUNAMI_RCMASK    0x00000C00
#define TSUNAMI_IENAB     0x00000200
#define TSUNAMI_DENAB     0x00000100
#define TSUNAMI_NF        0x00000002
#define TSUNAMI_ME        0x00000001

static inline void tsunami_flush_icache(void)
{
	__asm__ __volatile__("sta %%g0, [%%g0] %0\n\t"
			     : /* no outputs */
			     : "i" (ASI_M_IC_FLCLEAR)
			     : "memory");
}

static inline void tsunami_flush_dcache(void)
{
	__asm__ __volatile__("sta %%g0, [%%g0] %0\n\t"
			     : /* no outputs */
			     : "i" (ASI_M_DC_FLCLEAR)
			     : "memory");
}

#endif /* !(_SPARC_TSUNAMI_H) */
