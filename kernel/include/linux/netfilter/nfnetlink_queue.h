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

#ifndef _NFNETLINK_QUEUE_H
#define _NFNETLINK_QUEUE_H

#include <linux/types.h>
#include <linux/netfilter/nfnetlink.h>

enum nfqnl_msg_types {
	NFQNL_MSG_PACKET,		/* packet from kernel to userspace */
	NFQNL_MSG_VERDICT,		/* verdict from userspace to kernel */
	NFQNL_MSG_CONFIG,		/* connect to a particular queue */

	NFQNL_MSG_MAX
};

struct nfqnl_msg_packet_hdr {
	__be32		packet_id;	/* unique ID of packet in queue */
	__be16		hw_protocol;	/* hw protocol (network order) */
	u_int8_t	hook;		/* netfilter hook */
} __attribute__ ((packed));

struct nfqnl_msg_packet_hw {
	__be16		hw_addrlen;
	u_int16_t	_pad;
	u_int8_t	hw_addr[8];
};

struct nfqnl_msg_packet_timestamp {
	aligned_be64	sec;
	aligned_be64	usec;
};

enum nfqnl_attr_type {
	NFQA_UNSPEC,
	NFQA_PACKET_HDR,
	NFQA_VERDICT_HDR,		/* nfqnl_msg_verdict_hrd */
	NFQA_MARK,			/* u_int32_t nfmark */
	NFQA_TIMESTAMP,			/* nfqnl_msg_packet_timestamp */
	NFQA_IFINDEX_INDEV,		/* u_int32_t ifindex */
	NFQA_IFINDEX_OUTDEV,		/* u_int32_t ifindex */
	NFQA_IFINDEX_PHYSINDEV,		/* u_int32_t ifindex */
	NFQA_IFINDEX_PHYSOUTDEV,	/* u_int32_t ifindex */
	NFQA_HWADDR,			/* nfqnl_msg_packet_hw */
	NFQA_PAYLOAD,			/* opaque data payload */

	__NFQA_MAX
};
#define NFQA_MAX (__NFQA_MAX - 1)

struct nfqnl_msg_verdict_hdr {
	__be32 verdict;
	__be32 id;
};


enum nfqnl_msg_config_cmds {
	NFQNL_CFG_CMD_NONE,
	NFQNL_CFG_CMD_BIND,
	NFQNL_CFG_CMD_UNBIND,
	NFQNL_CFG_CMD_PF_BIND,
	NFQNL_CFG_CMD_PF_UNBIND,
};

struct nfqnl_msg_config_cmd {
	u_int8_t	command;	/* nfqnl_msg_config_cmds */
	u_int8_t	_pad;
	__be16		pf;		/* AF_xxx for PF_[UN]BIND */
};

enum nfqnl_config_mode {
	NFQNL_COPY_NONE,
	NFQNL_COPY_META,
	NFQNL_COPY_PACKET,
};

struct nfqnl_msg_config_params {
	__be32		copy_range;
	u_int8_t	copy_mode;	/* enum nfqnl_config_mode */
} __attribute__ ((packed));


enum nfqnl_attr_config {
	NFQA_CFG_UNSPEC,
	NFQA_CFG_CMD,			/* nfqnl_msg_config_cmd */
	NFQA_CFG_PARAMS,		/* nfqnl_msg_config_params */
	NFQA_CFG_QUEUE_MAXLEN,		/* u_int32_t */
	__NFQA_CFG_MAX
};
#define NFQA_CFG_MAX (__NFQA_CFG_MAX-1)

#endif /* _NFNETLINK_QUEUE_H */
