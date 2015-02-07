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
# Makefile for MIPS-specific library files..
#

lib-y	+= csum_partial.o memcpy.o memcpy-inatomic.o memset.o strlen_user.o \
	   strncpy_user.o strnlen_user.o uncached.o

obj-y			+= iomap.o
obj-$(CONFIG_PCI)	+= iomap-pci.o

obj-$(CONFIG_CPU_LOONGSON2)	+= dump_tlb.o
obj-$(CONFIG_CPU_MIPS32)	+= dump_tlb.o
obj-$(CONFIG_CPU_MIPS64)	+= dump_tlb.o
obj-$(CONFIG_CPU_NEVADA)	+= dump_tlb.o
obj-$(CONFIG_CPU_R10000)	+= dump_tlb.o
obj-$(CONFIG_CPU_R3000)		+= r3k_dump_tlb.o
obj-$(CONFIG_CPU_R4300)		+= dump_tlb.o
obj-$(CONFIG_CPU_R4X00)		+= dump_tlb.o
obj-$(CONFIG_CPU_R5000)		+= dump_tlb.o
obj-$(CONFIG_CPU_R5432)		+= dump_tlb.o
obj-$(CONFIG_CPU_R5500)		+= dump_tlb.o
obj-$(CONFIG_CPU_R6000)		+=
obj-$(CONFIG_CPU_R8000)		+=
obj-$(CONFIG_CPU_RM7000)	+= dump_tlb.o
obj-$(CONFIG_CPU_RM9000)	+= dump_tlb.o
obj-$(CONFIG_CPU_SB1)		+= dump_tlb.o
obj-$(CONFIG_CPU_TX39XX)	+= r3k_dump_tlb.o
obj-$(CONFIG_CPU_TX49XX)	+= dump_tlb.o
obj-$(CONFIG_CPU_VR41XX)	+= dump_tlb.o
obj-$(CONFIG_CPU_CAVIUM_OCTEON)	+= dump_tlb.o

# libgcc-style stuff needed in the kernel
obj-y += ashldi3.o ashrdi3.o cmpdi2.o lshrdi3.o ucmpdi2.o
