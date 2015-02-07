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
 *	nettel.h -- Lineo (formerly Moreton Bay) NETtel support.
 *
 *	(C) Copyright 1999-2000, Moreton Bay (www.moretonbay.com)
 * 	(C) Copyright 2000-2001, Lineo Inc. (www.lineo.com) 
 * 	(C) Copyright 2001-2002, SnapGear Inc., (www.snapgear.com) 
 */

/****************************************************************************/
#ifndef	nettel_h
#define	nettel_h
/****************************************************************************/


/****************************************************************************/
#ifdef CONFIG_NETtel
/****************************************************************************/

#ifdef CONFIG_COLDFIRE
#include <asm/coldfire.h>
#include <asm/mcfsim.h>
#endif

/*---------------------------------------------------------------------------*/
#if defined(CONFIG_M5307)
/*
 *	NETtel/5307 based hardware first. DTR/DCD lines are wired to
 *	GPIO lines. Most of the LED's are driver through a latch
 *	connected to CS2.
 */
#define	MCFPP_DCD1	0x0001
#define	MCFPP_DCD0	0x0002
#define	MCFPP_DTR1	0x0004
#define	MCFPP_DTR0	0x0008

#define	NETtel_LEDADDR	0x30400000

#ifndef __ASSEMBLY__

extern volatile unsigned short ppdata;

/*
 *	These functions defined to give quasi generic access to the
 *	PPIO bits used for DTR/DCD.
 */
static __inline__ unsigned int mcf_getppdata(void)
{
	volatile unsigned short *pp;
	pp = (volatile unsigned short *) (MCF_MBAR + MCFSIM_PADAT);
	return((unsigned int) *pp);
}

static __inline__ void mcf_setppdata(unsigned int mask, unsigned int bits)
{
	volatile unsigned short *pp;
	pp = (volatile unsigned short *) (MCF_MBAR + MCFSIM_PADAT);
	ppdata = (ppdata & ~mask) | bits;
	*pp = ppdata;
}
#endif

/*---------------------------------------------------------------------------*/
#elif defined(CONFIG_M5206e)
/*
 *	NETtel/5206e based hardware has leds on latch on CS3.
 *	No support modem for lines??
 */
#define	NETtel_LEDADDR	0x50000000

/*---------------------------------------------------------------------------*/
#elif defined(CONFIG_M5272)
/*
 *	NETtel/5272 based hardware. DTR/DCD lines are wired to GPB lines.
 */
#define	MCFPP_DCD0	0x0080
#define	MCFPP_DCD1	0x0000		/* Port 1 no DCD support */
#define	MCFPP_DTR0	0x0040
#define	MCFPP_DTR1	0x0000		/* Port 1 no DTR support */

#ifndef __ASSEMBLY__
/*
 *	These functions defined to give quasi generic access to the
 *	PPIO bits used for DTR/DCD.
 */
static __inline__ unsigned int mcf_getppdata(void)
{
	volatile unsigned short *pp;
	pp = (volatile unsigned short *) (MCF_MBAR + MCFSIM_PBDAT);
	return((unsigned int) *pp);
}

static __inline__ void mcf_setppdata(unsigned int mask, unsigned int bits)
{
	volatile unsigned short *pp;
	pp = (volatile unsigned short *) (MCF_MBAR + MCFSIM_PBDAT);
	*pp = (*pp & ~mask) | bits;
}
#endif

#endif
/*---------------------------------------------------------------------------*/

/****************************************************************************/
#endif /* CONFIG_NETtel */
/****************************************************************************/
#endif	/* nettel_h */
