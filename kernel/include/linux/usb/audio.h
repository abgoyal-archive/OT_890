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
 * <linux/usb/audio.h> -- USB Audio definitions.
 *
 * Copyright (C) 2006 Thumtronics Pty Ltd.
 * Developed for Thumtronics by Grey Innovation
 * Ben Williamson <ben.williamson@greyinnovation.com>
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 2, as published by the Free Software Foundation.
 *
 * This file holds USB constants and structures defined
 * by the USB Device Class Definition for Audio Devices.
 * Comments below reference relevant sections of that document:
 *
 * http://www.usb.org/developers/devclass_docs/audio10.pdf
 */

#ifndef __LINUX_USB_AUDIO_H
#define __LINUX_USB_AUDIO_H

#include <linux/types.h>

/* A.2 Audio Interface Subclass Codes */
#define USB_SUBCLASS_AUDIOCONTROL	0x01
#define USB_SUBCLASS_AUDIOSTREAMING	0x02
#define USB_SUBCLASS_MIDISTREAMING	0x03

/* 4.3.2  Class-Specific AC Interface Descriptor */
struct usb_ac_header_descriptor {
	__u8  bLength;			/* 8+n */
	__u8  bDescriptorType;		/* USB_DT_CS_INTERFACE */
	__u8  bDescriptorSubtype;	/* USB_MS_HEADER */
	__le16 bcdADC;			/* 0x0100 */
	__le16 wTotalLength;		/* includes Unit and Terminal desc. */
	__u8  bInCollection;		/* n */
	__u8  baInterfaceNr[];		/* [n] */
} __attribute__ ((packed));

#define USB_DT_AC_HEADER_SIZE(n)	(8+(n))

/* As above, but more useful for defining your own descriptors: */
#define DECLARE_USB_AC_HEADER_DESCRIPTOR(n) 			\
struct usb_ac_header_descriptor_##n {				\
	__u8  bLength;						\
	__u8  bDescriptorType;					\
	__u8  bDescriptorSubtype;				\
	__le16 bcdADC;						\
	__le16 wTotalLength;					\
	__u8  bInCollection;					\
	__u8  baInterfaceNr[n];					\
} __attribute__ ((packed))

#endif /* __LINUX_USB_AUDIO_H */
