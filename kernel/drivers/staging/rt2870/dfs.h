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
    dfs.h

    Abstract:
    Support DFS function.

    Revision History:
    Who       When            What
    --------  ----------      ----------------------------------------------
    Fonchi    03-12-2007      created
*/

#define RADAR_PULSE 1
#define RADAR_WIDTH 2

#define WIDTH_RD_IDLE 0
#define WIDTH_RD_CHECK 1


VOID BbpRadarDetectionStart(
	IN PRTMP_ADAPTER pAd);

VOID BbpRadarDetectionStop(
	IN PRTMP_ADAPTER pAd);

VOID RadarDetectionStart(
	IN PRTMP_ADAPTER pAd,
	IN BOOLEAN CTS_Protect,
	IN UINT8 CTSPeriod);

VOID RadarDetectionStop(
	IN PRTMP_ADAPTER	pAd);

VOID RadarDetectPeriodic(
	IN PRTMP_ADAPTER	pAd);


BOOLEAN RadarChannelCheck(
	IN PRTMP_ADAPTER	pAd,
	IN UCHAR			Ch);

ULONG JapRadarType(
	IN PRTMP_ADAPTER pAd);

ULONG RTMPBbpReadRadarDuration(
	IN PRTMP_ADAPTER	pAd);

ULONG RTMPReadRadarDuration(
	IN PRTMP_ADAPTER	pAd);

VOID RTMPCleanRadarDuration(
	IN PRTMP_ADAPTER	pAd);

VOID RTMPPrepareRDCTSFrame(
	IN	PRTMP_ADAPTER	pAd,
	IN	PUCHAR			pDA,
	IN	ULONG			Duration,
	IN  UCHAR           RTSRate,
	IN  ULONG           CTSBaseAddr,
	IN  UCHAR			FrameGap);

VOID RTMPPrepareRadarDetectParams(
	IN PRTMP_ADAPTER	pAd);


INT Set_ChMovingTime_Proc(
	IN PRTMP_ADAPTER pAd,
	IN PUCHAR arg);

INT Set_LongPulseRadarTh_Proc(
	IN PRTMP_ADAPTER pAd,
	IN PUCHAR arg);


