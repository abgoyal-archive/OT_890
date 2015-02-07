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
# Makefile for the PCI bus specific drivers.
#

obj-y		+= access.o bus.o probe.o remove.o pci.o quirks.o slot.o \
			pci-driver.o search.o pci-sysfs.o rom.o setup-res.o \
			irq.o
obj-$(CONFIG_PROC_FS) += proc.o

# Build PCI Express stuff if needed
obj-$(CONFIG_PCIEPORTBUS) += pcie/

obj-$(CONFIG_HOTPLUG) += hotplug.o

# Build the PCI Hotplug drivers if we were asked to
obj-$(CONFIG_HOTPLUG_PCI) += hotplug/
ifdef CONFIG_HOTPLUG_PCI
obj-y += hotplug-pci.o
endif

# Build the PCI MSI interrupt support
obj-$(CONFIG_PCI_MSI) += msi.o

# Build the Hypertransport interrupt support
obj-$(CONFIG_HT_IRQ) += htirq.o

# Build Intel IOMMU support
obj-$(CONFIG_DMAR) += dmar.o iova.o intel-iommu.o

obj-$(CONFIG_INTR_REMAP) += dmar.o intr_remapping.o

#
# Some architectures use the generic PCI setup functions
#
obj-$(CONFIG_X86) += setup-bus.o
obj-$(CONFIG_ALPHA) += setup-bus.o setup-irq.o
obj-$(CONFIG_ARM) += setup-bus.o setup-irq.o
obj-$(CONFIG_PARISC) += setup-bus.o
obj-$(CONFIG_SUPERH) += setup-bus.o setup-irq.o
obj-$(CONFIG_PPC32) += setup-irq.o
obj-$(CONFIG_PPC) += setup-bus.o
obj-$(CONFIG_MIPS) += setup-bus.o setup-irq.o
obj-$(CONFIG_X86_VISWS) += setup-irq.o
obj-$(CONFIG_MN10300) += setup-bus.o

#
# ACPI Related PCI FW Functions
#
obj-$(CONFIG_ACPI)    += pci-acpi.o

# Cardbus & CompactPCI use setup-bus
obj-$(CONFIG_HOTPLUG) += setup-bus.o

obj-$(CONFIG_PCI_SYSCALL) += syscall.o

obj-$(CONFIG_PCI_STUB) += pci-stub.o

ifeq ($(CONFIG_PCI_DEBUG),y)
EXTRA_CFLAGS += -DDEBUG
endif
