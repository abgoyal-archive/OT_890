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

/******************************************************************************
 *
 *	(C)Copyright 1998,1999 SysKonnect,
 *	a business unit of Schneider & Koch & Co. Datensysteme GmbH.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	The information in this file is provided "AS IS" without warranty.
 *
 ******************************************************************************/

/*
 *	Operating system specific definitions for driver and
 *	hardware module.
 */

#ifndef	TARGETOS_H
#define TARGETOS_H


//-------- those should go into include/linux/pci.h
#define PCI_VENDOR_ID_SK		0x1148
#define PCI_DEVICE_ID_SK_FP		0x4000
//--------



//-------- those should go into include/linux/if_fddi.h
#define FDDI_MAC_HDR_LEN 13

#define FDDI_RII	0x01 /* routing information bit */
#define FDDI_RCF_DIR_BIT 0x80
#define FDDI_RCF_LEN_MASK 0x1f
#define FDDI_RCF_BROADCAST 0x8000
#define FDDI_RCF_LIMITED_BROADCAST 0xA000
#define FDDI_RCF_FRAME2K 0x20
#define FDDI_RCF_FRAME4K 0x30
//--------


#undef ADDR

#include <asm/io.h>
#include <linux/netdevice.h>
#include <linux/fddidevice.h>
#include <linux/skbuff.h>
#include <linux/pci.h>
#include <linux/init.h>

// is redefined by linux, but we need our definition
#undef ADDR
#ifdef MEM_MAPPED_IO
#define	ADDR(a) (smc->hw.iop+(a))
#else
#define	ADDR(a) (((a)>>7) ? (outp(smc->hw.iop+B0_RAP,(a)>>7), (smc->hw.iop+( ((a)&0x7F) | ((a)>>7 ? 0x80:0)) )) : (smc->hw.iop+(((a)&0x7F)|((a)>>7 ? 0x80:0))))
#endif

#include "h/hwmtm.h"

#define TRUE  1
#define FALSE 0

// HWM Definitions
// -----------------------
#define FDDI_TRACE(string, arg1, arg2, arg3)	// Performance analysis.
#ifdef PCI
#define NDD_TRACE(string, arg1, arg2, arg3)	// Performance analysis.
#endif	// PCI
#define SMT_PAGESIZE	PAGE_SIZE	// Size of a memory page (power of 2).
// -----------------------


// SMT Definitions
// -----------------------
#define	TICKS_PER_SECOND	HZ
#define SMC_VERSION    		1
// -----------------------


// OS-Driver Definitions
// -----------------------
#define NO_ADDRESS 0xffe0	/* No Device (I/O) Address */
#define SKFP_MAX_NUM_BOARDS 8	/* maximum number of PCI boards */

#define SK_BUS_TYPE_PCI		0
#define SK_BUS_TYPE_EISA	1

#define FP_IO_LEN		256	/* length of IO area used */

#define u8	unsigned char
#define u16	unsigned short
#define u32	unsigned int

#define MAX_TX_QUEUE_LEN	20 // number of packets queued by driver
#define MAX_FRAME_SIZE		4550

#define	RX_LOW_WATERMARK	NUM_RECEIVE_BUFFERS  / 2
#define TX_LOW_WATERMARK	NUM_TRANSMIT_BUFFERS - 2

/*
** Include the IOCTL stuff
*/
#include <linux/sockios.h>

#define	SKFPIOCTL	SIOCDEVPRIVATE

struct s_skfp_ioctl {
	unsigned short cmd;                /* Command to run */
	unsigned short len;                /* Length of the data buffer */
	unsigned char __user *data;        /* Pointer to the data buffer */
};

/* 
** Recognised ioctl commands for the driver 
*/
#define SKFP_GET_STATS		0x05 /* Get the driver statistics */
#define SKFP_CLR_STATS		0x06 /* Zero out the driver statistics */

// The per-adapter driver structure
struct s_smt_os {
	struct net_device *dev;
	struct net_device *next_module;
	u32	bus_type;		/* bus type (0 == PCI, 1 == EISA) */
	struct pci_dev 	pdev;		/* PCI device structure */
	
	unsigned long base_addr;
	unsigned char factory_mac_addr[8];
	ulong	SharedMemSize;
	ulong	SharedMemHeap;
	void*	SharedMemAddr;
	dma_addr_t SharedMemDMA;

	ulong	QueueSkb;
	struct	sk_buff_head SendSkbQueue;

	ulong	MaxFrameSize;
	u8	ResetRequested;

	// MAC statistics structure
	struct fddi_statistics MacStat;

	// receive into this local buffer if no skb available
	// data will be not valid, because multiple RxDs can
	// point here at the same time, it must be at least
	// MAX_FRAME_SIZE bytes in size
	unsigned char *LocalRxBuffer;
	dma_addr_t LocalRxBufferDMA;
	
	// Version (required by SMT module).
	u_long smc_version ;

	// Required by Hardware Module (HWM).
	struct hw_modul hwm ;
	
	// For SMP-savety
	spinlock_t DriverLock;
	
};

typedef struct s_smt_os skfddi_priv;

#endif	 // _TARGETOS_
