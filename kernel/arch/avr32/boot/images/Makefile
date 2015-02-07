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
# Copyright (C) 2004-2006 Atmel Corporation
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#

MKIMAGE		:= $(srctree)/scripts/mkuboot.sh

extra-y		:= vmlinux.bin vmlinux.gz

OBJCOPYFLAGS_vmlinux.bin := -O binary -R .note.gnu.build-id
$(obj)/vmlinux.bin: vmlinux FORCE
	$(call if_changed,objcopy)

$(obj)/vmlinux.gz: $(obj)/vmlinux.bin FORCE
	$(call if_changed,gzip)

quiet_cmd_uimage = UIMAGE $@
      cmd_uimage = $(CONFIG_SHELL) $(MKIMAGE) -A avr32 -O linux -T kernel	\
		-C gzip -a $(CONFIG_LOAD_ADDRESS) -e $(CONFIG_ENTRY_ADDRESS)	\
		-n 'Linux-$(KERNELRELEASE)' -d $< $@

targets += uImage uImage.srec
$(obj)/uImage: $(obj)/vmlinux.gz
	$(call if_changed,uimage)
	@echo '  Image $@ is ready'

OBJCOPYFLAGS_uImage.srec := -I binary -O srec
$(obj)/uImage.srec: $(obj)/uImage
	$(call if_changed,objcopy)

OBJCOPYFLAGS_vmlinux.elf := --change-section-lma .text-0x80000000 \
			    --change-section-lma __ex_table-0x80000000 \
			    --change-section-lma .rodata-0x80000000 \
			    --change-section-lma .data-0x80000000 \
			    --change-section-lma .init-0x80000000 \
			    --change-section-lma .bss-0x80000000 \
			    --change-section-lma __param-0x80000000 \
			    --change-section-lma __ksymtab-0x80000000 \
			    --change-section-lma __ksymtab_gpl-0x80000000 \
			    --change-section-lma __kcrctab-0x80000000 \
			    --change-section-lma __kcrctab_gpl-0x80000000 \
			    --change-section-lma __ksymtab_strings-0x80000000 \
			    --set-start 0xa0000000
$(obj)/vmlinux.elf: vmlinux FORCE
	$(call if_changed,objcopy)

quiet_cmd_sfdwarf = SFDWARF $@
      cmd_sfdwarf = sfdwarf $< TO $@ GNUAVR IW $(SFDWARF_FLAGS) > $(obj)/sfdwarf.log

$(obj)/vmlinux.cso: $(obj)/vmlinux.elf FORCE
	$(call if_changed,sfdwarf)

install: $(BOOTIMAGE)
	sh $(srctree)/install-kernel.sh $<

# Generated files to be removed upon make clean
clean-files	:= vmlinux.elf vmlinux.bin vmlinux.gz uImage uImage.srec
