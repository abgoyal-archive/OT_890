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
 * stk-webcam.h : Driver for Syntek 1125 USB webcam controller
 *
 * Copyright (C) 2006 Nicolas VIVIEN
 * Copyright 2007-2008 Jaime Velasco Juan <jsagarribay@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef STKWEBCAM_H
#define STKWEBCAM_H

#include <linux/usb.h>
#include <media/v4l2-common.h>

#define DRIVER_VERSION		"v0.0.1"
#define DRIVER_VERSION_NUM	0x000001

#define MAX_ISO_BUFS		3
#define ISO_FRAMES_PER_DESC	16
#define ISO_MAX_FRAME_SIZE	3 * 1024
#define ISO_BUFFER_SIZE		(ISO_FRAMES_PER_DESC * ISO_MAX_FRAME_SIZE)


#define PREFIX				"stkwebcam: "
#define STK_INFO(str, args...)		printk(KERN_INFO PREFIX str, ##args)
#define STK_ERROR(str, args...)		printk(KERN_ERR PREFIX str, ##args)
#define STK_WARNING(str, args...)	printk(KERN_WARNING PREFIX str, ##args)

struct stk_iso_buf {
	void *data;
	int length;
	int read;
	struct urb *urb;
};

/* Streaming IO buffers */
struct stk_sio_buffer {
	struct v4l2_buffer v4lbuf;
	char *buffer;
	int mapcount;
	struct stk_camera *dev;
	struct list_head list;
};

enum stk_mode {MODE_VGA, MODE_SXGA, MODE_CIF, MODE_QVGA, MODE_QCIF};

struct stk_video {
	enum stk_mode mode;
	int brightness;
	__u32 palette;
	int hflip;
	int vflip;
};

enum stk_status {
	S_PRESENT = 1,
	S_INITIALISED = 2,
	S_MEMALLOCD = 4,
	S_STREAMING = 8,
};
#define is_present(dev)		((dev)->status & S_PRESENT)
#define is_initialised(dev)	((dev)->status & S_INITIALISED)
#define is_streaming(dev)	((dev)->status & S_STREAMING)
#define is_memallocd(dev)	((dev)->status & S_MEMALLOCD)
#define set_present(dev)	((dev)->status = S_PRESENT)
#define unset_present(dev)	((dev)->status &= \
					~(S_PRESENT|S_INITIALISED|S_STREAMING))
#define set_initialised(dev)	((dev)->status |= S_INITIALISED)
#define unset_initialised(dev)	((dev)->status &= ~S_INITIALISED)
#define set_memallocd(dev)	((dev)->status |= S_MEMALLOCD)
#define unset_memallocd(dev)	((dev)->status &= ~S_MEMALLOCD)
#define set_streaming(dev)	((dev)->status |= S_STREAMING)
#define unset_streaming(dev)	((dev)->status &= ~S_STREAMING)

struct regval {
	unsigned reg;
	unsigned val;
};

struct stk_camera {
	struct video_device vdev;
	struct usb_device *udev;
	struct usb_interface *interface;
	int webcam_model;
	struct file *owner;

	u8 isoc_ep;

	/* Not sure if this is right */
	atomic_t urbs_used;

	struct stk_video vsettings;

	enum stk_status status;

	spinlock_t spinlock;
	wait_queue_head_t wait_frame;

	struct stk_iso_buf *isobufs;

	int frame_size;
	/* Streaming buffers */
	unsigned int n_sbufs;
	struct stk_sio_buffer *sio_bufs;
	struct list_head sio_avail;
	struct list_head sio_full;
	unsigned sequence;
};

#define vdev_to_camera(d) container_of(d, struct stk_camera, vdev)

int stk_camera_write_reg(struct stk_camera *, u16, u8);
int stk_camera_read_reg(struct stk_camera *, u16, int *);

int stk_sensor_init(struct stk_camera *);
int stk_sensor_configure(struct stk_camera *);
int stk_sensor_sleep(struct stk_camera *dev);
int stk_sensor_wakeup(struct stk_camera *dev);
int stk_sensor_set_brightness(struct stk_camera *dev, int br);

#endif
