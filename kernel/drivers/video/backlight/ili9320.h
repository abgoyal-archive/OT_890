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

/* drivers/video/backlight/ili9320.h
 *
 * ILI9320 LCD controller driver core.
 *
 * Copyright 2007 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * http://armlinux.simtec.co.uk/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

/* Holder for register and value pairs. */
struct ili9320_reg {
	unsigned short		address;
	unsigned short		value;
};

struct ili9320;

struct ili9320_client {
	const char	*name;
	int	(*init)(struct ili9320 *ili, struct ili9320_platdata *cfg);

};
/* Device attached via an SPI bus. */
struct  ili9320_spi {
	struct spi_device	*dev;
	struct spi_message	message;
	struct spi_transfer	xfer[2];

	unsigned char		id;
	unsigned char		buffer_addr[4];
	unsigned char		buffer_data[4];
};

/* ILI9320 device state. */
struct ili9320 {
	union {
		struct ili9320_spi	spi;	/* SPI attachged device. */
	} access;				/* Register access method. */

	struct device			*dev;
	struct lcd_device		*lcd;	/* LCD device we created. */
	struct ili9320_client		*client;
	struct ili9320_platdata		*platdata;

	int				 power; /* current power state. */
	int				 initialised;

	unsigned short			 display1;
	unsigned short			 power1;

	int (*write)(struct ili9320 *ili, unsigned int reg, unsigned int val);
};


/* ILI9320 register access routines */

extern int ili9320_write(struct ili9320 *ili,
			 unsigned int reg, unsigned int value);

extern int ili9320_write_regs(struct ili9320 *ili,
			      struct ili9320_reg *values,
			      int nr_values);

/* Device probe */

extern int ili9320_probe_spi(struct spi_device *spi,
			     struct ili9320_client *cli);

extern int ili9320_remove(struct ili9320 *lcd);
extern void ili9320_shutdown(struct ili9320 *lcd);

/* PM */

extern int ili9320_suspend(struct ili9320 *lcd, pm_message_t state);
extern int ili9320_resume(struct ili9320 *lcd);
