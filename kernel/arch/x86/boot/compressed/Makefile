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
# linux/arch/x86/boot/compressed/Makefile
#
# create a compressed vmlinux image from the original vmlinux
#

targets := vmlinux vmlinux.bin vmlinux.bin.gz head_$(BITS).o misc.o piggy.o

KBUILD_CFLAGS := -m$(BITS) -D__KERNEL__ $(LINUX_INCLUDE) -O2
KBUILD_CFLAGS += -fno-strict-aliasing -fPIC
cflags-$(CONFIG_X86_64) := -mcmodel=small
KBUILD_CFLAGS += $(cflags-y)
KBUILD_CFLAGS += $(call cc-option,-ffreestanding)
KBUILD_CFLAGS += $(call cc-option,-fno-stack-protector)

KBUILD_AFLAGS  := $(KBUILD_CFLAGS) -D__ASSEMBLY__

LDFLAGS := -m elf_$(UTS_MACHINE)
LDFLAGS_vmlinux := -T

$(obj)/vmlinux: $(src)/vmlinux_$(BITS).lds $(obj)/head_$(BITS).o $(obj)/misc.o $(obj)/piggy.o FORCE
	$(call if_changed,ld)
	@:

OBJCOPYFLAGS_vmlinux.bin :=  -R .comment -S
$(obj)/vmlinux.bin: vmlinux FORCE
	$(call if_changed,objcopy)


targets += vmlinux.bin.all vmlinux.relocs relocs
hostprogs-$(CONFIG_X86_32) += relocs

quiet_cmd_relocs = RELOCS  $@
      cmd_relocs = $(obj)/relocs $< > $@;$(obj)/relocs --abs-relocs $<
$(obj)/vmlinux.relocs: vmlinux $(obj)/relocs FORCE
	$(call if_changed,relocs)

vmlinux.bin.all-y := $(obj)/vmlinux.bin
vmlinux.bin.all-$(CONFIG_RELOCATABLE) += $(obj)/vmlinux.relocs
quiet_cmd_relocbin = BUILD   $@
      cmd_relocbin = cat $(filter-out FORCE,$^) > $@
$(obj)/vmlinux.bin.all: $(vmlinux.bin.all-y) FORCE
	$(call if_changed,relocbin)

ifeq ($(CONFIG_X86_32),y)

ifdef CONFIG_RELOCATABLE
$(obj)/vmlinux.bin.gz: $(obj)/vmlinux.bin.all FORCE
	$(call if_changed,gzip)
else
$(obj)/vmlinux.bin.gz: $(obj)/vmlinux.bin FORCE
	$(call if_changed,gzip)
endif
LDFLAGS_piggy.o := -r --format binary --oformat elf32-i386 -T

else
$(obj)/vmlinux.bin.gz: $(obj)/vmlinux.bin FORCE
	$(call if_changed,gzip)

LDFLAGS_piggy.o := -r --format binary --oformat elf64-x86-64 -T
endif

$(obj)/piggy.o: $(obj)/vmlinux.scr $(obj)/vmlinux.bin.gz FORCE
	$(call if_changed,ld)
