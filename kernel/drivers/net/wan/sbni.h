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

/* sbni.h:  definitions for a Granch SBNI12 driver, version 5.0.0
 * Written 2001 Denis I.Timofeev (timofeev@granch.ru)
 * This file is distributed under the GNU GPL
 */

#ifndef SBNI_H
#define SBNI_H

#ifdef SBNI_DEBUG
#define DP( A ) A
#else
#define DP( A )
#endif


/* We don't have official vendor id yet... */
#define SBNI_PCI_VENDOR 	0x55 
#define SBNI_PCI_DEVICE 	0x9f

#define ISA_MODE 0x00
#define PCI_MODE 0x01

#define	SBNI_IO_EXTENT	4

enum sbni_reg {
	CSR0 = 0,
	CSR1 = 1,
	DAT  = 2
};

/* CSR0 mapping */
enum {
	BU_EMP = 0x02,
	RC_CHK = 0x04,
	CT_ZER = 0x08,
	TR_REQ = 0x10,
	TR_RDY = 0x20,
	EN_INT = 0x40,
	RC_RDY = 0x80
};


/* CSR1 mapping */
#define PR_RES 0x80

struct sbni_csr1 {
#ifdef __LITTLE_ENDIAN_BITFIELD
	u8 rxl	: 5;
	u8 rate	: 2;
	u8 	: 1;
#else
	u8 	: 1;
	u8 rate	: 2;
	u8 rxl	: 5;
#endif
};

/* fields in frame header */
#define FRAME_ACK_MASK  (unsigned short)0x7000
#define FRAME_LEN_MASK  (unsigned short)0x03FF
#define FRAME_FIRST     (unsigned short)0x8000
#define FRAME_RETRY     (unsigned short)0x0800

#define FRAME_SENT_BAD  (unsigned short)0x4000
#define FRAME_SENT_OK   (unsigned short)0x3000


/* state flags */
enum {
	FL_WAIT_ACK    = 0x01,
	FL_NEED_RESEND = 0x02,
	FL_PREV_OK     = 0x04,
	FL_SLOW_MODE   = 0x08,
	FL_SECONDARY   = 0x10,
#ifdef CONFIG_SBNI_MULTILINE
	FL_SLAVE       = 0x20,
#endif
	FL_LINE_DOWN   = 0x40
};


enum {
	DEFAULT_IOBASEADDR = 0x210,
	DEFAULT_INTERRUPTNUMBER = 5,
	DEFAULT_RATE = 0,
	DEFAULT_FRAME_LEN = 1012
};

#define DEF_RXL_DELTA	-1
#define DEF_RXL		0xf

#define SBNI_SIG 0x5a

#define	SBNI_MIN_LEN	60	/* Shortest Ethernet frame without FCS */
#define SBNI_MAX_FRAME	1023
#define ETHER_MAX_LEN	1518

#define SBNI_TIMEOUT	(HZ/10)

#define TR_ERROR_COUNT	32
#define CHANGE_LEVEL_START_TICKS 4

#define SBNI_MAX_NUM_CARDS	16

/* internal SBNI-specific statistics */
struct sbni_in_stats {
	u32	all_rx_number;
	u32	bad_rx_number;
	u32	timeout_number;
	u32	all_tx_number;
	u32	resend_tx_number;
};

/* SBNI ioctl params */
#define SIOCDEVGETINSTATS 	SIOCDEVPRIVATE
#define SIOCDEVRESINSTATS 	SIOCDEVPRIVATE+1
#define SIOCDEVGHWSTATE   	SIOCDEVPRIVATE+2
#define SIOCDEVSHWSTATE   	SIOCDEVPRIVATE+3
#define SIOCDEVENSLAVE  	SIOCDEVPRIVATE+4
#define SIOCDEVEMANSIPATE  	SIOCDEVPRIVATE+5


/* data packet for SIOCDEVGHWSTATE/SIOCDEVSHWSTATE ioctl requests */
struct sbni_flags {
	u32	rxl		: 4;
	u32	rate		: 2;
	u32	fixed_rxl	: 1;
	u32	slow_mode	: 1;
	u32	mac_addr	: 24;
};

/*
 * CRC-32 stuff
 */
#define CRC32(c,crc) (crc32tab[((size_t)(crc) ^ (c)) & 0xff] ^ (((crc) >> 8) & 0x00FFFFFF))
      /* CRC generator 0xEDB88320 */
      /* CRC remainder 0x2144DF1C */
      /* CRC initial value 0x00000000 */
#define CRC32_REMAINDER 0x2144DF1C
#define CRC32_INITIAL 0x00000000

#ifndef __initdata
#define __initdata
#endif

#endif

