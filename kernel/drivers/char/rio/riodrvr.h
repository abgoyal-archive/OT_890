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
**	Module		: riodrvr.h
**	SID		: 1.3
**	Last Modified	: 11/6/98 09:22:46
**	Retrieved	: 11/6/98 09:22:46
**
**  ident @(#)riodrvr.h	1.3
**
** -----------------------------------------------------------------------------
*/

#ifndef __riodrvr_h
#define __riodrvr_h

#include <asm/param.h>		/* for HZ */

#define MEMDUMP_SIZE	32
#define	MOD_DISABLE	(RIO_NOREAD|RIO_NOWRITE|RIO_NOXPRINT)


struct rio_info {
	int mode;		/* Intr or polled, word/byte */
	spinlock_t RIOIntrSem;	/* Interrupt thread sem */
	int current_chan;	/* current channel */
	int RIOFailed;		/* Not initialised ? */
	int RIOInstallAttempts;	/* no. of rio-install() calls */
	int RIOLastPCISearch;	/* status of last search */
	int RIONumHosts;	/* Number of RIO Hosts */
	struct Host *RIOHosts;	/* RIO Host values */
	struct Port **RIOPortp;	/* RIO port values */
/*
** 02.03.1999 ARG - ESIL 0820 fix
** We no longer use RIOBootMode
**
	int			RIOBootMode;		* RIO boot mode *
**
*/
	int RIOPrintDisabled;	/* RIO printing disabled ? */
	int RIOPrintLogState;	/* RIO printing state ? */
	int RIOPolling;		/* Polling ? */
/*
** 09.12.1998 ARG - ESIL 0776 part fix
** The 'RIO_QUICK_CHECK' ioctl was using RIOHalted.
** The fix for this ESIL introduces another member (RIORtaDisCons) here to be
** updated in RIOConCon() - to keep track of RTA connections/disconnections.
** 'RIO_QUICK_CHECK' now returns the value of RIORtaDisCons.
*/
	int RIOHalted;		/* halted ? */
	int RIORtaDisCons;	/* RTA connections/disconnections */
	unsigned int RIOReadCheck;	/* Rio read check */
	unsigned int RIONoMessage;	/* To display message or not */
	unsigned int RIONumBootPkts;	/* how many packets for an RTA */
	unsigned int RIOBootCount;	/* size of RTA code */
	unsigned int RIOBooting;	/* count of outstanding boots */
	unsigned int RIOSystemUp;	/* Booted ?? */
	unsigned int RIOCounting;	/* for counting interrupts */
	unsigned int RIOIntCount;	/* # of intr since last check */
	unsigned int RIOTxCount;	/* number of xmit intrs  */
	unsigned int RIORxCount;	/* number of rx intrs */
	unsigned int RIORupCount;	/* number of rup intrs */
	int RIXTimer;
	int RIOBufferSize;	/* Buffersize */
	int RIOBufferMask;	/* Buffersize */

	int RIOFirstMajor;	/* First host card's major no */

	unsigned int RIOLastPortsMapped;	/* highest port number known */
	unsigned int RIOFirstPortsMapped;	/* lowest port number known */

	unsigned int RIOLastPortsBooted;	/* highest port number running */
	unsigned int RIOFirstPortsBooted;	/* lowest port number running */

	unsigned int RIOLastPortsOpened;	/* highest port number running */
	unsigned int RIOFirstPortsOpened;	/* lowest port number running */

	/* Flag to say that the topology information has been changed. */
	unsigned int RIOQuickCheck;
	unsigned int CdRegister;	/* ??? */
	int RIOSignalProcess;	/* Signalling process */
	int rio_debug;		/* To debug ... */
	int RIODebugWait;	/* For what ??? */
	int tpri;		/* Thread prio */
	int tid;		/* Thread id */
	unsigned int _RIO_Polled;	/* Counter for polling */
	unsigned int _RIO_Interrupted;	/* Counter for interrupt */
	int intr_tid;		/* iointset return value */
	int TxEnSem;		/* TxEnable Semaphore */


	struct Error RIOError;	/* to Identify what went wrong */
	struct Conf RIOConf;	/* Configuration ??? */
	struct ttystatics channel[RIO_PORTS];	/* channel information */
	char RIOBootPackets[1 + (SIXTY_FOUR_K / RTA_BOOT_DATA_SIZE)]
	    [RTA_BOOT_DATA_SIZE];
	struct Map RIOConnectTable[TOTAL_MAP_ENTRIES];
	struct Map RIOSavedTable[TOTAL_MAP_ENTRIES];

	/* RTA to host binding table for master/slave operation */
	unsigned long RIOBindTab[MAX_RTA_BINDINGS];
	/* RTA memory dump variable */
	unsigned char RIOMemDump[MEMDUMP_SIZE];
	struct ModuleInfo RIOModuleTypes[MAX_MODULE_TYPES];

};


#ifdef linux
#define debug(x)        printk x
#else
#define debug(x)	kkprintf x
#endif



#define RIO_RESET_INT	0x7d80

#endif				/* __riodrvr.h */
