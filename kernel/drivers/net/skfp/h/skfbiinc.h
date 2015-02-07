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

/******************************************************************************
 *
 *	(C)Copyright 1998,1999 SysKonnect,
 *	a business unit of Schneider & Koch & Co. Datensysteme GmbH.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	The information in this file is provided "AS IS" without warranty.
 *
 ******************************************************************************/

#ifndef	_SKFBIINC_
#define _SKFBIINC_

#include "h/supern_2.h"

/*
 * special defines for use into .asm files
 */
#define ERR_FLAGS (FS_MSRABT | FS_SEAC2 | FS_SFRMERR | FS_SFRMTY1)

#ifdef PCI
#define	IMASK_FAST	(IS_PLINT1 | IS_PLINT2 | IS_TIMINT | IS_TOKEN | \
			 IS_MINTR2 | IS_MINTR3 | IS_R1_P | \
			 IS_R1_C | IS_XA_C | IS_XS_C)
#endif

#ifdef	PCI
#define	ISR_MASK	(IS_MINTR1 | IS_R1_F | IS_XS_F| IS_XA_F | IMASK_FAST)
#else
#define	ISR_MASK	(IS_MINTR1 | IS_MINTR2 | IMASK_FAST)
#endif

#define	FMA_FM_CMDREG1	FMA(FM_CMDREG1)
#define	FMA_FM_CMDREG2	FMA(FM_CMDREG2)
#define	FMA_FM_STMCHN	FMA(FM_STMCHN)
#define	FMA_FM_RPR	FMA(FM_RPR)
#define	FMA_FM_WPXA0	FMA(FM_WPXA0)
#define	FMA_FM_WPXA2	FMA(FM_WPXA2)
#define	FMA_FM_MARR	FMA(FM_MARR)
#define	FMA_FM_MARW	FMA(FM_MARW)
#define	FMA_FM_MDRU	FMA(FM_MDRU)
#define	FMA_FM_MDRL	FMA(FM_MDRL)
#define	FMA_ST1L	FMA(FM_ST1L)
#define	FMA_ST1U	FMA(FM_ST1U)
#define	FMA_ST2L	FMA(FM_ST2L)
#define	FMA_ST2U	FMA(FM_ST2U)
#ifdef	SUPERNET_3
#define FMA_ST3L	FMA(FM_ST3L)
#define FMA_ST3U	FMA(FM_ST3U)
#endif

#define TMODE_RRQ	RQ_RRQ
#define TMODE_WAQ2	RQ_WA2
#define	HSRA		HSR(0)


#define FMA_FM_ST1L	FMA_ST1L
#define FMA_FM_ST1U	FMA_ST1U
#define FMA_FM_ST2L	FMA_ST2L
#define FMA_FM_ST2U	FMA_ST2U
#ifdef	SUPERNET_3
#define FMA_FM_ST3L	FMA_ST3L
#define FMA_FM_ST3U	FMA_ST3U
#endif

#define FMA_FM_SWPR	FMA(FM_SWPR)

#define FMA_FM_RPXA0	FMA(FM_RPXA0)

#define	FMA_FM_RPXS	FMA(FM_RPXS)
#define	FMA_FM_WPXS	FMA(FM_WPXS)

#define	FMA_FM_IMSK1U	FMA(FM_IMSK1U)
#define	FMA_FM_IMSK1L	FMA(FM_IMSK1L)

#define	FMA_FM_EAS	FMA(FM_EAS)
#define	FMA_FM_EAA0	FMA(FM_EAA0)

#define	TMODE_WAQ0	RQ_WA0
#define TMODE_WSQ	RQ_WSQ

/* Define default for DRV_PCM_STATE_CHANGE */
#ifndef	DRV_PCM_STATE_CHANGE
#define	DRV_PCM_STATE_CHANGE(smc,plc,p_state)	/* nothing */
#endif

/* Define default for DRV_RMT_INDICATION */
#ifndef	DRV_RMT_INDICATION
#define	DRV_RMT_INDICATION(smc,i)	/* nothing */
#endif

#endif	/* n_SKFBIINC_ */

