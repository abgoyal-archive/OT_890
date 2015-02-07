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
 *  pSeries firmware setup code.
 *
 *  Portions from arch/powerpc/platforms/pseries/setup.c:
 *   Copyright (C) 1995  Linus Torvalds
 *   Adapted from 'alpha' version by Gary Thomas
 *   Modified by Cort Dougan (cort@cs.nmt.edu)
 *   Modified by PPC64 Team, IBM Corp
 *
 *  Portions from arch/powerpc/kernel/firmware.c
 *   Copyright (C) 2001 Ben. Herrenschmidt (benh@kernel.crashing.org)
 *   Modifications for ppc64:
 *    Copyright (C) 2003 Dave Engebretsen <engebret@us.ibm.com>
 *    Copyright (C) 2005 Stephen Rothwell, IBM Corporation
 *
 *  Copyright 2006 IBM Corporation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */


#include <asm/firmware.h>
#include <asm/prom.h>
#include <asm/udbg.h>

#include "pseries.h"

typedef struct {
    unsigned long val;
    char * name;
} firmware_feature_t;

static __initdata firmware_feature_t
firmware_features_table[FIRMWARE_MAX_FEATURES] = {
	{FW_FEATURE_PFT,		"hcall-pft"},
	{FW_FEATURE_TCE,		"hcall-tce"},
	{FW_FEATURE_SPRG0,		"hcall-sprg0"},
	{FW_FEATURE_DABR,		"hcall-dabr"},
	{FW_FEATURE_COPY,		"hcall-copy"},
	{FW_FEATURE_ASR,		"hcall-asr"},
	{FW_FEATURE_DEBUG,		"hcall-debug"},
	{FW_FEATURE_PERF,		"hcall-perf"},
	{FW_FEATURE_DUMP,		"hcall-dump"},
	{FW_FEATURE_INTERRUPT,		"hcall-interrupt"},
	{FW_FEATURE_MIGRATE,		"hcall-migrate"},
	{FW_FEATURE_PERFMON,		"hcall-perfmon"},
	{FW_FEATURE_CRQ,		"hcall-crq"},
	{FW_FEATURE_VIO,		"hcall-vio"},
	{FW_FEATURE_RDMA,		"hcall-rdma"},
	{FW_FEATURE_LLAN,		"hcall-lLAN"},
	{FW_FEATURE_BULK,		"hcall-bulk"},
	{FW_FEATURE_XDABR,		"hcall-xdabr"},
	{FW_FEATURE_MULTITCE,		"hcall-multi-tce"},
	{FW_FEATURE_SPLPAR,		"hcall-splpar"},
	{FW_FEATURE_BULK_REMOVE,	"hcall-bulk"},
};

/* Build up the firmware features bitmask using the contents of
 * device-tree/ibm,hypertas-functions.  Ultimately this functionality may
 * be moved into prom.c prom_init().
 */
void __init fw_feature_init(const char *hypertas, unsigned long len)
{
	const char *s;
	int i;

	pr_debug(" -> fw_feature_init()\n");

	for (s = hypertas; s < hypertas + len; s += strlen(s) + 1) {
		for (i = 0; i < FIRMWARE_MAX_FEATURES; i++) {
			/* check value against table of strings */
			if (!firmware_features_table[i].name ||
			    strcmp(firmware_features_table[i].name, s))
				continue;

			/* we have a match */
			powerpc_firmware_features |=
				firmware_features_table[i].val;
			break;
		}
	}

	pr_debug(" <- fw_feature_init()\n");
}
