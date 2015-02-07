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

#ifndef __NET_WIRELESS_REG_H
#define __NET_WIRELESS_REG_H

bool is_world_regdom(const char *alpha2);
bool reg_is_valid_request(const char *alpha2);

void reg_device_remove(struct wiphy *wiphy);

int regulatory_init(void);
void regulatory_exit(void);

int set_regdom(const struct ieee80211_regdomain *rd);

enum environment_cap {
	ENVIRON_ANY,
	ENVIRON_INDOOR,
	ENVIRON_OUTDOOR,
};


/**
 * __regulatory_hint - hint to the wireless core a regulatory domain
 * @wiphy: if the hint comes from country information from an AP, this
 *	is required to be set to the wiphy that received the information
 * @alpha2: the ISO/IEC 3166 alpha2 being claimed the regulatory domain
 *	should be in.
 * @country_ie_checksum: checksum of processed country IE, set this to 0
 * 	if the hint did not come from a country IE
 * @country_ie_env: the environment the IE told us we are in, %ENVIRON_*
 *
 * The Wireless subsystem can use this function to hint to the wireless core
 * what it believes should be the current regulatory domain by giving it an
 * ISO/IEC 3166 alpha2 country code it knows its regulatory domain should be
 * in.
 *
 * Returns zero if all went fine, %-EALREADY if a regulatory domain had
 * already been set or other standard error codes.
 *
 */
extern int __regulatory_hint(struct wiphy *wiphy, enum reg_set_by set_by,
			     const char *alpha2, u32 country_ie_checksum,
			     enum environment_cap country_ie_env);

#endif  /* __NET_WIRELESS_REG_H */
