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
# Makefile for the linux ppc-specific parts of the memory manager.
#

ifeq ($(CONFIG_PPC64),y)
EXTRA_CFLAGS	+= -mno-minimal-toc
endif

obj-y				:= fault.o mem.o pgtable.o \
				   init_$(CONFIG_WORD_SIZE).o \
				   pgtable_$(CONFIG_WORD_SIZE).o
obj-$(CONFIG_PPC_MMU_NOHASH)	+= mmu_context_nohash.o tlb_nohash.o \
				   tlb_nohash_low.o
hash-$(CONFIG_PPC_NATIVE)	:= hash_native_64.o
obj-$(CONFIG_PPC64)		+= hash_utils_64.o \
				   slb_low.o slb.o stab.o \
				   gup.o mmap.o $(hash-y)
obj-$(CONFIG_PPC_STD_MMU_32)	+= ppc_mmu_32.o
obj-$(CONFIG_PPC_STD_MMU)	+= hash_low_$(CONFIG_WORD_SIZE).o \
				   tlb_hash$(CONFIG_WORD_SIZE).o \
				   mmu_context_hash$(CONFIG_WORD_SIZE).o
obj-$(CONFIG_40x)		+= 40x_mmu.o
obj-$(CONFIG_44x)		+= 44x_mmu.o
obj-$(CONFIG_FSL_BOOKE)		+= fsl_booke_mmu.o
obj-$(CONFIG_NEED_MULTIPLE_NODES) += numa.o
obj-$(CONFIG_PPC_MM_SLICES)	+= slice.o
obj-$(CONFIG_HUGETLB_PAGE)	+= hugetlbpage.o
obj-$(CONFIG_PPC_SUBPAGE_PROT)	+= subpage-prot.o
