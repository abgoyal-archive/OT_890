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
# Makefile for the linux kernel.
#

ifdef CONFIG_FUNCTION_TRACER
# Do not trace early boot code
CFLAGS_REMOVE_early.o = -pg
endif

#
# Passing null pointers is ok for smp code, since we access the lowcore here.
#
CFLAGS_smp.o	:= -Wno-nonnull

#
# Pass UTS_MACHINE for user_regset definition
#
CFLAGS_ptrace.o		+= -DUTS_MACHINE='"$(UTS_MACHINE)"'

obj-y	:=  bitmap.o traps.o time.o process.o base.o early.o setup.o \
	    processor.o sys_s390.o ptrace.o signal.o cpcmd.o ebcdic.o \
	    s390_ext.o debug.o irq.o ipl.o dis.o diag.o mem_detect.o \
	    vdso.o vtime.o

obj-y	+= $(if $(CONFIG_64BIT),entry64.o,entry.o)
obj-y	+= $(if $(CONFIG_64BIT),reipl64.o,reipl.o)

extra-y				+= head.o init_task.o vmlinux.lds

obj-$(CONFIG_MODULES)		+= s390_ksyms.o module.o
obj-$(CONFIG_SMP)		+= smp.o topology.o

obj-$(CONFIG_AUDIT)		+= audit.o
compat-obj-$(CONFIG_AUDIT)	+= compat_audit.o
obj-$(CONFIG_COMPAT)		+= compat_linux.o compat_signal.o \
					compat_wrapper.o compat_exec_domain.o \
					$(compat-obj-y)

obj-$(CONFIG_STACKTRACE)	+= stacktrace.o
obj-$(CONFIG_KPROBES)		+= kprobes.o
obj-$(CONFIG_FUNCTION_TRACER)	+= mcount.o

# Kexec part
S390_KEXEC_OBJS := machine_kexec.o crash.o
S390_KEXEC_OBJS += $(if $(CONFIG_64BIT),relocate_kernel64.o,relocate_kernel.o)
obj-$(CONFIG_KEXEC) += $(S390_KEXEC_OBJS)

# vdso
obj-$(CONFIG_64BIT)		+= vdso64/
obj-$(CONFIG_32BIT)		+= vdso32/
obj-$(CONFIG_COMPAT)		+= vdso32/
