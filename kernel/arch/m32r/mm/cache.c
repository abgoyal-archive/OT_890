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
 *  linux/arch/m32r/mm/cache.c
 *
 *  Copyright (C) 2002-2005  Hirokazu Takata, Hayato Fujiwara
 */

#include <asm/pgtable.h>

#undef MCCR

#if defined(CONFIG_CHIP_XNUX2) || defined(CONFIG_CHIP_M32700) \
	|| defined(CONFIG_CHIP_VDEC2) || defined(CONFIG_CHIP_OPSP)
/* Cache Control Register */
#define MCCR		((volatile unsigned long*)0xfffffffc)
#define MCCR_CC		(1UL << 7)	/* Cache mode modify bit */
#define MCCR_IIV	(1UL << 6)	/* I-cache invalidate */
#define MCCR_DIV	(1UL << 5)	/* D-cache invalidate */
#define MCCR_DCB	(1UL << 4)	/* D-cache copy back */
#define MCCR_ICM	(1UL << 1)	/* I-cache mode [0:off,1:on] */
#define MCCR_DCM	(1UL << 0)	/* D-cache mode [0:off,1:on] */
#define MCCR_ICACHE_INV		(MCCR_CC|MCCR_IIV)
#define MCCR_DCACHE_CB		(MCCR_CC|MCCR_DCB)
#define MCCR_DCACHE_CBINV	(MCCR_CC|MCCR_DIV|MCCR_DCB)
#define CHECK_MCCR(mccr)	(mccr = *MCCR)
#elif defined(CONFIG_CHIP_M32102)
#define MCCR		((volatile unsigned char*)0xfffffffe)
#define MCCR_IIV	(1UL << 0)	/* I-cache invalidate */
#define MCCR_ICACHE_INV		MCCR_IIV
#elif defined(CONFIG_CHIP_M32104)
#define MCCR		((volatile unsigned short*)0xfffffffe)
#define MCCR_IIV	(1UL << 8)	/* I-cache invalidate */
#define MCCR_DIV	(1UL << 9)	/* D-cache invalidate */
#define MCCR_DCB	(1UL << 10)	/* D-cache copy back */
#define MCCR_ICM	(1UL << 0)	/* I-cache mode [0:off,1:on] */
#define MCCR_DCM	(1UL << 1)	/* D-cache mode [0:off,1:on] */
#define MCCR_ICACHE_INV		MCCR_IIV
#define MCCR_DCACHE_CB		MCCR_DCB
#define MCCR_DCACHE_CBINV	(MCCR_DIV|MCCR_DCB)
#endif

#ifndef MCCR
#error Unknown cache type.
#endif


/* Copy back and invalidate D-cache and invalidate I-cache all */
void _flush_cache_all(void)
{
#if defined(CONFIG_CHIP_M32102)
	unsigned char mccr;
	*MCCR = MCCR_ICACHE_INV;
#elif defined(CONFIG_CHIP_M32104)
	unsigned short mccr;

	/* Copyback and invalidate D-cache */
	/* Invalidate I-cache */
	*MCCR |= (MCCR_ICACHE_INV | MCCR_DCACHE_CBINV);
#else
	unsigned long mccr;

	/* Copyback and invalidate D-cache */
	/* Invalidate I-cache */
	*MCCR = MCCR_ICACHE_INV | MCCR_DCACHE_CBINV;
#endif
	while ((mccr = *MCCR) & MCCR_IIV); /* loop while invalidating... */
}

/* Copy back D-cache and invalidate I-cache all */
void _flush_cache_copyback_all(void)
{
#if defined(CONFIG_CHIP_M32102)
	unsigned char mccr;
	*MCCR = MCCR_ICACHE_INV;
#elif defined(CONFIG_CHIP_M32104)
	unsigned short mccr;

	/* Copyback and invalidate D-cache */
	/* Invalidate I-cache */
	*MCCR |= (MCCR_ICACHE_INV | MCCR_DCACHE_CB);
#else
	unsigned long mccr;

	/* Copyback D-cache */
	/* Invalidate I-cache */
	*MCCR = MCCR_ICACHE_INV | MCCR_DCACHE_CB;
#endif
	while ((mccr = *MCCR) & MCCR_IIV); /* loop while invalidating... */
}
