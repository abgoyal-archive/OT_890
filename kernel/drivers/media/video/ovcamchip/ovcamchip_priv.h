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

/* OmniVision* camera chip driver private definitions for core code and
 * chip-specific code
 *
 * Copyright (c) 1999-2004 Mark McClelland
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version. NO WARRANTY OF ANY KIND is expressed or implied.
 *
 * * OmniVision is a trademark of OmniVision Technologies, Inc. This driver
 * is not sponsored or developed by them.
 */

#ifndef __LINUX_OVCAMCHIP_PRIV_H
#define __LINUX_OVCAMCHIP_PRIV_H

#include <linux/i2c.h>
#include <media/ovcamchip.h>

#ifdef DEBUG
extern int ovcamchip_debug;
#endif

#define PDEBUG(level, fmt, args...) \
	if (ovcamchip_debug >= (level))	pr_debug("[%s:%d] " fmt "\n", \
		__func__, __LINE__ , ## args)

#define DDEBUG(level, dev, fmt, args...) \
	if (ovcamchip_debug >= (level))	dev_dbg(dev, "[%s:%d] " fmt "\n", \
		__func__, __LINE__ , ## args)

/* Number of times to retry chip detection. Increase this if you are getting
 * "Failed to init camera chip" */
#define I2C_DETECT_RETRIES	10

struct ovcamchip_regvals {
	unsigned char reg;
	unsigned char val;
};

struct ovcamchip_ops {
	int (*init)(struct i2c_client *);
	int (*free)(struct i2c_client *);
	int (*command)(struct i2c_client *, unsigned int, void *);
};

struct ovcamchip {
	struct ovcamchip_ops *sops;
	void *spriv;               /* Private data for OV7x10.c etc... */
	int subtype;               /* = SEN_OV7610 etc... */
	int mono;                  /* Monochrome chip? (invalid until init) */
	int initialized;           /* OVCAMCHIP_CMD_INITIALIZE was successful */
};

extern struct ovcamchip_ops ov6x20_ops;
extern struct ovcamchip_ops ov6x30_ops;
extern struct ovcamchip_ops ov7x10_ops;
extern struct ovcamchip_ops ov7x20_ops;
extern struct ovcamchip_ops ov76be_ops;

/* --------------------------------- */
/*              I2C I/O              */
/* --------------------------------- */

static inline int ov_read(struct i2c_client *c, unsigned char reg,
			  unsigned char *value)
{
	int rc;

	rc = i2c_smbus_read_byte_data(c, reg);
	*value = (unsigned char) rc;
	return rc;
}

static inline int ov_write(struct i2c_client *c, unsigned char reg,
			   unsigned char value )
{
	return i2c_smbus_write_byte_data(c, reg, value);
}

/* --------------------------------- */
/*        FUNCTION PROTOTYPES        */
/* --------------------------------- */

/* Functions in ovcamchip_core.c */

extern int ov_write_regvals(struct i2c_client *c,
			    struct ovcamchip_regvals *rvals);

extern int ov_write_mask(struct i2c_client *c, unsigned char reg,
			 unsigned char value, unsigned char mask);

#endif
