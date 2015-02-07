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
# Makefile for rescue (bootstrap) code
#

ccflags-y += -O2 $(LINUXINCLUDE)
asflags-y += $(LINUXINCLUDE)
ldflags-y += -T $(srctree)/$(src)/rescue.lds
OBJCOPYFLAGS = -O binary --remove-section=.bss
obj-$(CONFIG_ETRAX_AXISFLASHMAP) = head.o
OBJECT := $(obj)/head.o

targets := rescue.o rescue.bin

$(obj)/rescue.o: $(OBJECT) FORCE
	$(call if_changed,ld)

$(obj)/rescue.bin: $(obj)/rescue.o FORCE
	$(call if_changed,objcopy)
	cp -p $(obj)/rescue.bin $(objtree)

$(obj)/testrescue.bin: $(obj)/testrescue.o
	$(OBJCOPY) $(OBJCOPYFLAGS) $(obj)/testrescue.o tr.bin
# Pad it to 784 bytes
	dd if=/dev/zero of=tmp2423 bs=1 count=784
	cat tr.bin tmp2423 >testrescue_tmp.bin
	dd if=testrescue_tmp.bin of=$(obj)/testrescue.bin bs=1 count=784
	rm tr.bin tmp2423 testrescue_tmp.bin

$(obj)/kimagerescue.bin: $(obj)/kimagerescue.o
	$(OBJCOPY) $(OBJCOPYFLAGS) $(obj)/kimagerescue.o ktr.bin
# Pad it to 784 bytes, that's what the rescue loader expects
	dd if=/dev/zero of=tmp2423 bs=1 count=784
	cat ktr.bin tmp2423 >kimagerescue_tmp.bin
	dd if=kimagerescue_tmp.bin of=$(obj)/kimagerescue.bin bs=1 count=784
	rm ktr.bin tmp2423 kimagerescue_tmp.bin
