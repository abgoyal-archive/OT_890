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
# Makefile for the linux kernel.
#

# Common support
obj-y := io.o id.o sram.o clock.o irq.o mux.o serial.o devices.o

obj-$(CONFIG_OMAP_MCBSP) += mcbsp.o

obj-$(CONFIG_OMAP_MPU_TIMER)	+= time.o
obj-$(CONFIG_OMAP_32K_TIMER)	+= timer32k.o

# Power Management
obj-$(CONFIG_PM) += pm.o sleep.o

led-y := leds.o

# Specific board support
obj-$(CONFIG_MACH_OMAP_H2)		+= board-h2.o board-h2-mmc.o
obj-$(CONFIG_MACH_OMAP_INNOVATOR)	+= board-innovator.o
obj-$(CONFIG_MACH_OMAP_GENERIC)		+= board-generic.o
obj-$(CONFIG_MACH_OMAP_PERSEUS2)	+= board-perseus2.o
obj-$(CONFIG_MACH_OMAP_FSAMPLE)		+= board-fsample.o
obj-$(CONFIG_MACH_OMAP_OSK)		+= board-osk.o
obj-$(CONFIG_MACH_OMAP_H3)		+= board-h3.o board-h3-mmc.o
obj-$(CONFIG_MACH_VOICEBLUE)		+= board-voiceblue.o
obj-$(CONFIG_MACH_OMAP_PALMTE)		+= board-palmte.o
obj-$(CONFIG_MACH_OMAP_PALMZ71)		+= board-palmz71.o
obj-$(CONFIG_MACH_OMAP_PALMTT)		+= board-palmtt.o
obj-$(CONFIG_MACH_NOKIA770)		+= board-nokia770.o
obj-$(CONFIG_MACH_AMS_DELTA)		+= board-ams-delta.o
obj-$(CONFIG_MACH_SX1)			+= board-sx1.o board-sx1-mmc.o

ifeq ($(CONFIG_ARCH_OMAP15XX),y)
# Innovator-1510 FPGA
obj-$(CONFIG_MACH_OMAP_INNOVATOR)	+= fpga.o
endif

# LEDs support
led-$(CONFIG_MACH_OMAP_H2)		+= leds-h2p2-debug.o
led-$(CONFIG_MACH_OMAP_H3)		+= leds-h2p2-debug.o
led-$(CONFIG_MACH_OMAP_INNOVATOR)	+= leds-innovator.o
led-$(CONFIG_MACH_OMAP_PERSEUS2)	+= leds-h2p2-debug.o
led-$(CONFIG_MACH_OMAP_OSK)		+= leds-osk.o
obj-$(CONFIG_LEDS)			+= $(led-y)
