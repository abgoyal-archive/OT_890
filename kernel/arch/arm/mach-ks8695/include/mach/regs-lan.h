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
 * arch/arm/mach-ks8695/include/mach/regs-lan.h
 *
 * Copyright (C) 2006 Andrew Victor
 *
 * KS8695 - LAN Registers and bit definitions.
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef KS8695_LAN_H
#define KS8695_LAN_H

#define KS8695_LAN_OFFSET	(0xF0000 + 0x8000)
#define KS8695_LAN_VA		(KS8695_IO_VA + KS8695_LAN_OFFSET)
#define KS8695_LAN_PA		(KS8695_IO_PA + KS8695_LAN_OFFSET)


/*
 * LAN registers
 */
#define KS8695_LMDTXC		(0x00)		/* DMA Transmit Control */
#define KS8695_LMDRXC		(0x04)		/* DMA Receive Control */
#define KS8695_LMDTSC		(0x08)		/* DMA Transmit Start Command */
#define KS8695_LMDRSC		(0x0c)		/* DMA Receive Start Command */
#define KS8695_LTDLB		(0x10)		/* Transmit Descriptor List Base Address */
#define KS8695_LRDLB		(0x14)		/* Receive Descriptor List Base Address */
#define KS8695_LMAL		(0x18)		/* MAC Station Address Low */
#define KS8695_LMAH		(0x1c)		/* MAC Station Address High */
#define KS8695_LMAAL(n)		(0x80 + ((n)*8))	/* MAC Additional Station Address (0..15) Low */
#define KS8695_LMAAH(n)		(0x84 + ((n)*8))	/* MAC Additional Station Address (0..15) High */


/* DMA Transmit Control Register */
#define LMDTXC_LMTRST		(1    << 31)	/* Soft Reset */
#define LMDTXC_LMTBS		(0x3f << 24)	/* Transmit Burst Size */
#define LMDTXC_LMTUCG		(1    << 18)	/* Transmit UDP Checksum Generate */
#define LMDTXC_LMTTCG		(1    << 17)	/* Transmit TCP Checksum Generate */
#define LMDTXC_LMTICG		(1    << 16)	/* Transmit IP Checksum Generate */
#define LMDTXC_LMTFCE		(1    <<  9)	/* Transmit Flow Control Enable */
#define LMDTXC_LMTLB		(1    <<  8)	/* Loopback mode */
#define LMDTXC_LMTEP		(1    <<  2)	/* Transmit Enable Padding */
#define LMDTXC_LMTAC		(1    <<  1)	/* Transmit Add CRC */
#define LMDTXC_LMTE		(1    <<  0)	/* TX Enable */

/* DMA Receive Control Register */
#define LMDRXC_LMRBS		(0x3f << 24)	/* Receive Burst Size */
#define LMDRXC_LMRUCC		(1    << 18)	/* Receive UDP Checksum check */
#define LMDRXC_LMRTCG		(1    << 17)	/* Receive TCP Checksum check */
#define LMDRXC_LMRICG		(1    << 16)	/* Receive IP Checksum check */
#define LMDRXC_LMRFCE		(1    <<  9)	/* Receive Flow Control Enable */
#define LMDRXC_LMRB		(1    <<  6)	/* Receive Broadcast */
#define LMDRXC_LMRM		(1    <<  5)	/* Receive Multicast */
#define LMDRXC_LMRU		(1    <<  4)	/* Receive Unicast */
#define LMDRXC_LMRERR		(1    <<  3)	/* Receive Error Frame */
#define LMDRXC_LMRA		(1    <<  2)	/* Receive All */
#define LMDRXC_LMRE		(1    <<  1)	/* RX Enable */

/* Additional Station Address High */
#define LMAAH_E			(1    << 31)	/* Address Enabled */


#endif
