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

/*****************************************************************************
 *  File: drivers/usb/misc/vstusb.h
 *
 *  Purpose: Support for the bulk USB Vernier Spectrophotometers
 *
 *  Author:     EQware Engineering, Inc.
 *              Oregon City, OR, USA 97045
 *
 *  Copyright:  2007, 2008
 *              Vernier Software & Technology
 *              Beaverton, OR, USA 97005
 *
 *  Web:        www.vernier.com
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *****************************************************************************/
/*****************************************************************************
 *
 *  The vstusb module is a standard usb 'client' driver running on top of the
 *  standard usb host controller stack.
 *
 *  In general, vstusb supports standard bulk usb pipes.  It supports multiple
 *  devices and multiple pipes per device.
 *
 *  The vstusb driver supports two interfaces:
 *  1 - ioctl SEND_PIPE/RECV_PIPE - a general bulk write/read msg
 *  	interface to any pipe with timeout support;
 *  2 - standard read/write with ioctl config - offers standard read/write
 *  	interface with ioctl configured pipes and timeouts.
 *
 *  Both interfaces can be signal from other process and will abort its i/o
 *  operation.
 *
 *  A timeout of 0 means NO timeout.  The user can still terminate the read via
 *  signal.
 *
 *  If using multiple threads with this driver, the user should ensure that
 *  any reads, writes, or ioctls are complete before closing the device.
 *  Changing read/write timeouts or pipes takes effect on next read/write.
 *
 *****************************************************************************/

struct vstusb_args {
	union {
		/* this struct is used for IOCTL_VSTUSB_SEND_PIPE,	*
		 * IOCTL_VSTUSB_RECV_PIPE, and read()/write() fops	*/
		struct {
			void __user	*buffer;
			size_t          count;
			unsigned int    timeout_ms;
			int             pipe;
		};

		/* this one is used for IOCTL_VSTUSB_CONFIG_RW  	*/
		struct {
			int rd_pipe;
			int rd_timeout_ms;
			int wr_pipe;
			int wr_timeout_ms;
		};
	};
};

#define VST_IOC_MAGIC 'L'
#define VST_IOC_FIRST 0x20
#define IOCTL_VSTUSB_SEND_PIPE	_IO(VST_IOC_MAGIC, VST_IOC_FIRST)
#define IOCTL_VSTUSB_RECV_PIPE	_IO(VST_IOC_MAGIC, VST_IOC_FIRST + 1)
#define IOCTL_VSTUSB_CONFIG_RW	_IO(VST_IOC_MAGIC, VST_IOC_FIRST + 2)
