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
# Makefile for the kernel pcmcia subsystem (c/o David Hinds)
#

pcmcia_core-y					+= cs.o cistpl.o rsrc_mgr.o socket_sysfs.o
pcmcia_core-$(CONFIG_CARDBUS)			+= cardbus.o
obj-$(CONFIG_PCCARD)				+= pcmcia_core.o

pcmcia-y					+= ds.o pcmcia_resource.o
pcmcia-$(CONFIG_PCMCIA_IOCTL)			+= pcmcia_ioctl.o
obj-$(CONFIG_PCMCIA)				+= pcmcia.o

obj-$(CONFIG_PCCARD_NONSTATIC)			+= rsrc_nonstatic.o


# socket drivers

obj-$(CONFIG_YENTA) 				+= yenta_socket.o

obj-$(CONFIG_PD6729)				+= pd6729.o
obj-$(CONFIG_I82365)				+= i82365.o
obj-$(CONFIG_I82092)				+= i82092.o
obj-$(CONFIG_TCIC)				+= tcic.o
obj-$(CONFIG_PCMCIA_M8XX)			+= m8xx_pcmcia.o
obj-$(CONFIG_PCMCIA_SA1100)			+= sa11xx_core.o sa1100_cs.o
obj-$(CONFIG_PCMCIA_SA1111)			+= sa11xx_core.o sa1111_cs.o
obj-$(CONFIG_M32R_PCC)				+= m32r_pcc.o
obj-$(CONFIG_M32R_CFC)				+= m32r_cfc.o
obj-$(CONFIG_PCMCIA_AU1X00)			+= au1x00_ss.o
obj-$(CONFIG_PCMCIA_VRC4171)			+= vrc4171_card.o
obj-$(CONFIG_PCMCIA_VRC4173)			+= vrc4173_cardu.o
obj-$(CONFIG_OMAP_CF)				+= omap_cf.o
obj-$(CONFIG_BFIN_CFPCMCIA)			+= bfin_cf_pcmcia.o
obj-$(CONFIG_AT91_CF)				+= at91_cf.o
obj-$(CONFIG_ELECTRA_CF)			+= electra_cf.o

sa11xx_core-y					+= soc_common.o sa11xx_base.o
pxa2xx_core-y					+= soc_common.o pxa2xx_base.o

au1x00_ss-y					+= au1000_generic.o
au1x00_ss-$(CONFIG_MIPS_PB1000)			+= au1000_pb1x00.o
au1x00_ss-$(CONFIG_MIPS_PB1100)			+= au1000_pb1x00.o
au1x00_ss-$(CONFIG_MIPS_PB1200)			+= au1000_db1x00.o
au1x00_ss-$(CONFIG_MIPS_PB1500)			+= au1000_pb1x00.o
au1x00_ss-$(CONFIG_MIPS_DB1000)			+= au1000_db1x00.o
au1x00_ss-$(CONFIG_MIPS_DB1100)			+= au1000_db1x00.o
au1x00_ss-$(CONFIG_MIPS_DB1200)			+= au1000_db1x00.o
au1x00_ss-$(CONFIG_MIPS_DB1500)			+= au1000_db1x00.o
au1x00_ss-$(CONFIG_MIPS_DB1550)			+= au1000_db1x00.o
au1x00_ss-$(CONFIG_MIPS_XXS1500)		+= au1000_xxs1500.o

sa1111_cs-y					+= sa1111_generic.o
sa1111_cs-$(CONFIG_ASSABET_NEPONSET)		+= sa1100_neponset.o
sa1111_cs-$(CONFIG_SA1100_BADGE4)		+= sa1100_badge4.o
sa1111_cs-$(CONFIG_SA1100_JORNADA720)		+= sa1100_jornada720.o

sa1100_cs-y					+= sa1100_generic.o
sa1100_cs-$(CONFIG_SA1100_ASSABET)		+= sa1100_assabet.o
sa1100_cs-$(CONFIG_SA1100_CERF)			+= sa1100_cerf.o
sa1100_cs-$(CONFIG_SA1100_COLLIE)              += pxa2xx_sharpsl.o
sa1100_cs-$(CONFIG_SA1100_H3600)		+= sa1100_h3600.o
sa1100_cs-$(CONFIG_SA1100_SHANNON)		+= sa1100_shannon.o
sa1100_cs-$(CONFIG_SA1100_SIMPAD)		+= sa1100_simpad.o

pxa2xx_lubbock_cs-y				+= pxa2xx_lubbock.o sa1111_generic.o
pxa2xx_cm_x2xx_cs-y				+= pxa2xx_cm_x2xx.o pxa2xx_cm_x255.o pxa2xx_cm_x270.o
pxa2xx-obj-$(CONFIG_ARCH_LUBBOCK)		+= pxa2xx_lubbock_cs.o
pxa2xx-obj-$(CONFIG_MACH_MAINSTONE)		+= pxa2xx_mainstone.o
pxa2xx-obj-$(CONFIG_PXA_SHARPSL)		+= pxa2xx_sharpsl.o
pxa2xx-obj-$(CONFIG_MACH_ARMCORE)		+= pxa2xx_cm_x2xx_cs.o
pxa2xx-obj-$(CONFIG_ARCH_VIPER)			+= pxa2xx_viper.o
pxa2xx-obj-$(CONFIG_TRIZEPS_PCMCIA)		+= pxa2xx_trizeps4.o
pxa2xx-obj-$(CONFIG_MACH_PALMTX)		+= pxa2xx_palmtx.o
pxa2xx-obj-$(CONFIG_MACH_PALMLD)		+= pxa2xx_palmld.o
pxa2xx-obj-$(CONFIG_MACH_E740)			+= pxa2xx_e740.o

obj-$(CONFIG_PCMCIA_PXA2XX)			+= pxa2xx_core.o $(pxa2xx-obj-y)
