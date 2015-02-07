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
 *  drivers/s390/net/qeth_l3.h
 *
 *    Copyright IBM Corp. 2007
 *    Author(s): Utz Bacher <utz.bacher@de.ibm.com>,
 *		 Frank Pavlic <fpavlic@de.ibm.com>,
 *		 Thomas Spatzier <tspat@de.ibm.com>,
 *		 Frank Blaschka <frank.blaschka@de.ibm.com>
 */

#ifndef __QETH_L3_H__
#define __QETH_L3_H__

#include "qeth_core.h"

struct qeth_ipaddr {
	struct list_head entry;
	enum qeth_ip_types type;
	enum qeth_ipa_setdelip_flags set_flags;
	enum qeth_ipa_setdelip_flags del_flags;
	int is_multicast;
	int users;
	enum qeth_prot_versions proto;
	unsigned char mac[OSA_ADDR_LEN];
	union {
		struct {
			unsigned int addr;
			unsigned int mask;
		} a4;
		struct {
			struct in6_addr addr;
			unsigned int pfxlen;
		} a6;
	} u;
};

struct qeth_ipato_entry {
	struct list_head entry;
	enum qeth_prot_versions proto;
	char addr[16];
	int mask_bits;
};


void qeth_l3_ipaddr4_to_string(const __u8 *, char *);
int qeth_l3_string_to_ipaddr4(const char *, __u8 *);
void qeth_l3_ipaddr6_to_string(const __u8 *, char *);
int qeth_l3_string_to_ipaddr6(const char *, __u8 *);
void qeth_l3_ipaddr_to_string(enum qeth_prot_versions, const __u8 *, char *);
int qeth_l3_string_to_ipaddr(const char *, enum qeth_prot_versions, __u8 *);
int qeth_l3_create_device_attributes(struct device *);
void qeth_l3_remove_device_attributes(struct device *);
int qeth_l3_setrouting_v4(struct qeth_card *);
int qeth_l3_setrouting_v6(struct qeth_card *);
int qeth_l3_add_ipato_entry(struct qeth_card *, struct qeth_ipato_entry *);
void qeth_l3_del_ipato_entry(struct qeth_card *, enum qeth_prot_versions,
			u8 *, int);
int qeth_l3_add_vipa(struct qeth_card *, enum qeth_prot_versions, const u8 *);
void qeth_l3_del_vipa(struct qeth_card *, enum qeth_prot_versions, const u8 *);
int qeth_l3_add_rxip(struct qeth_card *, enum qeth_prot_versions, const u8 *);
void qeth_l3_del_rxip(struct qeth_card *card, enum qeth_prot_versions,
			const u8 *);

#endif /* __QETH_L3_H__ */
