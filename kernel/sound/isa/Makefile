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
# Makefile for ALSA
# Copyright (c) 2001 by Jaroslav Kysela <perex@perex.cz>
#

snd-adlib-objs := adlib.o
snd-als100-objs := als100.o
snd-azt2320-objs := azt2320.o
snd-cmi8330-objs := cmi8330.o
snd-dt019x-objs := dt019x.o
snd-es18xx-objs := es18xx.o
snd-opl3sa2-objs := opl3sa2.o
snd-sc6000-objs := sc6000.o
snd-sgalaxy-objs := sgalaxy.o
snd-sscape-objs := sscape.o

# Toplevel Module Dependency
obj-$(CONFIG_SND_ADLIB) += snd-adlib.o
obj-$(CONFIG_SND_ALS100) += snd-als100.o
obj-$(CONFIG_SND_AZT2320) += snd-azt2320.o
obj-$(CONFIG_SND_CMI8330) += snd-cmi8330.o
obj-$(CONFIG_SND_DT019X) += snd-dt019x.o
obj-$(CONFIG_SND_ES18XX) += snd-es18xx.o
obj-$(CONFIG_SND_OPL3SA2) += snd-opl3sa2.o
obj-$(CONFIG_SND_SC6000) += snd-sc6000.o
obj-$(CONFIG_SND_SGALAXY) += snd-sgalaxy.o
obj-$(CONFIG_SND_SSCAPE) += snd-sscape.o

obj-$(CONFIG_SND) += ad1816a/ ad1848/ cs423x/ es1688/ gus/ opti9xx/ \
		     sb/ wavefront/ wss/
