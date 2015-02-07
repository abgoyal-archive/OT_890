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

#ifndef _HIDRAW_H
#define _HIDRAW_H

/*
 *  Copyright (c) 2007 Jiri Kosina
 */

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <linux/hid.h>
#include <linux/types.h>

struct hidraw_report_descriptor {
	__u32 size;
	__u8 value[HID_MAX_DESCRIPTOR_SIZE];
};

struct hidraw_devinfo {
	__u32 bustype;
	__s16 vendor;
	__s16 product;
};

/* ioctl interface */
#define HIDIOCGRDESCSIZE	_IOR('H', 0x01, int)
#define HIDIOCGRDESC		_IOR('H', 0x02, struct hidraw_report_descriptor)
#define HIDIOCGRAWINFO		_IOR('H', 0x03, struct hidraw_devinfo)
#define HIDIOCGRAWNAME(len)     _IOC(_IOC_READ, 'H', 0x04, len)
#define HIDIOCGRAWPHYS(len)     _IOC(_IOC_READ, 'H', 0x05, len)

#define HIDRAW_FIRST_MINOR 0
#define HIDRAW_MAX_DEVICES 64
/* number of reports to buffer */
#define HIDRAW_BUFFER_SIZE 64


/* kernel-only API declarations */
#ifdef __KERNEL__

struct hidraw {
	unsigned int minor;
	int exist;
	int open;
	wait_queue_head_t wait;
	struct hid_device *hid;
	struct device *dev;
	struct list_head list;
};

struct hidraw_report {
	__u8 *value;
	int len;
};

struct hidraw_list {
	struct hidraw_report buffer[HIDRAW_BUFFER_SIZE];
	int head;
	int tail;
	struct fasync_struct *fasync;
	struct hidraw *hidraw;
	struct list_head node;
	struct mutex read_mutex;
};

#ifdef CONFIG_HIDRAW
int hidraw_init(void);
void hidraw_exit(void);
void hidraw_report_event(struct hid_device *, u8 *, int);
int hidraw_connect(struct hid_device *);
void hidraw_disconnect(struct hid_device *);
#else
static inline int hidraw_init(void) { return 0; }
static inline void hidraw_exit(void) { }
static inline void hidraw_report_event(struct hid_device *hid, u8 *data, int len) { }
static inline int hidraw_connect(struct hid_device *hid) { return -1; }
static inline void hidraw_disconnect(struct hid_device *hid) { }
#endif

#endif

#endif
