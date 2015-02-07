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
# arch/x86/kernel/acpi/realmode/Makefile
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#

always		:= wakeup.bin
targets		:= wakeup.elf wakeup.lds

wakeup-y	+= wakeup.o wakemain.o video-mode.o copy.o

# The link order of the video-*.o modules can matter.  In particular,
# video-vga.o *must* be listed first, followed by video-vesa.o.
# Hardware-specific drivers should follow in the order they should be
# probed, and video-bios.o should typically be last.
wakeup-y	+= video-vga.o
wakeup-y	+= video-vesa.o
wakeup-y	+= video-bios.o

targets		+= $(wakeup-y)

bootsrc		:= $(src)/../../../boot

# ---------------------------------------------------------------------------

# How to compile the 16-bit code.  Note we always compile for -march=i386,
# that way we can complain to the user if the CPU is insufficient.
# Compile with _SETUP since this is similar to the boot-time setup code.
KBUILD_CFLAGS	:= $(LINUXINCLUDE) -g -Os -D_SETUP -D_WAKEUP -D__KERNEL__ \
		   -I$(srctree)/$(bootsrc) \
		   $(cflags-y) \
		   -Wall -Wstrict-prototypes \
		   -march=i386 -mregparm=3 \
		   -include $(srctree)/$(bootsrc)/code16gcc.h \
		   -fno-strict-aliasing -fomit-frame-pointer \
		   $(call cc-option, -ffreestanding) \
		   $(call cc-option, -fno-toplevel-reorder,\
			$(call cc-option, -fno-unit-at-a-time)) \
		   $(call cc-option, -fno-stack-protector) \
		   $(call cc-option, -mpreferred-stack-boundary=2)
KBUILD_CFLAGS	+= $(call cc-option, -m32)
KBUILD_AFLAGS	:= $(KBUILD_CFLAGS) -D__ASSEMBLY__

WAKEUP_OBJS = $(addprefix $(obj)/,$(wakeup-y))

LDFLAGS_wakeup.elf	:= -T

CPPFLAGS_wakeup.lds += -P -C

$(obj)/wakeup.elf: $(obj)/wakeup.lds $(WAKEUP_OBJS) FORCE
	$(call if_changed,ld)

OBJCOPYFLAGS_wakeup.bin	:= -O binary

$(obj)/wakeup.bin: $(obj)/wakeup.elf FORCE
	$(call if_changed,objcopy)
