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

#ifndef _TRACE_BLOCK_H
#define _TRACE_BLOCK_H

#include <linux/blkdev.h>
#include <linux/tracepoint.h>

DECLARE_TRACE(block_rq_abort,
	TPPROTO(struct request_queue *q, struct request *rq),
		TPARGS(q, rq));

DECLARE_TRACE(block_rq_insert,
	TPPROTO(struct request_queue *q, struct request *rq),
		TPARGS(q, rq));

DECLARE_TRACE(block_rq_issue,
	TPPROTO(struct request_queue *q, struct request *rq),
		TPARGS(q, rq));

DECLARE_TRACE(block_rq_requeue,
	TPPROTO(struct request_queue *q, struct request *rq),
		TPARGS(q, rq));

DECLARE_TRACE(block_rq_complete,
	TPPROTO(struct request_queue *q, struct request *rq),
		TPARGS(q, rq));

DECLARE_TRACE(block_bio_bounce,
	TPPROTO(struct request_queue *q, struct bio *bio),
		TPARGS(q, bio));

DECLARE_TRACE(block_bio_complete,
	TPPROTO(struct request_queue *q, struct bio *bio),
		TPARGS(q, bio));

DECLARE_TRACE(block_bio_backmerge,
	TPPROTO(struct request_queue *q, struct bio *bio),
		TPARGS(q, bio));

DECLARE_TRACE(block_bio_frontmerge,
	TPPROTO(struct request_queue *q, struct bio *bio),
		TPARGS(q, bio));

DECLARE_TRACE(block_bio_queue,
	TPPROTO(struct request_queue *q, struct bio *bio),
		TPARGS(q, bio));

DECLARE_TRACE(block_getrq,
	TPPROTO(struct request_queue *q, struct bio *bio, int rw),
		TPARGS(q, bio, rw));

DECLARE_TRACE(block_sleeprq,
	TPPROTO(struct request_queue *q, struct bio *bio, int rw),
		TPARGS(q, bio, rw));

DECLARE_TRACE(block_plug,
	TPPROTO(struct request_queue *q),
		TPARGS(q));

DECLARE_TRACE(block_unplug_timer,
	TPPROTO(struct request_queue *q),
		TPARGS(q));

DECLARE_TRACE(block_unplug_io,
	TPPROTO(struct request_queue *q),
		TPARGS(q));

DECLARE_TRACE(block_split,
	TPPROTO(struct request_queue *q, struct bio *bio, unsigned int pdu),
		TPARGS(q, bio, pdu));

DECLARE_TRACE(block_remap,
	TPPROTO(struct request_queue *q, struct bio *bio, dev_t dev,
		sector_t from, sector_t to),
		TPARGS(q, bio, dev, from, to));

#endif
