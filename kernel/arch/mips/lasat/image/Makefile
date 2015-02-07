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
# MAKEFILE FOR THE MIPS LINUX BOOTLOADER AND ROM DEBUGGER
#
# i-data Networks
#
# Author: Thomas Horsten <thh@i-data.com>
#

ifndef Version
 Version = "$(USER)-test"
endif

MKLASATIMG = mklasatimg
MKLASATIMG_ARCH = mq2,mqpro,sp100,sp200
KERNEL_IMAGE = vmlinux
KERNEL_START = $(shell $(NM) $(KERNEL_IMAGE) | grep " _text" | cut -f1 -d\ )
KERNEL_ENTRY = $(shell $(NM) $(KERNEL_IMAGE) | grep kernel_entry | cut -f1 -d\ )

LDSCRIPT= -L$(srctree)/$(src) -Tromscript.normal

HEAD_DEFINES := -D_kernel_start=0x$(KERNEL_START) \
		-D_kernel_entry=0x$(KERNEL_ENTRY) \
		-D VERSION="\"$(Version)\"" \
		-D TIMESTAMP=$(shell date +%s)

$(obj)/head.o: $(obj)/head.S $(KERNEL_IMAGE)
	$(CC) -fno-pic $(HEAD_DEFINES) $(LINUXINCLUDE) -c -o $@ $<

OBJECTS = head.o kImage.o

rom.sw:	$(obj)/rom.sw
rom.bin:	$(obj)/rom.bin

$(obj)/rom.sw:	$(obj)/rom.bin
	$(MKLASATIMG) -o $@ -k $^ -m $(MKLASATIMG_ARCH)

$(obj)/rom.bin: $(obj)/rom
	$(OBJCOPY) -O binary -S $^ $@

# Rule to make the bootloader
$(obj)/rom: $(addprefix $(obj)/,$(OBJECTS))
	$(LD) $(LDFLAGS) $(LDSCRIPT) -o $@ $^

$(obj)/%.o: $(obj)/%.gz
	$(LD) -r -o $@ -b binary $<

$(obj)/%.gz: $(obj)/%.bin
	gzip -cf -9 $< > $@

$(obj)/kImage.bin: $(KERNEL_IMAGE)
	$(OBJCOPY) -O binary -S $^ $@

clean:
	rm -f rom rom.bin rom.sw kImage.bin kImage.o
