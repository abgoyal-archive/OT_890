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


obj-$(CONFIG_RT2870)	+= rt2870sta.o

# TODO: all of these should be removed
EXTRA_CFLAGS += -DLINUX -DAGGREGATION_SUPPORT -DPIGGYBACK_SUPPORT -DWMM_SUPPORT
EXTRA_CFLAGS += -DRT2870
EXTRA_CFLAGS += -DCONFIG_STA_SUPPORT
EXTRA_CFLAGS += -DDBG
EXTRA_CFLAGS += -DDOT11_N_SUPPORT
EXTRA_CFLAGS += -DWPA_SUPPLICANT_SUPPORT
EXTRA_CFLAGS += -DNATIVE_WPA_SUPPLICANT_SUPPORT

rt2870sta-objs :=		\
	common/md5.o		\
	common/mlme.o		\
	common/rtmp_wep.o	\
	common/action.o		\
	common/cmm_data.o	\
	common/rtmp_init.o	\
	common/rtmp_tkip.o	\
	common/cmm_sync.o	\
	common/eeprom.o		\
	common/cmm_sanity.o	\
	common/cmm_info.o	\
	common/cmm_wpa.o	\
	common/dfs.o		\
	common/spectrum.o	\
	sta/assoc.o		\
	sta/aironet.o		\
	sta/auth.o		\
	sta/auth_rsp.o		\
	sta/sync.o		\
	sta/sanity.o		\
	sta/rtmp_data.o		\
	sta/connect.o		\
	sta/wpa.o		\
	rt_linux.o		\
	rt_profile.o		\
	rt_main_dev.o		\
	sta_ioctl.o		\
	common/ba_action.o	\
	2870_main_dev.o		\
	common/2870_rtmp_init.o	\
	common/rtusb_io.o	\
	common/rtusb_bulk.o	\
	common/rtusb_data.o	\
	common/cmm_data_2870.o

