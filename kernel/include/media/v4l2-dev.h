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
 *
 *	V 4 L 2   D R I V E R   H E L P E R   A P I
 *
 * Moved from videodev2.h
 *
 *	Some commonly needed functions for drivers (v4l2-common.o module)
 */
#ifndef _V4L2_DEV_H
#define _V4L2_DEV_H

#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/mutex.h>
#include <linux/videodev2.h>

#define VIDEO_MAJOR	81

#define VFL_TYPE_GRABBER	0
#define VFL_TYPE_VBI		1
#define VFL_TYPE_RADIO		2
#define VFL_TYPE_VTX		3
#define VFL_TYPE_MAX		4

struct v4l2_ioctl_callbacks;
struct video_device;
struct v4l2_device;

/* Flag to mark the video_device struct as unregistered.
   Drivers can set this flag if they want to block all future
   device access. It is set by video_unregister_device. */
#define V4L2_FL_UNREGISTERED	(0)

struct v4l2_file_operations {
	struct module *owner;
	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
	unsigned int (*poll) (struct file *, struct poll_table_struct *);
	long (*ioctl) (struct file *, unsigned int, unsigned long);
	long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
	int (*mmap) (struct file *, struct vm_area_struct *);
	int (*open) (struct file *);
	int (*release) (struct file *);
};

/*
 * Newer version of video_device, handled by videodev2.c
 * 	This version moves redundant code from video device code to
 *	the common handler
 */

struct video_device
{
	/* device ops */
	const struct v4l2_file_operations *fops;

	/* sysfs */
	struct device dev;		/* v4l device */
	struct cdev *cdev;		/* character device */

	/* Set either parent or v4l2_dev if your driver uses v4l2_device */
	struct device *parent;		/* device parent */
	struct v4l2_device *v4l2_dev;	/* v4l2_device parent */

	/* device info */
	char name[32];
	int vfl_type;
	/* 'minor' is set to -1 if the registration failed */
	int minor;
	u16 num;
	/* use bitops to set/clear/test flags */
	unsigned long flags;
	/* attribute to differentiate multiple indices on one physical device */
	int index;

	int debug;			/* Activates debug level*/

	/* Video standard vars */
	v4l2_std_id tvnorms;		/* Supported tv norms */
	v4l2_std_id current_norm;	/* Current tvnorm */

	/* callbacks */
	void (*release)(struct video_device *vdev);

	/* ioctl callbacks */
	const struct v4l2_ioctl_ops *ioctl_ops;
};

/* dev to video-device */
#define to_video_device(cd) container_of(cd, struct video_device, dev)

/* Register video devices. Note that if video_register_device fails,
   the release() callback of the video_device structure is *not* called, so
   the caller is responsible for freeing any data. Usually that means that
   you call video_device_release() on failure.

   Also note that vdev->minor is set to -1 if the registration failed. */
int __must_check video_register_device(struct video_device *vdev, int type, int nr);
int __must_check video_register_device_index(struct video_device *vdev,
						int type, int nr, int index);

/* Unregister video devices. Will do nothing if vdev == NULL or
   vdev->minor < 0. */
void video_unregister_device(struct video_device *vdev);

/* helper functions to alloc/release struct video_device, the
   latter can also be used for video_device->release(). */
struct video_device * __must_check video_device_alloc(void);

/* this release function frees the vdev pointer */
void video_device_release(struct video_device *vdev);

/* this release function does nothing, use when the video_device is a
   static global struct. Note that having a static video_device is
   a dubious construction at best. */
void video_device_release_empty(struct video_device *vdev);

/* helper functions to access driver private data. */
static inline void *video_get_drvdata(struct video_device *vdev)
{
	return dev_get_drvdata(&vdev->dev);
}

static inline void video_set_drvdata(struct video_device *vdev, void *data)
{
	dev_set_drvdata(&vdev->dev, data);
}

struct video_device *video_devdata(struct file *file);

/* Combine video_get_drvdata and video_devdata as this is
   used very often. */
static inline void *video_drvdata(struct file *file)
{
	return video_get_drvdata(video_devdata(file));
}

static inline int video_is_unregistered(struct video_device *vdev)
{
	return test_bit(V4L2_FL_UNREGISTERED, &vdev->flags);
}

#endif /* _V4L2_DEV_H */
