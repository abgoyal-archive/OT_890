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

#ifndef __LINUX_GEN_STATS_H
#define __LINUX_GEN_STATS_H

#include <linux/types.h>

enum {
	TCA_STATS_UNSPEC,
	TCA_STATS_BASIC,
	TCA_STATS_RATE_EST,
	TCA_STATS_QUEUE,
	TCA_STATS_APP,
	__TCA_STATS_MAX,
};
#define TCA_STATS_MAX (__TCA_STATS_MAX - 1)

/**
 * struct gnet_stats_basic - byte/packet throughput statistics
 * @bytes: number of seen bytes
 * @packets: number of seen packets
 */
struct gnet_stats_basic
{
	__u64	bytes;
	__u32	packets;
};

/**
 * struct gnet_stats_rate_est - rate estimator
 * @bps: current byte rate
 * @pps: current packet rate
 */
struct gnet_stats_rate_est
{
	__u32	bps;
	__u32	pps;
};

/**
 * struct gnet_stats_queue - queuing statistics
 * @qlen: queue length
 * @backlog: backlog size of queue
 * @drops: number of dropped packets
 * @requeues: number of requeues
 * @overlimits: number of enqueues over the limit
 */
struct gnet_stats_queue
{
	__u32	qlen;
	__u32	backlog;
	__u32	drops;
	__u32	requeues;
	__u32	overlimits;
};

/**
 * struct gnet_estimator - rate estimator configuration
 * @interval: sampling period
 * @ewma_log: the log of measurement window weight
 */
struct gnet_estimator
{
	signed char	interval;
	unsigned char	ewma_log;
};


#endif /* __LINUX_GEN_STATS_H */
