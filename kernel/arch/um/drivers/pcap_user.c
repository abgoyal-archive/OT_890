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
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL.
 */

#include <errno.h>
#include <pcap.h>
#include <string.h>
#include <asm/types.h>
#include "net_user.h"
#include "pcap_user.h"
#include "kern_constants.h"
#include "um_malloc.h"
#include "user.h"

#define PCAP_FD(p) (*(int *)(p))

static int pcap_user_init(void *data, void *dev)
{
	struct pcap_data *pri = data;
	pcap_t *p;
	char errors[PCAP_ERRBUF_SIZE];

	p = pcap_open_live(pri->host_if, ETH_MAX_PACKET + ETH_HEADER_OTHER,
			   pri->promisc, 0, errors);
	if (p == NULL) {
		printk(UM_KERN_ERR "pcap_user_init : pcap_open_live failed - "
		       "'%s'\n", errors);
		return -EINVAL;
	}

	pri->dev = dev;
	pri->pcap = p;
	return 0;
}

static int pcap_open(void *data)
{
	struct pcap_data *pri = data;
	__u32 netmask;
	int err;

	if (pri->pcap == NULL)
		return -ENODEV;

	if (pri->filter != NULL) {
		err = dev_netmask(pri->dev, &netmask);
		if (err < 0) {
			printk(UM_KERN_ERR "pcap_open : dev_netmask failed\n");
			return -EIO;
		}

		pri->compiled = uml_kmalloc(sizeof(struct bpf_program),
					UM_GFP_KERNEL);
		if (pri->compiled == NULL) {
			printk(UM_KERN_ERR "pcap_open : kmalloc failed\n");
			return -ENOMEM;
		}

		err = pcap_compile(pri->pcap,
				   (struct bpf_program *) pri->compiled,
				   pri->filter, pri->optimize, netmask);
		if (err < 0) {
			printk(UM_KERN_ERR "pcap_open : pcap_compile failed - "
			       "'%s'\n", pcap_geterr(pri->pcap));
			goto out;
		}

		err = pcap_setfilter(pri->pcap, pri->compiled);
		if (err < 0) {
			printk(UM_KERN_ERR "pcap_open : pcap_setfilter "
			       "failed - '%s'\n", pcap_geterr(pri->pcap));
			goto out;
		}
	}

	return PCAP_FD(pri->pcap);

 out:
	kfree(pri->compiled);
	return -EIO;
}

static void pcap_remove(void *data)
{
	struct pcap_data *pri = data;

	if (pri->compiled != NULL)
		pcap_freecode(pri->compiled);

	if (pri->pcap != NULL)
		pcap_close(pri->pcap);
}

struct pcap_handler_data {
	char *buffer;
	int len;
};

static void handler(u_char *data, const struct pcap_pkthdr *header,
		    const u_char *packet)
{
	int len;

	struct pcap_handler_data *hdata = (struct pcap_handler_data *) data;

	len = hdata->len < header->caplen ? hdata->len : header->caplen;
	memcpy(hdata->buffer, packet, len);
	hdata->len = len;
}

int pcap_user_read(int fd, void *buffer, int len, struct pcap_data *pri)
{
	struct pcap_handler_data hdata = ((struct pcap_handler_data)
		                          { .buffer  	= buffer,
					    .len 	= len });
	int n;

	n = pcap_dispatch(pri->pcap, 1, handler, (u_char *) &hdata);
	if (n < 0) {
		printk(UM_KERN_ERR "pcap_dispatch failed - %s\n",
		       pcap_geterr(pri->pcap));
		return -EIO;
	}
	else if (n == 0)
		return 0;
	return hdata.len;
}

const struct net_user_info pcap_user_info = {
	.init		= pcap_user_init,
	.open		= pcap_open,
	.close	 	= NULL,
	.remove	 	= pcap_remove,
	.add_address	= NULL,
	.delete_address = NULL,
	.mtu		= ETH_MAX_PACKET,
	.max_packet	= ETH_MAX_PACKET + ETH_HEADER_OTHER,
};
