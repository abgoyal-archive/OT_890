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
 *
 *  Copyright (C) 2005 Mike Isely <isely@pobox.com>
 *  Copyright (C) 2004 Aurelien Alleaume <slts@free.fr>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/videodev2.h>

#include "pvrusb2-hdw.h"
#include "pvrusb2-devattr.h"
#include "pvrusb2-context.h"
#include "pvrusb2-debug.h"
#include "pvrusb2-v4l2.h"
#ifdef CONFIG_VIDEO_PVRUSB2_SYSFS
#include "pvrusb2-sysfs.h"
#endif /* CONFIG_VIDEO_PVRUSB2_SYSFS */

#define DRIVER_AUTHOR "Mike Isely <isely@pobox.com>"
#define DRIVER_DESC "Hauppauge WinTV-PVR-USB2 MPEG2 Encoder/Tuner"
#define DRIVER_VERSION "V4L in-tree version"

#define DEFAULT_DEBUG_MASK (PVR2_TRACE_ERROR_LEGS| \
			    PVR2_TRACE_INFO| \
			    PVR2_TRACE_STD| \
			    PVR2_TRACE_TOLERANCE| \
			    PVR2_TRACE_TRAP| \
			    0)

int pvrusb2_debug = DEFAULT_DEBUG_MASK;

module_param_named(debug,pvrusb2_debug,int,S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(debug, "Debug trace mask");

#ifdef CONFIG_VIDEO_PVRUSB2_SYSFS
static struct pvr2_sysfs_class *class_ptr = NULL;
#endif /* CONFIG_VIDEO_PVRUSB2_SYSFS */

static void pvr_setup_attach(struct pvr2_context *pvr)
{
	/* Create association with v4l layer */
	pvr2_v4l2_create(pvr);
#ifdef CONFIG_VIDEO_PVRUSB2_DVB
	/* Create association with dvb layer */
	pvr2_dvb_create(pvr);
#endif
#ifdef CONFIG_VIDEO_PVRUSB2_SYSFS
	pvr2_sysfs_create(pvr,class_ptr);
#endif /* CONFIG_VIDEO_PVRUSB2_SYSFS */
}

static int pvr_probe(struct usb_interface *intf,
		     const struct usb_device_id *devid)
{
	struct pvr2_context *pvr;

	/* Create underlying hardware interface */
	pvr = pvr2_context_create(intf,devid,pvr_setup_attach);
	if (!pvr) {
		pvr2_trace(PVR2_TRACE_ERROR_LEGS,
			   "Failed to create hdw handler");
		return -ENOMEM;
	}

	pvr2_trace(PVR2_TRACE_INIT,"pvr_probe(pvr=%p)",pvr);

	usb_set_intfdata(intf, pvr);

	return 0;
}

/*
 * pvr_disconnect()
 *
 */
static void pvr_disconnect(struct usb_interface *intf)
{
	struct pvr2_context *pvr = usb_get_intfdata(intf);

	pvr2_trace(PVR2_TRACE_INIT,"pvr_disconnect(pvr=%p) BEGIN",pvr);

	usb_set_intfdata (intf, NULL);
	pvr2_context_disconnect(pvr);

	pvr2_trace(PVR2_TRACE_INIT,"pvr_disconnect(pvr=%p) DONE",pvr);

}

static struct usb_driver pvr_driver = {
	.name =         "pvrusb2",
	.id_table =     pvr2_device_table,
	.probe =        pvr_probe,
	.disconnect =   pvr_disconnect
};

/*
 * pvr_init() / pvr_exit()
 *
 * This code is run to initialize/exit the driver.
 *
 */
static int __init pvr_init(void)
{
	int ret;

	pvr2_trace(PVR2_TRACE_INIT,"pvr_init");

	ret = pvr2_context_global_init();
	if (ret != 0) {
		pvr2_trace(PVR2_TRACE_INIT,"pvr_init failure code=%d",ret);
		return ret;
	}

#ifdef CONFIG_VIDEO_PVRUSB2_SYSFS
	class_ptr = pvr2_sysfs_class_create();
#endif /* CONFIG_VIDEO_PVRUSB2_SYSFS */

	ret = usb_register(&pvr_driver);

	if (ret == 0)
		printk(KERN_INFO KBUILD_MODNAME ": " DRIVER_VERSION ":"
		       DRIVER_DESC "\n");
	if (pvrusb2_debug)
		printk(KERN_INFO KBUILD_MODNAME ": Debug mask is %d (0x%x)\n",
		       pvrusb2_debug,pvrusb2_debug);

	pvr2_trace(PVR2_TRACE_INIT,"pvr_init complete");

	return ret;
}

static void __exit pvr_exit(void)
{
	pvr2_trace(PVR2_TRACE_INIT,"pvr_exit");

	usb_deregister(&pvr_driver);

#ifdef CONFIG_VIDEO_PVRUSB2_SYSFS
	pvr2_sysfs_class_destroy(class_ptr);
#endif /* CONFIG_VIDEO_PVRUSB2_SYSFS */

	pvr2_context_global_done();

	pvr2_trace(PVR2_TRACE_INIT,"pvr_exit complete");
}

module_init(pvr_init);
module_exit(pvr_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");


/*
  Stuff for Emacs to see, in order to encourage consistent editing style:
  *** Local Variables: ***
  *** mode: c ***
  *** fill-column: 70 ***
  *** tab-width: 8 ***
  *** c-basic-offset: 8 ***
  *** End: ***
  */
