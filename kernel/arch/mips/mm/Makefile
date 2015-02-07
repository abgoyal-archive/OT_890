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
# Makefile for the Linux/MIPS-specific parts of the memory manager.
#

obj-y				+= cache.o dma-default.o extable.o fault.o \
				   init.o tlbex.o tlbex-fault.o uasm.o page.o

obj-$(CONFIG_32BIT)		+= ioremap.o pgtable-32.o
obj-$(CONFIG_64BIT)		+= pgtable-64.o
obj-$(CONFIG_HIGHMEM)		+= highmem.o

obj-$(CONFIG_CPU_LOONGSON2)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_MIPS32)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_MIPS64)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_NEVADA)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_R10000)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_R3000)		+= c-r3k.o tlb-r3k.o
obj-$(CONFIG_CPU_R4300)		+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_R4X00)		+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_R5000)		+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_R5432)		+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_R5500)		+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_R8000)		+= c-r4k.o cex-gen.o tlb-r8k.o
obj-$(CONFIG_CPU_RM7000)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_RM9000)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_SB1)		+= c-r4k.o cerr-sb1.o cex-sb1.o tlb-r4k.o
obj-$(CONFIG_CPU_TX39XX)	+= c-tx39.o tlb-r3k.o
obj-$(CONFIG_CPU_TX49XX)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_VR41XX)	+= c-r4k.o cex-gen.o tlb-r4k.o
obj-$(CONFIG_CPU_CAVIUM_OCTEON)	+= c-octeon.o cex-oct.o tlb-r4k.o

obj-$(CONFIG_IP22_CPU_SCACHE)	+= sc-ip22.o
obj-$(CONFIG_R5000_CPU_SCACHE)  += sc-r5k.o
obj-$(CONFIG_RM7000_CPU_SCACHE)	+= sc-rm7k.o
obj-$(CONFIG_MIPS_CPU_SCACHE)	+= sc-mips.o

EXTRA_CFLAGS += -Werror
