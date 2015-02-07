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


ifeq ($(CONFIG_PPC64),y)
EXTRA_CFLAGS			+= -mno-minimal-toc
endif

mpic-msi-obj-$(CONFIG_PCI_MSI)	+= mpic_msi.o mpic_u3msi.o mpic_pasemi_msi.o
obj-$(CONFIG_MPIC)		+= mpic.o $(mpic-msi-obj-y)
fsl-msi-obj-$(CONFIG_PCI_MSI)	+= fsl_msi.o
obj-$(CONFIG_PPC_MSI_BITMAP)	+= msi_bitmap.o

obj-$(CONFIG_PPC_MPC106)	+= grackle.o
obj-$(CONFIG_PPC_DCR_NATIVE)	+= dcr-low.o
obj-$(CONFIG_PPC_PMI)		+= pmi.o
obj-$(CONFIG_U3_DART)		+= dart_iommu.o
obj-$(CONFIG_MMIO_NVRAM)	+= mmio_nvram.o
obj-$(CONFIG_FSL_SOC)		+= fsl_soc.o
obj-$(CONFIG_FSL_PCI)		+= fsl_pci.o $(fsl-msi-obj-y)
obj-$(CONFIG_FSL_LBC)		+= fsl_lbc.o
obj-$(CONFIG_FSL_GTM)		+= fsl_gtm.o
obj-$(CONFIG_MPC8xxx_GPIO)	+= mpc8xxx_gpio.o
obj-$(CONFIG_SIMPLE_GPIO)	+= simple_gpio.o
obj-$(CONFIG_RAPIDIO)		+= fsl_rio.o
obj-$(CONFIG_TSI108_BRIDGE)	+= tsi108_pci.o tsi108_dev.o
obj-$(CONFIG_QUICC_ENGINE)	+= qe_lib/
obj-$(CONFIG_PPC_BESTCOMM)	+= bestcomm/
mv64x60-$(CONFIG_PCI)		+= mv64x60_pci.o
obj-$(CONFIG_MV64X60)		+= $(mv64x60-y) mv64x60_pic.o mv64x60_dev.o \
				   mv64x60_udbg.o
obj-$(CONFIG_RTC_DRV_CMOS)	+= rtc_cmos_setup.o
obj-$(CONFIG_AXON_RAM)		+= axonram.o

obj-$(CONFIG_PPC_INDIRECT_PCI)	+= indirect_pci.o
obj-$(CONFIG_PPC_I8259)		+= i8259.o
obj-$(CONFIG_IPIC)		+= ipic.o
obj-$(CONFIG_4xx)		+= uic.o
obj-$(CONFIG_4xx_SOC)		+= ppc4xx_soc.o
obj-$(CONFIG_XILINX_VIRTEX)	+= xilinx_intc.o
obj-$(CONFIG_OF_RTC)		+= of_rtc.o
ifeq ($(CONFIG_PCI),y)
obj-$(CONFIG_4xx)		+= ppc4xx_pci.o
endif
obj-$(CONFIG_PPC4xx_GPIO)	+= ppc4xx_gpio.o

obj-$(CONFIG_CPM)		+= cpm_common.o
obj-$(CONFIG_CPM2)		+= cpm2.o cpm2_pic.o
obj-$(CONFIG_QUICC_ENGINE)	+= cpm_common.o
obj-$(CONFIG_PPC_DCR)		+= dcr.o
obj-$(CONFIG_8xx)		+= mpc8xx_pic.o cpm1.o
obj-$(CONFIG_UCODE_PATCH)	+= micropatch.o

ifeq ($(CONFIG_SUSPEND),y)
obj-$(CONFIG_6xx)		+= 6xx-suspend.o
endif
