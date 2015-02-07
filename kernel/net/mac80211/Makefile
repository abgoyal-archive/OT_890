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


obj-$(CONFIG_MAC80211) += mac80211.o

# mac80211 objects
mac80211-y := \
	main.o \
	wext.o \
	sta_info.o \
	wep.o \
	wpa.o \
	scan.o \
	ht.o \
	mlme.o \
	iface.o \
	rate.o \
	michael.o \
	tkip.o \
	aes_ccm.o \
	cfg.o \
	rx.o \
	spectmgmt.o \
	tx.o \
	key.o \
	util.o \
	wme.o \
	event.o

mac80211-$(CONFIG_MAC80211_LEDS) += led.o
mac80211-$(CONFIG_MAC80211_DEBUGFS) += \
	debugfs.o \
	debugfs_sta.o \
	debugfs_netdev.o \
	debugfs_key.o

mac80211-$(CONFIG_MAC80211_MESH) += \
	mesh.o \
	mesh_pathtbl.o \
	mesh_plink.o \
	mesh_hwmp.o

# objects for PID algorithm
rc80211_pid-y := rc80211_pid_algo.o
rc80211_pid-$(CONFIG_MAC80211_DEBUGFS) += rc80211_pid_debugfs.o

rc80211_minstrel-y := rc80211_minstrel.o
rc80211_minstrel-$(CONFIG_MAC80211_DEBUGFS) += rc80211_minstrel_debugfs.o

mac80211-$(CONFIG_MAC80211_RC_PID) += $(rc80211_pid-y)
mac80211-$(CONFIG_MAC80211_RC_MINSTREL) += $(rc80211_minstrel-y)

ccflags-y += -D__CHECK_ENDIAN__
