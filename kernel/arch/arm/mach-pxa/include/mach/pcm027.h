/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/*
 * arch/arm/mach-pxa/include/mach/pcm027.h
 *
 * (c) 2003 Phytec Messtechnik GmbH <armlinux@phytec.de>
 * (c) 2007 Juergen Beisert <j.beisert@pengutronix.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * Definitions of CPU card resources only
 */

/* I2C RTC */
#define PCM027_RTC_IRQ_GPIO	0
#define PCM027_RTC_IRQ		IRQ_GPIO(PCM027_RTC_IRQ_GPIO)
#define PCM027_RTC_IRQ_EDGE	IRQ_TYPE_EDGE_FALLING
#define ADR_PCM027_RTC		0x51	/* I2C address */

/* I2C EEPROM */
#define ADR_PCM027_EEPROM	0x54	/* I2C address */

/* Ethernet chip (SMSC91C111) */
#define PCM027_ETH_IRQ_GPIO	52
#define PCM027_ETH_IRQ		IRQ_GPIO(PCM027_ETH_IRQ_GPIO)
#define PCM027_ETH_IRQ_EDGE	IRQ_TYPE_EDGE_RISING
#define PCM027_ETH_PHYS		PXA_CS5_PHYS
#define PCM027_ETH_SIZE		(1*1024*1024)

/* CAN controller SJA1000 (unsupported yet) */
#define PCM027_CAN_IRQ_GPIO	114
#define PCM027_CAN_IRQ		IRQ_GPIO(PCM027_CAN_IRQ_GPIO)
#define PCM027_CAN_IRQ_EDGE	IRQ_TYPE_EDGE_FALLING
#define PCM027_CAN_PHYS		0x22000000
#define PCM027_CAN_SIZE		0x100

/* SPI GPIO expander (unsupported yet) */
#define PCM027_EGPIO_IRQ_GPIO	27
#define PCM027_EGPIO_IRQ	IRQ_GPIO(PCM027_EGPIO_IRQ_GPIO)
#define PCM027_EGPIO_IRQ_EDGE	IRQ_TYPE_EDGE_FALLING
#define PCM027_EGPIO_CS		24
/*
 * TODO: Switch this pin from dedicated usage to GPIO if
 * more than the MAX7301 device is connected to this SPI bus
 */
#define PCM027_EGPIO_CS_MODE	GPIO24_SFRM_MD

/* Flash memory */
#define PCM027_FLASH_PHYS	0x00000000
#define PCM027_FLASH_SIZE	0x02000000

/* onboard LEDs connected to GPIO */
#define PCM027_LED_CPU		90
#define PCM027_LED_HEARD_BEAT	91

/*
 * This CPU module needs a baseboard to work. After basic initializing
 * its own devices, it calls baseboard's init function.
 * TODO: Add your own basebaord init function and call it from
 * inside pcm027_init(). This example here is for the developmen board.
 * Refer pcm990-baseboard.c
 */
extern void pcm990_baseboard_init(void);
