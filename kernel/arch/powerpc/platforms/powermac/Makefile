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


CFLAGS_bootx_init.o  		+= -fPIC

ifdef CONFIG_FUNCTION_TRACER
# Do not trace early boot code
CFLAGS_REMOVE_bootx_init.o = -pg -mno-sched-epilog
endif

obj-y				+= pic.o setup.o time.o feature.o pci.o \
				   sleep.o low_i2c.o cache.o pfunc_core.o \
				   pfunc_base.o udbg_scc.o udbg_adb.o
obj-$(CONFIG_PMAC_BACKLIGHT)	+= backlight.o
obj-$(CONFIG_CPU_FREQ_PMAC)	+= cpufreq_32.o
obj-$(CONFIG_CPU_FREQ_PMAC64)	+= cpufreq_64.o
# CONFIG_NVRAM is an arch. independant tristate symbol, for pmac32 we really
# need this to be a bool.  Cheat here and pretend CONFIG_NVRAM=m is really
# CONFIG_NVRAM=y
obj-$(CONFIG_NVRAM:m=y)		+= nvram.o
# ppc64 pmac doesn't define CONFIG_NVRAM but needs nvram stuff
obj-$(CONFIG_PPC64)		+= nvram.o
obj-$(CONFIG_PPC32)		+= bootx_init.o
obj-$(CONFIG_SMP)		+= smp.o
