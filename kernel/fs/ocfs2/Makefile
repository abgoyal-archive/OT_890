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


EXTRA_CFLAGS += -Ifs/ocfs2

EXTRA_CFLAGS += -DCATCH_BH_JBD_RACES

obj-$(CONFIG_OCFS2_FS) += 	\
	ocfs2.o			\
	ocfs2_stackglue.o

obj-$(CONFIG_OCFS2_FS_O2CB) += ocfs2_stack_o2cb.o
obj-$(CONFIG_OCFS2_FS_USERSPACE_CLUSTER) += ocfs2_stack_user.o

ocfs2-objs := \
	alloc.o 		\
	aops.o 			\
	blockcheck.o		\
	buffer_head_io.o	\
	dcache.o 		\
	dir.o 			\
	dlmglue.o 		\
	export.o 		\
	extent_map.o 		\
	file.o 			\
	heartbeat.o 		\
	inode.o 		\
	ioctl.o 		\
	journal.o 		\
	localalloc.o 		\
	locks.o			\
	mmap.o 			\
	namei.o 		\
	resize.o		\
	slot_map.o 		\
	suballoc.o 		\
	super.o 		\
	symlink.o 		\
	sysfile.o 		\
	uptodate.o		\
	ver.o			\
	quota_local.o		\
	quota_global.o		\
	xattr.o

ifeq ($(CONFIG_OCFS2_FS_POSIX_ACL),y)
ocfs2-objs += acl.o
endif

ocfs2_stackglue-objs := stackglue.o
ocfs2_stack_o2cb-objs := stack_o2cb.o
ocfs2_stack_user-objs := stack_user.o

# cluster/ is always needed when OCFS2_FS for masklog support
obj-$(CONFIG_OCFS2_FS) += cluster/
obj-$(CONFIG_OCFS2_FS_O2CB) += dlm/
