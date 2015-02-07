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

/***************************************************************************
 * Linux PPP over L2TP (PPPoL2TP) Socket Implementation (RFC 2661)
 *
 * This file supplies definitions required by the PPP over L2TP driver
 * (pppol2tp.c).  All version information wrt this file is located in pppol2tp.c
 *
 * License:
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 */

#ifndef __LINUX_IF_PPPOL2TP_H
#define __LINUX_IF_PPPOL2TP_H

#include <linux/types.h>

#ifdef __KERNEL__
#include <linux/in.h>
#endif

/* Structure used to connect() the socket to a particular tunnel UDP
 * socket.
 */
struct pppol2tp_addr
{
	pid_t	pid;			/* pid that owns the fd.
					 * 0 => current */
	int	fd;			/* FD of UDP socket to use */

	struct sockaddr_in addr;	/* IP address and port to send to */

	__u16 s_tunnel, s_session;	/* For matching incoming packets */
	__u16 d_tunnel, d_session;	/* For sending outgoing packets */
};

/* Socket options:
 * DEBUG	- bitmask of debug message categories
 * SENDSEQ	- 0 => don't send packets with sequence numbers
 *		  1 => send packets with sequence numbers
 * RECVSEQ	- 0 => receive packet sequence numbers are optional
 *		  1 => drop receive packets without sequence numbers
 * LNSMODE	- 0 => act as LAC.
 *		  1 => act as LNS.
 * REORDERTO	- reorder timeout (in millisecs). If 0, don't try to reorder.
 */
enum {
	PPPOL2TP_SO_DEBUG	= 1,
	PPPOL2TP_SO_RECVSEQ	= 2,
	PPPOL2TP_SO_SENDSEQ	= 3,
	PPPOL2TP_SO_LNSMODE	= 4,
	PPPOL2TP_SO_REORDERTO	= 5,
};

/* Debug message categories for the DEBUG socket option */
enum {
	PPPOL2TP_MSG_DEBUG	= (1 << 0),	/* verbose debug (if
						 * compiled in) */
	PPPOL2TP_MSG_CONTROL	= (1 << 1),	/* userspace - kernel
						 * interface */
	PPPOL2TP_MSG_SEQ	= (1 << 2),	/* sequence numbers */
	PPPOL2TP_MSG_DATA	= (1 << 3),	/* data packets */
};



#endif
