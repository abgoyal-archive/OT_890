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
 * This file contains the generic PMU register description tables
 * and pmc checker used by perfmon.c.
 *
 * Copyright (C) 2002-2003  Hewlett Packard Co
 *               Stephane Eranian <eranian@hpl.hp.com>
 */

static pfm_reg_desc_t pfm_gen_pmc_desc[PMU_MAX_PMCS]={
/* pmc0  */ { PFM_REG_CONTROL , 0, 0x1UL, -1UL, NULL, NULL, {0UL,0UL, 0UL, 0UL}, {0UL,0UL, 0UL, 0UL}},
/* pmc1  */ { PFM_REG_CONTROL , 0, 0x0UL, -1UL, NULL, NULL, {0UL,0UL, 0UL, 0UL}, {0UL,0UL, 0UL, 0UL}},
/* pmc2  */ { PFM_REG_CONTROL , 0, 0x0UL, -1UL, NULL, NULL, {0UL,0UL, 0UL, 0UL}, {0UL,0UL, 0UL, 0UL}},
/* pmc3  */ { PFM_REG_CONTROL , 0, 0x0UL, -1UL, NULL, NULL, {0UL,0UL, 0UL, 0UL}, {0UL,0UL, 0UL, 0UL}},
/* pmc4  */ { PFM_REG_COUNTING, 0, 0x0UL, -1UL, NULL, NULL, {RDEP(4),0UL, 0UL, 0UL}, {0UL,0UL, 0UL, 0UL}},
/* pmc5  */ { PFM_REG_COUNTING, 0, 0x0UL, -1UL, NULL, NULL, {RDEP(5),0UL, 0UL, 0UL}, {0UL,0UL, 0UL, 0UL}},
/* pmc6  */ { PFM_REG_COUNTING, 0, 0x0UL, -1UL, NULL, NULL, {RDEP(6),0UL, 0UL, 0UL}, {0UL,0UL, 0UL, 0UL}},
/* pmc7  */ { PFM_REG_COUNTING, 0, 0x0UL, -1UL, NULL, NULL, {RDEP(7),0UL, 0UL, 0UL}, {0UL,0UL, 0UL, 0UL}},
	    { PFM_REG_END     , 0, 0x0UL, -1UL, NULL, NULL, {0,}, {0,}}, /* end marker */
};

static pfm_reg_desc_t pfm_gen_pmd_desc[PMU_MAX_PMDS]={
/* pmd0  */ { PFM_REG_NOTIMPL , 0, 0x0UL, -1UL, NULL, NULL, {0,}, {0,}},
/* pmd1  */ { PFM_REG_NOTIMPL , 0, 0x0UL, -1UL, NULL, NULL, {0,}, {0,}},
/* pmd2  */ { PFM_REG_NOTIMPL , 0, 0x0UL, -1UL, NULL, NULL, {0,}, {0,}},
/* pmd3  */ { PFM_REG_NOTIMPL , 0, 0x0UL, -1UL, NULL, NULL, {0,}, {0,}},
/* pmd4  */ { PFM_REG_COUNTING, 0, 0x0UL, -1UL, NULL, NULL, {0UL,0UL, 0UL, 0UL}, {RDEP(4),0UL, 0UL, 0UL}},
/* pmd5  */ { PFM_REG_COUNTING, 0, 0x0UL, -1UL, NULL, NULL, {0UL,0UL, 0UL, 0UL}, {RDEP(5),0UL, 0UL, 0UL}},
/* pmd6  */ { PFM_REG_COUNTING, 0, 0x0UL, -1UL, NULL, NULL, {0UL,0UL, 0UL, 0UL}, {RDEP(6),0UL, 0UL, 0UL}},
/* pmd7  */ { PFM_REG_COUNTING, 0, 0x0UL, -1UL, NULL, NULL, {0UL,0UL, 0UL, 0UL}, {RDEP(7),0UL, 0UL, 0UL}},
	    { PFM_REG_END     , 0, 0x0UL, -1UL, NULL, NULL, {0,}, {0,}}, /* end marker */
};

/*
 * impl_pmcs, impl_pmds are computed at runtime to minimize errors!
 */
static pmu_config_t pmu_conf_gen={
	.pmu_name   = "Generic",
	.pmu_family = 0xff, /* any */
	.ovfl_val   = (1UL << 32) - 1,
	.num_ibrs   = 0, /* does not use */
	.num_dbrs   = 0, /* does not use */
	.pmd_desc   = pfm_gen_pmd_desc,
	.pmc_desc   = pfm_gen_pmc_desc
};

