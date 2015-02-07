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
# Makefile for the i2c bus drivers.
#

# PC SMBus host controller drivers
obj-$(CONFIG_I2C_ALI1535)	+= i2c-ali1535.o
obj-$(CONFIG_I2C_ALI1563)	+= i2c-ali1563.o
obj-$(CONFIG_I2C_ALI15X3)	+= i2c-ali15x3.o
obj-$(CONFIG_I2C_AMD756)	+= i2c-amd756.o
obj-$(CONFIG_I2C_AMD756_S4882)	+= i2c-amd756-s4882.o
obj-$(CONFIG_I2C_AMD8111)	+= i2c-amd8111.o
obj-$(CONFIG_I2C_I801)		+= i2c-i801.o
obj-$(CONFIG_I2C_ISCH)		+= i2c-isch.o
obj-$(CONFIG_I2C_NFORCE2)	+= i2c-nforce2.o
obj-$(CONFIG_I2C_NFORCE2_S4985)	+= i2c-nforce2-s4985.o
obj-$(CONFIG_I2C_PIIX4)		+= i2c-piix4.o
obj-$(CONFIG_I2C_SIS5595)	+= i2c-sis5595.o
obj-$(CONFIG_I2C_SIS630)	+= i2c-sis630.o
obj-$(CONFIG_I2C_SIS96X)	+= i2c-sis96x.o
obj-$(CONFIG_I2C_VIA)		+= i2c-via.o
obj-$(CONFIG_I2C_VIAPRO)	+= i2c-viapro.o

# Mac SMBus host controller drivers
obj-$(CONFIG_I2C_HYDRA)		+= i2c-hydra.o
obj-$(CONFIG_I2C_POWERMAC)	+= i2c-powermac.o

# Embebbed system I2C/SMBus host controller drivers
obj-$(CONFIG_I2C_AT91)		+= i2c-at91.o
obj-$(CONFIG_I2C_AU1550)	+= i2c-au1550.o
obj-$(CONFIG_I2C_BLACKFIN_TWI)	+= i2c-bfin-twi.o
obj-$(CONFIG_I2C_CPM)		+= i2c-cpm.o
obj-$(CONFIG_I2C_DAVINCI)	+= i2c-davinci.o
obj-$(CONFIG_I2C_GPIO)		+= i2c-gpio.o
obj-$(CONFIG_I2C_HIGHLANDER)	+= i2c-highlander.o
obj-$(CONFIG_I2C_IBM_IIC)	+= i2c-ibm_iic.o
obj-$(CONFIG_I2C_IOP3XX)	+= i2c-iop3xx.o
obj-$(CONFIG_I2C_IXP2000)	+= i2c-ixp2000.o
obj-$(CONFIG_I2C_MPC)		+= i2c-mpc.o
obj-$(CONFIG_I2C_MV64XXX)	+= i2c-mv64xxx.o
obj-$(CONFIG_I2C_OCORES)	+= i2c-ocores.o
obj-$(CONFIG_I2C_OMAP)		+= i2c-omap.o
obj-$(CONFIG_I2C_PASEMI)	+= i2c-pasemi.o
obj-$(CONFIG_I2C_PNX)		+= i2c-pnx.o
obj-$(CONFIG_I2C_PXA)		+= i2c-pxa.o
obj-$(CONFIG_I2C_S3C2410)	+= i2c-s3c2410.o
obj-$(CONFIG_I2C_SH7760)	+= i2c-sh7760.o
obj-$(CONFIG_I2C_SH_MOBILE)	+= i2c-sh_mobile.o
obj-$(CONFIG_I2C_SIMTEC)	+= i2c-simtec.o
obj-$(CONFIG_I2C_VERSATILE)	+= i2c-versatile.o

# External I2C/SMBus adapter drivers
obj-$(CONFIG_I2C_PARPORT)	+= i2c-parport.o
obj-$(CONFIG_I2C_PARPORT_LIGHT)	+= i2c-parport-light.o
obj-$(CONFIG_I2C_TAOS_EVM)	+= i2c-taos-evm.o
obj-$(CONFIG_I2C_TINY_USB)	+= i2c-tiny-usb.o

# Graphics adapter I2C/DDC channel drivers
obj-$(CONFIG_I2C_VOODOO3)	+= i2c-voodoo3.o

# Other I2C/SMBus bus drivers
obj-$(CONFIG_I2C_ACORN)		+= i2c-acorn.o
obj-$(CONFIG_I2C_ELEKTOR)	+= i2c-elektor.o
obj-$(CONFIG_I2C_PCA_ISA)	+= i2c-pca-isa.o
obj-$(CONFIG_I2C_PCA_PLATFORM)	+= i2c-pca-platform.o
obj-$(CONFIG_I2C_PMCMSP)	+= i2c-pmcmsp.o
obj-$(CONFIG_I2C_SIBYTE)	+= i2c-sibyte.o
obj-$(CONFIG_I2C_STUB)		+= i2c-stub.o
obj-$(CONFIG_SCx200_ACB)	+= scx200_acb.o
obj-$(CONFIG_SCx200_I2C)	+= scx200_i2c.o
obj-$(CONFIG_I2C_MT3351)	+= i2c-mt3351.o
obj-$(CONFIG_I2C_MT6516)	+= i2c-mt6516.o

ifeq ($(CONFIG_I2C_DEBUG_BUS),y)
EXTRA_CFLAGS += -DDEBUG
endif
