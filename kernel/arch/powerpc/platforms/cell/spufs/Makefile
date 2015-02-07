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



obj-$(CONFIG_SPU_FS) += spufs.o
spufs-y += inode.o file.o context.o syscalls.o coredump.o
spufs-y += sched.o backing_ops.o hw_ops.o run.o gang.o
spufs-y += switch.o fault.o lscsa_alloc.o

obj-$(CONFIG_SPU_TRACE)	+= sputrace.o

# Rules to build switch.o with the help of SPU tool chain
SPU_CROSS	:= spu-
SPU_CC		:= $(SPU_CROSS)gcc
SPU_AS		:= $(SPU_CROSS)gcc
SPU_LD		:= $(SPU_CROSS)ld
SPU_OBJCOPY	:= $(SPU_CROSS)objcopy
SPU_CFLAGS	:= -O2 -Wall -I$(srctree)/include \
		   -I$(objtree)/include2 -D__KERNEL__
SPU_AFLAGS	:= -c -D__ASSEMBLY__ -I$(srctree)/include \
		   -I$(objtree)/include2 -D__KERNEL__
SPU_LDFLAGS	:= -N -Ttext=0x0

$(obj)/switch.o: $(obj)/spu_save_dump.h $(obj)/spu_restore_dump.h
clean-files := spu_save_dump.h spu_restore_dump.h

# Compile SPU files
      cmd_spu_cc = $(SPU_CC) $(SPU_CFLAGS) -c -o $@ $<
quiet_cmd_spu_cc = SPU_CC  $@
$(obj)/spu_%.o: $(src)/spu_%.c
	$(call if_changed,spu_cc)

# Assemble SPU files
      cmd_spu_as = $(SPU_AS) $(SPU_AFLAGS) -o $@ $<
quiet_cmd_spu_as = SPU_AS  $@
$(obj)/spu_%.o: $(src)/spu_%.S
	$(call if_changed,spu_as)

# Link SPU Executables
      cmd_spu_ld = $(SPU_LD) $(SPU_LDFLAGS) -o $@ $^
quiet_cmd_spu_ld = SPU_LD  $@
$(obj)/spu_%: $(obj)/spu_%_crt0.o $(obj)/spu_%.o
	$(call if_changed,spu_ld)

# Copy into binary format
      cmd_spu_objcopy = $(SPU_OBJCOPY) -O binary $< $@
quiet_cmd_spu_objcopy = OBJCOPY $@
$(obj)/spu_%.bin: $(src)/spu_%
	$(call if_changed,spu_objcopy)

# create C code from ELF executable
cmd_hexdump   = ( \
		echo "/*" ; \
		echo " * $*_dump.h: Copyright (C) 2005 IBM." ; \
		echo " * Hex-dump auto generated from $*.c." ; \
		echo " * Do not edit!" ; \
		echo " */" ; \
		echo "static unsigned int $*_code[] " \
			"__attribute__((__aligned__(128))) = {" ; \
		hexdump -v -e '"0x" 4/1 "%02x" "," "\n"' $< ; \
		echo "};" ; \
		) > $@
quiet_cmd_hexdump = HEXDUMP $@
$(obj)/%_dump.h: $(obj)/%.bin
	$(call if_changed,hexdump)
