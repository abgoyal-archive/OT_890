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
# arch/arm/boot/Makefile
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies.
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1995-2002 Russell King
#

MKIMAGE         := $(srctree)/scripts/mkuboot.sh

ifneq ($(MACHINE),)
include $(srctree)/$(MACHINE)/Makefile.boot
endif

# Note: the following conditions must always be true:
#   ZRELADDR == virt_to_phys(PAGE_OFFSET + TEXT_OFFSET)
#   PARAMS_PHYS must be within 4MB of ZRELADDR
#   INITRD_PHYS must be in RAM
ZRELADDR    := $(zreladdr-y)
PARAMS_PHYS := $(params_phys-y)
INITRD_PHYS := $(initrd_phys-y)

export ZRELADDR INITRD_PHYS PARAMS_PHYS

targets := Image zImage xipImage bootpImage uImage

ifeq ($(CONFIG_XIP_KERNEL),y)

$(obj)/xipImage: vmlinux FORCE
	$(call if_changed,objcopy)
	@echo '  Kernel: $@ is ready (physical address: $(CONFIG_XIP_PHYS_ADDR))'

$(obj)/Image $(obj)/zImage: FORCE
	@echo 'Kernel configured for XIP (CONFIG_XIP_KERNEL=y)'
	@echo 'Only the xipImage target is available in this case'
	@false

else

$(obj)/xipImage: FORCE
	@echo 'Kernel not configured for XIP (CONFIG_XIP_KERNEL!=y)'
	@false

$(obj)/Image: vmlinux FORCE
	$(call if_changed,objcopy)
	@echo '  Kernel: $@ is ready'

$(obj)/compressed/vmlinux: $(obj)/Image FORCE
	$(Q)$(MAKE) $(build)=$(obj)/compressed $@

$(obj)/zImage:	$(obj)/compressed/vmlinux FORCE
	$(call if_changed,objcopy)
	@echo '  Kernel: $@ is ready'

endif

quiet_cmd_uimage = UIMAGE  $@
      cmd_uimage = $(CONFIG_SHELL) $(MKIMAGE) -A arm -O linux -T kernel \
		   -C none -a $(LOADADDR) -e $(LOADADDR) \
		   -n 'Linux-$(KERNELRELEASE)' -d $< $@

ifeq ($(CONFIG_ZBOOT_ROM),y)
$(obj)/uImage: LOADADDR=$(CONFIG_ZBOOT_ROM_TEXT)
else
$(obj)/uImage: LOADADDR=$(ZRELADDR)
endif

$(obj)/uImage:	$(obj)/zImage FORCE
	$(call if_changed,uimage)
	@echo '  Image $@ is ready'

$(obj)/bootp/bootp: $(obj)/zImage initrd FORCE
	$(Q)$(MAKE) $(build)=$(obj)/bootp $@
	@:

$(obj)/bootpImage: $(obj)/bootp/bootp FORCE
	$(call if_changed,objcopy)
	@echo '  Kernel: $@ is ready'

PHONY += initrd FORCE
initrd:
	@test "$(INITRD_PHYS)" != "" || \
	(echo This machine does not support INITRD; exit -1)
	@test "$(INITRD)" != "" || \
	(echo You must specify INITRD; exit -1)

install: $(obj)/Image
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh $(KERNELRELEASE) \
	$(obj)/Image System.map "$(INSTALL_PATH)"

zinstall: $(obj)/zImage
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh $(KERNELRELEASE) \
	$(obj)/zImage System.map "$(INSTALL_PATH)"

zi:
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh $(KERNELRELEASE) \
	$(obj)/zImage System.map "$(INSTALL_PATH)"

i:
	$(CONFIG_SHELL) $(srctree)/$(src)/install.sh $(KERNELRELEASE) \
	$(obj)/Image System.map "$(INSTALL_PATH)"

subdir-	    := bootp compressed
