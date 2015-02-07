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


obj-y += vsyscall.o vsyscall-syscall.o

$(obj)/vsyscall-syscall.o: \
	$(foreach F,trapa,$(obj)/vsyscall-$F.so)

# Teach kbuild about targets
targets += $(foreach F,trapa,vsyscall-$F.o vsyscall-$F.so)
targets += vsyscall-note.o vsyscall.lds

# The DSO images are built using a special linker script
quiet_cmd_syscall = SYSCALL $@
      cmd_syscall = $(CC) -nostdlib $(SYSCFLAGS_$(@F)) \
			   -Wl,-T,$(filter-out FORCE,$^) -o $@

export CPPFLAGS_vsyscall.lds += -P -C -Ush

vsyscall-flags = -shared -s -Wl,-soname=linux-gate.so.1 \
		$(call ld-option, -Wl$(comma)--hash-style=sysv)

SYSCFLAGS_vsyscall-trapa.so	= $(vsyscall-flags)

$(obj)/vsyscall-trapa.so: \
$(obj)/vsyscall-%.so: $(src)/vsyscall.lds $(obj)/vsyscall-%.o FORCE
	$(call if_changed,syscall)

# We also create a special relocatable object that should mirror the symbol
# table and layout of the linked DSO.  With ld -R we can then refer to
# these symbols in the kernel code rather than hand-coded addresses.
extra-y += vsyscall-syms.o
$(obj)/built-in.o: $(obj)/vsyscall-syms.o
$(obj)/built-in.o: ld_flags += -R $(obj)/vsyscall-syms.o

SYSCFLAGS_vsyscall-syms.o = -r
$(obj)/vsyscall-syms.o: $(src)/vsyscall.lds \
			$(obj)/vsyscall-trapa.o $(obj)/vsyscall-note.o FORCE
	$(call if_changed,syscall)
