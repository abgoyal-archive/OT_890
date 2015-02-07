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


# Makefile for the Linux logos

obj-$(CONFIG_LOGO)			+= logo.o
obj-$(CONFIG_LOGO_LINUX_MONO)		+= logo_linux_mono.o
obj-$(CONFIG_LOGO_LINUX_VGA16)		+= logo_linux_vga16.o
obj-$(CONFIG_LOGO_LINUX_CLUT224)	+= logo_linux_clut224.o
obj-$(CONFIG_LOGO_BLACKFIN_CLUT224)	+= logo_blackfin_clut224.o
obj-$(CONFIG_LOGO_BLACKFIN_VGA16)	+= logo_blackfin_vga16.o
obj-$(CONFIG_LOGO_DEC_CLUT224)		+= logo_dec_clut224.o
obj-$(CONFIG_LOGO_MAC_CLUT224)		+= logo_mac_clut224.o
obj-$(CONFIG_LOGO_PARISC_CLUT224)	+= logo_parisc_clut224.o
obj-$(CONFIG_LOGO_SGI_CLUT224)		+= logo_sgi_clut224.o
obj-$(CONFIG_LOGO_SUN_CLUT224)		+= logo_sun_clut224.o
obj-$(CONFIG_LOGO_SUPERH_MONO)		+= logo_superh_mono.o
obj-$(CONFIG_LOGO_SUPERH_VGA16)		+= logo_superh_vga16.o
obj-$(CONFIG_LOGO_SUPERH_CLUT224)	+= logo_superh_clut224.o
obj-$(CONFIG_LOGO_M32R_CLUT224)		+= logo_m32r_clut224.o

obj-$(CONFIG_SPU_BASE)			+= logo_spe_clut224.o

# How to generate logo's

# Use logo-cfiles to retrieve list of .c files to be built
logo-cfiles = $(notdir $(patsubst %.$(2), %.c, \
              $(wildcard $(srctree)/$(src)/*$(1).$(2))))


# Mono logos
extra-y += $(call logo-cfiles,_mono,pbm)

# VGA16 logos
extra-y += $(call logo-cfiles,_vga16,ppm)

# 224 Logos
extra-y += $(call logo-cfiles,_clut224,ppm)

# Gray 256
extra-y += $(call logo-cfiles,_gray256,pgm)

# Create commands like "pnmtologo -t mono -n logo_mac_mono -o ..."
quiet_cmd_logo = LOGO    $@
	cmd_logo = scripts/pnmtologo \
			-t $(patsubst $*_%,%,$(notdir $(basename $<))) \
			-n $(notdir $(basename $<)) -o $@ $<

$(obj)/%_mono.c: $(src)/%_mono.pbm FORCE
	$(call if_changed,logo)

$(obj)/%_vga16.c: $(src)/%_vga16.ppm FORCE
	$(call if_changed,logo)

$(obj)/%_clut224.c: $(src)/%_clut224.ppm FORCE
	$(call if_changed,logo)

$(obj)/%_gray256.c: $(src)/%_gray256.pgm FORCE
	$(call if_changed,logo)

# Files generated that shall be removed upon make clean
clean-files := *.o *_mono.c *_vga16.c *_clut224.c *_gray256.c
