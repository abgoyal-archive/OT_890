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
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1995-2000 Russell King
#

targets := Image zImage bootpImage

SYSTEM	=$(LINUX)

ZTEXTADDR	 = 0x02080000
PARAMS_PHYS	 = 0x0207c000
INITRD_PHYS	 = 0x02180000
INITRD_VIRT	 = 0x02180000

#
# If you don't define ZRELADDR above,
# then it defaults to ZTEXTADDR
#
ifeq ($(ZRELADDR),)
ZRELADDR	= $(ZTEXTADDR)
endif

export	SYSTEM ZTEXTADDR ZBSSADDR ZRELADDR INITRD_PHYS INITRD_VIRT PARAMS_PHYS

Image: $(obj)/Image

targets: $(obj)/Image

$(obj)/Image: vmlinux FORCE
	$(OBJCOPY) -O binary -R .note -R .comment -S vmlinux $@

#$(obj)/Image:	$(CONFIGURE) $(SYSTEM)
#	$(OBJCOPY) -O binary -R .note -R .comment -g -S $(SYSTEM) $@

bzImage: zImage

zImage:	$(CONFIGURE) compressed/$(LINUX)
	$(OBJCOPY) -O binary -R .note -R .comment -S compressed/$(LINUX) $@

bootpImage: bootp/bootp
	$(OBJCOPY) -O binary -R .note -R .comment -S bootp/bootp $@

compressed/$(LINUX): $(LINUX) dep
	@$(MAKE) -C compressed $(LINUX)

bootp/bootp: zImage initrd
	@$(MAKE) -C bootp bootp

initrd:
	@test "$(INITRD_VIRT)" != "" || (echo This architecture does not support INITRD; exit -1)
	@test "$(INITRD)" != "" || (echo You must specify INITRD; exit -1)

#
# installation
#
install: $(CONFIGURE) Image
	sh ./install.sh $(KERNELRELEASE) Image System.map "$(INSTALL_PATH)"

zinstall: $(CONFIGURE) zImage
	sh ./install.sh $(KERNELRELEASE) zImage System.map "$(INSTALL_PATH)"

#
# miscellany
#
mrproper clean:
#	@$(MAKE) -C compressed clean
#	@$(MAKE) -C bootp clean

dep:
