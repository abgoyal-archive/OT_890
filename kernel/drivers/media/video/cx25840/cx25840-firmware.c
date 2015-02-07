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

/* cx25840 firmware functions
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/firmware.h>
#include <media/v4l2-common.h>
#include <media/cx25840.h>

#include "cx25840-core.h"

#define FWFILE "v4l-cx25840.fw"
#define FWFILE_CX23885 "v4l-cx23885-avcore-01.fw"

/*
 * Mike Isely <isely@pobox.com> - The FWSEND parameter controls the
 * size of the firmware chunks sent down the I2C bus to the chip.
 * Previously this had been set to 1024 but unfortunately some I2C
 * implementations can't transfer data in such big gulps.
 * Specifically, the pvrusb2 driver has a hard limit of around 60
 * bytes, due to the encapsulation there of I2C traffic into USB
 * messages.  So we have to significantly reduce this parameter.
 */
#define FWSEND 48

#define FWDEV(x) &((x)->dev)

static char *firmware = FWFILE;

module_param(firmware, charp, 0444);

MODULE_PARM_DESC(firmware, "Firmware image [default: " FWFILE "]");

static void start_fw_load(struct i2c_client *client)
{
	/* DL_ADDR_LB=0 DL_ADDR_HB=0 */
	cx25840_write(client, 0x800, 0x00);
	cx25840_write(client, 0x801, 0x00);
	// DL_MAP=3 DL_AUTO_INC=0 DL_ENABLE=1
	cx25840_write(client, 0x803, 0x0b);
	/* AUTO_INC_DIS=1 */
	cx25840_write(client, 0x000, 0x20);
}

static void end_fw_load(struct i2c_client *client)
{
	/* AUTO_INC_DIS=0 */
	cx25840_write(client, 0x000, 0x00);
	/* DL_ENABLE=0 */
	cx25840_write(client, 0x803, 0x03);
}

static int check_fw_load(struct i2c_client *client, int size)
{
	/* DL_ADDR_HB DL_ADDR_LB */
	int s = cx25840_read(client, 0x801) << 8;
	s |= cx25840_read(client, 0x800);

	if (size != s) {
		v4l_err(client, "firmware %s load failed\n", firmware);
		return -EINVAL;
	}

	v4l_info(client, "loaded %s firmware (%d bytes)\n", firmware, size);
	return 0;
}

static int fw_write(struct i2c_client *client, const u8 *data, int size)
{
	if (i2c_master_send(client, data, size) < size) {
		v4l_err(client, "firmware load i2c failure\n");
		return -ENOSYS;
	}

	return 0;
}

int cx25840_loadfw(struct i2c_client *client)
{
	struct cx25840_state *state = to_state(i2c_get_clientdata(client));
	const struct firmware *fw = NULL;
	u8 buffer[FWSEND];
	const u8 *ptr;
	int size, retval;

	if (state->is_cx23885)
		firmware = FWFILE_CX23885;

	if (request_firmware(&fw, firmware, FWDEV(client)) != 0) {
		v4l_err(client, "unable to open firmware %s\n", firmware);
		return -EINVAL;
	}

	start_fw_load(client);

	buffer[0] = 0x08;
	buffer[1] = 0x02;

	size = fw->size;
	ptr = fw->data;
	while (size > 0) {
		int len = min(FWSEND - 2, size);

		memcpy(buffer + 2, ptr, len);

		retval = fw_write(client, buffer, len + 2);

		if (retval < 0) {
			release_firmware(fw);
			return retval;
		}

		size -= len;
		ptr += len;
	}

	end_fw_load(client);

	size = fw->size;
	release_firmware(fw);

	return check_fw_load(client, size);
}
