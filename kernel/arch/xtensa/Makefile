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
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 2001 - 2005  Tensilica Inc.
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies. Remember to do have actions
# for "archclean" and "archdep" for cleaning up and making dependencies for
# this architecture

# Core configuration.
# (Use VAR=<xtensa_config> to use another default compiler.)

variant-$(CONFIG_XTENSA_VARIANT_FSF)		:= fsf
variant-$(CONFIG_XTENSA_VARIANT_DC232B)		:= dc232b
variant-$(CONFIG_XTENSA_VARIANT_LINUX_CUSTOM)	:= custom

VARIANT = $(variant-y)
export VARIANT

# Platform configuration

platform-$(CONFIG_XTENSA_PLATFORM_XT2000)	:= xt2000
platform-$(CONFIG_XTENSA_PLATFORM_ISS)		:= iss

PLATFORM = $(platform-y)
export PLATFORM

# temporarily until string.h is fixed
KBUILD_CFLAGS += -ffreestanding

KBUILD_CFLAGS += -pipe -mlongcalls

vardirs := $(patsubst %,arch/xtensa/variants/%/,$(variant-y))
plfdirs := $(patsubst %,arch/xtensa/platforms/%/,$(platform-y))

ifeq ($(KBUILD_SRC),)
KBUILD_CPPFLAGS += $(patsubst %,-I%include,$(vardirs) $(plfdirs))
else
KBUILD_CPPFLAGS += $(patsubst %,-I$(srctree)/%include,$(vardirs) $(plfdirs))
endif

KBUILD_DEFCONFIG := iss_defconfig

# ramdisk/initrd support
# You need a compressed ramdisk image, named ramdisk.gz in
# arch/xtensa/boot/ramdisk

core-$(CONFIG_EMBEDDED_RAMDISK)	+= arch/xtensa/boot/ramdisk/

# Test for cross compiling

ifneq ($(VARIANT),)
  COMPILE_ARCH = $(shell uname -m)

  ifneq ($(COMPILE_ARCH), xtensa)
    ifndef CROSS_COMPILE
      CROSS_COMPILE = xtensa_$(VARIANT)-
    endif
  endif
endif

#

LIBGCC := $(shell $(CC) $(KBUILD_CFLAGS) -print-libgcc-file-name)

head-y		:= arch/xtensa/kernel/head.o
core-y		+= arch/xtensa/kernel/ arch/xtensa/mm/
ifneq ($(PLATFORM),)
core-y		+= arch/xtensa/platforms/$(PLATFORM)/
endif
libs-y		+= arch/xtensa/lib/ $(LIBGCC)

boot		:= arch/xtensa/boot


all: zImage

bzImage : zImage

zImage zImage.initrd: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $@

define archhelp
  @echo '* zImage      - Compressed kernel image (arch/xtensa/boot/images/zImage.*)'
endef

