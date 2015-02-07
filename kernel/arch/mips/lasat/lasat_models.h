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
 * Model description tables
 */
#include <linux/kernel.h>

struct product_info {
	const char     *pi_name;
	const char     *pi_type;
};

struct vendor_info {
	const char     *vi_name;
	const struct product_info *vi_product_info;
};

/*
 * Base models
 */
static const char * const txt_base_models[] = {
	"MQ 2", "MQ Pro", "SP 25", "SP 50", "SP 100", "SP 5000", "SP 7000",
	"SP 1000", "Unknown"
};
#define N_BASE_MODELS (ARRAY_SIZE(txt_base_models) - 1)

/*
 * Eicon Networks
 */
static const char txt_en_mq[] = "Masquerade";
static const char txt_en_sp[] = "Safepipe";

static const struct product_info product_info_eicon[] = {
	{ txt_en_mq, "II"   }, /*  0 */
	{ txt_en_mq, "Pro"  }, /*  1 */
	{ txt_en_sp, "25"   }, /*  2 */
	{ txt_en_sp, "50"   }, /*  3 */
	{ txt_en_sp, "100"  }, /*  4 */
	{ txt_en_sp, "5000" }, /*  5 */
	{ txt_en_sp, "7000" }, /*  6 */
	{ txt_en_sp, "30"   }, /*  7 */
	{ txt_en_sp, "5100" }, /*  8 */
	{ txt_en_sp, "7100" }, /*  9 */
	{ txt_en_sp, "1110" }, /* 10 */
	{ txt_en_sp, "3020" }, /* 11 */
	{ txt_en_sp, "3030" }, /* 12 */
	{ txt_en_sp, "5020" }, /* 13 */
	{ txt_en_sp, "5030" }, /* 14 */
	{ txt_en_sp, "1120" }, /* 15 */
	{ txt_en_sp, "1130" }, /* 16 */
	{ txt_en_sp, "6010" }, /* 17 */
	{ txt_en_sp, "6110" }, /* 18 */
	{ txt_en_sp, "6210" }, /* 19 */
	{ txt_en_sp, "1020" }, /* 20 */
	{ txt_en_sp, "1040" }, /* 21 */
	{ txt_en_sp, "1050" }, /* 22 */
	{ txt_en_sp, "1060" }, /* 23 */
};

#define N_PRIDS ARRAY_SIZE(product_info_eicon)

/*
 * The vendor table
 */
static struct vendor_info const vendor_info_table[] = {
	{ "Eicon Networks",	product_info_eicon   },
};

#define N_VENDORS ARRAY_SIZE(vendor_info_table)
