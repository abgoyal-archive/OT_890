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

/***************************************************************************
 * API for image sensors connected to the ZC0301[P] Image Processor and    *
 * Control Chip                                                            *
 *                                                                         *
 * Copyright (C) 2006-2007 by Luca Risolia <luca.risolia@studio.unibo.it>  *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this program; if not, write to the Free Software             *
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.               *
 ***************************************************************************/

#ifndef _ZC0301_SENSOR_H_
#define _ZC0301_SENSOR_H_

#include <linux/usb.h>
#include <linux/videodev2.h>
#include <linux/device.h>
#include <linux/stddef.h>
#include <linux/errno.h>
#include <asm/types.h>

struct zc0301_device;
struct zc0301_sensor;

/*****************************************************************************/

extern int zc0301_probe_pas202bcb(struct zc0301_device* cam);
extern int zc0301_probe_pb0330(struct zc0301_device* cam);

#define ZC0301_SENSOR_TABLE                                                   \
/* Weak detections must go at the end of the list */                          \
static int (*zc0301_sensor_table[])(struct zc0301_device*) = {                \
	&zc0301_probe_pas202bcb,                                              \
	&zc0301_probe_pb0330,                                                 \
	NULL,                                                                 \
};

extern struct zc0301_device*
zc0301_match_id(struct zc0301_device* cam, const struct usb_device_id *id);

extern void
zc0301_attach_sensor(struct zc0301_device* cam, struct zc0301_sensor* sensor);

#define ZC0301_USB_DEVICE(vend, prod, intclass)                               \
	.match_flags = USB_DEVICE_ID_MATCH_DEVICE |                           \
		       USB_DEVICE_ID_MATCH_INT_CLASS,                         \
	.idVendor = (vend),                                                   \
	.idProduct = (prod),                                                  \
	.bInterfaceClass = (intclass)

#define ZC0301_ID_TABLE                                                       \
static const struct usb_device_id zc0301_id_table[] =  {                      \
	{ ZC0301_USB_DEVICE(0x046d, 0x08ae, 0xff), }, /* PAS202 */            \
	{ ZC0301_USB_DEVICE(0x0ac8, 0x303b, 0xff), }, /* PB-0330 */           \
	{ }                                                                   \
};

/*****************************************************************************/

extern int zc0301_write_reg(struct zc0301_device*, u16 index, u16 value);
extern int zc0301_read_reg(struct zc0301_device*, u16 index);
extern int zc0301_i2c_write(struct zc0301_device*, u16 address, u16 value);
extern int zc0301_i2c_read(struct zc0301_device*, u16 address, u8 length);

/*****************************************************************************/

#define ZC0301_MAX_CTRLS (V4L2_CID_LASTP1 - V4L2_CID_BASE + 10)
#define ZC0301_V4L2_CID_DAC_MAGNITUDE (V4L2_CID_PRIVATE_BASE + 0)
#define ZC0301_V4L2_CID_GREEN_BALANCE (V4L2_CID_PRIVATE_BASE + 1)

struct zc0301_sensor {
	char name[32];

	struct v4l2_queryctrl qctrl[ZC0301_MAX_CTRLS];
	struct v4l2_cropcap cropcap;
	struct v4l2_pix_format pix_format;

	int (*init)(struct zc0301_device*);
	int (*get_ctrl)(struct zc0301_device*, struct v4l2_control* ctrl);
	int (*set_ctrl)(struct zc0301_device*,
			const struct v4l2_control* ctrl);
	int (*set_crop)(struct zc0301_device*, const struct v4l2_rect* rect);

	/* Private */
	struct v4l2_queryctrl _qctrl[ZC0301_MAX_CTRLS];
	struct v4l2_rect _rect;
};

#endif /* _ZC0301_SENSOR_H_ */
