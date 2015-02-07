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


snd-hda-intel-objs := hda_intel.o

snd-hda-codec-y := hda_codec.o
snd-hda-codec-$(CONFIG_SND_HDA_GENERIC) += hda_generic.o
snd-hda-codec-$(CONFIG_PROC_FS) += hda_proc.o
# snd-hda-codec-$(CONFIG_SND_HDA_ELD) += hda_eld.o
snd-hda-codec-$(CONFIG_SND_HDA_HWDEP) += hda_hwdep.o
snd-hda-codec-$(CONFIG_SND_HDA_INPUT_BEEP) += hda_beep.o

snd-hda-codec-realtek-objs :=	patch_realtek.o
snd-hda-codec-cmedia-objs :=	patch_cmedia.o
snd-hda-codec-analog-objs :=	patch_analog.o
snd-hda-codec-idt-objs :=	patch_sigmatel.o
snd-hda-codec-si3054-objs :=	patch_si3054.o
snd-hda-codec-atihdmi-objs :=	patch_atihdmi.o
snd-hda-codec-conexant-objs :=	patch_conexant.o
snd-hda-codec-via-objs :=	patch_via.o
snd-hda-codec-nvhdmi-objs :=	patch_nvhdmi.o
snd-hda-codec-intelhdmi-objs :=	patch_intelhdmi.o hda_eld.o

# common driver
obj-$(CONFIG_SND_HDA_INTEL) := snd-hda-codec.o

# codec drivers (note: CONFIG_SND_HDA_CODEC_XXX are booleans)
ifdef CONFIG_SND_HDA_CODEC_REALTEK
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-realtek.o
endif
ifdef CONFIG_SND_HDA_CODEC_CMEDIA
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-cmedia.o
endif
ifdef CONFIG_SND_HDA_CODEC_ANALOG
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-analog.o
endif
ifdef CONFIG_SND_HDA_CODEC_SIGMATEL
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-idt.o
endif
ifdef CONFIG_SND_HDA_CODEC_SI3054
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-si3054.o
endif
ifdef CONFIG_SND_HDA_CODEC_ATIHDMI
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-atihdmi.o
endif
ifdef CONFIG_SND_HDA_CODEC_CONEXANT
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-conexant.o
endif
ifdef CONFIG_SND_HDA_CODEC_VIA
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-via.o
endif
ifdef CONFIG_SND_HDA_CODEC_NVHDMI
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-nvhdmi.o
endif
ifdef CONFIG_SND_HDA_CODEC_INTELHDMI
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-codec-intelhdmi.o
endif

# this must be the last entry after codec drivers;
# otherwise the codec patches won't be hooked before the PCI probe
# when built in kernel
obj-$(CONFIG_SND_HDA_INTEL) += snd-hda-intel.o
