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

#ifndef _ASM_IA64_SN_FEATURE_SETS_H
#define _ASM_IA64_SN_FEATURE_SETS_H

/*
 * SN PROM Features
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 2005-2006 Silicon Graphics, Inc.  All rights reserved.
 */


/* --------------------- PROM Features -----------------------------*/
extern int sn_prom_feature_available(int id);

#define MAX_PROM_FEATURE_SETS			2

/*
 * The following defines features that may or may not be supported by the
 * current PROM. The OS uses sn_prom_feature_available(feature) to test for
 * the presence of a PROM feature. Down rev (old) PROMs will always test
 * "false" for new features.
 *
 * Use:
 * 		if (sn_prom_feature_available(PRF_XXX))
 * 			...
 */

#define PRF_PAL_CACHE_FLUSH_SAFE	0
#define PRF_DEVICE_FLUSH_LIST		1
#define PRF_HOTPLUG_SUPPORT		2
#define PRF_CPU_DISABLE_SUPPORT		3

/* --------------------- OS Features -------------------------------*/

/*
 * The following defines OS features that are optionally present in
 * the operating system.
 * During boot, PROM is notified of these features via a series of calls:
 *
 * 		ia64_sn_set_os_feature(feature1);
 *
 * Once enabled, a feature cannot be disabled.
 *
 * By default, features are disabled unless explicitly enabled.
 *
 * These defines must be kept in sync with the corresponding
 * PROM definitions in feature_sets.h.
 */
#define  OSF_MCA_SLV_TO_OS_INIT_SLV	0
#define  OSF_FEAT_LOG_SBES		1
#define  OSF_ACPI_ENABLE		2
#define  OSF_PCISEGMENT_ENABLE		3


#endif /* _ASM_IA64_SN_FEATURE_SETS_H */
