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
# Makefile for the Linux ACPI interpreter
#

ccflags-y			:= -Os
ccflags-$(CONFIG_ACPI_DEBUG)	+= -DACPI_DEBUG_OUTPUT

#
# ACPI Boot-Time Table Parsing
#
obj-y				+= tables.o
obj-$(CONFIG_X86)		+= blacklist.o

#
# ACPI Core Subsystem (Interpreter)
#
obj-y				+= osl.o utils.o reboot.o\
					acpica/

# sleep related files
obj-y				+= wakeup.o
obj-y				+= sleep.o
obj-$(CONFIG_ACPI_SLEEP)	+= proc.o


#
# ACPI Bus and Device Drivers
#
processor-objs	+= processor_core.o processor_throttling.o \
				processor_idle.o processor_thermal.o
ifdef CONFIG_CPU_FREQ
processor-objs	+= processor_perflib.o
endif

obj-y				+= bus.o glue.o
obj-y				+= scan.o
# Keep EC driver first. Initialization of others depend on it.
obj-y				+= ec.o
obj-$(CONFIG_ACPI_AC) 		+= ac.o
obj-$(CONFIG_ACPI_BATTERY)	+= battery.o
obj-$(CONFIG_ACPI_BUTTON)	+= button.o
obj-$(CONFIG_ACPI_FAN)		+= fan.o
obj-$(CONFIG_ACPI_DOCK)		+= dock.o
obj-$(CONFIG_ACPI_VIDEO)	+= video.o
ifdef CONFIG_ACPI_VIDEO
obj-y				+= video_detect.o
endif

obj-y				+= pci_root.o pci_link.o pci_irq.o pci_bind.o
obj-$(CONFIG_ACPI_PCI_SLOT)	+= pci_slot.o
obj-$(CONFIG_ACPI_PROCESSOR)	+= processor.o
obj-$(CONFIG_ACPI_CONTAINER)	+= container.o
obj-$(CONFIG_ACPI_THERMAL)	+= thermal.o
obj-y				+= power.o
obj-y				+= system.o event.o
obj-$(CONFIG_ACPI_DEBUG)	+= debug.o
obj-$(CONFIG_ACPI_NUMA)		+= numa.o
obj-$(CONFIG_ACPI_HOTPLUG_MEMORY)	+= acpi_memhotplug.o
obj-$(CONFIG_ACPI_PROCFS_POWER)	+= cm_sbs.o
obj-$(CONFIG_ACPI_SBS)		+= sbshc.o
obj-$(CONFIG_ACPI_SBS)		+= sbs.o
