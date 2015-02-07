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
# linux/arch/sh/boot/compressed/Makefile
#
# create a compressed vmlinux image from the original vmlinux
#

targets		:= vmlinux vmlinux.bin vmlinux.bin.gz head.o misc.o piggy.o
EXTRA_AFLAGS	:= -traditional

OBJECTS = $(obj)/head.o $(obj)/misc.o

#
# IMAGE_OFFSET is the load offset of the compression loader
# Assign dummy values if these 2 variables are not defined,
# in order to suppress error message.
#
CONFIG_MEMORY_START     ?= 0x00400000
CONFIG_BOOT_LINK_OFFSET ?= 0x00140000
IMAGE_OFFSET := $(shell printf "0x%08x" $$[$(CONFIG_MEMORY_START)+$(CONFIG_BOOT_LINK_OFFSET)])

LDFLAGS_vmlinux := -Ttext $(IMAGE_OFFSET) -estartup $(obj)/vmlinux.lds

$(obj)/vmlinux: $(OBJECTS) $(obj)/piggy.o FORCE
	$(call if_changed,ld)
	@:

$(obj)/vmlinux.bin: vmlinux FORCE
	$(call if_changed,objcopy)

$(obj)/vmlinux.bin.gz: $(obj)/vmlinux.bin FORCE
	$(call if_changed,gzip)

LDFLAGS_piggy.o := -r --format binary --oformat elf32-h8300 -T
OBJCOPYFLAGS := -O binary

$(obj)/piggy.o: $(obj)/vmlinux.scr $(obj)/vmlinux.bin.gz FORCE
	$(call if_changed,ld)
