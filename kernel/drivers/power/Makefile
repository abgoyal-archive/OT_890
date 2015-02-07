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
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.


power_supply-objs := power_supply_core.o

ifeq ($(CONFIG_SYSFS),y)
power_supply-objs += power_supply_sysfs.o
endif

ifeq ($(CONFIG_LEDS_TRIGGERS),y)
power_supply-objs += power_supply_leds.o
endif

ifeq ($(CONFIG_POWER_SUPPLY_DEBUG),y)
EXTRA_CFLAGS += -DDEBUG
endif

obj-$(CONFIG_POWER_SUPPLY)	+= power_supply.o

obj-$(CONFIG_PDA_POWER)		+= pda_power.o
obj-$(CONFIG_APM_POWER)		+= apm_power.o
obj-$(CONFIG_WM8350_POWER)	+= wm8350_power.o

obj-$(CONFIG_BATTERY_DS2760)	+= ds2760_battery.o
obj-$(CONFIG_BATTERY_PMU)	+= pmu_battery.o
obj-$(CONFIG_BATTERY_OLPC)	+= olpc_battery.o
obj-$(CONFIG_BATTERY_TOSA)	+= tosa_battery.o
obj-$(CONFIG_BATTERY_WM97XX)	+= wm97xx_battery.o
obj-$(CONFIG_BATTERY_BQ27x00)	+= bq27x00_battery.o
obj-$(CONFIG_BATTERY_DA9030)	+= da9030_battery.o
obj-$(CONFIG_CHARGER_PCF50633)	+= pcf50633-charger.o
obj-$(CONFIG_BATTERY_MT3351)	+= mt3351_battery.o
obj-$(CONFIG_PMIC_MT6326)	+= pmic_mt6326.o
obj-$(CONFIG_SMART_BATTERY_MT6516)	+= smart_battery_mt6516.o
# Infinity, temporary solution, FIXME {
obj-$(CONFIG_DUMMY_BATTERY_MT6516)	+= mt6516_dummy_battery.o
# Infinity, temporary solution, FIXME }
ifneq ($(CONFIG_PMIC_DCT),y)
obj-y	+= pmic_drv_nodct.o
endif
