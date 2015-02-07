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


# List of files in the vdso, has to be asm only for now

obj-vdso32 = gettimeofday.o clock_getres.o clock_gettime.o note.o

# Build rules

targets := $(obj-vdso32) vdso32.so vdso32.so.dbg
obj-vdso32 := $(addprefix $(obj)/, $(obj-vdso32))

KBUILD_AFLAGS_31 := $(filter-out -m64,$(KBUILD_AFLAGS))
KBUILD_AFLAGS_31 += -m31 -s

KBUILD_CFLAGS_31 := $(filter-out -m64,$(KBUILD_CFLAGS))
KBUILD_CFLAGS_31 += -m31 -fPIC -shared -fno-common -fno-builtin
KBUILD_CFLAGS_31 += -nostdlib -Wl,-soname=linux-vdso32.so.1 \
			$(call ld-option, -Wl$(comma)--hash-style=sysv)

$(targets:%=$(obj)/%.dbg): KBUILD_CFLAGS = $(KBUILD_CFLAGS_31)
$(targets:%=$(obj)/%.dbg): KBUILD_AFLAGS = $(KBUILD_AFLAGS_31)

obj-y += vdso32_wrapper.o
extra-y += vdso32.lds
CPPFLAGS_vdso32.lds += -P -C -U$(ARCH)

# Force dependency (incbin is bad)
$(obj)/vdso32_wrapper.o : $(obj)/vdso32.so

# link rule for the .so file, .lds has to be first
$(obj)/vdso32.so.dbg: $(src)/vdso32.lds $(obj-vdso32)
	$(call if_changed,vdso32ld)

# strip rule for the .so file
$(obj)/%.so: OBJCOPYFLAGS := -S
$(obj)/%.so: $(obj)/%.so.dbg FORCE
	$(call if_changed,objcopy)

# assembly rules for the .S files
$(obj-vdso32): %.o: %.S
	$(call if_changed_dep,vdso32as)

# actual build commands
quiet_cmd_vdso32ld = VDSO32L $@
      cmd_vdso32ld = $(CC) $(c_flags) -Wl,-T $^ -o $@
quiet_cmd_vdso32as = VDSO32A $@
      cmd_vdso32as = $(CC) $(a_flags) -c -o $@ $<

# install commands for the unstripped file
quiet_cmd_vdso_install = INSTALL $@
      cmd_vdso_install = cp $(obj)/$@.dbg $(MODLIB)/vdso/$@

vdso32.so: $(obj)/vdso32.so.dbg
	@mkdir -p $(MODLIB)/vdso
	$(call cmd,vdso_install)

vdso_install: vdso32.so
