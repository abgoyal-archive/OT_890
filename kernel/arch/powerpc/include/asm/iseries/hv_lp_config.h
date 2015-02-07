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
 * Copyright (C) 2001  Mike Corrigan IBM Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */
#ifndef _ASM_POWERPC_ISERIES_HV_LP_CONFIG_H
#define _ASM_POWERPC_ISERIES_HV_LP_CONFIG_H

/*
 * This file contains the interface to the LPAR configuration data
 * to determine which resources should be allocated to each partition.
 */

#include <asm/iseries/hv_call_sc.h>
#include <asm/iseries/hv_types.h>

enum {
	HvCallCfg_Cur	= 0,
	HvCallCfg_Init	= 1,
	HvCallCfg_Max	= 2,
	HvCallCfg_Min	= 3
};

#define HvCallCfgGetSystemPhysicalProcessors		HvCallCfg +  6
#define HvCallCfgGetPhysicalProcessors			HvCallCfg +  7
#define HvCallCfgGetMsChunks				HvCallCfg +  9
#define HvCallCfgGetSharedPoolIndex			HvCallCfg + 20
#define HvCallCfgGetSharedProcUnits			HvCallCfg + 21
#define HvCallCfgGetNumProcsInSharedPool		HvCallCfg + 22
#define HvCallCfgGetVirtualLanIndexMap			HvCallCfg + 30
#define HvCallCfgGetHostingLpIndex			HvCallCfg + 32

extern HvLpIndex HvLpConfig_getLpIndex_outline(void);
extern HvLpIndex HvLpConfig_getLpIndex(void);
extern HvLpIndex HvLpConfig_getPrimaryLpIndex(void);

static inline u64 HvLpConfig_getMsChunks(void)
{
	return HvCall2(HvCallCfgGetMsChunks, HvLpConfig_getLpIndex(),
			HvCallCfg_Cur);
}

static inline u64 HvLpConfig_getSystemPhysicalProcessors(void)
{
	return HvCall0(HvCallCfgGetSystemPhysicalProcessors);
}

static inline u64 HvLpConfig_getNumProcsInSharedPool(HvLpSharedPoolIndex sPI)
{
	return (u16)HvCall1(HvCallCfgGetNumProcsInSharedPool, sPI);
}

static inline u64 HvLpConfig_getPhysicalProcessors(void)
{
	return HvCall2(HvCallCfgGetPhysicalProcessors, HvLpConfig_getLpIndex(),
			HvCallCfg_Cur);
}

static inline HvLpSharedPoolIndex HvLpConfig_getSharedPoolIndex(void)
{
	return HvCall1(HvCallCfgGetSharedPoolIndex, HvLpConfig_getLpIndex());
}

static inline u64 HvLpConfig_getSharedProcUnits(void)
{
	return HvCall2(HvCallCfgGetSharedProcUnits, HvLpConfig_getLpIndex(),
			HvCallCfg_Cur);
}

static inline u64 HvLpConfig_getMaxSharedProcUnits(void)
{
	return HvCall2(HvCallCfgGetSharedProcUnits, HvLpConfig_getLpIndex(),
			HvCallCfg_Max);
}

static inline u64 HvLpConfig_getMaxPhysicalProcessors(void)
{
	return HvCall2(HvCallCfgGetPhysicalProcessors, HvLpConfig_getLpIndex(),
			HvCallCfg_Max);
}

static inline HvLpVirtualLanIndexMap HvLpConfig_getVirtualLanIndexMapForLp(
		HvLpIndex lp)
{
	/*
	 * This is a new function in V5R1 so calls to this on older
	 * hypervisors will return -1
	 */
	u64 retVal = HvCall1(HvCallCfgGetVirtualLanIndexMap, lp);
	if (retVal == -1)
		retVal = 0;
	return retVal;
}

static inline HvLpVirtualLanIndexMap HvLpConfig_getVirtualLanIndexMap(void)
{
	return HvLpConfig_getVirtualLanIndexMapForLp(
			HvLpConfig_getLpIndex_outline());
}

static inline int HvLpConfig_doLpsCommunicateOnVirtualLan(HvLpIndex lp1,
		HvLpIndex lp2)
{
	HvLpVirtualLanIndexMap virtualLanIndexMap1 =
		HvLpConfig_getVirtualLanIndexMapForLp(lp1);
	HvLpVirtualLanIndexMap virtualLanIndexMap2 =
		HvLpConfig_getVirtualLanIndexMapForLp(lp2);
	return ((virtualLanIndexMap1 & virtualLanIndexMap2) != 0);
}

static inline HvLpIndex HvLpConfig_getHostingLpIndex(HvLpIndex lp)
{
	return HvCall1(HvCallCfgGetHostingLpIndex, lp);
}

#endif /* _ASM_POWERPC_ISERIES_HV_LP_CONFIG_H */
