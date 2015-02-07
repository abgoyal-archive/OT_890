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
 * drivers/net/phy/ste10Xp.c
 *
 * Driver for STMicroelectronics STe10Xp PHYs
 *
 * Author: Giuseppe Cavallaro <peppe.cavallaro@st.com>
 *
 * Copyright (c) 2008 STMicroelectronics Limited
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/interrupt.h>
#include <linux/netdevice.h>
#include <linux/ethtool.h>
#include <linux/mii.h>
#include <linux/phy.h>

#define MII_XCIIS   	0x11	/* Configuration Info IRQ & Status Reg */
#define MII_XIE     	0x12	/* Interrupt Enable Register */
#define MII_XIE_DEFAULT_MASK 0x0070 /* ANE complete, Remote Fault, Link Down */

#define STE101P_PHY_ID		0x00061c50
#define STE100P_PHY_ID       	0x1c040011

static int ste10Xp_config_init(struct phy_device *phydev)
{
	int value, err;

	/* Software Reset PHY */
	value = phy_read(phydev, MII_BMCR);
	if (value < 0)
		return value;

	value |= BMCR_RESET;
	err = phy_write(phydev, MII_BMCR, value);
	if (err < 0)
		return err;

	do {
		value = phy_read(phydev, MII_BMCR);
	} while (value & BMCR_RESET);

	return 0;
}

static int ste10Xp_config_intr(struct phy_device *phydev)
{
	int err, value;

	if (phydev->interrupts == PHY_INTERRUPT_ENABLED) {
		/* Enable all STe101P interrupts (PR12) */
		err = phy_write(phydev, MII_XIE, MII_XIE_DEFAULT_MASK);
		/* clear any pending interrupts */
		if (err == 0) {
			value = phy_read(phydev, MII_XCIIS);
			if (value < 0)
				err = value;
		}
	} else
		err = phy_write(phydev, MII_XIE, 0);

	return err;
}

static int ste10Xp_ack_interrupt(struct phy_device *phydev)
{
	int err = phy_read(phydev, MII_XCIIS);
	if (err < 0)
		return err;

	return 0;
}

static struct phy_driver ste101p_pdriver = {
	.phy_id = STE101P_PHY_ID,
	.phy_id_mask = 0xfffffff0,
	.name = "STe101p",
	.features = PHY_BASIC_FEATURES | SUPPORTED_Pause,
	.flags = PHY_HAS_INTERRUPT,
	.config_init = ste10Xp_config_init,
	.config_aneg = genphy_config_aneg,
	.read_status = genphy_read_status,
	.ack_interrupt = ste10Xp_ack_interrupt,
	.config_intr = ste10Xp_config_intr,
	.suspend = genphy_suspend,
	.resume = genphy_resume,
	.driver = {.owner = THIS_MODULE,}
};

static struct phy_driver ste100p_pdriver = {
	.phy_id = STE100P_PHY_ID,
	.phy_id_mask = 0xffffffff,
	.name = "STe100p",
	.features = PHY_BASIC_FEATURES | SUPPORTED_Pause,
	.flags = PHY_HAS_INTERRUPT,
	.config_init = ste10Xp_config_init,
	.config_aneg = genphy_config_aneg,
	.read_status = genphy_read_status,
	.ack_interrupt = ste10Xp_ack_interrupt,
	.config_intr = ste10Xp_config_intr,
	.suspend = genphy_suspend,
	.resume = genphy_resume,
	.driver = {.owner = THIS_MODULE,}
};

static int __init ste10Xp_init(void)
{
	int retval;

	retval = phy_driver_register(&ste100p_pdriver);
	if (retval < 0)
		return retval;
	return phy_driver_register(&ste101p_pdriver);
}

static void __exit ste10Xp_exit(void)
{
	phy_driver_unregister(&ste100p_pdriver);
	phy_driver_unregister(&ste101p_pdriver);
}

module_init(ste10Xp_init);
module_exit(ste10Xp_exit);

MODULE_DESCRIPTION("STMicroelectronics STe10Xp PHY driver");
MODULE_AUTHOR("Giuseppe Cavallaro <peppe.cavallaro@st.com>");
MODULE_LICENSE("GPL");
