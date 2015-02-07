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
# Makefile for the Macintosh-specific device drivers.
#

# Each configuration option enables a list of files.

obj-$(CONFIG_PPC_PMAC)		+= macio_asic.o macio_sysfs.o

obj-$(CONFIG_PMAC_MEDIABAY)	+= mediabay.o
obj-$(CONFIG_MAC_EMUMOUSEBTN)	+= mac_hid.o
obj-$(CONFIG_INPUT_ADBHID)	+= adbhid.o
obj-$(CONFIG_ANSLCD)		+= ans-lcd.o

obj-$(CONFIG_ADB_PMU)		+= via-pmu.o via-pmu-event.o
obj-$(CONFIG_ADB_PMU_LED)	+= via-pmu-led.o
obj-$(CONFIG_PMAC_BACKLIGHT)	+= via-pmu-backlight.o
obj-$(CONFIG_ADB_CUDA)		+= via-cuda.o
obj-$(CONFIG_PMAC_APM_EMU)	+= apm_emu.o
obj-$(CONFIG_PMAC_SMU)		+= smu.o

obj-$(CONFIG_ADB)		+= adb.o
obj-$(CONFIG_ADB_MACII)		+= via-macii.o
obj-$(CONFIG_ADB_MACIISI)	+= via-maciisi.o
obj-$(CONFIG_ADB_IOP)		+= adb-iop.o
obj-$(CONFIG_ADB_PMU68K)	+= via-pmu68k.o
obj-$(CONFIG_ADB_MACIO)		+= macio-adb.o

obj-$(CONFIG_THERM_PM72)	+= therm_pm72.o
obj-$(CONFIG_THERM_WINDTUNNEL)	+= therm_windtunnel.o
obj-$(CONFIG_THERM_ADT746X)	+= therm_adt746x.o
obj-$(CONFIG_WINDFARM)	        += windfarm_core.o
obj-$(CONFIG_WINDFARM_PM81)     += windfarm_smu_controls.o \
				   windfarm_smu_sensors.o \
				   windfarm_lm75_sensor.o windfarm_pid.o \
				   windfarm_cpufreq_clamp.o windfarm_pm81.o
obj-$(CONFIG_WINDFARM_PM91)     += windfarm_smu_controls.o \
				   windfarm_smu_sensors.o \
				   windfarm_lm75_sensor.o windfarm_pid.o \
				   windfarm_cpufreq_clamp.o windfarm_pm91.o
obj-$(CONFIG_WINDFARM_PM112)	+= windfarm_pm112.o windfarm_smu_sat.o \
				   windfarm_smu_controls.o \
				   windfarm_smu_sensors.o \
				   windfarm_max6690_sensor.o \
				   windfarm_lm75_sensor.o windfarm_pid.o
obj-$(CONFIG_WINDFARM_PM121)	+= windfarm_pm121.o windfarm_smu_sat.o \
				   windfarm_smu_controls.o \
				   windfarm_smu_sensors.o \
				   windfarm_max6690_sensor.o \
				   windfarm_lm75_sensor.o windfarm_pid.o
obj-$(CONFIG_PMAC_RACKMETER)	+= rack-meter.o
