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
 *  Include file for NEC VR4100 series General-purpose I/O Unit.
 *
 *  Copyright (C) 2005  Yoichi Yuasa <yoichi_yuasa@tripeaks.co.jp>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __NEC_VR41XX_GIU_H
#define __NEC_VR41XX_GIU_H

/*
 * NEC VR4100 series GIU platform device IDs.
 */
enum {
	GPIO_50PINS_PULLUPDOWN,
	GPIO_36PINS,
	GPIO_48PINS_EDGE_SELECT,
};

typedef enum {
	IRQ_TRIGGER_LEVEL,
	IRQ_TRIGGER_EDGE,
	IRQ_TRIGGER_EDGE_FALLING,
	IRQ_TRIGGER_EDGE_RISING,
} irq_trigger_t;

typedef enum {
	IRQ_SIGNAL_THROUGH,
	IRQ_SIGNAL_HOLD,
} irq_signal_t;

extern void vr41xx_set_irq_trigger(unsigned int pin, irq_trigger_t trigger, irq_signal_t signal);

typedef enum {
	IRQ_LEVEL_LOW,
	IRQ_LEVEL_HIGH,
} irq_level_t;

extern void vr41xx_set_irq_level(unsigned int pin, irq_level_t level);

typedef enum {
	GPIO_DATA_LOW,
	GPIO_DATA_HIGH,
	GPIO_DATA_INVAL,
} gpio_data_t;

extern gpio_data_t vr41xx_gpio_get_pin(unsigned int pin);
extern int vr41xx_gpio_set_pin(unsigned int pin, gpio_data_t data);

typedef enum {
	GPIO_INPUT,
	GPIO_OUTPUT,
	GPIO_OUTPUT_DISABLE,
} gpio_direction_t;

extern int vr41xx_gpio_set_direction(unsigned int pin, gpio_direction_t dir);

typedef enum {
	GPIO_PULL_DOWN,
	GPIO_PULL_UP,
	GPIO_PULL_DISABLE,
} gpio_pull_t;

extern int vr41xx_gpio_pullupdown(unsigned int pin, gpio_pull_t pull);

#endif /* __NEC_VR41XX_GIU_H */
