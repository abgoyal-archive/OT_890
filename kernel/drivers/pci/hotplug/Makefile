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
# Makefile for the Linux kernel pci hotplug controller drivers.
#

obj-$(CONFIG_HOTPLUG_PCI)		+= pci_hotplug.o
obj-$(CONFIG_HOTPLUG_PCI_COMPAQ)	+= cpqphp.o
obj-$(CONFIG_HOTPLUG_PCI_IBM)		+= ibmphp.o

# pciehp should be linked before acpiphp in order to allow the native driver
# to attempt to bind first. We can then fall back to generic support.

obj-$(CONFIG_HOTPLUG_PCI_PCIE)		+= pciehp.o
obj-$(CONFIG_HOTPLUG_PCI_ACPI)		+= acpiphp.o
obj-$(CONFIG_HOTPLUG_PCI_ACPI_IBM)	+= acpiphp_ibm.o
obj-$(CONFIG_HOTPLUG_PCI_CPCI_ZT5550)	+= cpcihp_zt5550.o
obj-$(CONFIG_HOTPLUG_PCI_CPCI_GENERIC)	+= cpcihp_generic.o
obj-$(CONFIG_HOTPLUG_PCI_SHPC)		+= shpchp.o
obj-$(CONFIG_HOTPLUG_PCI_RPA)		+= rpaphp.o
obj-$(CONFIG_HOTPLUG_PCI_RPA_DLPAR)	+= rpadlpar_io.o
obj-$(CONFIG_HOTPLUG_PCI_SGI)		+= sgi_hotplug.o

# Link this last so it doesn't claim devices that have a real hotplug driver
obj-$(CONFIG_HOTPLUG_PCI_FAKE)		+= fakephp.o

pci_hotplug-objs	:=	pci_hotplug_core.o

ifdef CONFIG_HOTPLUG_PCI_CPCI
pci_hotplug-objs	+=	cpci_hotplug_core.o	\
				cpci_hotplug_pci.o
endif
ifdef CONFIG_ACPI
pci_hotplug-objs 	+= 	acpi_pcihp.o
endif

cpqphp-objs		:=	cpqphp_core.o	\
				cpqphp_ctrl.o	\
				cpqphp_sysfs.o	\
				cpqphp_pci.o
cpqphp-$(CONFIG_HOTPLUG_PCI_COMPAQ_NVRAM) += cpqphp_nvram.o
cpqphp-objs += $(cpqphp-y)

ibmphp-objs		:=	ibmphp_core.o	\
				ibmphp_ebda.o	\
				ibmphp_pci.o	\
				ibmphp_res.o	\
				ibmphp_hpc.o

acpiphp-objs		:=	acpiphp_core.o	\
				acpiphp_glue.o

rpaphp-objs		:=	rpaphp_core.o	\
				rpaphp_pci.o	\
				rpaphp_slot.o

rpadlpar_io-objs	:=	rpadlpar_core.o \
				rpadlpar_sysfs.o

pciehp-objs		:=	pciehp_core.o	\
				pciehp_ctrl.o	\
				pciehp_pci.o	\
				pciehp_hpc.o
ifdef CONFIG_ACPI
pciehp-objs		+=	pciehp_acpi.o
endif

shpchp-objs		:=	shpchp_core.o	\
				shpchp_ctrl.o	\
				shpchp_pci.o	\
				shpchp_sysfs.o	\
				shpchp_hpc.o
