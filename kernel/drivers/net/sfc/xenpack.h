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

/****************************************************************************
 * Driver for Solarflare Solarstorm network controllers and boards
 * Copyright 2006 Solarflare Communications Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, incorporated herein by reference.
 */

#ifndef EFX_XENPACK_H
#define EFX_XENPACK_H

/* Exported functions from Xenpack standard PHY control */

#include "mdio_10g.h"

/****************************************************************************/
/* XENPACK MDIO register extensions */
#define MDIO_XP_LASI_RX_CTRL	(0x9000)
#define MDIO_XP_LASI_TX_CTRL	(0x9001)
#define MDIO_XP_LASI_CTRL	(0x9002)
#define MDIO_XP_LASI_RX_STAT	(0x9003)
#define MDIO_XP_LASI_TX_STAT	(0x9004)
#define MDIO_XP_LASI_STAT	(0x9005)

/* Control/Status bits */
#define XP_LASI_LS_ALARM	(1 << 0)
#define XP_LASI_TX_ALARM	(1 << 1)
#define XP_LASI_RX_ALARM	(1 << 2)
/* These two are Quake vendor extensions to the standard XENPACK defines */
#define XP_LASI_LS_INTB		(1 << 3)
#define XP_LASI_TEST		(1 << 7)

/* Enable LASI interrupts for PHY */
static inline void xenpack_enable_lasi_irqs(struct efx_nic *efx)
{
	int reg;
	int phy_id = efx->mii.phy_id;
	/* Read to clear LASI status register */
	reg = mdio_clause45_read(efx, phy_id, MDIO_MMD_PMAPMD,
				 MDIO_XP_LASI_STAT);

	mdio_clause45_write(efx, phy_id, MDIO_MMD_PMAPMD,
			    MDIO_XP_LASI_CTRL, XP_LASI_LS_ALARM);
}

/* Read the LASI interrupt status to clear the interrupt. */
static inline int xenpack_clear_lasi_irqs(struct efx_nic *efx)
{
	/* Read to clear link status alarm */
	return mdio_clause45_read(efx, efx->mii.phy_id,
				  MDIO_MMD_PMAPMD, MDIO_XP_LASI_STAT);
}

/* Turn off LASI interrupts */
static inline void xenpack_disable_lasi_irqs(struct efx_nic *efx)
{
	mdio_clause45_write(efx, efx->mii.phy_id, MDIO_MMD_PMAPMD,
			    MDIO_XP_LASI_CTRL, 0);
}

#endif /* EFX_XENPACK_H */
