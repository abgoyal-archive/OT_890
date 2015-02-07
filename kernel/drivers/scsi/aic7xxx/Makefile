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
# Makefile for the Linux aic7xxx SCSI driver.
#
# $Id: //depot/linux-aic79xx-2.5.0/drivers/scsi/aic7xxx/Makefile#8 $
#

# Let kbuild descend into aicasm when cleaning
subdir-				+= aicasm

obj-$(CONFIG_SCSI_AIC7XXX)	+= aic7xxx.o
obj-$(CONFIG_SCSI_AIC79XX)	+= aic79xx.o

# Core Fast -> U160 files
aic7xxx-y					+= aic7xxx_core.o	\
						   aic7xxx_93cx6.o
aic7xxx-$(CONFIG_EISA)				+= aic7770.o
aic7xxx-$(CONFIG_PCI)				+= aic7xxx_pci.o
aic7xxx-$(CONFIG_AIC7XXX_REG_PRETTY_PRINT)	+= aic7xxx_reg_print.o

# Platform Specific Fast -> U160 Files
aic7xxx-y					+= aic7xxx_osm.o	\
						   aic7xxx_proc.o
aic7xxx-$(CONFIG_EISA)				+= aic7770_osm.o
aic7xxx-$(CONFIG_PCI)				+= aic7xxx_osm_pci.o

# Core U320 files
aic79xx-y					+= aic79xx_core.o	\
						   aic79xx_pci.o
aic79xx-$(CONFIG_AIC79XX_REG_PRETTY_PRINT)	+= aic79xx_reg_print.o

# Platform Specific U320 Files
aic79xx-y					+= aic79xx_osm.o	\
						   aic79xx_proc.o	\
						   aic79xx_osm_pci.o

ccflags-y += -Idrivers/scsi
ifdef WARNINGS_BECOME_ERRORS
ccflags-y += -Werror
endif

# Files generated that shall be removed upon make clean
clean-files := aic7xxx_seq.h aic7xxx_reg.h aic7xxx_reg_print.c
clean-files += aic79xx_seq.h aic79xx_reg.h aic79xx_reg_print.c

# Dependencies for generated files need to be listed explicitly

$(addprefix $(obj)/,$(aic7xxx-y)): $(obj)/aic7xxx_seq.h $(obj)/aic7xxx_reg.h
$(addprefix $(obj)/,$(aic79xx-y)): $(obj)/aic79xx_seq.h $(obj)/aic79xx_reg.h

aic7xxx-gen-$(CONFIG_AIC7XXX_BUILD_FIRMWARE)	:= $(obj)/aic7xxx_reg.h
aic7xxx-gen-$(CONFIG_AIC7XXX_REG_PRETTY_PRINT)	+= $(obj)/aic7xxx_reg_print.c

aicasm-7xxx-opts-$(CONFIG_AIC7XXX_REG_PRETTY_PRINT) := \
	-p $(obj)/aic7xxx_reg_print.c -i aic7xxx_osm.h

ifeq ($(CONFIG_AIC7XXX_BUILD_FIRMWARE),y)
$(obj)/aic7xxx_seq.h: $(src)/aic7xxx.seq $(src)/aic7xxx.reg $(obj)/aicasm/aicasm
	$(obj)/aicasm/aicasm -I$(src) -r $(obj)/aic7xxx_reg.h \
			      $(aicasm-7xxx-opts-y) -o $(obj)/aic7xxx_seq.h \
			      $(src)/aic7xxx.seq

$(aic7xxx-gen-y): $(obj)/aic7xxx_seq.h
else
$(obj)/aic7xxx_reg_print.c: $(src)/aic7xxx_reg_print.c_shipped
endif

aic79xx-gen-$(CONFIG_AIC79XX_BUILD_FIRMWARE)	:= $(obj)/aic79xx_reg.h
aic79xx-gen-$(CONFIG_AIC79XX_REG_PRETTY_PRINT)	+= $(obj)/aic79xx_reg_print.c

aicasm-79xx-opts-$(CONFIG_AIC79XX_REG_PRETTY_PRINT) := \
	-p $(obj)/aic79xx_reg_print.c -i aic79xx_osm.h

ifeq ($(CONFIG_AIC79XX_BUILD_FIRMWARE),y)
$(obj)/aic79xx_seq.h: $(src)/aic79xx.seq $(src)/aic79xx.reg $(obj)/aicasm/aicasm
	$(obj)/aicasm/aicasm -I$(src) -r $(obj)/aic79xx_reg.h \
			      $(aicasm-79xx-opts-y) -o $(obj)/aic79xx_seq.h \
			      $(src)/aic79xx.seq

$(aic79xx-gen-y): $(obj)/aic79xx_seq.h
else
$(obj)/aic79xx_reg_print.c: $(src)/aic79xx_reg_print.c_shipped
endif

$(obj)/aicasm/aicasm: $(src)/aicasm/*.[chyl]
	$(MAKE) -C $(src)/aicasm
