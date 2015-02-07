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

#ifndef _IPX_H_
#define _IPX_H_
#include <linux/types.h>
#include <linux/sockios.h>
#include <linux/socket.h>
#define IPX_NODE_LEN	6
#define IPX_MTU		576

struct sockaddr_ipx {
	sa_family_t	sipx_family;
	__be16		sipx_port;
	__be32		sipx_network;
	unsigned char 	sipx_node[IPX_NODE_LEN];
	__u8		sipx_type;
	unsigned char	sipx_zero;	/* 16 byte fill */
};

/*
 * So we can fit the extra info for SIOCSIFADDR into the address nicely
 */
#define sipx_special	sipx_port
#define sipx_action	sipx_zero
#define IPX_DLTITF	0
#define IPX_CRTITF	1

struct ipx_route_definition {
	__be32        ipx_network;
	__be32        ipx_router_network;
	unsigned char ipx_router_node[IPX_NODE_LEN];
};

struct ipx_interface_definition {
	__be32        ipx_network;
	unsigned char ipx_device[16];
	unsigned char ipx_dlink_type;
#define IPX_FRAME_NONE		0
#define IPX_FRAME_SNAP		1
#define IPX_FRAME_8022		2
#define IPX_FRAME_ETHERII	3
#define IPX_FRAME_8023		4
#define IPX_FRAME_TR_8022       5 /* obsolete */
	unsigned char ipx_special;
#define IPX_SPECIAL_NONE	0
#define IPX_PRIMARY		1
#define IPX_INTERNAL		2
	unsigned char ipx_node[IPX_NODE_LEN];
};
	
struct ipx_config_data {
	unsigned char	ipxcfg_auto_select_primary;
	unsigned char	ipxcfg_auto_create_interfaces;
};

/*
 * OLD Route Definition for backward compatibility.
 */

struct ipx_route_def {
	__be32		ipx_network;
	__be32		ipx_router_network;
#define IPX_ROUTE_NO_ROUTER	0
	unsigned char	ipx_router_node[IPX_NODE_LEN];
	unsigned char	ipx_device[16];
	unsigned short	ipx_flags;
#define IPX_RT_SNAP		8
#define IPX_RT_8022		4
#define IPX_RT_BLUEBOOK		2
#define IPX_RT_ROUTED		1
};

#define SIOCAIPXITFCRT		(SIOCPROTOPRIVATE)
#define SIOCAIPXPRISLT		(SIOCPROTOPRIVATE + 1)
#define SIOCIPXCFGDATA		(SIOCPROTOPRIVATE + 2)
#define SIOCIPXNCPCONN		(SIOCPROTOPRIVATE + 3)
#endif /* _IPX_H_ */
