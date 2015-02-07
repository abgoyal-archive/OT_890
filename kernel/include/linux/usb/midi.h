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
 * <linux/usb/midi.h> -- USB MIDI definitions.
 *
 * Copyright (C) 2006 Thumtronics Pty Ltd.
 * Developed for Thumtronics by Grey Innovation
 * Ben Williamson <ben.williamson@greyinnovation.com>
 *
 * This software is distributed under the terms of the GNU General Public
 * License ("GPL") version 2, as published by the Free Software Foundation.
 *
 * This file holds USB constants and structures defined
 * by the USB Device Class Definition for MIDI Devices.
 * Comments below reference relevant sections of that document:
 *
 * http://www.usb.org/developers/devclass_docs/midi10.pdf
 */

#ifndef __LINUX_USB_MIDI_H
#define __LINUX_USB_MIDI_H

#include <linux/types.h>

/* A.1  MS Class-Specific Interface Descriptor Subtypes */
#define USB_MS_HEADER		0x01
#define USB_MS_MIDI_IN_JACK	0x02
#define USB_MS_MIDI_OUT_JACK	0x03
#define USB_MS_ELEMENT		0x04

/* A.2  MS Class-Specific Endpoint Descriptor Subtypes */
#define USB_MS_GENERAL		0x01

/* A.3  MS MIDI IN and OUT Jack Types */
#define USB_MS_EMBEDDED		0x01
#define USB_MS_EXTERNAL		0x02

/* 6.1.2.1  Class-Specific MS Interface Header Descriptor */
struct usb_ms_header_descriptor {
	__u8  bLength;
	__u8  bDescriptorType;
	__u8  bDescriptorSubtype;
	__le16 bcdMSC;
	__le16 wTotalLength;
} __attribute__ ((packed));

#define USB_DT_MS_HEADER_SIZE	7

/* 6.1.2.2  MIDI IN Jack Descriptor */
struct usb_midi_in_jack_descriptor {
	__u8  bLength;
	__u8  bDescriptorType;		/* USB_DT_CS_INTERFACE */
	__u8  bDescriptorSubtype;	/* USB_MS_MIDI_IN_JACK */
	__u8  bJackType;		/* USB_MS_EMBEDDED/EXTERNAL */
	__u8  bJackID;
	__u8  iJack;
} __attribute__ ((packed));

#define USB_DT_MIDI_IN_SIZE	6

struct usb_midi_source_pin {
	__u8  baSourceID;
	__u8  baSourcePin;
} __attribute__ ((packed));

/* 6.1.2.3  MIDI OUT Jack Descriptor */
struct usb_midi_out_jack_descriptor {
	__u8  bLength;
	__u8  bDescriptorType;		/* USB_DT_CS_INTERFACE */
	__u8  bDescriptorSubtype;	/* USB_MS_MIDI_OUT_JACK */
	__u8  bJackType;		/* USB_MS_EMBEDDED/EXTERNAL */
	__u8  bJackID;
	__u8  bNrInputPins;		/* p */
	struct usb_midi_source_pin pins[]; /* [p] */
	/*__u8  iJack;  -- ommitted due to variable-sized pins[] */
} __attribute__ ((packed));

#define USB_DT_MIDI_OUT_SIZE(p)	(7 + 2 * (p))

/* As above, but more useful for defining your own descriptors: */
#define DECLARE_USB_MIDI_OUT_JACK_DESCRIPTOR(p)			\
struct usb_midi_out_jack_descriptor_##p {			\
	__u8  bLength;						\
	__u8  bDescriptorType;					\
	__u8  bDescriptorSubtype;				\
	__u8  bJackType;					\
	__u8  bJackID;						\
	__u8  bNrInputPins;					\
	struct usb_midi_source_pin pins[p];			\
	__u8  iJack;						\
} __attribute__ ((packed))

/* 6.2.2  Class-Specific MS Bulk Data Endpoint Descriptor */
struct usb_ms_endpoint_descriptor {
	__u8  bLength;			/* 4+n */
	__u8  bDescriptorType;		/* USB_DT_CS_ENDPOINT */
	__u8  bDescriptorSubtype;	/* USB_MS_GENERAL */
	__u8  bNumEmbMIDIJack;		/* n */
	__u8  baAssocJackID[];		/* [n] */
} __attribute__ ((packed));

#define USB_DT_MS_ENDPOINT_SIZE(n)	(4 + (n))

/* As above, but more useful for defining your own descriptors: */
#define DECLARE_USB_MS_ENDPOINT_DESCRIPTOR(n)			\
struct usb_ms_endpoint_descriptor_##n {				\
	__u8  bLength;						\
	__u8  bDescriptorType;					\
	__u8  bDescriptorSubtype;				\
	__u8  bNumEmbMIDIJack;					\
	__u8  baAssocJackID[n];					\
} __attribute__ ((packed))

#endif /* __LINUX_USB_MIDI_H */
