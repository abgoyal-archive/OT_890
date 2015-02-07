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
# Makefile for the linux reiser-filesystem routines.
#

obj-$(CONFIG_REISERFS_FS) += reiserfs.o

reiserfs-objs := bitmap.o do_balan.o namei.o inode.o file.o dir.o fix_node.o \
		 super.o prints.o objectid.o lbalance.o ibalance.o stree.o \
		 hashes.o tail_conversion.o journal.o resize.o \
		 item_ops.o ioctl.o procfs.o

ifeq ($(CONFIG_REISERFS_FS_XATTR),y)
reiserfs-objs += xattr.o xattr_user.o xattr_trusted.o
endif

ifeq ($(CONFIG_REISERFS_FS_SECURITY),y)
reiserfs-objs += xattr_security.o
endif

ifeq ($(CONFIG_REISERFS_FS_POSIX_ACL),y)
reiserfs-objs += xattr_acl.o
endif

# gcc -O2 (the kernel default)  is overaggressive on ppc32 when many inline
# functions are used.  This causes the compiler to advance the stack
# pointer out of the available stack space, corrupting kernel space,
# and causing a panic. Since this behavior only affects ppc32, this ifeq
# will work around it. If any other architecture displays this behavior,
# add it here.
ifeq ($(CONFIG_PPC32),y)
EXTRA_CFLAGS := $(call cc-ifversion, -lt, 0400, -O1)
endif

TAGS:
	etags *.c

