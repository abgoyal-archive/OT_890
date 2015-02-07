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

obj-vdso64 = sigtramp.o gettimeofday.o datapage.o cacheflush.o note.o

# Build rules

targets := $(obj-vdso64) vdso64.so vdso64.so.dbg
obj-vdso64 := $(addprefix $(obj)/, $(obj-vdso64))

EXTRA_CFLAGS := -shared -fno-common -fno-builtin
EXTRA_CFLAGS += -nostdlib -Wl,-soname=linux-vdso64.so.1 \
		$(call ld-option, -Wl$(comma)--hash-style=sysv)
EXTRA_AFLAGS := -D__VDSO64__ -s

obj-y += vdso64_wrapper.o
extra-y += vdso64.lds
CPPFLAGS_vdso64.lds += -P -C -U$(ARCH)

# Force dependency (incbin is bad)
$(obj)/vdso64_wrapper.o : $(obj)/vdso64.so

# link rule for the .so file, .lds has to be first
$(obj)/vdso64.so.dbg: $(src)/vdso64.lds $(obj-vdso64)
	$(call if_changed,vdso64ld)

# strip rule for the .so file
$(obj)/%.so: OBJCOPYFLAGS := -S
$(obj)/%.so: $(obj)/%.so.dbg FORCE
	$(call if_changed,objcopy)

# assembly rules for the .S files
$(obj-vdso64): %.o: %.S
	$(call if_changed_dep,vdso64as)

# actual build commands
quiet_cmd_vdso64ld = VDSO64L $@
      cmd_vdso64ld = $(CC) $(c_flags) -Wl,-T $^ -o $@
quiet_cmd_vdso64as = VDSO64A $@
      cmd_vdso64as = $(CC) $(a_flags) -c -o $@ $<

# install commands for the unstripped file
quiet_cmd_vdso_install = INSTALL $@
      cmd_vdso_install = cp $(obj)/$@.dbg $(MODLIB)/vdso/$@

vdso64.so: $(obj)/vdso64.so.dbg
	@mkdir -p $(MODLIB)/vdso
	$(call cmd,vdso_install)

vdso_install: vdso64.so
