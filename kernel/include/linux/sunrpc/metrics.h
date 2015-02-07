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
 *  linux/include/linux/sunrpc/metrics.h
 *
 *  Declarations for RPC client per-operation metrics
 *
 *  Copyright (C) 2005	Chuck Lever <cel@netapp.com>
 *
 *  RPC client per-operation statistics provide latency and retry
 *  information about each type of RPC procedure in a given RPC program.
 *  These statistics are not for detailed problem diagnosis, but simply
 *  to indicate whether the problem is local or remote.
 *
 *  These counters are not meant to be human-readable, but are meant to be
 *  integrated into system monitoring tools such as "sar" and "iostat".  As
 *  such, the counters are sampled by the tools over time, and are never
 *  zeroed after a file system is mounted.  Moving averages can be computed
 *  by the tools by taking the difference between two instantaneous samples
 *  and dividing that by the time between the samples.
 *
 *  The counters are maintained in a single array per RPC client, indexed
 *  by procedure number.  There is no need to maintain separate counter
 *  arrays per-CPU because these counters are always modified behind locks.
 */

#ifndef _LINUX_SUNRPC_METRICS_H
#define _LINUX_SUNRPC_METRICS_H

#include <linux/seq_file.h>

#define RPC_IOSTATS_VERS	"1.0"

struct rpc_iostats {
	/*
	 * These counters give an idea about how many request
	 * transmissions are required, on average, to complete that
	 * particular procedure.  Some procedures may require more
	 * than one transmission because the server is unresponsive,
	 * the client is retransmitting too aggressively, or the
	 * requests are large and the network is congested.
	 */
	unsigned long		om_ops,		/* count of operations */
				om_ntrans,	/* count of RPC transmissions */
				om_timeouts;	/* count of major timeouts */

	/*
	 * These count how many bytes are sent and received for a
	 * given RPC procedure type.  This indicates how much load a
	 * particular procedure is putting on the network.  These
	 * counts include the RPC and ULP headers, and the request
	 * payload.
	 */
	unsigned long long      om_bytes_sent,	/* count of bytes out */
				om_bytes_recv;	/* count of bytes in */

	/*
	 * The length of time an RPC request waits in queue before
	 * transmission, the network + server latency of the request,
	 * and the total time the request spent from init to release
	 * are measured.
	 */
	unsigned long long	om_queue,	/* jiffies queued for xmit */
				om_rtt,		/* jiffies for RPC RTT */
				om_execute;	/* jiffies for RPC execution */
} ____cacheline_aligned;

struct rpc_task;
struct rpc_clnt;

/*
 * EXPORTed functions for managing rpc_iostats structures
 */

#ifdef CONFIG_PROC_FS

struct rpc_iostats *	rpc_alloc_iostats(struct rpc_clnt *);
void			rpc_count_iostats(struct rpc_task *);
void			rpc_print_iostats(struct seq_file *, struct rpc_clnt *);
void			rpc_free_iostats(struct rpc_iostats *);

#else  /*  CONFIG_PROC_FS  */

static inline struct rpc_iostats *rpc_alloc_iostats(struct rpc_clnt *clnt) { return NULL; }
static inline void rpc_count_iostats(struct rpc_task *task) {}
static inline void rpc_print_iostats(struct seq_file *seq, struct rpc_clnt *clnt) {}
static inline void rpc_free_iostats(struct rpc_iostats *stats) {}

#endif  /*  CONFIG_PROC_FS  */

#endif /* _LINUX_SUNRPC_METRICS_H */
