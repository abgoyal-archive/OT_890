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
 * Copyright (C) 2002 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#ifndef __UM_NET_KERN_H
#define __UM_NET_KERN_H

#include <linux/netdevice.h>
#include <linux/platform_device.h>
#include <linux/skbuff.h>
#include <linux/socket.h>
#include <linux/list.h>
#include <linux/workqueue.h>

struct uml_net {
	struct list_head list;
	struct net_device *dev;
	struct platform_device pdev;
	int index;
	unsigned char mac[ETH_ALEN];
};

struct uml_net_private {
	struct list_head list;
	spinlock_t lock;
	struct net_device *dev;
	struct timer_list tl;
	struct net_device_stats stats;
	struct work_struct work;
	int fd;
	unsigned char mac[ETH_ALEN];
	int max_packet;
	unsigned short (*protocol)(struct sk_buff *);
	int (*open)(void *);
	void (*close)(int, void *);
	void (*remove)(void *);
	int (*read)(int, struct sk_buff *skb, struct uml_net_private *);
	int (*write)(int, struct sk_buff *skb, struct uml_net_private *);

	void (*add_address)(unsigned char *, unsigned char *, void *);
	void (*delete_address)(unsigned char *, unsigned char *, void *);
	char user[0];
};

struct net_kern_info {
	void (*init)(struct net_device *, void *);
	unsigned short (*protocol)(struct sk_buff *);
	int (*read)(int, struct sk_buff *skb, struct uml_net_private *);
	int (*write)(int, struct sk_buff *skb, struct uml_net_private *);
};

struct transport {
	struct list_head list;
	const char *name;
	int (* const setup)(char *, char **, void *);
	const struct net_user_info *user;
	const struct net_kern_info *kern;
	const int private_size;
	const int setup_size;
};

extern struct net_device *ether_init(int);
extern unsigned short ether_protocol(struct sk_buff *);
extern int tap_setup_common(char *str, char *type, char **dev_name,
			    char **mac_out, char **gate_addr);
extern void register_transport(struct transport *new);
extern unsigned short eth_protocol(struct sk_buff *skb);

#endif
