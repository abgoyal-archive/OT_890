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
 * Copyright (c) 2003-2007 Network Appliance, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the BSD-type
 * license below:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *      Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 *      Neither the name of the Network Appliance, Inc. nor the names of
 *      its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written
 *      permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _LINUX_SUNRPC_RPC_RDMA_H
#define _LINUX_SUNRPC_RPC_RDMA_H

struct rpcrdma_segment {
	__be32 rs_handle;	/* Registered memory handle */
	__be32 rs_length;	/* Length of the chunk in bytes */
	__be64 rs_offset;	/* Chunk virtual address or offset */
};

/*
 * read chunk(s), encoded as a linked list.
 */
struct rpcrdma_read_chunk {
	__be32 rc_discrim;	/* 1 indicates presence */
	__be32 rc_position;	/* Position in XDR stream */
	struct rpcrdma_segment rc_target;
};

/*
 * write chunk, and reply chunk.
 */
struct rpcrdma_write_chunk {
	struct rpcrdma_segment wc_target;
};

/*
 * write chunk(s), encoded as a counted array.
 */
struct rpcrdma_write_array {
	__be32 wc_discrim;	/* 1 indicates presence */
	__be32 wc_nchunks;	/* Array count */
	struct rpcrdma_write_chunk wc_array[0];
};

struct rpcrdma_msg {
	__be32 rm_xid;	/* Mirrors the RPC header xid */
	__be32 rm_vers;	/* Version of this protocol */
	__be32 rm_credit;	/* Buffers requested/granted */
	__be32 rm_type;	/* Type of message (enum rpcrdma_proc) */
	union {

		struct {			/* no chunks */
			__be32 rm_empty[3];	/* 3 empty chunk lists */
		} rm_nochunks;

		struct {			/* no chunks and padded */
			__be32 rm_align;	/* Padding alignment */
			__be32 rm_thresh;	/* Padding threshold */
			__be32 rm_pempty[3];	/* 3 empty chunk lists */
		} rm_padded;

		__be32 rm_chunks[0];	/* read, write and reply chunks */

	} rm_body;
};

#define RPCRDMA_HDRLEN_MIN	28

enum rpcrdma_errcode {
	ERR_VERS = 1,
	ERR_CHUNK = 2
};

struct rpcrdma_err_vers {
	uint32_t rdma_vers_low;	/* Version range supported by peer */
	uint32_t rdma_vers_high;
};

enum rpcrdma_proc {
	RDMA_MSG = 0,		/* An RPC call or reply msg */
	RDMA_NOMSG = 1,		/* An RPC call or reply msg - separate body */
	RDMA_MSGP = 2,		/* An RPC call or reply msg with padding */
	RDMA_DONE = 3,		/* Client signals reply completion */
	RDMA_ERROR = 4		/* An RPC RDMA encoding error */
};

#endif				/* _LINUX_SUNRPC_RPC_RDMA_H */
