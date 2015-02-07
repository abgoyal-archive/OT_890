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


# Makefile for the Linux graphics to console drivers.
# 5 Aug 1999, James Simmons, <mailto:jsimmons@users.sf.net>
# Rewritten to use lists instead of if-statements.

# Font handling
font-objs := fonts.o

font-objs-$(CONFIG_FONT_SUN8x16)   += font_sun8x16.o
font-objs-$(CONFIG_FONT_SUN12x22)  += font_sun12x22.o
font-objs-$(CONFIG_FONT_8x8)       += font_8x8.o
font-objs-$(CONFIG_FONT_8x16)      += font_8x16.o
font-objs-$(CONFIG_FONT_6x11)      += font_6x11.o
font-objs-$(CONFIG_FONT_7x14)      += font_7x14.o
font-objs-$(CONFIG_FONT_10x18)     += font_10x18.o
font-objs-$(CONFIG_FONT_PEARL_8x8) += font_pearl_8x8.o
font-objs-$(CONFIG_FONT_ACORN_8x8) += font_acorn_8x8.o
font-objs-$(CONFIG_FONT_MINI_4x6)  += font_mini_4x6.o

font-objs += $(font-objs-y)

# Each configuration option enables a list of files.

obj-$(CONFIG_DUMMY_CONSOLE)       += dummycon.o
obj-$(CONFIG_SGI_NEWPORT_CONSOLE) += newport_con.o font.o
obj-$(CONFIG_PROM_CONSOLE)        += promcon.o promcon_tbl.o
obj-$(CONFIG_STI_CONSOLE)         += sticon.o sticore.o font.o
obj-$(CONFIG_VGA_CONSOLE)         += vgacon.o
obj-$(CONFIG_MDA_CONSOLE)         += mdacon.o
obj-$(CONFIG_FRAMEBUFFER_CONSOLE) += fbcon.o bitblit.o font.o softcursor.o
ifeq ($(CONFIG_FB_TILEBLITTING),y)
obj-$(CONFIG_FRAMEBUFFER_CONSOLE)     += tileblit.o
endif
ifeq ($(CONFIG_FRAMEBUFFER_CONSOLE_ROTATION),y)
obj-$(CONFIG_FRAMEBUFFER_CONSOLE)     += fbcon_rotate.o fbcon_cw.o fbcon_ud.o \
                                         fbcon_ccw.o
endif

obj-$(CONFIG_FB_STI)              += sticore.o font.o

ifeq ($(CONFIG_USB_SISUSBVGA_CON),y)
obj-$(CONFIG_USB_SISUSBVGA)           += font.o
endif

# Targets that kbuild needs to know about
targets := promcon_tbl.c

quiet_cmd_conmakehash = CNMKHSH $@
      cmd_conmakehash = scripts/conmakehash $< | \
		sed -e '/\#include <[^>]*>/p' -e 's/types/init/' \
		-e 's/dfont\(_uni.*\]\)/promfont\1 /' > $@

$(obj)/promcon_tbl.c: $(src)/prom.uni
	$(call cmd,conmakehash)
