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
# Copyright (c) 1999,2001 by Jaroslav Kysela <perex@perex.cz>
#

snd-y     := sound.o init.o memory.o info.o control.o misc.o device.o
snd-$(CONFIG_ISA_DMA_API) += isadma.o
snd-$(CONFIG_SND_OSSEMUL) += sound_oss.o info_oss.o
snd-$(CONFIG_SND_VMASTER) += vmaster.o
snd-$(CONFIG_SND_JACK)	  += jack.o

snd-pcm-objs := pcm.o pcm_native.o pcm_lib.o pcm_timer.o pcm_misc.o \
		pcm_memory.o

snd-page-alloc-y := memalloc.o
snd-page-alloc-$(CONFIG_HAS_DMA) += sgbuf.o

snd-rawmidi-objs  := rawmidi.o
snd-timer-objs    := timer.o
snd-hrtimer-objs  := hrtimer.o
snd-rtctimer-objs := rtctimer.o
snd-hwdep-objs    := hwdep.o

obj-$(CONFIG_SND) 		+= snd.o
obj-$(CONFIG_SND_HWDEP)		+= snd-hwdep.o
obj-$(CONFIG_SND_TIMER)		+= snd-timer.o
obj-$(CONFIG_SND_HRTIMER)	+= snd-hrtimer.o
obj-$(CONFIG_SND_RTCTIMER)	+= snd-rtctimer.o
obj-$(CONFIG_SND_PCM)		+= snd-pcm.o snd-page-alloc.o
obj-$(CONFIG_SND_RAWMIDI)	+= snd-rawmidi.o

obj-$(CONFIG_SND_OSSEMUL)	+= oss/
obj-$(CONFIG_SND_SEQUENCER)	+= seq/
