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

/* drivers/atm/eni.h - Efficient Networks ENI155P device driver declarations */
 
/* Written 1995-2000 by Werner Almesberger, EPFL LRC/ICA */
 
 
#ifndef DRIVER_ATM_ENI_H
#define DRIVER_ATM_ENI_H

#include <linux/atm.h>
#include <linux/atmdev.h>
#include <linux/sonet.h>
#include <linux/skbuff.h>
#include <linux/time.h>
#include <linux/pci.h>
#include <linux/spinlock.h>
#include <asm/atomic.h>

#include "midway.h"


#define DEV_LABEL	"eni"

#define UBR_BUFFER	(128*1024)	/* UBR buffer size */

#define RX_DMA_BUF	  8		/* burst and skip a few things */
#define TX_DMA_BUF	100		/* should be enough for 64 kB */

#define DEFAULT_RX_MULT	300		/* max_sdu*3 */
#define DEFAULT_TX_MULT	300		/* max_sdu*3 */

#define ENI_ZEROES_SIZE	  4		/* need that many DMA-able zero bytes */


struct eni_free {
	void __iomem *start;		/* counting in bytes */
	int order;
};

struct eni_tx {
	void __iomem *send;		/* base, 0 if unused */
	int prescaler;			/* shaping prescaler */
	int resolution;			/* shaping divider */
	unsigned long tx_pos;		/* current TX write position */
	unsigned long words;		/* size of TX queue */
	int index;			/* TX channel number */
	int reserved;			/* reserved peak cell rate */
	int shaping;			/* shaped peak cell rate */
	struct sk_buff_head backlog;	/* queue of waiting TX buffers */
};

struct eni_vcc {
	int (*rx)(struct atm_vcc *vcc);	/* RX function, NULL if none */
	void __iomem *recv;		/* receive buffer */
	unsigned long words;		/* its size in words */
	unsigned long descr;		/* next descriptor (RX) */
	unsigned long rx_pos;		/* current RX descriptor pos */
	struct eni_tx *tx;		/* TXer, NULL if none */
	int rxing;			/* number of pending PDUs */
	int servicing;			/* number of waiting VCs (0 or 1) */
	int txing;			/* number of pending TX bytes */
	ktime_t timestamp;		/* for RX timing */
	struct atm_vcc *next;		/* next pending RX */
	struct sk_buff *last;		/* last PDU being DMAed (used to carry
					   discard information) */
};

struct eni_dev {
	/*-------------------------------- spinlock */
	spinlock_t lock;		/* sync with interrupt */
	struct tasklet_struct task;	/* tasklet for interrupt work */
	u32 events;			/* pending events */
	/*-------------------------------- base pointers into Midway address
					   space */
	void __iomem *phy;		/* PHY interface chip registers */
	void __iomem *reg;		/* register base */
	void __iomem *ram;		/* RAM base */
	void __iomem *vci;		/* VCI table */
	void __iomem *rx_dma;		/* RX DMA queue */
	void __iomem *tx_dma;		/* TX DMA queue */
	void __iomem *service;		/* service list */
	/*-------------------------------- TX part */
	struct eni_tx tx[NR_CHAN];	/* TX channels */
	struct eni_tx *ubr;		/* UBR channel */
	struct sk_buff_head tx_queue;	/* PDUs currently being TX DMAed*/
	wait_queue_head_t tx_wait;	/* for close */
	int tx_bw;			/* remaining bandwidth */
	u32 dma[TX_DMA_BUF*2];		/* DMA request scratch area */
	int tx_mult;			/* buffer size multiplier (percent) */
	/*-------------------------------- RX part */
	u32 serv_read;			/* host service read index */
	struct atm_vcc *fast,*last_fast;/* queues of VCCs with pending PDUs */
	struct atm_vcc *slow,*last_slow;
	struct atm_vcc **rx_map;	/* for fast lookups */
	struct sk_buff_head rx_queue;	/* PDUs currently being RX-DMAed */
	wait_queue_head_t rx_wait;	/* for close */
	int rx_mult;			/* buffer size multiplier (percent) */
	/*-------------------------------- statistics */
	unsigned long lost;		/* number of lost cells (RX) */
	/*-------------------------------- memory management */
	unsigned long base_diff;	/* virtual-real base address */
	int free_len;			/* free list length */
	struct eni_free *free_list;	/* free list */
	int free_list_size;		/* maximum size of free list */
	/*-------------------------------- ENI links */
	struct atm_dev *more;		/* other ENI devices */
	/*-------------------------------- general information */
	int mem;			/* RAM on board (in bytes) */
	int asic;			/* PCI interface type, 0 for FPGA */
	unsigned int irq;		/* IRQ */
	struct pci_dev *pci_dev;	/* PCI stuff */
};


#define ENI_DEV(d) ((struct eni_dev *) (d)->dev_data)
#define ENI_VCC(d) ((struct eni_vcc *) (d)->dev_data)


struct eni_skb_prv {
	struct atm_skb_data _;		/* reserved */
	unsigned long pos;		/* position of next descriptor */
	int size;			/* PDU size in reassembly buffer */
	dma_addr_t paddr;		/* DMA handle */
};

#define ENI_PRV_SIZE(skb) (((struct eni_skb_prv *) (skb)->cb)->size)
#define ENI_PRV_POS(skb) (((struct eni_skb_prv *) (skb)->cb)->pos)
#define ENI_PRV_PADDR(skb) (((struct eni_skb_prv *) (skb)->cb)->paddr)

#endif
