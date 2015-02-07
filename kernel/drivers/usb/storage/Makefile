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
# Makefile for the USB Mass Storage device drivers.
#
# 15 Aug 2000, Christoph Hellwig <hch@infradead.org>
# Rewritten to use lists instead of if-statements.
#

EXTRA_CFLAGS	:= -Idrivers/scsi

obj-$(CONFIG_USB_STORAGE)	+= usb-storage.o

usb-storage-obj-$(CONFIG_USB_STORAGE_DEBUG)	+= debug.o
usb-storage-obj-$(CONFIG_USB_STORAGE_USBAT)	+= shuttle_usbat.o
usb-storage-obj-$(CONFIG_USB_STORAGE_SDDR09)	+= sddr09.o
usb-storage-obj-$(CONFIG_USB_STORAGE_SDDR55)	+= sddr55.o
usb-storage-obj-$(CONFIG_USB_STORAGE_FREECOM)	+= freecom.o
usb-storage-obj-$(CONFIG_USB_STORAGE_ISD200)	+= isd200.o
usb-storage-obj-$(CONFIG_USB_STORAGE_DATAFAB)	+= datafab.o
usb-storage-obj-$(CONFIG_USB_STORAGE_JUMPSHOT)	+= jumpshot.o
usb-storage-obj-$(CONFIG_USB_STORAGE_ALAUDA)	+= alauda.o
usb-storage-obj-$(CONFIG_USB_STORAGE_ONETOUCH)	+= onetouch.o
usb-storage-obj-$(CONFIG_USB_STORAGE_KARMA)	+= karma.o
usb-storage-obj-$(CONFIG_USB_STORAGE_CYPRESS_ATACB) += cypress_atacb.o

usb-storage-objs :=	scsiglue.o protocol.o transport.o usb.o \
			initializers.o sierra_ms.o option_ms.o $(usb-storage-obj-y)

ifneq ($(CONFIG_USB_LIBUSUAL),)
	obj-$(CONFIG_USB)	+= libusual.o
endif
