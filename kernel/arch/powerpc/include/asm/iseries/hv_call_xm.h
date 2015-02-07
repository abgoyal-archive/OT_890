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
 * This file contains the "hypervisor call" interface which is used to
 * drive the hypervisor from SLIC.
 */
#ifndef _ASM_POWERPC_ISERIES_HV_CALL_XM_H
#define _ASM_POWERPC_ISERIES_HV_CALL_XM_H

#include <asm/iseries/hv_call_sc.h>
#include <asm/iseries/hv_types.h>

#define HvCallXmGetTceTableParms	HvCallXm +  0
#define HvCallXmTestBus			HvCallXm +  1
#define HvCallXmConnectBusUnit		HvCallXm +  2
#define HvCallXmLoadTod			HvCallXm +  8
#define HvCallXmTestBusUnit		HvCallXm +  9
#define HvCallXmSetTce			HvCallXm + 11
#define HvCallXmSetTces			HvCallXm + 13

static inline void HvCallXm_getTceTableParms(u64 cb)
{
	HvCall1(HvCallXmGetTceTableParms, cb);
}

static inline u64 HvCallXm_setTce(u64 tceTableToken, u64 tceOffset, u64 tce)
{
	return HvCall3(HvCallXmSetTce, tceTableToken, tceOffset, tce);
}

static inline u64 HvCallXm_setTces(u64 tceTableToken, u64 tceOffset,
		u64 numTces, u64 tce1, u64 tce2, u64 tce3, u64 tce4)
{
	return HvCall7(HvCallXmSetTces, tceTableToken, tceOffset, numTces,
			     tce1, tce2, tce3, tce4);
}

static inline u64 HvCallXm_testBus(u16 busNumber)
{
	return HvCall1(HvCallXmTestBus, busNumber);
}

static inline u64 HvCallXm_testBusUnit(u16 busNumber, u8 subBusNumber,
		u8 deviceId)
{
	return HvCall2(HvCallXmTestBusUnit, busNumber,
			(subBusNumber << 8) | deviceId);
}

static inline u64 HvCallXm_connectBusUnit(u16 busNumber, u8 subBusNumber,
		u8 deviceId, u64 interruptToken)
{
	return HvCall5(HvCallXmConnectBusUnit, busNumber,
			(subBusNumber << 8) | deviceId, interruptToken, 0,
			0 /* HvLpConfig::mapDsaToQueueIndex(HvLpDSA(busNumber, xBoard, xCard)) */);
}

static inline u64 HvCallXm_loadTod(void)
{
	return HvCall0(HvCallXmLoadTod);
}

#endif /* _ASM_POWERPC_ISERIES_HV_CALL_XM_H */
