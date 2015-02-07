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

snd-ad1889-objs := ad1889.o
snd-als300-objs := als300.o
snd-als4000-objs := als4000.o
snd-atiixp-objs := atiixp.o
snd-atiixp-modem-objs := atiixp_modem.o
snd-azt3328-objs := azt3328.o
snd-bt87x-objs := bt87x.o
snd-cmipci-objs := cmipci.o
snd-cs4281-objs := cs4281.o
snd-cs5530-objs := cs5530.o
snd-ens1370-objs := ens1370.o ak4531_codec.o
snd-ens1371-objs := ens1371.o
snd-es1938-objs := es1938.o
snd-es1968-objs := es1968.o
snd-fm801-objs := fm801.o
snd-intel8x0-objs := intel8x0.o
snd-intel8x0m-objs := intel8x0m.o
snd-maestro3-objs := maestro3.o
snd-rme32-objs := rme32.o
snd-rme96-objs := rme96.o
snd-sis7019-objs := sis7019.o
snd-sonicvibes-objs := sonicvibes.o
snd-via82xx-objs := via82xx.o
snd-via82xx-modem-objs := via82xx_modem.o

# Toplevel Module Dependency
obj-$(CONFIG_SND_AD1889) += snd-ad1889.o
obj-$(CONFIG_SND_ALS300) += snd-als300.o
obj-$(CONFIG_SND_ALS4000) += snd-als4000.o
obj-$(CONFIG_SND_ATIIXP) += snd-atiixp.o
obj-$(CONFIG_SND_ATIIXP_MODEM) += snd-atiixp-modem.o
obj-$(CONFIG_SND_AZT3328) += snd-azt3328.o
obj-$(CONFIG_SND_BT87X) += snd-bt87x.o
obj-$(CONFIG_SND_CMIPCI) += snd-cmipci.o
obj-$(CONFIG_SND_CS4281) += snd-cs4281.o
obj-$(CONFIG_SND_CS5530) += snd-cs5530.o
obj-$(CONFIG_SND_ENS1370) += snd-ens1370.o
obj-$(CONFIG_SND_ENS1371) += snd-ens1371.o
obj-$(CONFIG_SND_ES1938) += snd-es1938.o
obj-$(CONFIG_SND_ES1968) += snd-es1968.o
obj-$(CONFIG_SND_FM801) += snd-fm801.o
obj-$(CONFIG_SND_INTEL8X0) += snd-intel8x0.o
obj-$(CONFIG_SND_INTEL8X0M) += snd-intel8x0m.o
obj-$(CONFIG_SND_MAESTRO3) += snd-maestro3.o
obj-$(CONFIG_SND_RME32) += snd-rme32.o
obj-$(CONFIG_SND_RME96) += snd-rme96.o
obj-$(CONFIG_SND_SIS7019) += snd-sis7019.o
obj-$(CONFIG_SND_SONICVIBES) += snd-sonicvibes.o
obj-$(CONFIG_SND_VIA82XX) += snd-via82xx.o
obj-$(CONFIG_SND_VIA82XX_MODEM) += snd-via82xx-modem.o

obj-$(CONFIG_SND) += \
	ac97/ \
	ali5451/ \
	au88x0/ \
	aw2/ \
	ca0106/ \
	cs46xx/ \
	cs5535audio/ \
	echoaudio/ \
	emu10k1/ \
	hda/ \
	ice1712/ \
	korg1212/ \
	mixart/ \
	nm256/ \
	oxygen/ \
	pcxhr/ \
	riptide/ \
	rme9652/ \
	trident/ \
	ymfpci/ \
	vx222/
