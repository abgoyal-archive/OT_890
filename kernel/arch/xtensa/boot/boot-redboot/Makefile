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
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#

GZIP = gzip
GZIP_FLAGS = -v9fc
ifeq ($(BIG_ENDIAN),1)
OBJCOPY_ARGS 	:= -O elf32-xtensa-be
else
OBJCOPY_ARGS 	:= -O elf32-xtensa-le
endif

LD_ARGS	= -T $(srctree)/$(obj)/boot.ld

boot-y	:= bootstrap.o

OBJS	:= $(addprefix $(obj)/,$(boot-y))
LIBS	:= arch/xtensa/boot/lib/lib.a arch/xtensa/lib/lib.a

LIBGCC := $(shell $(CC) $(KBUILD_CFLAGS) -print-libgcc-file-name)

zImage: vmlinux $(OBJS) $(LIBS)
	$(OBJCOPY) --strip-all -R .comment -R .note.gnu.build-id -O binary \
		vmlinux vmlinux.tmp
	gzip -vf9 vmlinux.tmp
	$(OBJCOPY) $(OBJCOPY_ARGS) -R .comment \
		--add-section image=vmlinux.tmp.gz \
		--set-section-flags image=contents,alloc,load,load,data \
		$(OBJS) $@.tmp
	$(LD) $(LD_ARGS) -o $@.elf $@.tmp $(LIBS) -L/xtensa-elf/lib $(LIBGCC)
	$(OBJCOPY) -S -O binary $@.elf arch/$(ARCH)/boot/$@.redboot
	rm -f $@.tmp $@.elf vmlinux.tmp.gz

