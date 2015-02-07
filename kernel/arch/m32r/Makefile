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
# m32r/Makefile
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies.
#

KBUILD_DEFCONFIG := m32700ut.smp_defconfig

LDFLAGS		:=
OBJCOPYFLAGS	:= -O binary -R .note -R .comment -S
LDFLAGS_vmlinux	:=

KBUILD_CFLAGS += -pipe -fno-schedule-insns
CFLAGS_KERNEL += -mmodel=medium
CFLAGS_MODULE += -mmodel=large

ifdef CONFIG_CHIP_VDEC2
cflags-$(CONFIG_ISA_M32R2)	+= -DNO_FPU -Wa,-bitinst
aflags-$(CONFIG_ISA_M32R2)	+= -DNO_FPU -O2 -Wa,-bitinst -Wa,-no-parallel
else
cflags-$(CONFIG_ISA_M32R2)	+= -DNO_FPU -m32r2
aflags-$(CONFIG_ISA_M32R2)	+= -DNO_FPU -m32r2 -O2
endif

cflags-$(CONFIG_ISA_M32R)	+= -DNO_FPU
aflags-$(CONFIG_ISA_M32R)	+= -DNO_FPU -O2 -Wa,-no-bitinst

KBUILD_CFLAGS += $(cflags-y)
KBUILD_AFLAGS += $(aflags-y)

CHECKFLAGS	+= -D__m32r__ -D__BIG_ENDIAN__=1

head-y	:= arch/m32r/kernel/head.o arch/m32r/kernel/init_task.o

LIBGCC	:= $(shell $(CC) $(KBUILD_CFLAGS) -print-libgcc-file-name)

libs-y	+= arch/m32r/lib/ $(LIBGCC)
core-y	+= arch/m32r/kernel/	\
	   arch/m32r/mm/	\
	   arch/m32r/boot/	\
	   arch/m32r/platforms/

drivers-$(CONFIG_OPROFILE)	+= arch/m32r/oprofile/

boot := arch/m32r/boot

PHONY += zImage

all: zImage

zImage: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@

compressed: zImage

archclean:
	$(Q)$(MAKE) $(clean)=$(boot)

define archhelp
	echo  '* zImage          - Compressed kernel image (arch/$(ARCH)/boot/zImage)'
endef
