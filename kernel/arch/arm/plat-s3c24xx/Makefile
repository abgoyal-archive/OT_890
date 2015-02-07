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


# arch/arm/plat-s3c24xx/Makefile
#
# Copyright 2007 Simtec Electronics
#
# Licensed under GPLv2

obj-y				:=
obj-m				:=
obj-n				:=
obj-				:=


# Core files

obj-y				+= cpu.o
obj-y				+= irq.o
obj-y				+= devs.o
obj-y				+= gpio.o
obj-y				+= gpiolib.o
obj-y				+= clock.o
obj-$(CONFIG_S3C24XX_DCLK)	+= clock-dclk.o

# Architecture dependant builds

obj-$(CONFIG_CPU_S3C244X)	+= s3c244x.o
obj-$(CONFIG_CPU_S3C244X)	+= s3c244x-irq.o
obj-$(CONFIG_CPU_S3C244X)	+= s3c244x-clock.o
obj-$(CONFIG_PM_SIMTEC)		+= pm-simtec.o
obj-$(CONFIG_PM)		+= pm.o
obj-$(CONFIG_PM)		+= sleep.o
obj-$(CONFIG_HAVE_PWM)		+= pwm.o
obj-$(CONFIG_S3C2410_CLOCK)	+= s3c2410-clock.o
obj-$(CONFIG_S3C2410_DMA)	+= dma.o
obj-$(CONFIG_S3C24XX_ADC)	+= adc.o

# device specific setup and/or initialisation
obj-$(CONFIG_ARCH_S3C2410)	+= setup-i2c.o

# SPI gpio central GPIO functions

obj-$(CONFIG_S3C24XX_SPI_BUS0_GPE11_GPE12_GPE13) += spi-bus0-gpe11_12_13.o
obj-$(CONFIG_S3C24XX_SPI_BUS1_GPG5_GPG6_GPG7)    += spi-bus1-gpg5_6_7.o

# machine common support

obj-$(CONFIG_MACH_SMDK)		+= common-smdk.o
