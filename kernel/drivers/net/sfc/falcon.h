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
 * Copyright 2005-2006 Fen Systems Ltd.
 * Copyright 2006-2008 Solarflare Communications Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, incorporated herein by reference.
 */

#ifndef EFX_FALCON_H
#define EFX_FALCON_H

#include "net_driver.h"
#include "efx.h"

/*
 * Falcon hardware control
 */

enum falcon_revision {
	FALCON_REV_A0 = 0,
	FALCON_REV_A1 = 1,
	FALCON_REV_B0 = 2,
};

static inline int falcon_rev(struct efx_nic *efx)
{
	return efx->pci_dev->revision;
}

extern struct efx_nic_type falcon_a_nic_type;
extern struct efx_nic_type falcon_b_nic_type;

/**************************************************************************
 *
 * Externs
 *
 **************************************************************************
 */

/* TX data path */
extern int falcon_probe_tx(struct efx_tx_queue *tx_queue);
extern void falcon_init_tx(struct efx_tx_queue *tx_queue);
extern void falcon_fini_tx(struct efx_tx_queue *tx_queue);
extern void falcon_remove_tx(struct efx_tx_queue *tx_queue);
extern void falcon_push_buffers(struct efx_tx_queue *tx_queue);

/* RX data path */
extern int falcon_probe_rx(struct efx_rx_queue *rx_queue);
extern void falcon_init_rx(struct efx_rx_queue *rx_queue);
extern void falcon_fini_rx(struct efx_rx_queue *rx_queue);
extern void falcon_remove_rx(struct efx_rx_queue *rx_queue);
extern void falcon_notify_rx_desc(struct efx_rx_queue *rx_queue);

/* Event data path */
extern int falcon_probe_eventq(struct efx_channel *channel);
extern void falcon_init_eventq(struct efx_channel *channel);
extern void falcon_fini_eventq(struct efx_channel *channel);
extern void falcon_remove_eventq(struct efx_channel *channel);
extern int falcon_process_eventq(struct efx_channel *channel, int rx_quota);
extern void falcon_eventq_read_ack(struct efx_channel *channel);

/* Ports */
extern int falcon_probe_port(struct efx_nic *efx);
extern void falcon_remove_port(struct efx_nic *efx);

/* MAC/PHY */
extern int falcon_switch_mac(struct efx_nic *efx);
extern bool falcon_xaui_link_ok(struct efx_nic *efx);
extern int falcon_dma_stats(struct efx_nic *efx,
			    unsigned int done_offset);
extern void falcon_drain_tx_fifo(struct efx_nic *efx);
extern void falcon_deconfigure_mac_wrapper(struct efx_nic *efx);
extern void falcon_reconfigure_mac_wrapper(struct efx_nic *efx);

/* Interrupts and test events */
extern int falcon_init_interrupt(struct efx_nic *efx);
extern void falcon_enable_interrupts(struct efx_nic *efx);
extern void falcon_generate_test_event(struct efx_channel *channel,
				       unsigned int magic);
extern void falcon_sim_phy_event(struct efx_nic *efx);
extern void falcon_generate_interrupt(struct efx_nic *efx);
extern void falcon_set_int_moderation(struct efx_channel *channel);
extern void falcon_disable_interrupts(struct efx_nic *efx);
extern void falcon_fini_interrupt(struct efx_nic *efx);

/* Global Resources */
extern int falcon_probe_nic(struct efx_nic *efx);
extern int falcon_probe_resources(struct efx_nic *efx);
extern int falcon_init_nic(struct efx_nic *efx);
extern int falcon_flush_queues(struct efx_nic *efx);
extern int falcon_reset_hw(struct efx_nic *efx, enum reset_type method);
extern void falcon_remove_resources(struct efx_nic *efx);
extern void falcon_remove_nic(struct efx_nic *efx);
extern void falcon_update_nic_stats(struct efx_nic *efx);
extern void falcon_set_multicast_hash(struct efx_nic *efx);
extern int falcon_reset_xaui(struct efx_nic *efx);

/* Tests */
struct falcon_nvconfig;
extern int falcon_read_nvram(struct efx_nic *efx,
			     struct falcon_nvconfig *nvconfig);
extern int falcon_test_registers(struct efx_nic *efx);

/**************************************************************************
 *
 * Falcon MAC stats
 *
 **************************************************************************
 */

#define FALCON_STAT_OFFSET(falcon_stat) EFX_VAL(falcon_stat, offset)
#define FALCON_STAT_WIDTH(falcon_stat) EFX_VAL(falcon_stat, WIDTH)

/* Retrieve statistic from statistics block */
#define FALCON_STAT(efx, falcon_stat, efx_stat) do {		\
	if (FALCON_STAT_WIDTH(falcon_stat) == 16)		\
		(efx)->mac_stats.efx_stat += le16_to_cpu(	\
			*((__force __le16 *)				\
			  (efx->stats_buffer.addr +		\
			   FALCON_STAT_OFFSET(falcon_stat))));	\
	else if (FALCON_STAT_WIDTH(falcon_stat) == 32)		\
		(efx)->mac_stats.efx_stat += le32_to_cpu(	\
			*((__force __le32 *)				\
			  (efx->stats_buffer.addr +		\
			   FALCON_STAT_OFFSET(falcon_stat))));	\
	else							\
		(efx)->mac_stats.efx_stat += le64_to_cpu(	\
			*((__force __le64 *)				\
			  (efx->stats_buffer.addr +		\
			   FALCON_STAT_OFFSET(falcon_stat))));	\
	} while (0)

#define FALCON_MAC_STATS_SIZE 0x100

#define MAC_DATA_LBN 0
#define MAC_DATA_WIDTH 32

extern void falcon_generate_event(struct efx_channel *channel,
				  efx_qword_t *event);

#endif /* EFX_FALCON_H */
