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


EXTRA_CFLAGS += -DIPATH_IDSTR='"QLogic kernel.org driver"' \
	-DIPATH_KERN_TYPE=0

obj-$(CONFIG_INFINIBAND_IPATH) += ib_ipath.o

ib_ipath-y := \
	ipath_cq.o \
	ipath_diag.o \
	ipath_dma.o \
	ipath_driver.o \
	ipath_eeprom.o \
	ipath_file_ops.o \
	ipath_fs.o \
	ipath_init_chip.o \
	ipath_intr.o \
	ipath_keys.o \
	ipath_mad.o \
	ipath_mmap.o \
	ipath_mr.o \
	ipath_qp.o \
	ipath_rc.o \
	ipath_ruc.o \
	ipath_sdma.o \
	ipath_srq.o \
	ipath_stats.o \
	ipath_sysfs.o \
	ipath_uc.o \
	ipath_ud.o \
	ipath_user_pages.o \
	ipath_user_sdma.o \
	ipath_verbs_mcast.o \
	ipath_verbs.o \
	ipath_iba7220.o \
	ipath_sd7220.o \
	ipath_sd7220_img.o

ib_ipath-$(CONFIG_HT_IRQ) += ipath_iba6110.o
ib_ipath-$(CONFIG_PCI_MSI) += ipath_iba6120.o

ib_ipath-$(CONFIG_X86_64) += ipath_wc_x86_64.o
ib_ipath-$(CONFIG_PPC64) += ipath_wc_ppc64.o
