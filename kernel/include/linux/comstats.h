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

/*****************************************************************************/

/*
 *	comstats.h  -- Serial Port Stats.
 *
 *	Copyright (C) 1996-1998  Stallion Technologies
 *	Copyright (C) 1994-1996  Greg Ungerer.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*****************************************************************************/
#ifndef	_COMSTATS_H
#define	_COMSTATS_H
/*****************************************************************************/

/*
 *	Serial port stats structure. The structure itself is UART
 *	independent, but some fields may be UART/driver specific (for
 *	example state).
 */

typedef struct {
	unsigned long	brd;
	unsigned long	panel;
	unsigned long	port;
	unsigned long	hwid;
	unsigned long	type;
	unsigned long	txtotal;
	unsigned long	rxtotal;
	unsigned long	txbuffered;
	unsigned long	rxbuffered;
	unsigned long	rxoverrun;
	unsigned long	rxparity;
	unsigned long	rxframing;
	unsigned long	rxlost;
	unsigned long	txbreaks;
	unsigned long	rxbreaks;
	unsigned long	txxon;
	unsigned long	txxoff;
	unsigned long	rxxon;
	unsigned long	rxxoff;
	unsigned long	txctson;
	unsigned long	txctsoff;
	unsigned long	rxrtson;
	unsigned long	rxrtsoff;
	unsigned long	modem;
	unsigned long	state;
	unsigned long	flags;
	unsigned long	ttystate;
	unsigned long	cflags;
	unsigned long	iflags;
	unsigned long	oflags;
	unsigned long	lflags;
	unsigned long	signals;
} comstats_t;


/*
 *	Board stats structure. Returns useful info about the board.
 */

#define	COM_MAXPANELS	8

typedef struct {
	unsigned long	panel;
	unsigned long	type;
	unsigned long	hwid;
	unsigned long	nrports;
} companel_t;

typedef struct {
	unsigned long	brd;
	unsigned long	type;
	unsigned long	hwid;
	unsigned long	state;
	unsigned long	ioaddr;
	unsigned long	ioaddr2;
	unsigned long	memaddr;
	unsigned long	irq;
	unsigned long	nrpanels;
	unsigned long	nrports;
	companel_t	panels[COM_MAXPANELS];
} combrd_t;


/*
 *	Define the ioctl operations for stats stuff.
 */
#include <linux/ioctl.h>

#define	COM_GETPORTSTATS	_IO('c',30)
#define	COM_CLRPORTSTATS	_IO('c',31)
#define	COM_GETBRDSTATS		_IO('c',32)


/*
 *	Define the set of ioctls that give user level access to the
 *	private port, panel and board structures. The argument required
 *	will be driver dependent!  
 */
#define	COM_READPORT		_IO('c',40)
#define	COM_READBOARD		_IO('c',41)
#define	COM_READPANEL		_IO('c',42)

/*****************************************************************************/
#endif
