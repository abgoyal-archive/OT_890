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
 * Wire Adapter Host Controller Driver
 * Common items to HWA and DWA based HCDs
 *
 * Copyright (C) 2005-2006 Intel Corporation
 * Inaky Perez-Gonzalez <inaky.perez-gonzalez@intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 *
 * FIXME: docs
 */
#include "wusbhc.h"
#include "wa-hc.h"

/**
 * Assumes
 *
 * wa->usb_dev and wa->usb_iface initialized and refcounted,
 * wa->wa_descr initialized.
 */
int wa_create(struct wahc *wa, struct usb_interface *iface)
{
	int result;
	struct device *dev = &iface->dev;

	result = wa_rpipes_create(wa);
	if (result < 0)
		goto error_rpipes_create;
	/* Fill up Data Transfer EP pointers */
	wa->dti_epd = &iface->cur_altsetting->endpoint[1].desc;
	wa->dto_epd = &iface->cur_altsetting->endpoint[2].desc;
	wa->xfer_result_size = le16_to_cpu(wa->dti_epd->wMaxPacketSize);
	wa->xfer_result = kmalloc(wa->xfer_result_size, GFP_KERNEL);
	if (wa->xfer_result == NULL)
		goto error_xfer_result_alloc;
	result = wa_nep_create(wa, iface);
	if (result < 0) {
		dev_err(dev, "WA-CDS: can't initialize notif endpoint: %d\n",
			result);
		goto error_nep_create;
	}
	return 0;

error_nep_create:
	kfree(wa->xfer_result);
error_xfer_result_alloc:
	wa_rpipes_destroy(wa);
error_rpipes_create:
	return result;
}
EXPORT_SYMBOL_GPL(wa_create);


void __wa_destroy(struct wahc *wa)
{
	if (wa->dti_urb) {
		usb_kill_urb(wa->dti_urb);
		usb_put_urb(wa->dti_urb);
		usb_kill_urb(wa->buf_in_urb);
		usb_put_urb(wa->buf_in_urb);
	}
	kfree(wa->xfer_result);
	wa_nep_destroy(wa);
	wa_rpipes_destroy(wa);
}
EXPORT_SYMBOL_GPL(__wa_destroy);

/**
 * wa_reset_all - reset the WA device
 * @wa: the WA to be reset
 *
 * For HWAs the radio controller and all other PALs are also reset.
 */
void wa_reset_all(struct wahc *wa)
{
	/* FIXME: assuming HWA. */
	wusbhc_reset_all(wa->wusb);
}

MODULE_AUTHOR("Inaky Perez-Gonzalez <inaky.perez-gonzalez@intel.com>");
MODULE_DESCRIPTION("Wireless USB Wire Adapter core");
MODULE_LICENSE("GPL");
