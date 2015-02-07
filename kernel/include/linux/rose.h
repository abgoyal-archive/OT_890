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
 * These are the public elements of the Linux kernel Rose implementation.
 * For kernel AX.25 see the file ax25.h. This file requires ax25.h for the
 * definition of the ax25_address structure.
 */

#ifndef	ROSE_KERNEL_H
#define	ROSE_KERNEL_H

#define ROSE_MTU	251

#define ROSE_MAX_DIGIS 6

#define	ROSE_DEFER	1
#define ROSE_T1		2
#define	ROSE_T2		3
#define	ROSE_T3		4
#define	ROSE_IDLE	5
#define	ROSE_QBITINCL	6
#define	ROSE_HOLDBACK	7

#define	SIOCRSGCAUSE		(SIOCPROTOPRIVATE+0)
#define	SIOCRSSCAUSE		(SIOCPROTOPRIVATE+1)
#define	SIOCRSL2CALL		(SIOCPROTOPRIVATE+2)
#define	SIOCRSSL2CALL		(SIOCPROTOPRIVATE+2)
#define	SIOCRSACCEPT		(SIOCPROTOPRIVATE+3)
#define	SIOCRSCLRRT		(SIOCPROTOPRIVATE+4)
#define	SIOCRSGL2CALL		(SIOCPROTOPRIVATE+5)
#define	SIOCRSGFACILITIES	(SIOCPROTOPRIVATE+6)

#define	ROSE_DTE_ORIGINATED	0x00
#define	ROSE_NUMBER_BUSY	0x01
#define	ROSE_INVALID_FACILITY	0x03
#define	ROSE_NETWORK_CONGESTION	0x05
#define	ROSE_OUT_OF_ORDER	0x09
#define	ROSE_ACCESS_BARRED	0x0B
#define	ROSE_NOT_OBTAINABLE	0x0D
#define	ROSE_REMOTE_PROCEDURE	0x11
#define	ROSE_LOCAL_PROCEDURE	0x13
#define	ROSE_SHIP_ABSENT	0x39

typedef struct {
	char		rose_addr[5];
} rose_address;

struct sockaddr_rose {
	sa_family_t	srose_family;
	rose_address	srose_addr;
	ax25_address	srose_call;
	int		srose_ndigis;
	ax25_address	srose_digi;
};

struct full_sockaddr_rose {
	sa_family_t	srose_family;
	rose_address	srose_addr;
	ax25_address	srose_call;
	unsigned int	srose_ndigis;
	ax25_address	srose_digis[ROSE_MAX_DIGIS];
};

struct rose_route_struct {
	rose_address	address;
	unsigned short	mask;
	ax25_address	neighbour;
	char		device[16];
	unsigned char	ndigis;
	ax25_address	digipeaters[AX25_MAX_DIGIS];
};

struct rose_cause_struct {
	unsigned char	cause;
	unsigned char	diagnostic;
};

struct rose_facilities_struct {
	rose_address	source_addr,   dest_addr;
	ax25_address	source_call,   dest_call;
	unsigned char	source_ndigis, dest_ndigis;
	ax25_address	source_digis[ROSE_MAX_DIGIS];
	ax25_address	dest_digis[ROSE_MAX_DIGIS];
	unsigned int	rand;
	rose_address	fail_addr;
	ax25_address	fail_call;
};

#endif
