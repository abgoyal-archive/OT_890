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

#ifndef __LINUX_PCF857X_H
#define __LINUX_PCF857X_H

/**
 * struct pcf857x_platform_data - data to set up pcf857x driver
 * @gpio_base: number of the chip's first GPIO
 * @n_latch: optional bit-inverse of initial register value; if
 *	you leave this initialized to zero the driver will act
 *	like the chip was just reset
 * @setup: optional callback issued once the GPIOs are valid
 * @teardown: optional callback issued before the GPIOs are invalidated
 * @context: optional parameter passed to setup() and teardown()
 *
 * In addition to the I2C_BOARD_INFO() state appropriate to each chip,
 * the i2c_board_info used with the pcf875x driver must provide its
 * platform_data (pointer to one of these structures) with at least
 * the gpio_base value initialized.
 *
 * The @setup callback may be used with the kind of board-specific glue
 * which hands the (now-valid) GPIOs to other drivers, or which puts
 * devices in their initial states using these GPIOs.
 *
 * These GPIO chips are only "quasi-bidirectional"; read the chip specs
 * to understand the behavior.  They don't have separate registers to
 * record which pins are used for input or output, record which output
 * values are driven, or provide access to input values.  That must be
 * inferred by reading the chip's value and knowing the last value written
 * to it.  If you leave n_latch initialized to zero, that last written
 * value is presumed to be all ones (as if the chip were just reset).
 */
struct pcf857x_platform_data {
	unsigned	gpio_base;
	unsigned	n_latch;

	int		(*setup)(struct i2c_client *client,
					int gpio, unsigned ngpio,
					void *context);
	int		(*teardown)(struct i2c_client *client,
					int gpio, unsigned ngpio,
					void *context);
	void		*context;
};

#endif /* __LINUX_PCF857X_H */
