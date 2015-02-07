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
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.


#
# Makefile for the linux kernel.
#

AFLAGS_head.o := -DTEXT_OFFSET=$(TEXT_OFFSET)

ifdef CONFIG_DYNAMIC_FTRACE
CFLAGS_REMOVE_ftrace.o = -pg
endif

# Object file lists.

obj-y		:= compat.o elf.o entry-armv.o entry-common.o irq.o \
		   process.o ptrace.o setup.o signal.o \
		   sys_arm.o stacktrace.o time.o traps.o

obj-$(CONFIG_ISA_DMA_API)	+= dma.o
obj-$(CONFIG_ARCH_ACORN)	+= ecard.o 
obj-$(CONFIG_FIQ)		+= fiq.o
obj-$(CONFIG_MODULES)		+= armksyms.o module.o
obj-$(CONFIG_ARTHUR)		+= arthur.o
obj-$(CONFIG_ISA_DMA)		+= dma-isa.o
obj-$(CONFIG_PCI)		+= bios32.o isa.o
obj-$(CONFIG_SMP)		+= smp.o
obj-$(CONFIG_DYNAMIC_FTRACE)	+= ftrace.o
obj-$(CONFIG_KEXEC)		+= machine_kexec.o relocate_kernel.o
obj-$(CONFIG_KPROBES)		+= kprobes.o kprobes-decode.o
obj-$(CONFIG_ATAGS_PROC)	+= atags.o
obj-$(CONFIG_OABI_COMPAT)	+= sys_oabi-compat.o
obj-$(CONFIG_ARM_THUMBEE)	+= thumbee.o
obj-$(CONFIG_KGDB)		+= kgdb.o
obj-$(CONFIG_HAVE_TCM)		+= tcm.o

obj-$(CONFIG_CRUNCH)		+= crunch.o crunch-bits.o
AFLAGS_crunch-bits.o		:= -Wa,-mcpu=ep9312

obj-$(CONFIG_CPU_XSCALE)	+= xscale-cp0.o
obj-$(CONFIG_CPU_XSC3)		+= xscale-cp0.o
obj-$(CONFIG_IWMMXT)		+= iwmmxt.o
AFLAGS_iwmmxt.o			:= -Wa,-mcpu=iwmmxt

ifneq ($(CONFIG_ARCH_EBSA110),y)
  obj-y		+= io.o
endif

head-y			:= head$(MMUEXT).o
obj-$(CONFIG_DEBUG_LL)	+= debug.o

extra-y := $(head-y) init_task.o vmlinux.lds
