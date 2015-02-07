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
 * Intel Wireless UWB Link 1480
 * Main driver
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
 * Common code for firmware upload used by the USB and PCI version;
 * i1480_fw_upload() takes a device descriptor and uses the function
 * pointers it provides to upload firmware and prepare the PHY.
 *
 * As well, provides common functions used by the rest of the code.
 */
#include "i1480-dfu.h"
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/pci.h>
#include <linux/device.h>
#include <linux/uwb.h>
#include <linux/random.h>

/*
 * i1480_rceb_check - Check RCEB for expected field values
 * @i1480: pointer to device for which RCEB is being checked
 * @rceb: RCEB being checked
 * @cmd: which command the RCEB is related to
 * @context: expected context
 * @expected_type: expected event type
 * @expected_event: expected event
 *
 * If @cmd is NULL, do not print error messages, but still return an error
 * code.
 *
 * Return 0 if @rceb matches the expected values, -EINVAL otherwise.
 */
int i1480_rceb_check(const struct i1480 *i1480, const struct uwb_rceb *rceb,
		     const char *cmd, u8 context, u8 expected_type,
		     unsigned expected_event)
{
	int result = 0;
	struct device *dev = i1480->dev;
	if (rceb->bEventContext != context) {
		if (cmd)
			dev_err(dev, "%s: unexpected context id 0x%02x "
				"(expected 0x%02x)\n", cmd,
				rceb->bEventContext, context);
		result = -EINVAL;
	}
	if (rceb->bEventType != expected_type) {
		if (cmd)
			dev_err(dev, "%s: unexpected event type 0x%02x "
				"(expected 0x%02x)\n", cmd,
				rceb->bEventType, expected_type);
		result = -EINVAL;
	}
	if (le16_to_cpu(rceb->wEvent) != expected_event) {
		if (cmd)
			dev_err(dev, "%s: unexpected event 0x%04x "
				"(expected 0x%04x)\n", cmd,
				le16_to_cpu(rceb->wEvent), expected_event);
		result = -EINVAL;
	}
	return result;
}
EXPORT_SYMBOL_GPL(i1480_rceb_check);


/*
 * Execute a Radio Control Command
 *
 * Command data has to be in i1480->cmd_buf.
 *
 * @returns size of the reply data filled in i1480->evt_buf or < 0 errno
 *          code on error.
 */
ssize_t i1480_cmd(struct i1480 *i1480, const char *cmd_name, size_t cmd_size,
		  size_t reply_size)
{
	ssize_t result;
	struct uwb_rceb *reply = i1480->evt_buf;
	struct uwb_rccb *cmd = i1480->cmd_buf;
	u16 expected_event = reply->wEvent;
	u8 expected_type = reply->bEventType;
	u8 context;

	init_completion(&i1480->evt_complete);
	i1480->evt_result = -EINPROGRESS;
	do {
		get_random_bytes(&context, 1);
	} while (context == 0x00 || context == 0xff);
	cmd->bCommandContext = context;
	result = i1480->cmd(i1480, cmd_name, cmd_size);
	if (result < 0)
		goto error;
	/* wait for the callback to report a event was received */
	result = wait_for_completion_interruptible_timeout(
		&i1480->evt_complete, HZ);
	if (result == 0) {
		result = -ETIMEDOUT;
		goto error;
	}
	if (result < 0)
		goto error;
	result = i1480->evt_result;
	if (result < 0) {
		dev_err(i1480->dev, "%s: command reply reception failed: %zd\n",
			cmd_name, result);
		goto error;
	}
	/*
	 * Firmware versions >= 1.4.12224 for IOGear GUWA100U generate a
	 * spurious notification after firmware is downloaded. So check whether
	 * the receibed RCEB is such notification before assuming that the
	 * command has failed.
	 */
	if (i1480_rceb_check(i1480, i1480->evt_buf, NULL,
			     0, 0xfd, 0x0022) == 0) {
		/* Now wait for the actual RCEB for this command. */
		result = i1480->wait_init_done(i1480);
		if (result < 0)
			goto error;
		result = i1480->evt_result;
	}
	if (result != reply_size) {
		dev_err(i1480->dev, "%s returned only %zu bytes, %zu expected\n",
			cmd_name, result, reply_size);
		result = -EINVAL;
		goto error;
	}
	/* Verify we got the right event in response */
	result = i1480_rceb_check(i1480, i1480->evt_buf, cmd_name, context,
				  expected_type, expected_event);
error:
	return result;
}
EXPORT_SYMBOL_GPL(i1480_cmd);


static
int i1480_print_state(struct i1480 *i1480)
{
	int result;
	u32 *buf = (u32 *) i1480->cmd_buf;

	result = i1480->read(i1480, 0x80080000, 2 * sizeof(*buf));
	if (result < 0) {
		dev_err(i1480->dev, "cannot read U & L states: %d\n", result);
		goto error;
	}
	dev_info(i1480->dev, "state U 0x%08x, L 0x%08x\n", buf[0], buf[1]);
error:
	return result;
}


/*
 * PCI probe, firmware uploader
 *
 * _mac_fw_upload() will call rc_setup(), which needs an rc_release().
 */
int i1480_fw_upload(struct i1480 *i1480)
{
	int result;

	result = i1480_pre_fw_upload(i1480);	/* PHY pre fw */
	if (result < 0 && result != -ENOENT) {
		i1480_print_state(i1480);
		goto error;
	}
	result = i1480_mac_fw_upload(i1480);	/* MAC fw */
	if (result < 0) {
		if (result == -ENOENT)
			dev_err(i1480->dev, "Cannot locate MAC FW file '%s'\n",
				i1480->mac_fw_name);
		else
			i1480_print_state(i1480);
		goto error;
	}
	result = i1480_phy_fw_upload(i1480);	/* PHY fw */
	if (result < 0 && result != -ENOENT) {
		i1480_print_state(i1480);
		goto error_rc_release;
	}
	/*
	 * FIXME: find some reliable way to check whether firmware is running
	 * properly. Maybe use some standard request that has no side effects?
	 */
	dev_info(i1480->dev, "firmware uploaded successfully\n");
error_rc_release:
	if (i1480->rc_release)
		i1480->rc_release(i1480);
	result = 0;
error:
	return result;
}
EXPORT_SYMBOL_GPL(i1480_fw_upload);
