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
# Makefile for the Linux kernel EDAC drivers.
#
# Copyright 02 Jul 2003, Linux Networx (http://lnxi.com)
# This file may be distributed under the terms of the
# GNU General Public License.
#


obj-$(CONFIG_EDAC)			:= edac_stub.o
obj-$(CONFIG_EDAC_MM_EDAC)		+= edac_core.o

edac_core-objs	:= edac_mc.o edac_device.o edac_mc_sysfs.o edac_pci_sysfs.o
edac_core-objs	+= edac_module.o edac_device_sysfs.o

ifdef CONFIG_PCI
edac_core-objs	+= edac_pci.o edac_pci_sysfs.o
endif

obj-$(CONFIG_EDAC_AMD76X)		+= amd76x_edac.o
obj-$(CONFIG_EDAC_I5000)		+= i5000_edac.o
obj-$(CONFIG_EDAC_I5100)		+= i5100_edac.o
obj-$(CONFIG_EDAC_I5400)		+= i5400_edac.o
obj-$(CONFIG_EDAC_E7XXX)		+= e7xxx_edac.o
obj-$(CONFIG_EDAC_E752X)		+= e752x_edac.o
obj-$(CONFIG_EDAC_I82443BXGX)		+= i82443bxgx_edac.o
obj-$(CONFIG_EDAC_I82875P)		+= i82875p_edac.o
obj-$(CONFIG_EDAC_I82975X)		+= i82975x_edac.o
obj-$(CONFIG_EDAC_I3000)		+= i3000_edac.o
obj-$(CONFIG_EDAC_X38)			+= x38_edac.o
obj-$(CONFIG_EDAC_I82860)		+= i82860_edac.o
obj-$(CONFIG_EDAC_R82600)		+= r82600_edac.o
obj-$(CONFIG_EDAC_PASEMI)		+= pasemi_edac.o
obj-$(CONFIG_EDAC_MPC85XX)		+= mpc85xx_edac.o
obj-$(CONFIG_EDAC_MV64X60)		+= mv64x60_edac.o
obj-$(CONFIG_EDAC_CELL)			+= cell_edac.o

