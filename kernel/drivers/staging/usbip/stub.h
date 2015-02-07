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
 * Copyright (C) 2003-2008 Takahiro Hirofuchi
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 * USA.
 */

#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/net.h>

struct stub_device {
	struct usb_interface *interface;
	struct list_head list;

	struct usbip_device ud;
	__u32 devid;

	/*
	 * stub_priv preserves private data of each urb.
	 * It is allocated as stub_priv_cache and assigned to urb->context.
	 *
	 * stub_priv is always linked to any one of 3 lists;
	 *	priv_init: linked to this until the comletion of a urb.
	 *	priv_tx  : linked to this after the completion of a urb.
	 *	priv_free: linked to this after the sending of the result.
	 *
	 * Any of these list operations should be locked by priv_lock.
	 */
	spinlock_t priv_lock;
	struct list_head priv_init;
	struct list_head priv_tx;
	struct list_head priv_free;

	/* see comments for unlinking in stub_rx.c */
	struct list_head unlink_tx;
	struct list_head unlink_free;


	wait_queue_head_t tx_waitq;
};

/* private data into urb->priv */
struct stub_priv {
	unsigned long seqnum;
	struct list_head list;
	struct stub_device *sdev;
	struct urb *urb;

	int unlinking;
};

struct stub_unlink {
	unsigned long seqnum;
	struct list_head list;
	__u32 status;
};


extern struct kmem_cache *stub_priv_cache;


/*-------------------------------------------------------------------------*/
/* prototype declarations */

/* stub_tx.c */
void stub_complete(struct urb *);
void stub_tx_loop(struct usbip_task *);

/* stub_dev.c */
extern struct usb_driver stub_driver;

/* stub_rx.c */
void stub_rx_loop(struct usbip_task *);
void stub_enqueue_ret_unlink(struct stub_device *, __u32, __u32);

/* stub_main.c */
int match_busid(const char *busid);
void stub_device_cleanup_urbs(struct stub_device *sdev);
