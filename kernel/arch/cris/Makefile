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
# cris/Makefile
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies. Remember to do have actions
# for "archclean" and "archdep" for cleaning up and making dependencies for
# this architecture
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.

KBUILD_DEFCONFIG := etrax-100lx_v2_defconfig

arch-y := v10
arch-$(CONFIG_ETRAX_ARCH_V10) := v10
arch-$(CONFIG_ETRAX_ARCH_V32) := v32

# No config available for make clean etc
mach-y := fs
mach-$(CONFIG_CRIS_MACH_ARTPEC3) := a3
mach-$(CONFIG_ETRAXFS) := fs

ifneq ($(arch-y),)
SARCH := arch-$(arch-y)
inc := -Iarch/cris/include/$(SARCH)
inc += -Iarch/cris/include/$(SARCH)/arch
else
SARCH :=
inc :=
endif

ifneq ($(mach-y),)
MACH := mach-$(mach-y)
inc += -Iarch/cris/include/$(SARCH)/$(MACH)/
inc += -Iarch/cris/include/$(SARCH)/$(MACH)/mach
else
MACH :=
endif

LD = $(CROSS_COMPILE)ld -mcrislinux

OBJCOPYFLAGS := -O binary -R .note -R .comment -S

CPPFLAGS_vmlinux.lds = -DDRAM_VIRTUAL_BASE=0x$(CONFIG_ETRAX_DRAM_VIRTUAL_BASE)

KBUILD_AFLAGS += -mlinux -march=$(arch-y) $(inc)
KBUILD_CFLAGS += -mlinux -march=$(arch-y) -pipe $(inc)
KBUILD_CPPFLAGS += $(inc)

ifdef CONFIG_FRAME_POINTER
KBUILD_CFLAGS := $(subst -fomit-frame-pointer,,$(KBUILD_CFLAGS)) -g
KBUILD_CFLAGS += -fno-omit-frame-pointer
endif

head-y := arch/cris/$(SARCH)/kernel/head.o

LIBGCC = $(shell $(CC) $(KBUILD_CFLAGS) -print-file-name=libgcc.a)

core-y		+= arch/cris/kernel/ arch/cris/mm/
core-y		+= arch/cris/$(SARCH)/kernel/ arch/cris/$(SARCH)/mm/
ifdef CONFIG_ETRAX_ARCH_V32
core-y		+= arch/cris/$(SARCH)/$(MACH)/
endif
drivers-y	+= arch/cris/$(SARCH)/drivers/
libs-y		+= arch/cris/$(SARCH)/lib/ $(LIBGCC)

# cris source path
SRC_ARCH              = $(srctree)/arch/cris
# cris object files path
OBJ_ARCH              = $(objtree)/arch/cris

boot := arch/cris/$(SARCH)/boot
MACHINE := arch/cris/$(SARCH)

all: zImage

zImage Image: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $(boot)/$@

archprepare:

archclean:
	$(Q)if [ -e arch/cris/$(SARCH)/boot ]; then \
		$(MAKE) $(clean)=arch/cris/$(SARCH)/boot; \
	fi

CLEAN_FILES += \
	$(MACHINE)/boot/zImage \
	$(MACHINE)/boot/compressed/decompress.bin \
	$(MACHINE)/boot/compressed/piggy.gz \
	$(MACHINE)/boot/rescue/rescue.bin


# MRPROPER_FILES +=

define archhelp
  echo  '* zImage        - Compressed kernel image (arch/cris/boot/zImage)'
  echo  '* Image         - Uncompressed kernel image (arch/cris/boot/Image)'
endef
