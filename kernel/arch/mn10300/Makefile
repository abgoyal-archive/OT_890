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


###############################################################################
#
# MN10300 Kernel makefile system specifications
#
# Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
# Modified by David Howells (dhowells@redhat.com)
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public Licence
# as published by the Free Software Foundation; either version
# 2 of the Licence, or (at your option) any later version.
#
###############################################################################

KBUILD_DEFCONFIG := asb2303_defconfig

CCSPECS	:= $(shell $(CC) -v 2>&1 | grep "^Reading specs from " | head -1 | cut -c20-)
CCDIR	:= $(strip $(patsubst %/specs,%,$(CCSPECS)))
KBUILD_CPPFLAGS += -nostdinc -I$(CCDIR)/include

LDFLAGS		:=
OBJCOPYFLAGS	:= -O binary -R .note -R .comment -S
#LDFLAGS_vmlinux := -Map linkmap.txt
CHECKFLAGS	+=

PROCESSOR	:= unset
UNIT		:= unset

KBUILD_CFLAGS	+= -mam33 -mmem-funcs -DCPU=AM33
KBUILD_AFLAGS	+= -mam33 -DCPU=AM33

ifeq ($(CONFIG_MN10300_CURRENT_IN_E2),y)
KBUILD_CFLAGS	+= -ffixed-e2 -fcall-saved-e5
endif

ifeq ($(CONFIG_MN10300_PROC_MN103E010),y)
PROCESSOR	:= mn103e010
endif

ifeq ($(CONFIG_MN10300_UNIT_ASB2303),y)
UNIT		:= asb2303
endif
ifeq ($(CONFIG_MN10300_UNIT_ASB2305),y)
UNIT		:= asb2305
endif


head-y		:= arch/mn10300/kernel/head.o arch/mn10300/kernel/init_task.o

core-y		+= arch/mn10300/kernel/ arch/mn10300/mm/

ifneq ($(PROCESSOR),unset)
core-y		+= arch/mn10300/proc-$(PROCESSOR)/
endif
ifneq ($(UNIT),unset)
core-y		+= arch/mn10300/unit-$(UNIT)/
endif
libs-y		+= arch/mn10300/lib/

drivers-$(CONFIG_OPROFILE)	+= arch/mn10300/oprofile/

boot := arch/mn10300/boot

.PHONY: zImage

KBUILD_IMAGE := $(boot)/zImage
CLEAN_FILES += $(boot)/zImage
CLEAN_FILES += $(boot)/compressed/vmlinux
CLEAN_FILES += $(boot)/compressed/vmlinux.bin
CLEAN_FILES += $(boot)/compressed/vmlinux.bin.gz

zImage: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@

all: zImage

bootstrap:
	$(Q)$(MAKEBOOT) bootstrap

archclean:
	$(Q)$(MAKE) $(clean)=arch/mn10300/proc-mn103e010
	$(Q)$(MAKE) $(clean)=arch/mn10300/unit-asb2303
	$(Q)$(MAKE) $(clean)=arch/mn10300/unit-asb2305

define archhelp
  echo  '* zImage        - Compressed kernel image (arch/$(ARCH)/boot/zImage)'
endef

# If you make sure the .S files get compiled with debug info,
# uncomment the following to disable optimisations
# that are unhelpful whilst debugging.
ifdef CONFIG_DEBUG_INFO
#KBUILD_CFLAGS	+= -O1
KBUILD_AFLAGS	+= -Wa,--gdwarf2
endif

###################################################################################################
#
# juggle some symlinks in the MN10300 asm include dir
#
#	Update machine proc and unit symlinks if something which affects
#	them changed.  We use .proc / .unit to indicate when they were
#	updated last, otherwise make uses the target directory mtime.
#
###################################################################################################

# processor specific definitions
include/asm-mn10300/.proc: $(wildcard include/config/proc/*.h) include/config/auto.conf
	@echo '  SYMLINK include/asm-mn10300/proc -> include/asm-mn10300/proc-$(PROCESSOR)'
ifneq ($(KBUILD_SRC),)
	$(Q)mkdir -p include/asm-mn10300
	$(Q)ln -fsn $(srctree)/include/asm-mn10300/proc-$(PROCESSOR) include/asm-mn10300/proc
else
	$(Q)ln -fsn proc-$(PROCESSOR) include/asm-mn10300/proc
endif
	@touch $@

CLEAN_FILES += include/asm-mn10300/proc include/asm-mn10300/.proc

prepare: include/asm-mn10300/.proc

# unit specific definitions
include/asm-mn10300/.unit: $(wildcard include/config/unit/*.h) include/config/auto.conf
	@echo '  SYMLINK include/asm-mn10300/unit -> include/asm-mn10300/unit-$(UNIT)'
ifneq ($(KBUILD_SRC),)
	$(Q)mkdir -p include/asm-mn10300
	$(Q)ln -fsn $(srctree)/include/asm-mn10300/unit-$(UNIT) include/asm-mn10300/unit
else
	$(Q)ln -fsn unit-$(UNIT) include/asm-mn10300/unit
endif
	@touch $@

CLEAN_FILES += include/asm-mn10300/unit include/asm-mn10300/.unit

prepare: include/asm-mn10300/.unit
