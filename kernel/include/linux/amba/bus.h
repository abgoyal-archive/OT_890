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
 *  linux/include/asm-arm/hardware/amba.h
 *
 *  Copyright (C) 2003 Deep Blue Solutions Ltd, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef ASMARM_AMBA_H
#define ASMARM_AMBA_H

#define AMBA_NR_IRQS	2

struct amba_device {
	struct device		dev;
	struct resource		res;
	u64			dma_mask;
	unsigned int		periphid;
	unsigned int		irq[AMBA_NR_IRQS];
};

struct amba_id {
	unsigned int		id;
	unsigned int		mask;
	void			*data;
};

struct amba_driver {
	struct device_driver	drv;
	int			(*probe)(struct amba_device *, void *);
	int			(*remove)(struct amba_device *);
	void			(*shutdown)(struct amba_device *);
	int			(*suspend)(struct amba_device *, pm_message_t);
	int			(*resume)(struct amba_device *);
	struct amba_id		*id_table;
};

#define amba_get_drvdata(d)	dev_get_drvdata(&d->dev)
#define amba_set_drvdata(d,p)	dev_set_drvdata(&d->dev, p)

int amba_driver_register(struct amba_driver *);
void amba_driver_unregister(struct amba_driver *);
int amba_device_register(struct amba_device *, struct resource *);
void amba_device_unregister(struct amba_device *);
struct amba_device *amba_find_device(const char *, struct device *, unsigned int, unsigned int);
int amba_request_regions(struct amba_device *, const char *);
void amba_release_regions(struct amba_device *);

#define amba_config(d)	(((d)->periphid >> 24) & 0xff)
#define amba_rev(d)	(((d)->periphid >> 20) & 0x0f)
#define amba_manf(d)	(((d)->periphid >> 12) & 0xff)
#define amba_part(d)	((d)->periphid & 0xfff)

#endif
