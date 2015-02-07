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

/* Header file for IP tables userspace logging, Version 1.8
 *
 * (C) 2000-2002 by Harald Welte <laforge@gnumonks.org>
 * 
 * Distributed under the terms of GNU GPL */

#ifndef _IPT_ULOG_H
#define _IPT_ULOG_H

#ifndef NETLINK_NFLOG
#define NETLINK_NFLOG 	5
#endif

#define ULOG_DEFAULT_NLGROUP	1
#define ULOG_DEFAULT_QTHRESHOLD	1

#define ULOG_MAC_LEN	80
#define ULOG_PREFIX_LEN	32

#define ULOG_MAX_QLEN	50
/* Why 50? Well... there is a limit imposed by the slab cache 131000
 * bytes. So the multipart netlink-message has to be < 131000 bytes.
 * Assuming a standard ethernet-mtu of 1500, we could define this up
 * to 80... but even 50 seems to be big enough. */

/* private data structure for each rule with a ULOG target */
struct ipt_ulog_info {
	unsigned int nl_group;
	size_t copy_range;
	size_t qthreshold;
	char prefix[ULOG_PREFIX_LEN];
};

/* Format of the ULOG packets passed through netlink */
typedef struct ulog_packet_msg {
	unsigned long mark;
	long timestamp_sec;
	long timestamp_usec;
	unsigned int hook;
	char indev_name[IFNAMSIZ];
	char outdev_name[IFNAMSIZ];
	size_t data_len;
	char prefix[ULOG_PREFIX_LEN];
	unsigned char mac_len;
	unsigned char mac[ULOG_MAC_LEN];
	unsigned char payload[0];
} ulog_packet_msg_t;

#endif /*_IPT_ULOG_H*/
