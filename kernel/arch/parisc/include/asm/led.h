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

#ifndef LED_H
#define LED_H

#define	LED7		0x80		/* top (or furthest right) LED */
#define	LED6		0x40
#define	LED5		0x20
#define	LED4		0x10
#define	LED3		0x08
#define	LED2		0x04
#define	LED1		0x02
#define	LED0		0x01		/* bottom (or furthest left) LED */

#define	LED_LAN_TX	LED0		/* for LAN transmit activity */
#define	LED_LAN_RCV	LED1		/* for LAN receive activity */
#define	LED_DISK_IO	LED2		/* for disk activity */
#define	LED_HEARTBEAT	LED3		/* heartbeat */

/* values for pdc_chassis_lcd_info_ret_block.model: */
#define DISPLAY_MODEL_LCD  0		/* KittyHawk LED or LCD */
#define DISPLAY_MODEL_NONE 1		/* no LED or LCD */
#define DISPLAY_MODEL_LASI 2		/* LASI style 8 bit LED */
#define DISPLAY_MODEL_OLD_ASP 0x7F	/* faked: ASP style 8 x 1 bit LED (only very old ASP versions) */

#define LED_CMD_REG_NONE 0		/* NULL == no addr for the cmd register */

/* register_led_driver() */
int __init register_led_driver(int model, unsigned long cmd_reg, unsigned long data_reg);

/* registers the LED regions for procfs */
void __init register_led_regions(void);

#ifdef CONFIG_CHASSIS_LCD_LED
/* writes a string to the LCD display (if possible on this h/w) */
int lcd_print(const char *str);
#else
#define lcd_print(str)
#endif

/* main LED initialization function (uses PDC) */ 
int __init led_init(void);

#endif /* LED_H */
