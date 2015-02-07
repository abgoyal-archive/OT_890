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
 * Operating system-dependent definitions that have to be defined
 * before any other header files are included.
 */

// HWM (HardWare Module) Definitions
// -----------------------

#include <asm/byteorder.h>

#ifdef __LITTLE_ENDIAN
#define LITTLE_ENDIAN
#else
#define BIG_ENDIAN
#endif

// this is set in the makefile
// #define PCI			/* only PCI adapters supported by this driver */
// #define MEM_MAPPED_IO	/* use memory mapped I/O */


#define USE_CAN_ADDR		/* DA and SA in MAC header are canonical. */

#define MB_OUTSIDE_SMC		/* SMT Mbufs outside of smc struct. */

// -----------------------


// SMT Definitions 
// -----------------------
#define SYNC	       		/* allow synchronous frames */

// #define SBA			/* Synchronous Bandwidth Allocator support */
				/* not available as free source */

#define ESS			/* SBA End Station Support */

#define	SMT_PANIC(smc, nr, msg)	printk(KERN_INFO "SMT PANIC: code: %d, msg: %s\n",nr,msg)


#ifdef DEBUG
#define printf(s,args...) printk(KERN_INFO s, ## args)
#endif

// #define HW_PTR	u_long
// -----------------------



// HWM and OS-specific buffer definitions
// -----------------------

// default number of receive buffers.
#define NUM_RECEIVE_BUFFERS		10

// default number of transmit buffers.
#define NUM_TRANSMIT_BUFFERS		10

// Number of SMT buffers (Mbufs).
#define NUM_SMT_BUF	4

// Number of TXDs for asynchronous transmit queue.
#define HWM_ASYNC_TXD_COUNT	(NUM_TRANSMIT_BUFFERS + NUM_SMT_BUF)

// Number of TXDs for synchronous transmit queue.
#define HWM_SYNC_TXD_COUNT	HWM_ASYNC_TXD_COUNT


// Number of RXDs for receive queue #1.
// Note: Workaround for ASIC Errata #7: One extra RXD is required.
#if (NUM_RECEIVE_BUFFERS > 100)
#define SMT_R1_RXD_COUNT	(1 + 100)
#else
#define SMT_R1_RXD_COUNT	(1 + NUM_RECEIVE_BUFFERS)
#endif

// Number of RXDs for receive queue #2.
#define SMT_R2_RXD_COUNT	0	// Not used.
// -----------------------



/*
 * OS-specific part of the transmit/receive descriptor structure (TXD/RXD).
 *
 * Note: The size of these structures must follow this rule:
 *
 *	sizeof(struct) + 2*sizeof(void*) == n * 16, n >= 1
 *
 * We use the dma_addr fields under Linux to keep track of the
 * DMA address of the packet data, for later pci_unmap_single. -DaveM
 */

struct s_txd_os {	// os-specific part of transmit descriptor
	struct sk_buff *skb;
	dma_addr_t dma_addr;
} ;

struct s_rxd_os {	// os-specific part of receive descriptor
	struct sk_buff *skb;
	dma_addr_t dma_addr;
} ;


/*
 * So we do not need to make too many modifications to the generic driver
 * parts, we take advantage of the AIX byte swapping macro interface.
 */

#define AIX_REVERSE(x)		((u32)le32_to_cpu((u32)(x)))
#define MDR_REVERSE(x)		((u32)le32_to_cpu((u32)(x)))
