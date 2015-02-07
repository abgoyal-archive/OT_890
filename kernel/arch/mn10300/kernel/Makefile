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
# Makefile for the MN10300-specific core kernel code
#
extra-y := head.o init_task.o vmlinux.lds

obj-y   := process.o signal.o entry.o fpu.o traps.o irq.o \
	   ptrace.o setup.o time.o sys_mn10300.o io.o kthread.o \
	   switch_to.o mn10300_ksyms.o kernel_execve.o

obj-$(CONFIG_MN10300_WD_TIMER) += mn10300-watchdog.o mn10300-watchdog-low.o

obj-$(CONFIG_FPU) += fpu-low.o

obj-$(CONFIG_MN10300_TTYSM) += mn10300-serial.o mn10300-serial-low.o \
			       mn10300-debug.o
obj-$(CONFIG_GDBSTUB) += gdb-stub.o gdb-low.o
obj-$(CONFIG_GDBSTUB_ON_TTYSx) += gdb-io-serial.o gdb-io-serial-low.o
obj-$(CONFIG_GDBSTUB_ON_TTYSMx) += gdb-io-ttysm.o gdb-io-ttysm-low.o

ifneq ($(CONFIG_MN10300_CACHE_DISABLED),y)
obj-$(CONFIG_GDBSTUB) += gdb-cache.o
endif

obj-$(CONFIG_MN10300_RTC) += rtc.o
obj-$(CONFIG_PROFILE) += profile.o profile-low.o
obj-$(CONFIG_MODULES) += module.o
obj-$(CONFIG_KPROBES) += kprobes.o
