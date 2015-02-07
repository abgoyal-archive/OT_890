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
 * include/asm-s390/qeth.h
 *
 * ioctl definitions for qeth driver
 *
 * Copyright (C) 2004 IBM Corporation
 *
 * Author(s):	Thomas Spatzier <tspat@de.ibm.com>
 *
 */
#ifndef __ASM_S390_QETH_IOCTL_H__
#define __ASM_S390_QETH_IOCTL_H__
#include <linux/types.h>
#include <linux/ioctl.h>

#define SIOC_QETH_ARP_SET_NO_ENTRIES    (SIOCDEVPRIVATE)
#define SIOC_QETH_ARP_QUERY_INFO        (SIOCDEVPRIVATE + 1)
#define SIOC_QETH_ARP_ADD_ENTRY         (SIOCDEVPRIVATE + 2)
#define SIOC_QETH_ARP_REMOVE_ENTRY      (SIOCDEVPRIVATE + 3)
#define SIOC_QETH_ARP_FLUSH_CACHE       (SIOCDEVPRIVATE + 4)
#define SIOC_QETH_ADP_SET_SNMP_CONTROL  (SIOCDEVPRIVATE + 5)
#define SIOC_QETH_GET_CARD_TYPE         (SIOCDEVPRIVATE + 6)

struct qeth_arp_cache_entry {
	__u8  macaddr[6];
	__u8  reserved1[2];
	__u8  ipaddr[16]; /* for both  IPv4 and IPv6 */
	__u8  reserved2[32];
} __attribute__ ((packed));

struct qeth_arp_qi_entry7 {
	__u8 media_specific[32];
	__u8 macaddr_type;
	__u8 ipaddr_type;
	__u8 macaddr[6];
	__u8 ipaddr[4];
} __attribute__((packed));

struct qeth_arp_qi_entry7_short {
	__u8 macaddr_type;
	__u8 ipaddr_type;
	__u8 macaddr[6];
	__u8 ipaddr[4];
} __attribute__((packed));

struct qeth_arp_qi_entry5 {
	__u8 media_specific[32];
	__u8 macaddr_type;
	__u8 ipaddr_type;
	__u8 ipaddr[4];
} __attribute__((packed));

struct qeth_arp_qi_entry5_short {
	__u8 macaddr_type;
	__u8 ipaddr_type;
	__u8 ipaddr[4];
} __attribute__((packed));

/*
 * can be set by user if no "media specific information" is wanted
 * -> saves a lot of space in user space buffer
 */
#define QETH_QARP_STRIP_ENTRIES  0x8000
#define QETH_QARP_REQUEST_MASK   0x00ff

/* data sent to user space as result of query arp ioctl */
#define QETH_QARP_USER_DATA_SIZE 20000
#define QETH_QARP_MASK_OFFSET    4
#define QETH_QARP_ENTRIES_OFFSET 6
struct qeth_arp_query_user_data {
	union {
		__u32 data_len;		/* set by user space program */
		__u32 no_entries;	/* set by kernel */
	} u;
	__u16 mask_bits;
	char *entries;
} __attribute__((packed));

#endif /* __ASM_S390_QETH_IOCTL_H__ */
