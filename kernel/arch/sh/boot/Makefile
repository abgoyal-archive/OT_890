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
# arch/sh/boot/Makefile
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1999 Stuart Menefy
#

MKIMAGE := $(srctree)/scripts/mkuboot.sh

#
# Assign safe dummy values if these variables are not defined,
# in order to suppress error message.
#
CONFIG_PAGE_OFFSET	?= 0x80000000
CONFIG_MEMORY_START	?= 0x0c000000
CONFIG_BOOT_LINK_OFFSET	?= 0x00800000
CONFIG_ZERO_PAGE_OFFSET	?= 0x00001000
CONFIG_ENTRY_OFFSET	?= 0x00001000

export CONFIG_PAGE_OFFSET CONFIG_MEMORY_START CONFIG_BOOT_LINK_OFFSET \
       CONFIG_ZERO_PAGE_OFFSET CONFIG_ENTRY_OFFSET

targets := zImage vmlinux.srec uImage uImage.srec
subdir- := compressed

$(obj)/zImage: $(obj)/compressed/vmlinux FORCE
	$(call if_changed,objcopy)
	@echo '  Kernel: $@ is ready'

$(obj)/compressed/vmlinux: FORCE
	$(Q)$(MAKE) $(build)=$(obj)/compressed $@

ifeq ($(CONFIG_32BIT),y)
KERNEL_LOAD	:= $(shell /bin/bash -c 'printf "0x%08x" \
		     $$[$(CONFIG_PAGE_OFFSET)  + \
			$(CONFIG_ZERO_PAGE_OFFSET)]')
else
KERNEL_LOAD	:= $(shell /bin/bash -c 'printf "0x%08x" \
		     $$[$(CONFIG_PAGE_OFFSET)  + \
			$(CONFIG_MEMORY_START) + \
			$(CONFIG_ZERO_PAGE_OFFSET)]')
endif

KERNEL_ENTRY	:= $(shell /bin/bash -c 'printf "0x%08x" \
		     $$[$(CONFIG_PAGE_OFFSET)  + \
			$(CONFIG_MEMORY_START) + \
			$(CONFIG_ZERO_PAGE_OFFSET) + $(CONFIG_ENTRY_OFFSET)]')

quiet_cmd_uimage = UIMAGE  $@
      cmd_uimage = $(CONFIG_SHELL) $(MKIMAGE) -A sh -O linux -T kernel \
		   -C gzip -a $(KERNEL_LOAD) -e $(KERNEL_ENTRY) \
		   -n 'Linux-$(KERNELRELEASE)' -d $< $@

$(obj)/uImage: $(obj)/vmlinux.bin.gz FORCE
	$(call if_changed,uimage)
	@echo '  Image $@ is ready'

$(obj)/vmlinux.bin: vmlinux FORCE
	$(call if_changed,objcopy)

$(obj)/vmlinux.bin.gz: $(obj)/vmlinux.bin FORCE
	$(call if_changed,gzip)

OBJCOPYFLAGS_vmlinux.srec := -I binary -O srec
$(obj)/vmlinux.srec: $(obj)/compressed/vmlinux
	$(call if_changed,objcopy)

OBJCOPYFLAGS_uImage.srec := -I binary -O srec
$(obj)/uImage.srec: $(obj)/uImage
	$(call if_changed,objcopy)

clean-files	+= uImage uImage.srec vmlinux.srec \
		   vmlinux.bin vmlinux.bin.gz
