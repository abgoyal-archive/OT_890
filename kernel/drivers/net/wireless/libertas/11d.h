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

/**
  * This header file contains data structures and
  * function declarations of 802.11d
  */
#ifndef _LBS_11D_
#define _LBS_11D_

#include "types.h"
#include "defs.h"

#define UNIVERSAL_REGION_CODE			0xff

/** (Beaconsize(256)-5(IEId,len,contrystr(3))/3(FirstChan,NoOfChan,MaxPwr)
 */
#define MRVDRV_MAX_SUBBAND_802_11D		83

#define COUNTRY_CODE_LEN			3
#define MAX_NO_OF_CHAN 				40

struct cmd_ds_command;

/** Data structure for Country IE*/
struct ieeetypes_subbandset {
	u8 firstchan;
	u8 nrchan;
	u8 maxtxpwr;
} __attribute__ ((packed));

struct ieeetypes_countryinfoset {
	u8 element_id;
	u8 len;
	u8 countrycode[COUNTRY_CODE_LEN];
	struct ieeetypes_subbandset subband[1];
};

struct ieeetypes_countryinfofullset {
	u8 element_id;
	u8 len;
	u8 countrycode[COUNTRY_CODE_LEN];
	struct ieeetypes_subbandset subband[MRVDRV_MAX_SUBBAND_802_11D];
} __attribute__ ((packed));

struct mrvlietypes_domainparamset {
	struct mrvlietypesheader header;
	u8 countrycode[COUNTRY_CODE_LEN];
	struct ieeetypes_subbandset subband[1];
} __attribute__ ((packed));

struct cmd_ds_802_11d_domain_info {
	__le16 action;
	struct mrvlietypes_domainparamset domain;
} __attribute__ ((packed));

/** domain regulatory information */
struct lbs_802_11d_domain_reg {
	/** country Code*/
	u8 countrycode[COUNTRY_CODE_LEN];
	/** No. of subband*/
	u8 nr_subband;
	struct ieeetypes_subbandset subband[MRVDRV_MAX_SUBBAND_802_11D];
};

struct chan_power_11d {
	u8 chan;
	u8 pwr;
} __attribute__ ((packed));

struct parsed_region_chan_11d {
	u8 band;
	u8 region;
	s8 countrycode[COUNTRY_CODE_LEN];
	struct chan_power_11d chanpwr[MAX_NO_OF_CHAN];
	u8 nr_chan;
} __attribute__ ((packed));

struct region_code_mapping {
	u8 region[COUNTRY_CODE_LEN];
	u8 code;
};

struct lbs_private;

u8 lbs_get_scan_type_11d(u8 chan,
			  struct parsed_region_chan_11d *parsed_region_chan);

u32 lbs_chan_2_freq(u8 chan);

void lbs_init_11d(struct lbs_private *priv);

int lbs_set_universaltable(struct lbs_private *priv, u8 band);

int lbs_cmd_802_11d_domain_info(struct lbs_private *priv,
				 struct cmd_ds_command *cmd, u16 cmdno,
				 u16 cmdOption);

int lbs_ret_802_11d_domain_info(struct cmd_ds_command *resp);

struct bss_descriptor;
int lbs_parse_dnld_countryinfo_11d(struct lbs_private *priv,
                                        struct bss_descriptor * bss);

int lbs_create_dnld_countryinfo_11d(struct lbs_private *priv);

#endif
