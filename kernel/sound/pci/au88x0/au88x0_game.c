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
 *  Manuel Jander.
 *
 *  Based on the work of:
 *  Vojtech Pavlik
 *  Raymond Ingles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Should you need to contact me, the author, you can do so either by
 * e-mail - mail your message to <vojtech@suse.cz>, or by paper mail:
 * Vojtech Pavlik, Ucitelska 1576, Prague 8, 182 00 Czech Republic
 *
 * Based 90% on Vojtech Pavlik pcigame driver.
 * Merged and modified by Manuel Jander, for the OpenVortex
 * driver. (email: mjander@embedded.cl).
 */

#include <linux/time.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <sound/core.h>
#include "au88x0.h"
#include <linux/gameport.h>

#if defined(CONFIG_GAMEPORT) || (defined(MODULE) && defined(CONFIG_GAMEPORT_MODULE))

#define VORTEX_GAME_DWAIT	20	/* 20 ms */

static unsigned char vortex_game_read(struct gameport *gameport)
{
	vortex_t *vortex = gameport_get_port_data(gameport);
	return hwread(vortex->mmio, VORTEX_GAME_LEGACY);
}

static void vortex_game_trigger(struct gameport *gameport)
{
	vortex_t *vortex = gameport_get_port_data(gameport);
	hwwrite(vortex->mmio, VORTEX_GAME_LEGACY, 0xff);
}

static int
vortex_game_cooked_read(struct gameport *gameport, int *axes, int *buttons)
{
	vortex_t *vortex = gameport_get_port_data(gameport);
	int i;

	*buttons = (~hwread(vortex->mmio, VORTEX_GAME_LEGACY) >> 4) & 0xf;

	for (i = 0; i < 4; i++) {
		axes[i] =
		    hwread(vortex->mmio, VORTEX_GAME_AXIS + (i * AXIS_SIZE));
		if (axes[i] == AXIS_RANGE)
			axes[i] = -1;
	}
	return 0;
}

static int vortex_game_open(struct gameport *gameport, int mode)
{
	vortex_t *vortex = gameport_get_port_data(gameport);

	switch (mode) {
	case GAMEPORT_MODE_COOKED:
		hwwrite(vortex->mmio, VORTEX_CTRL2,
			hwread(vortex->mmio,
			       VORTEX_CTRL2) | CTRL2_GAME_ADCMODE);
		msleep(VORTEX_GAME_DWAIT);
		return 0;
	case GAMEPORT_MODE_RAW:
		hwwrite(vortex->mmio, VORTEX_CTRL2,
			hwread(vortex->mmio,
			       VORTEX_CTRL2) & ~CTRL2_GAME_ADCMODE);
		return 0;
	default:
		return -1;
	}

	return 0;
}

static int __devinit vortex_gameport_register(vortex_t * vortex)
{
	struct gameport *gp;

	vortex->gameport = gp = gameport_allocate_port();
	if (!gp) {
		printk(KERN_ERR "vortex: cannot allocate memory for gameport\n");
		return -ENOMEM;
	};

	gameport_set_name(gp, "AU88x0 Gameport");
	gameport_set_phys(gp, "pci%s/gameport0", pci_name(vortex->pci_dev));
	gameport_set_dev_parent(gp, &vortex->pci_dev->dev);

	gp->read = vortex_game_read;
	gp->trigger = vortex_game_trigger;
	gp->cooked_read = vortex_game_cooked_read;
	gp->open = vortex_game_open;

	gameport_set_port_data(gp, vortex);
	gp->fuzz = 64;

	gameport_register_port(gp);

	return 0;
}

static void vortex_gameport_unregister(vortex_t * vortex)
{
	if (vortex->gameport) {
		gameport_unregister_port(vortex->gameport);
		vortex->gameport = NULL;
	}
}

#else
static inline int vortex_gameport_register(vortex_t * vortex) { return -ENOSYS; }
static inline void vortex_gameport_unregister(vortex_t * vortex) { }
#endif
