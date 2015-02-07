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
# arch/h8300/Makefile
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# (C) Copyright 2002,2003 Yoshinori Sato <ysato@users.sourceforge.jp>
#

platform-$(CONFIG_CPU_H8300H)	:= h8300h
platform-$(CONFIG_CPU_H8S)	:= h8s
PLATFORM := $(platform-y)

board-$(CONFIG_H8300H_GENERIC)		:= generic
board-$(CONFIG_H8300H_AKI3068NET)	:= aki3068net
board-$(CONFIG_H8300H_H8MAX)		:= h8max
board-$(CONFIG_H8300H_SIM)		:= generic
board-$(CONFIG_H8S_GENERIC)		:= generic
board-$(CONFIG_H8S_EDOSK2674)		:= edosk2674
board-$(CONFIG_H8S_SIM)			:= generic
BOARD := $(board-y)

model-$(CONFIG_RAMKERNEL)	:= ram
model-$(CONFIG_ROMKERNEL)	:= rom
MODEL := $(model-y)

cflags-$(CONFIG_CPU_H8300H)	:= -mh
ldflags-$(CONFIG_CPU_H8300H)	:= -mh8300helf
cflags-$(CONFIG_CPU_H8S)	:= -ms
ldflags-$(CONFIG_CPU_H8S)	:= -mh8300self

KBUILD_CFLAGS += $(cflags-y)
KBUILD_CFLAGS += -mint32 -fno-builtin
KBUILD_CFLAGS += -g
KBUILD_CFLAGS += -D__linux__
KBUILD_CFLAGS += -DUTS_SYSNAME=\"uClinux\"
KBUILD_AFLAGS += -DPLATFORM=$(PLATFORM) -DMODEL=$(MODEL) $(cflags-y)
LDFLAGS += $(ldflags-y)

CROSS_COMPILE = h8300-elf-
LIBGCC := $(shell $(CROSS-COMPILE)$(CC) $(KBUILD_CFLAGS) -print-libgcc-file-name)

head-y := arch/$(ARCH)/platform/$(PLATFORM)/$(BOARD)/crt0_$(MODEL).o

core-y	+= arch/$(ARCH)/kernel/ \
	   arch/$(ARCH)/mm/
ifdef PLATFORM
core-y	+= arch/$(ARCH)/platform/$(PLATFORM)/ \
	   arch/$(ARCH)/platform/$(PLATFORM)/$(BOARD)/
endif

libs-y	+= arch/$(ARCH)/lib/ $(LIBGCC)

boot := arch/h8300/boot

export MODEL PLATFORM BOARD

archmrproper:

archclean:
	$(Q)$(MAKE) $(clean)=$(boot)

vmlinux.srec vmlinux.bin zImage: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@

define archhelp
  @echo  'vmlinux.bin  - Create raw binary'
  @echo  'vmlinux.srec - Create srec binary'
  @echo  'zImage       - Compressed kernel image'
endef
