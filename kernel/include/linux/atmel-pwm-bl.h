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
 * Copyright (C) 2007 Atmel Corporation
 *
 * Driver for the AT32AP700X PS/2 controller (PSIF).
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 */

#ifndef __INCLUDE_ATMEL_PWM_BL_H
#define __INCLUDE_ATMEL_PWM_BL_H

/**
 * struct atmel_pwm_bl_platform_data
 * @pwm_channel: which PWM channel in the PWM module to use.
 * @pwm_frequency: PWM frequency to generate, the driver will try to be as
 *	close as the prescaler allows.
 * @pwm_compare_max: value to use in the PWM channel compare register.
 * @pwm_duty_max: maximum duty cycle value, must be less than or equal to
 *	pwm_compare_max.
 * @pwm_duty_min: minimum duty cycle value, must be less than pwm_duty_max.
 * @pwm_active_low: set to one if the low part of the PWM signal increases the
 *	brightness of the backlight.
 * @gpio_on: GPIO line to control the backlight on/off, set to -1 if not used.
 * @on_active_low: set to one if the on/off signal is on when GPIO is low.
 *
 * This struct must be added to the platform device in the board code. It is
 * used by the atmel-pwm-bl driver to setup the GPIO to control on/off and the
 * PWM device.
 */
struct atmel_pwm_bl_platform_data {
	unsigned int pwm_channel;
	unsigned int pwm_frequency;
	unsigned int pwm_compare_max;
	unsigned int pwm_duty_max;
	unsigned int pwm_duty_min;
	unsigned int pwm_active_low;
	int gpio_on;
	unsigned int on_active_low;
};

#endif /* __INCLUDE_ATMEL_PWM_BL_H */
