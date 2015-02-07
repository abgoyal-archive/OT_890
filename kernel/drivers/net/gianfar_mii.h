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
 * drivers/net/gianfar_mii.h
 *
 * Gianfar Ethernet Driver -- MII Management Bus Implementation
 * Driver for the MDIO bus controller in the Gianfar register space
 *
 * Author: Andy Fleming
 * Maintainer: Kumar Gala
 *
 * Copyright (c) 2002-2004 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */
#ifndef __GIANFAR_MII_H
#define __GIANFAR_MII_H

struct gfar_private; /* forward ref */

#define MIIMIND_BUSY            0x00000001
#define MIIMIND_NOTVALID        0x00000004

#define MII_READ_COMMAND       0x00000001

#define GFAR_SUPPORTED (SUPPORTED_10baseT_Half \
		| SUPPORTED_10baseT_Full \
		| SUPPORTED_100baseT_Half \
		| SUPPORTED_100baseT_Full \
		| SUPPORTED_Autoneg \
		| SUPPORTED_MII)

struct gfar_mii {
	u32	miimcfg;	/* 0x.520 - MII Management Config Register */
	u32	miimcom;	/* 0x.524 - MII Management Command Register */
	u32	miimadd;	/* 0x.528 - MII Management Address Register */
	u32	miimcon;	/* 0x.52c - MII Management Control Register */
	u32	miimstat;	/* 0x.530 - MII Management Status Register */
	u32	miimind;	/* 0x.534 - MII Management Indicator Register */
};

int gfar_mdio_read(struct mii_bus *bus, int mii_id, int regnum);
int gfar_mdio_write(struct mii_bus *bus, int mii_id, int regnum, u16 value);
int gfar_local_mdio_write(struct gfar_mii __iomem *regs, int mii_id,
			  int regnum, u16 value);
int gfar_local_mdio_read(struct gfar_mii __iomem *regs, int mii_id, int regnum);
struct mii_bus *gfar_get_miibus(const struct gfar_private *priv);
int __init gfar_mdio_init(void);
void gfar_mdio_exit(void);

void gfar_mdio_bus_name(char *name, struct device_node *np);
#endif /* GIANFAR_PHY_H */
