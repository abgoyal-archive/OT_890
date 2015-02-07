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
# Makefile for the Linux IrDA infrared port device drivers.
#
# 9 Aug 2000, Christoph Hellwig <hch@infradead.org>
# Rewritten to use lists instead of if-statements.
#

# FIR drivers
obj-$(CONFIG_USB_IRDA)		+= irda-usb.o
obj-$(CONFIG_SIGMATEL_FIR)	+= stir4200.o
obj-$(CONFIG_NSC_FIR)		+= nsc-ircc.o
obj-$(CONFIG_WINBOND_FIR)	+= w83977af_ir.o
obj-$(CONFIG_SA1100_FIR)	+= sa1100_ir.o
obj-$(CONFIG_TOSHIBA_FIR)	+= donauboe.o
obj-$(CONFIG_SMC_IRCC_FIR)	+= smsc-ircc2.o
obj-$(CONFIG_ALI_FIR)		+= ali-ircc.o
obj-$(CONFIG_VLSI_FIR)		+= vlsi_ir.o
obj-$(CONFIG_VIA_FIR)		+= via-ircc.o
obj-$(CONFIG_PXA_FICP)	        += pxaficp_ir.o
obj-$(CONFIG_MCS_FIR)	        += mcs7780.o
obj-$(CONFIG_AU1000_FIR)	+= au1k_ir.o
# SIR drivers
obj-$(CONFIG_IRTTY_SIR)		+= irtty-sir.o	sir-dev.o
# dongle drivers for SIR drivers
obj-$(CONFIG_ESI_DONGLE)	+= esi-sir.o
obj-$(CONFIG_TEKRAM_DONGLE)	+= tekram-sir.o
obj-$(CONFIG_ACTISYS_DONGLE)	+= actisys-sir.o
obj-$(CONFIG_LITELINK_DONGLE)	+= litelink-sir.o
obj-$(CONFIG_GIRBIL_DONGLE)	+= girbil-sir.o
obj-$(CONFIG_OLD_BELKIN_DONGLE)	+= old_belkin-sir.o
obj-$(CONFIG_MCP2120_DONGLE)	+= mcp2120-sir.o
obj-$(CONFIG_ACT200L_DONGLE)	+= act200l-sir.o
obj-$(CONFIG_MA600_DONGLE)	+= ma600-sir.o
obj-$(CONFIG_TOIM3232_DONGLE)	+= toim3232-sir.o
obj-$(CONFIG_EP7211_DONGLE)	+= ep7211-sir.o
obj-$(CONFIG_KINGSUN_DONGLE)	+= kingsun-sir.o
obj-$(CONFIG_KSDAZZLE_DONGLE)	+= ksdazzle-sir.o
obj-$(CONFIG_KS959_DONGLE)    	+= ks959-sir.o

# The SIR helper module
sir-dev-objs := sir_dev.o sir_dongle.o
