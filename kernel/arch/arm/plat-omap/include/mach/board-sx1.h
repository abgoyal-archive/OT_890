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
 * Siemens SX1 board definitions
 *
 * Copyright: Vovan888 at gmail com
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef __ASM_ARCH_SX1_I2C_CHIPS_H
#define __ASM_ARCH_SX1_I2C_CHIPS_H

#define SOFIA_MAX_LIGHT_VAL	0x2B

#define SOFIA_I2C_ADDR		0x32
/* Sofia reg 3 bits masks */
#define SOFIA_POWER1_REG	0x03

#define	SOFIA_USB_POWER		0x01
#define	SOFIA_MMC_POWER		0x04
#define	SOFIA_BLUETOOTH_POWER	0x08
#define	SOFIA_MMILIGHT_POWER	0x20

#define SOFIA_POWER2_REG	0x04
#define SOFIA_BACKLIGHT_REG	0x06
#define SOFIA_KEYLIGHT_REG	0x07
#define SOFIA_DIMMING_REG	0x09


/* Function Prototypes for SX1 devices control on I2C bus */

int sx1_setbacklight(u8 backlight);
int sx1_getbacklight(u8 *backlight);
int sx1_setkeylight(u8 keylight);
int sx1_getkeylight(u8 *keylight);

int sx1_setmmipower(u8 onoff);
int sx1_setusbpower(u8 onoff);
int sx1_i2c_read_byte(u8 devaddr, u8 regoffset, u8 *value);
int sx1_i2c_write_byte(u8 devaddr, u8 regoffset, u8 value);

/* MMC prototypes */

extern void sx1_mmc_init(void);
extern void sx1_mmc_slot_cover_handler(void *arg, int state);

#endif /* __ASM_ARCH_SX1_I2C_CHIPS_H */
