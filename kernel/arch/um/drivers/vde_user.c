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
 * Copyright (C) 2007 Luca Bigliardi (shammash@artha.org).
 * Licensed under the GPL.
 */

#include <stddef.h>
#include <errno.h>
#include <libvdeplug.h>
#include "kern_constants.h"
#include "net_user.h"
#include "um_malloc.h"
#include "user.h"
#include "vde.h"

static int vde_user_init(void *data, void *dev)
{
	struct vde_data *pri = data;
	VDECONN *conn = NULL;
	int err = -EINVAL;

	pri->dev = dev;

	conn = vde_open(pri->vde_switch, pri->descr, pri->args);

	if (conn == NULL) {
		err = -errno;
		printk(UM_KERN_ERR "vde_user_init: vde_open failed, "
		       "errno = %d\n", errno);
		return err;
	}

	printk(UM_KERN_INFO "vde backend - connection opened\n");

	pri->conn = conn;

	return 0;
}

static int vde_user_open(void *data)
{
	struct vde_data *pri = data;

	if (pri->conn != NULL)
		return vde_datafd(pri->conn);

	printk(UM_KERN_WARNING "vde_open - we have no VDECONN to open");
	return -EINVAL;
}

static void vde_remove(void *data)
{
	struct vde_data *pri = data;

	if (pri->conn != NULL) {
		printk(UM_KERN_INFO "vde backend - closing connection\n");
		vde_close(pri->conn);
		pri->conn = NULL;
		kfree(pri->args);
		pri->args = NULL;
		return;
	}

	printk(UM_KERN_WARNING "vde_remove - we have no VDECONN to remove");
}

const struct net_user_info vde_user_info = {
	.init		= vde_user_init,
	.open		= vde_user_open,
	.close	 	= NULL,
	.remove	 	= vde_remove,
	.add_address	= NULL,
	.delete_address = NULL,
	.mtu		= ETH_MAX_PACKET,
	.max_packet	= ETH_MAX_PACKET + ETH_HEADER_OTHER,
};

void vde_init_libstuff(struct vde_data *vpri, struct vde_init *init)
{
	struct vde_open_args *args;

	vpri->args = uml_kmalloc(sizeof(struct vde_open_args), UM_GFP_KERNEL);
	if (vpri->args == NULL) {
		printk(UM_KERN_ERR "vde_init_libstuff - vde_open_args "
		       "allocation failed");
		return;
	}

	args = vpri->args;

	args->port = init->port;
	args->group = init->group;
	args->mode = init->mode ? init->mode : 0700;

	args->port ?  printk("port %d", args->port) :
		printk("undefined port");
}

int vde_user_read(void *conn, void *buf, int len)
{
	VDECONN *vconn = conn;
	int rv;

	if (vconn == NULL)
		return 0;

	rv = vde_recv(vconn, buf, len, 0);
	if (rv < 0) {
		if (errno == EAGAIN)
			return 0;
		return -errno;
	}
	else if (rv == 0)
		return -ENOTCONN;

	return rv;
}

int vde_user_write(void *conn, void *buf, int len)
{
	VDECONN *vconn = conn;

	if (vconn == NULL)
		return 0;

	return vde_send(vconn, buf, len, 0);
}

