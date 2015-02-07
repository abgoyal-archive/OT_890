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
 * These are the public elements of the Linux kernel AX.25 code. A similar
 * file netrom.h exists for the NET/ROM protocol.
 */

#ifndef	AX25_KERNEL_H
#define	AX25_KERNEL_H

#include <linux/socket.h>

#define AX25_MTU	256
#define AX25_MAX_DIGIS  8

#define AX25_WINDOW	1
#define AX25_T1		2
#define AX25_N2		3
#define AX25_T3		4
#define AX25_T2		5
#define	AX25_BACKOFF	6
#define	AX25_EXTSEQ	7
#define	AX25_PIDINCL	8
#define AX25_IDLE	9
#define AX25_PACLEN	10
#define AX25_IAMDIGI	12

#define AX25_KILL	99

#define SIOCAX25GETUID		(SIOCPROTOPRIVATE+0)
#define SIOCAX25ADDUID		(SIOCPROTOPRIVATE+1)
#define SIOCAX25DELUID		(SIOCPROTOPRIVATE+2)
#define SIOCAX25NOUID		(SIOCPROTOPRIVATE+3)
#define SIOCAX25OPTRT		(SIOCPROTOPRIVATE+7)
#define SIOCAX25CTLCON		(SIOCPROTOPRIVATE+8)
#define SIOCAX25GETINFOOLD	(SIOCPROTOPRIVATE+9)
#define SIOCAX25ADDFWD		(SIOCPROTOPRIVATE+10)
#define SIOCAX25DELFWD		(SIOCPROTOPRIVATE+11)
#define SIOCAX25DEVCTL          (SIOCPROTOPRIVATE+12)
#define SIOCAX25GETINFO         (SIOCPROTOPRIVATE+13)

#define AX25_SET_RT_IPMODE	2

#define AX25_NOUID_DEFAULT	0
#define AX25_NOUID_BLOCK	1

typedef struct {
	char		ax25_call[7];	/* 6 call + SSID (shifted ascii!) */
} ax25_address;

struct sockaddr_ax25 {
	sa_family_t	sax25_family;
	ax25_address	sax25_call;
	int		sax25_ndigis;
	/* Digipeater ax25_address sets follow */
};

#define sax25_uid	sax25_ndigis

struct full_sockaddr_ax25 {
	struct sockaddr_ax25 fsa_ax25;
	ax25_address	fsa_digipeater[AX25_MAX_DIGIS];
};

struct ax25_routes_struct {
	ax25_address	port_addr;
	ax25_address	dest_addr;
	unsigned char	digi_count;
	ax25_address	digi_addr[AX25_MAX_DIGIS];
};

struct ax25_route_opt_struct {
	ax25_address	port_addr;
	ax25_address	dest_addr;
	int		cmd;
	int		arg;
};

struct ax25_ctl_struct {
        ax25_address            port_addr;
        ax25_address            source_addr;
        ax25_address            dest_addr;
        unsigned int            cmd;
        unsigned long           arg;
        unsigned char           digi_count;
        ax25_address            digi_addr[AX25_MAX_DIGIS];
};

/* this will go away. Please do not export to user land */
struct ax25_info_struct_deprecated {
	unsigned int	n2, n2count;
	unsigned int	t1, t1timer;
	unsigned int	t2, t2timer;
	unsigned int	t3, t3timer;
	unsigned int	idle, idletimer;
	unsigned int	state;
	unsigned int	rcv_q, snd_q;
};

struct ax25_info_struct {
	unsigned int	n2, n2count;
	unsigned int	t1, t1timer;
	unsigned int	t2, t2timer;
	unsigned int	t3, t3timer;
	unsigned int	idle, idletimer;
	unsigned int	state;
	unsigned int	rcv_q, snd_q;
	unsigned int	vs, vr, va, vs_max;
	unsigned int	paclen;
	unsigned int	window;
};

struct ax25_fwd_struct {
	ax25_address	port_from;
	ax25_address	port_to;
};

#endif
