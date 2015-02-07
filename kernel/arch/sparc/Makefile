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
# sparc/Makefile
#
# Makefile for the architecture dependent flags and dependencies on the
# Sparc and sparc64.
#
# Copyright (C) 1994,1996,1998 David S. Miller (davem@caip.rutgers.edu)
# Copyright (C) 1998 Jakub Jelinek (jj@ultra.linux.cz)

# We are not yet configured - so test on arch
ifeq ($(ARCH),sparc)
        KBUILD_DEFCONFIG := sparc32_defconfig
else
        KBUILD_DEFCONFIG := sparc64_defconfig
endif

ifeq ($(CONFIG_SPARC32),y)
#####
# sparc32
#

#
# Uncomment the first KBUILD_CFLAGS if you are doing kgdb source level
# debugging of the kernel to get the proper debugging information.

AS             := $(AS) -32
LDFLAGS        := -m elf32_sparc
CHECKFLAGS     += -D__sparc__
export BITS    := 32

#KBUILD_CFLAGS += -g -pipe -fcall-used-g5 -fcall-used-g7
KBUILD_CFLAGS += -m32 -pipe -mno-fpu -fcall-used-g5 -fcall-used-g7
KBUILD_AFLAGS += -m32
CPPFLAGS_vmlinux.lds += -m32

#LDFLAGS_vmlinux = -N -Ttext 0xf0004000
#  Since 2.5.40, the first stage is left not btfix-ed.
#  Actual linking is done with "make image".
LDFLAGS_vmlinux = -r

# Default target
all: zImage


else
#####
# sparc64
#

CHECKFLAGS      += -D__sparc__ -D__sparc_v9__ -D__arch64__ -m64

# Undefine sparc when processing vmlinux.lds - it is used
# And teach CPP we are doing 64 bit builds (for this case)
CPPFLAGS_vmlinux.lds += -m64 -Usparc
LDFLAGS              := -m elf64_sparc
export BITS          := 64

KBUILD_CFLAGS += -m64 -pipe -mno-fpu -mcpu=ultrasparc -mcmodel=medlow   \
                 -ffixed-g4 -ffixed-g5 -fcall-used-g7 -Wno-sign-compare \
                 -Wa,--undeclared-regs
KBUILD_CFLAGS += $(call cc-option,-mtune=ultrasparc3)
KBUILD_AFLAGS += -m64 -mcpu=ultrasparc -Wa,--undeclared-regs

ifeq ($(CONFIG_MCOUNT),y)
  KBUILD_CFLAGS += -pg
endif

endif

head-y                 := arch/sparc/kernel/head_$(BITS).o
head-y                 += arch/sparc/kernel/init_task.o

core-y                 += arch/sparc/kernel/
core-y                 += arch/sparc/mm/ arch/sparc/math-emu/

libs-y                 += arch/sparc/prom/
libs-y                 += arch/sparc/lib/

drivers-$(CONFIG_OPROFILE)	+= arch/sparc/oprofile/

# Export what is needed by arch/sparc/boot/Makefile
export VMLINUX_INIT VMLINUX_MAIN
VMLINUX_INIT := $(head-y) $(init-y)
VMLINUX_MAIN := $(core-y) kernel/ mm/ fs/ ipc/ security/ crypto/ block/
VMLINUX_MAIN += $(patsubst %/, %/lib.a, $(libs-y)) $(libs-y)
VMLINUX_MAIN += $(drivers-y) $(net-y)

ifdef CONFIG_KALLSYMS
export kallsyms.o := .tmp_kallsyms2.o
endif

boot := arch/sparc/boot

image zImage tftpboot.img vmlinux.aout: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@

archclean:
	$(Q)$(MAKE) $(clean)=$(boot)

# This is the image used for packaging
KBUILD_IMAGE := $(boot)/zImage

# Don't use tabs in echo arguments.
ifeq ($(ARCH),sparc)
define archhelp
  echo  '* image        - kernel image ($(boot)/image)'
  echo  '* zImage       - stripped kernel image ($(boot)/zImage)'
  echo  '  tftpboot.img - image prepared for tftp'
endef
else
define archhelp
  echo  '* vmlinux       - Standard sparc64 kernel'
  echo  '  vmlinux.aout  - a.out kernel for sparc64'
  echo  '  tftpboot.img - image prepared for tftp'
endef
endif
