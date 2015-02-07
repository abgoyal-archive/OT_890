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
 *************************************************************************
 * Ralink Tech Inc.
 * 5F., No.36, Taiyuan St., Jhubei City,
 * Hsinchu County 302,
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2007, Ralink Technology, Inc.
 *
 * This program is free software; you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation; either version 2 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program; if not, write to the                         *
 * Free Software Foundation, Inc.,                                       *
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                       *
 *************************************************************************

    Module Name:
	spectrum_def.h

    Abstract:
    Handle association related requests either from WSTA or from local MLME

    Revision History:
    Who          When          What
    ---------    ----------    ----------------------------------------------
	Fonchi Wu    2008	  	   created for 802.11h
 */

#ifndef __SPECTRUM_DEF_H__
#define __SPECTRUM_DEF_H__

#define MAX_MEASURE_REQ_TAB_SIZE		3
#define MAX_HASH_MEASURE_REQ_TAB_SIZE	MAX_MEASURE_REQ_TAB_SIZE

#define MAX_TPC_REQ_TAB_SIZE			3
#define MAX_HASH_TPC_REQ_TAB_SIZE		MAX_TPC_REQ_TAB_SIZE

#define MIN_RCV_PWR				100		/* Negative value ((dBm) */

#define RM_TPC_REQ				0
#define RM_MEASURE_REQ			1

#define RM_BASIC				0
#define RM_CCA					1
#define RM_RPI_HISTOGRAM		2

#define TPC_REQ_AGE_OUT			500		/* ms */
#define MQ_REQ_AGE_OUT			500		/* ms */

#define TPC_DIALOGTOKEN_HASH_INDEX(_DialogToken)	((_DialogToken) % MAX_HASH_TPC_REQ_TAB_SIZE)
#define MQ_DIALOGTOKEN_HASH_INDEX(_DialogToken)		((_DialogToken) % MAX_MEASURE_REQ_TAB_SIZE)

typedef struct _MEASURE_REQ_ENTRY
{
	struct _MEASURE_REQ_ENTRY *pNext;
	ULONG lastTime;
	BOOLEAN	Valid;
	UINT8 DialogToken;
	UINT8 MeasureDialogToken[3];	// 0:basic measure, 1: CCA measure, 2: RPI_Histogram measure.
} MEASURE_REQ_ENTRY, *PMEASURE_REQ_ENTRY;

typedef struct _MEASURE_REQ_TAB
{
	UCHAR Size;
	PMEASURE_REQ_ENTRY Hash[MAX_HASH_MEASURE_REQ_TAB_SIZE];
	MEASURE_REQ_ENTRY Content[MAX_MEASURE_REQ_TAB_SIZE];
} MEASURE_REQ_TAB, *PMEASURE_REQ_TAB;

typedef struct _TPC_REQ_ENTRY
{
	struct _TPC_REQ_ENTRY *pNext;
	ULONG lastTime;
	BOOLEAN Valid;
	UINT8 DialogToken;
} TPC_REQ_ENTRY, *PTPC_REQ_ENTRY;

typedef struct _TPC_REQ_TAB
{
	UCHAR Size;
	PTPC_REQ_ENTRY Hash[MAX_HASH_TPC_REQ_TAB_SIZE];
	TPC_REQ_ENTRY Content[MAX_TPC_REQ_TAB_SIZE];
} TPC_REQ_TAB, *PTPC_REQ_TAB;

#endif // __SPECTRUM_DEF_H__ //

