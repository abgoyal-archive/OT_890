# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.

# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.


#
# Makefile for the HID driver
#
hid-objs			:= hid-core.o hid-input.o

obj-$(CONFIG_HID)		+= hid.o

hid-$(CONFIG_HID_DEBUG)		+= hid-debug.o
hid-$(CONFIG_HIDRAW)		+= hidraw.o

ifdef CONFIG_HID_COMPAT
obj-m				+= hid-dummy.o
endif

hid-logitech-objs		:= hid-lg.o
ifdef CONFIG_LOGITECH_FF
	hid-logitech-objs	+= hid-lgff.o
endif
ifdef CONFIG_LOGIRUMBLEPAD2_FF
	hid-logitech-objs	+= hid-lg2ff.o
endif

obj-$(CONFIG_HID_A4TECH)	+= hid-a4tech.o
obj-$(CONFIG_HID_APPLE)		+= hid-apple.o
obj-$(CONFIG_HID_BELKIN)	+= hid-belkin.o
obj-$(CONFIG_HID_CHERRY)	+= hid-cherry.o
obj-$(CONFIG_HID_CHICONY)	+= hid-chicony.o
obj-$(CONFIG_HID_CYPRESS)	+= hid-cypress.o
obj-$(CONFIG_HID_EZKEY)		+= hid-ezkey.o
obj-$(CONFIG_HID_GYRATION)	+= hid-gyration.o
obj-$(CONFIG_HID_LOGITECH)	+= hid-logitech.o
obj-$(CONFIG_HID_MICROSOFT)	+= hid-microsoft.o
obj-$(CONFIG_HID_MONTEREY)	+= hid-monterey.o
obj-$(CONFIG_HID_NTRIG)		+= hid-ntrig.o
obj-$(CONFIG_HID_PANTHERLORD)	+= hid-pl.o
obj-$(CONFIG_HID_PETALYNX)	+= hid-petalynx.o
obj-$(CONFIG_HID_SAMSUNG)	+= hid-samsung.o
obj-$(CONFIG_HID_SONY)		+= hid-sony.o
obj-$(CONFIG_HID_SUNPLUS)	+= hid-sunplus.o
obj-$(CONFIG_GREENASIA_FF)	+= hid-gaff.o
obj-$(CONFIG_THRUSTMASTER_FF)	+= hid-tmff.o
obj-$(CONFIG_HID_TOPSEED)	+= hid-topseed.o
obj-$(CONFIG_ZEROPLUS_FF)	+= hid-zpff.o

obj-$(CONFIG_USB_HID)		+= usbhid/
obj-$(CONFIG_USB_MOUSE)		+= usbhid/
obj-$(CONFIG_USB_KBD)		+= usbhid/

