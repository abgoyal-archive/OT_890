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
 * Driver for simulating a mouse on GPIO lines.
 *
 * Copyright (C) 2007 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _GPIO_MOUSE_H
#define _GPIO_MOUSE_H

#define GPIO_MOUSE_POLARITY_ACT_HIGH	0x00
#define GPIO_MOUSE_POLARITY_ACT_LOW	0x01

#define GPIO_MOUSE_PIN_UP	0
#define GPIO_MOUSE_PIN_DOWN	1
#define GPIO_MOUSE_PIN_LEFT	2
#define GPIO_MOUSE_PIN_RIGHT	3
#define GPIO_MOUSE_PIN_BLEFT	4
#define GPIO_MOUSE_PIN_BMIDDLE	5
#define GPIO_MOUSE_PIN_BRIGHT	6
#define GPIO_MOUSE_PIN_MAX	7

/**
 * struct gpio_mouse_platform_data
 * @scan_ms: integer in ms specifying the scan periode.
 * @polarity: Pin polarity, active high or low.
 * @up: GPIO line for up value.
 * @down: GPIO line for down value.
 * @left: GPIO line for left value.
 * @right: GPIO line for right value.
 * @bleft: GPIO line for left button.
 * @bmiddle: GPIO line for middle button.
 * @bright: GPIO line for right button.
 *
 * This struct must be added to the platform_device in the board code.
 * It is used by the gpio_mouse driver to setup GPIO lines and to
 * calculate mouse movement.
 */
struct gpio_mouse_platform_data {
	int scan_ms;
	int polarity;

	union {
		struct {
			int up;
			int down;
			int left;
			int right;

			int bleft;
			int bmiddle;
			int bright;
		};
		int pins[GPIO_MOUSE_PIN_MAX];
	};
};

#endif /* _GPIO_MOUSE_H */
