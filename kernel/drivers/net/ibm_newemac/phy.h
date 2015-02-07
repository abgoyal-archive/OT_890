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
 * drivers/net/ibm_newemac/phy.h
 *
 * Driver for PowerPC 4xx on-chip ethernet controller, PHY support
 *
 * Copyright 2007 Benjamin Herrenschmidt, IBM Corp.
 *                <benh@kernel.crashing.org>
 *
 * Based on the arch/ppc version of the driver:
 *
 * Benjamin Herrenschmidt <benh@kernel.crashing.org>
 * February 2003
 *
 * Minor additions by Eugene Surovegin <ebs@ebshome.net>, 2004
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * This file basically duplicates sungem_phy.{c,h} with different PHYs
 * supported. I'm looking into merging that in a single mii layer more
 * flexible than mii.c
 */

#ifndef __IBM_NEWEMAC_PHY_H
#define __IBM_NEWEMAC_PHY_H

struct mii_phy;

/* Operations supported by any kind of PHY */
struct mii_phy_ops {
	int (*init) (struct mii_phy * phy);
	int (*suspend) (struct mii_phy * phy, int wol_options);
	int (*setup_aneg) (struct mii_phy * phy, u32 advertise);
	int (*setup_forced) (struct mii_phy * phy, int speed, int fd);
	int (*poll_link) (struct mii_phy * phy);
	int (*read_link) (struct mii_phy * phy);
};

/* Structure used to statically define an mii/gii based PHY */
struct mii_phy_def {
	u32 phy_id;		/* Concatenated ID1 << 16 | ID2 */
	u32 phy_id_mask;	/* Significant bits */
	u32 features;		/* Ethtool SUPPORTED_* defines or
				   0 for autodetect */
	int magic_aneg;		/* Autoneg does all speed test for us */
	const char *name;
	const struct mii_phy_ops *ops;
};

/* An instance of a PHY, partially borrowed from mii_if_info */
struct mii_phy {
	struct mii_phy_def *def;
	u32 advertising;	/* Ethtool ADVERTISED_* defines */
	u32 features;		/* Copied from mii_phy_def.features
				   or determined automaticaly */
	int address;		/* PHY address */
	int mode;		/* PHY mode */
	int gpcs_address;	/* GPCS PHY address */

	/* 1: autoneg enabled, 0: disabled */
	int autoneg;

	/* forced speed & duplex (no autoneg)
	 * partner speed & duplex & pause (autoneg)
	 */
	int speed;
	int duplex;
	int pause;
	int asym_pause;

	/* Provided by host chip */
	struct net_device *dev;
	int (*mdio_read) (struct net_device * dev, int addr, int reg);
	void (*mdio_write) (struct net_device * dev, int addr, int reg,
			    int val);
};

/* Pass in a struct mii_phy with dev, mdio_read and mdio_write
 * filled, the remaining fields will be filled on return
 */
int emac_mii_phy_probe(struct mii_phy *phy, int address);
int emac_mii_reset_phy(struct mii_phy *phy);
int emac_mii_reset_gpcs(struct mii_phy *phy);

#endif /* __IBM_NEWEMAC_PHY_H */
