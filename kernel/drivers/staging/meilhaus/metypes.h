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
 * Copyright (C) 2005 Meilhaus Electronic GmbH (support@meilhaus.de)
 *
 * Source File : metypes.h
 * Author      : GG (Guenter Gebhardt)  <g.gebhardt@meilhaus.de>
 */

#ifndef _METYPES_H_
#define _METYPES_H_


typedef int (*meErrorCB_t)(char *pcFunctionName, int iErrorCode);

typedef int (*meIOStreamCB_t)(
		int iDevice,
		int iSubdevice,
		int iCount,
	   	void *pvContext,
	   	int iErrorCode);

typedef int (*meIOIrqCB_t)(
		int iDevice,
	   	int iSubdevice,
	   	int iChannel,
		int iIrqCount,
	   	int iValue,
	   	void *pvContext,
	   	int iErrorCode);


typedef struct meIOSingle {
	int iDevice;
	int iSubdevice;
	int iChannel;
	int iDir;
	int iValue;
	int iTimeOut;
	int iFlags;
	int iErrno;
} meIOSingle_t;


typedef struct meIOStreamConfig {
	int iChannel;
	int iStreamConfig;
	int iRef;
	int iFlags;
} meIOStreamConfig_t;


typedef struct meIOStreamTrigger {
	int iAcqStartTrigType;
	int iAcqStartTrigEdge;
	int iAcqStartTrigChan;
	int iAcqStartTicksLow;
	int iAcqStartTicksHigh;
	int iAcqStartArgs[10];
	int iScanStartTrigType;
	int iScanStartTicksLow;
	int iScanStartTicksHigh;
	int iScanStartArgs[10];
	int iConvStartTrigType;
	int iConvStartTicksLow;
	int iConvStartTicksHigh;
	int iConvStartArgs[10];
	int iScanStopTrigType;
	int iScanStopCount;
	int iScanStopArgs[10];
	int iAcqStopTrigType;
	int iAcqStopCount;
	int iAcqStopArgs[10];
	int iFlags;
} meIOStreamTrigger_t;


typedef struct meIOStreamStart {
	int iDevice;
	int iSubdevice;
	int iStartMode;
	int iTimeOut;
	int iFlags;
	int iErrno;
} meIOStreamStart_t;


typedef struct meIOStreamStop {
	int iDevice;
	int iSubdevice;
	int iStopMode;
	int iFlags;
	int iErrno;
} meIOStreamStop_t;


#endif
