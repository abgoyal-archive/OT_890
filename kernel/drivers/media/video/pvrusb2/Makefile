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


obj-pvrusb2-sysfs-$(CONFIG_VIDEO_PVRUSB2_SYSFS) := pvrusb2-sysfs.o
obj-pvrusb2-debugifc-$(CONFIG_VIDEO_PVRUSB2_DEBUGIFC) := pvrusb2-debugifc.o
obj-pvrusb2-dvb-$(CONFIG_VIDEO_PVRUSB2_DVB) := pvrusb2-dvb.o

pvrusb2-objs	:= pvrusb2-i2c-core.o pvrusb2-i2c-cmd-v4l2.o \
		   pvrusb2-audio.o pvrusb2-i2c-chips-v4l2.o \
		   pvrusb2-encoder.o pvrusb2-video-v4l.o \
		   pvrusb2-eeprom.o pvrusb2-tuner.o \
		   pvrusb2-main.o pvrusb2-hdw.o pvrusb2-v4l2.o \
		   pvrusb2-ctrl.o pvrusb2-std.o pvrusb2-devattr.o \
		   pvrusb2-context.o pvrusb2-io.o pvrusb2-ioread.o \
		   pvrusb2-cx2584x-v4l.o pvrusb2-wm8775.o \
		   $(obj-pvrusb2-dvb-y) \
		   $(obj-pvrusb2-sysfs-y) $(obj-pvrusb2-debugifc-y)

obj-$(CONFIG_VIDEO_PVRUSB2) += pvrusb2.o

EXTRA_CFLAGS += -Idrivers/media/video
EXTRA_CFLAGS += -Idrivers/media/common/tuners
EXTRA_CFLAGS += -Idrivers/media/dvb/dvb-core
EXTRA_CFLAGS += -Idrivers/media/dvb/frontends
