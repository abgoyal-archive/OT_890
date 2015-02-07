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
# Makefile for the linux memory manager.
#

mmu-y			:= nommu.o
mmu-$(CONFIG_MMU)	:= fremap.o highmem.o madvise.o memory.o mincore.o \
			   mlock.o mmap.o mprotect.o mremap.o msync.o rmap.o \
			   vmalloc.o

obj-y			:= bootmem.o filemap.o mempool.o oom_kill.o fadvise.o \
			   maccess.o page_alloc.o page-writeback.o pdflush.o \
			   readahead.o swap.o truncate.o vmscan.o shmem.o \
			   prio_tree.o util.o mmzone.o vmstat.o backing-dev.o \
			   page_isolation.o mm_init.o $(mmu-y)

obj-$(CONFIG_PROC_PAGE_MONITOR) += pagewalk.o
obj-$(CONFIG_BOUNCE)	+= bounce.o
obj-$(CONFIG_SWAP)	+= page_io.o swap_state.o swapfile.o thrash.o
obj-$(CONFIG_HAS_DMA)	+= dmapool.o
obj-$(CONFIG_HUGETLBFS)	+= hugetlb.o
obj-$(CONFIG_NUMA) 	+= mempolicy.o
obj-$(CONFIG_SPARSEMEM)	+= sparse.o
obj-$(CONFIG_SPARSEMEM_VMEMMAP) += sparse-vmemmap.o
obj-$(CONFIG_ASHMEM) += ashmem.o
obj-$(CONFIG_TMPFS_POSIX_ACL) += shmem_acl.o
obj-$(CONFIG_SLOB) += slob.o
obj-$(CONFIG_MMU_NOTIFIER) += mmu_notifier.o
obj-$(CONFIG_SLAB) += slab.o
obj-$(CONFIG_SLUB) += slub.o
obj-$(CONFIG_FAILSLAB) += failslab.o
obj-$(CONFIG_MEMORY_HOTPLUG) += memory_hotplug.o
obj-$(CONFIG_FS_XIP) += filemap_xip.o
obj-$(CONFIG_MIGRATION) += migrate.o
obj-$(CONFIG_SMP) += allocpercpu.o
obj-$(CONFIG_QUICKLIST) += quicklist.o
obj-$(CONFIG_CGROUP_MEM_RES_CTLR) += memcontrol.o page_cgroup.o
