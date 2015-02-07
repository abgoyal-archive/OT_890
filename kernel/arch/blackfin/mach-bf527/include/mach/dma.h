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

/* mach/dma.h - arch-specific DMA defines
 *
 * Copyright 2004-2008 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef _MACH_DMA_H_
#define _MACH_DMA_H_

#define MAX_DMA_CHANNELS 16

#define CH_PPI 			0	/* PPI receive/transmit or NFC */
#define CH_EMAC_RX 		1	/* Ethernet MAC receive or HOSTDP */
#define CH_EMAC_HOSTDP 		1	/* Ethernet MAC receive or HOSTDP */
#define CH_EMAC_TX 		2	/* Ethernet MAC transmit or NFC */
#define CH_SPORT0_RX 		3	/* SPORT0 receive */
#define CH_SPORT0_TX 		4	/* SPORT0 transmit */
#define CH_SPORT1_RX 		5	/* SPORT1 receive */
#define CH_SPORT1_TX 		6	/* SPORT1 transmit */
#define CH_SPI 			7	/* SPI transmit/receive */
#define CH_UART0_RX 		8	/* UART0 receive */
#define CH_UART0_TX 		9	/* UART0 transmit */
#define CH_UART1_RX 		10	/* UART1 receive */
#define CH_UART1_TX 		11	/* UART1 transmit */

#define CH_MEM_STREAM0_DEST	12	/* TX */
#define CH_MEM_STREAM0_SRC  	13	/* RX */
#define CH_MEM_STREAM1_DEST	14	/* TX */
#define CH_MEM_STREAM1_SRC 	15	/* RX */

#if defined(CONFIG_BF527_NAND_D_PORTF)
#define CH_NFC			CH_PPI	/* PPI receive/transmit or NFC */
#elif defined(CONFIG_BF527_NAND_D_PORTH)
#define CH_NFC			CH_EMAC_TX /* PPI receive/transmit or NFC */
#endif

#endif
