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

/* Wrapper for DMA channel allocator that starts clocks etc */

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <asm/dma.h>
#include <hwregs/reg_map.h>
#include <hwregs/reg_rdwr.h>
#include <hwregs/marb_defs.h>
#include <hwregs/config_defs.h>
#include <hwregs/strmux_defs.h>
#include <linux/errno.h>
#include <asm/system.h>
#include <mach/arbiter.h>

static char used_dma_channels[MAX_DMA_CHANNELS];
static const char *used_dma_channels_users[MAX_DMA_CHANNELS];

static DEFINE_SPINLOCK(dma_lock);

int crisv32_request_dma(unsigned int dmanr, const char *device_id,
			unsigned options, unsigned int bandwidth,
			enum dma_owner owner)
{
	unsigned long flags;
	reg_config_rw_clk_ctrl clk_ctrl;
	reg_strmux_rw_cfg strmux_cfg;

	if (crisv32_arbiter_allocate_bandwidth(dmanr,
					       options & DMA_INT_MEM ?
					       INT_REGION : EXT_REGION,
					       bandwidth))
		return -ENOMEM;

	spin_lock_irqsave(&dma_lock, flags);

	if (used_dma_channels[dmanr]) {
		spin_unlock_irqrestore(&dma_lock, flags);
		if (options & DMA_VERBOSE_ON_ERROR) {
			printk(KERN_ERR "Failed to request DMA %i for %s, "
				"already allocated by %s\n",
				dmanr,
				device_id,
				used_dma_channels_users[dmanr]);
		}
		if (options & DMA_PANIC_ON_ERROR)
			panic("request_dma error!");
		spin_unlock_irqrestore(&dma_lock, flags);
		return -EBUSY;
	}
	clk_ctrl = REG_RD(config, regi_config, rw_clk_ctrl);
	strmux_cfg = REG_RD(strmux, regi_strmux, rw_cfg);

	switch (dmanr) {
	case 0:
	case 1:
		clk_ctrl.dma01_eth0 = 1;
		break;
	case 2:
	case 3:
		clk_ctrl.dma23 = 1;
		break;
	case 4:
	case 5:
		clk_ctrl.dma45 = 1;
		break;
	case 6:
	case 7:
		clk_ctrl.dma67 = 1;
		break;
	case 8:
	case 9:
		clk_ctrl.dma89_strcop = 1;
		break;
#if MAX_DMA_CHANNELS-1 != 9
#error Check dma.c
#endif
	default:
		spin_unlock_irqrestore(&dma_lock, flags);
		if (options & DMA_VERBOSE_ON_ERROR) {
			printk(KERN_ERR "Failed to request DMA %i for %s, "
				"only 0-%i valid)\n",
				dmanr, device_id, MAX_DMA_CHANNELS - 1);
		}

		if (options & DMA_PANIC_ON_ERROR)
			panic("request_dma error!");
		return -EINVAL;
	}

