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
 * iSCSI Initiator TCP Transport
 * Copyright (C) 2004 Dmitry Yusupov
 * Copyright (C) 2004 Alex Aizman
 * Copyright (C) 2005 - 2006 Mike Christie
 * Copyright (C) 2006 Red Hat, Inc.  All rights reserved.
 * maintained by open-iscsi@googlegroups.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * See the file COPYING included with this distribution for more details.
 */

#ifndef ISCSI_SW_TCP_H
#define ISCSI_SW_TCP_H

#include <scsi/libiscsi.h>
#include <scsi/libiscsi_tcp.h>

struct socket;
struct iscsi_tcp_conn;

/* Socket connection send helper */
struct iscsi_sw_tcp_send {
	struct iscsi_hdr	*hdr;
	struct iscsi_segment	segment;
	struct iscsi_segment	data_segment;
};

struct iscsi_sw_tcp_conn {
	struct iscsi_conn	*iscsi_conn;
	struct socket		*sock;

	struct iscsi_sw_tcp_send out;
	/* old values for socket callbacks */
	void			(*old_data_ready)(struct sock *, int);
	void			(*old_state_change)(struct sock *);
	void			(*old_write_space)(struct sock *);

	/* data and header digests */
	struct hash_desc	tx_hash;	/* CRC32C (Tx) */
	struct hash_desc	rx_hash;	/* CRC32C (Rx) */

	/* MIB custom statistics */
	uint32_t		sendpage_failures_cnt;
	uint32_t		discontiguous_hdr_cnt;

	ssize_t (*sendpage)(struct socket *, struct page *, int, size_t, int);
};

struct iscsi_sw_tcp_hdrbuf {
	struct iscsi_hdr	hdrbuf;
	char			hdrextbuf[ISCSI_MAX_AHS_SIZE +
		                                  ISCSI_DIGEST_SIZE];
};

#endif /* ISCSI_SW_TCP_H */
