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
** -----------------------------------------------------------------------------
**
**  Perle Specialix driver for Linux
**  Ported from existing RIO Driver for SCO sources.
 *
 *  (C) 1990 - 2000 Specialix International Ltd., Byfleet, Surrey, UK.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
**	Module		: riospace.h
**	SID		: 1.2
**	Last Modified	: 11/6/98 11:34:13
**	Retrieved	: 11/6/98 11:34:22
**
**  ident @(#)riospace.h	1.2
**
** -----------------------------------------------------------------------------
*/

#ifndef __rio_riospace_h__
#define __rio_riospace_h__

#define	RIO_LOCATOR_LEN	16
#define	MAX_RIO_BOARDS	4

/*
** DONT change this file. At all. Unless you can rebuild the entire
** device driver, which you probably can't, then the rest of the
** driver won't see any changes you make here. So don't make any.
** In particular, it won't be able to see changes to RIO_SLOTS
*/

struct Conf {
	char Locator[24];
	unsigned int StartupTime;
	unsigned int SlowCook;
	unsigned int IntrPollTime;
	unsigned int BreakInterval;
	unsigned int Timer;
	unsigned int RtaLoadBase;
	unsigned int HostLoadBase;
	unsigned int XpHz;
	unsigned int XpCps;
	char *XpOn;
	char *XpOff;
	unsigned int MaxXpCps;
	unsigned int MinXpCps;
	unsigned int SpinCmds;
	unsigned int FirstAddr;
	unsigned int LastAddr;
	unsigned int BufferSize;
	unsigned int LowWater;
	unsigned int LineLength;
	unsigned int CmdTime;
};

/*
**	Board types - these MUST correspond to product codes!
*/
#define	RIO_EMPTY	0x0
#define	RIO_EISA	0x3
#define	RIO_RTA_16	0x9
#define	RIO_AT		0xA
#define	RIO_MCA		0xB
#define	RIO_PCI		0xD
#define	RIO_RTA		0xE

/*
**	Board data structure. This is used for configuration info
*/
struct Brd {
	unsigned char Type;	/* RIO_EISA, RIO_MCA, RIO_AT, RIO_EMPTY... */
	unsigned char Ivec;	/* POLLED or ivec number */
	unsigned char Mode;	/* Control stuff, see below */
};

struct Board {
	char Locator[RIO_LOCATOR_LEN];
	int NumSlots;
	struct Brd Boards[MAX_RIO_BOARDS];
};

#define	BOOT_FROM_LINK		0x00
#define	BOOT_FROM_RAM		0x01
#define	EXTERNAL_BUS_OFF	0x00
#define	EXTERNAL_BUS_ON		0x02
#define	INTERRUPT_DISABLE	0x00
#define	INTERRUPT_ENABLE	0x04
#define	BYTE_OPERATION		0x00
#define	WORD_OPERATION		0x08
#define	POLLED			INTERRUPT_DISABLE
#define	IRQ_15			(0x00 | INTERRUPT_ENABLE)
#define	IRQ_12			(0x10 | INTERRUPT_ENABLE)
#define	IRQ_11			(0x20 | INTERRUPT_ENABLE)
#define	IRQ_9			(0x30 | INTERRUPT_ENABLE)
#define	SLOW_LINKS		0x00
#define	FAST_LINKS		0x40
#define	SLOW_AT_BUS		0x00
#define	FAST_AT_BUS		0x80
#define	SLOW_PCI_TP		0x00
#define	FAST_PCI_TP		0x80
/*
**	Debug levels
*/
#define	DBG_NONE	0x00000000

#define	DBG_INIT	0x00000001
#define	DBG_OPEN	0x00000002
#define	DBG_CLOSE	0x00000004
#define	DBG_IOCTL	0x00000008

#define	DBG_READ	0x00000010
#define	DBG_WRITE	0x00000020
#define	DBG_INTR	0x00000040
#define	DBG_PROC	0x00000080

#define	DBG_PARAM	0x00000100
#define	DBG_CMD		0x00000200
#define	DBG_XPRINT	0x00000400
#define	DBG_POLL	0x00000800

#define	DBG_DAEMON	0x00001000
#define	DBG_FAIL	0x00002000
#define DBG_MODEM	0x00004000
#define	DBG_LIST	0x00008000

#define	DBG_ROUTE	0x00010000
#define DBG_UTIL        0x00020000
#define DBG_BOOT	0x00040000
#define DBG_BUFFER	0x00080000

#define	DBG_MON		0x00100000
#define DBG_SPECIAL     0x00200000
#define	DBG_VPIX	0x00400000
#define	DBG_FLUSH	0x00800000

#define	DBG_QENABLE	0x01000000

#define	DBG_ALWAYS	0x80000000

#endif				/* __rio_riospace_h__ */