	switch (owner) {
	case dma_eth0:
		if (dmanr == 0)
			strmux_cfg.dma0 = regk_strmux_eth0;
		else if (dmanr == 1)
			strmux_cfg.dma1 = regk_strmux_eth0;
		else
			panic("Invalid DMA channel for eth0\n");
		break;
	case dma_eth1:
		if (dmanr == 6)
			strmux_cfg.dma6 = regk_strmux_eth1;
		else if (dmanr == 7)
			strmux_cfg.dma7 = regk_strmux_eth1;
		else
			panic("Invalid DMA channel for eth1\n");
		break;
	case dma_iop0:
		if (dmanr == 2)
			strmux_cfg.dma2 = regk_strmux_iop0;
		else if (dmanr == 3)
			strmux_cfg.dma3 = regk_strmux_iop0;
		else
			panic("Invalid DMA channel for iop0\n");
		break;
	case dma_iop1:
		if (dmanr == 4)
			strmux_cfg.dma4 = regk_strmux_iop1;
		else if (dmanr == 5)
			strmux_cfg.dma5 = regk_strmux_iop1;
		else
			panic("Invalid DMA channel for iop1\n");
		break;
	case dma_ser0:
		if (dmanr == 6)
			strmux_cfg.dma6 = regk_strmux_ser0;
		else if (dmanr == 7)
			strmux_cfg.dma7 = regk_strmux_ser0;
		else
			panic("Invalid DMA channel for ser0\n");
		break;
	case dma_ser1:
		if (dmanr == 4)
			strmux_cfg.dma4 = regk_strmux_ser1;
		else if (dmanr == 5)
			strmux_cfg.dma5 = regk_strmux_ser1;
		else
			panic("Invalid DMA channel for ser1\n");
		break;
	case dma_ser2:
		if (dmanr == 2)
			strmux_cfg.dma2 = regk_strmux_ser2;
		else if (dmanr == 3)
			strmux_cfg.dma3 = regk_strmux_ser2;
		else
			panic("Invalid DMA channel for ser2\n");
		break;
	case dma_ser3:
		if (dmanr == 8)
			strmux_cfg.dma8 = regk_strmux_ser3;
		else if (dmanr == 9)
			strmux_cfg.dma9 = regk_strmux_ser3;
		else
			panic("Invalid DMA channel for ser3\n");
		break;
	case dma_sser0:
		if (dmanr == 4)
			strmux_cfg.dma4 = regk_strmux_sser0;
		else if (dmanr == 5)
			strmux_cfg.dma5 = regk_strmux_sser0;
		else
			panic("Invalid DMA channel for sser0\n");
		break;
	case dma_sser1:
		if (dmanr == 6)
			strmux_cfg.dma6 = regk_strmux_sser1;
		else if (dmanr == 7)
			strmux_cfg.dma7 = regk_strmux_sser1;
		else
			panic("Invalid DMA channel for sser1\n");
		break;
	case dma_ata:
		if (dmanr == 2)
			strmux_cfg.dma2 = regk_strmux_ata;
		else if (dmanr == 3)
			strmux_cfg.dma3 = regk_strmux_ata;
		else
			panic("Invalid DMA channel for ata\n");
		break;
	case dma_strp:
		if (dmanr == 8)
			strmux_cfg.dma8 = regk_strmux_strcop;
		else if (dmanr == 9)
			strmux_cfg.dma9 = regk_strmux_strcop;
		else
			panic("Invalid DMA channel for strp\n");
		break;
	case dma_ext0:
		if (dmanr == 6)
			strmux_cfg.dma6 = regk_strmux_ext0;
		else
			panic("Invalid DMA channel for ext0\n");
		break;
	case dma_ext1:
		if (dmanr == 7)
			strmux_cfg.dma7 = regk_strmux_ext1;
		else
			panic("Invalid DMA channel for ext1\n");
		break;
	case dma_ext2:
		if (dmanr == 2)
			strmux_cfg.dma2 = regk_strmux_ext2;
		else if (dmanr == 8)
			strmux_cfg.dma8 = regk_strmux_ext2;
		else
			panic("Invalid DMA channel for ext2\n");
		break;
	case dma_ext3:
		if (dmanr == 3)
			strmux_cfg.dma3 = regk_strmux_ext3;
		else if (dmanr == 9)
			strmux_cfg.dma9 = regk_strmux_ext2;
		else
			panic("Invalid DMA channel for ext2\n");
		break;
	}

	used_dma_channels[dmanr] = 1;
	used_dma_channels_users[dmanr] = device_id;
	REG_WR(config, regi_config, rw_clk_ctrl, clk_ctrl);
	REG_WR(strmux, regi_strmux, rw_cfg, strmux_cfg);
	spin_unlock_irqrestore(&dma_lock, flags);
	return 0;
}

void crisv32_free_dma(unsigned int dmanr)
{
	spin_lock(&dma_lock);
	used_dma_channels[dmanr] = 0;
	spin_unlock(&dma_lock);
}
