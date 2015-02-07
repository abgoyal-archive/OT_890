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


obj-$(CONFIG_PPC_CELL_COMMON)		+= cbe_regs.o interrupt.o pervasive.o

obj-$(CONFIG_PPC_CELL_NATIVE)		+= iommu.o setup.o spider-pic.o \
					   pmu.o io-workarounds.o spider-pci.o
obj-$(CONFIG_CBE_RAS)			+= ras.o

obj-$(CONFIG_CBE_THERM)			+= cbe_thermal.o
obj-$(CONFIG_CBE_CPUFREQ_PMI)		+= cbe_cpufreq_pmi.o
obj-$(CONFIG_CBE_CPUFREQ)		+= cbe-cpufreq.o
cbe-cpufreq-y				+= cbe_cpufreq_pervasive.o cbe_cpufreq.o
obj-$(CONFIG_CBE_CPUFREQ_SPU_GOVERNOR)	+= cpufreq_spudemand.o

obj-$(CONFIG_PPC_IBM_CELL_POWERBUTTON)	+= cbe_powerbutton.o

ifeq ($(CONFIG_SMP),y)
obj-$(CONFIG_PPC_CELL_NATIVE)		+= smp.o
obj-$(CONFIG_PPC_CELL_QPACE)		+= smp.o
endif

# needed only when building loadable spufs.ko
spu-priv1-$(CONFIG_PPC_CELL_COMMON)	+= spu_priv1_mmio.o
spu-manage-$(CONFIG_PPC_CELL_COMMON)	+= spu_manage.o

obj-$(CONFIG_SPU_BASE)			+= spu_callbacks.o spu_base.o \
					   spu_notify.o \
					   spu_syscalls.o spu_fault.o \
					   $(spu-priv1-y) \
					   $(spu-manage-y) \
					   spufs/

obj-$(CONFIG_PCI_MSI)			+= axon_msi.o

# qpace setup
obj-$(CONFIG_PPC_CELL_QPACE)		+= qpace_setup.o

# celleb stuff
ifeq ($(CONFIG_PPC_CELLEB),y)
obj-y					+= celleb_setup.o \
					   celleb_pci.o celleb_scc_epci.o \
					   celleb_scc_pciex.o \
					   celleb_scc_uhc.o \
					   io-workarounds.o spider-pci.o \
					   beat.o beat_htab.o beat_hvCall.o \
					   beat_interrupt.o beat_iommu.o

obj-$(CONFIG_SMP)			+= beat_smp.o
obj-$(CONFIG_PPC_UDBG_BEAT)		+= beat_udbg.o
obj-$(CONFIG_SERIAL_TXX9)		+= celleb_scc_sio.o
obj-$(CONFIG_SPU_BASE)			+= beat_spu_priv1.o
endif
