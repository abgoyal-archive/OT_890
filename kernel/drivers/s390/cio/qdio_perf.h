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
 *  drivers/s390/cio/qdio_perf.h
 *
 *  Copyright IBM Corp. 2008
 *
 *  Author: Jan Glauber (jang@linux.vnet.ibm.com)
 */
#ifndef QDIO_PERF_H
#define QDIO_PERF_H

#include <linux/types.h>
#include <linux/device.h>
#include <asm/atomic.h>

struct qdio_perf_stats {
	/* interrupt handler calls */
	atomic_long_t qdio_int;
	atomic_long_t pci_int;
	atomic_long_t thin_int;

	/* tasklet runs */
	atomic_long_t tasklet_inbound;
	atomic_long_t tasklet_outbound;
	atomic_long_t tasklet_thinint;
	atomic_long_t tasklet_thinint_loop;
	atomic_long_t thinint_inbound;
	atomic_long_t thinint_inbound_loop;
	atomic_long_t thinint_inbound_loop2;

	/* signal adapter calls */
	atomic_long_t siga_out;
	atomic_long_t siga_in;
	atomic_long_t siga_sync;

	/* misc */
	atomic_long_t inbound_handler;
	atomic_long_t outbound_handler;
	atomic_long_t fast_requeue;
	atomic_long_t outbound_target_full;

	/* for debugging */
	atomic_long_t debug_tl_out_timer;
	atomic_long_t debug_stop_polling;
	atomic_long_t debug_eqbs_all;
	atomic_long_t debug_eqbs_incomplete;
	atomic_long_t debug_sqbs_all;
	atomic_long_t debug_sqbs_incomplete;
};

extern struct qdio_perf_stats perf_stats;
extern int qdio_performance_stats;

int qdio_setup_perf_stats(void);
void qdio_remove_perf_stats(void);

extern void qdio_perf_stat_inc(atomic_long_t *count);
extern void qdio_perf_stat_dec(atomic_long_t *count);

#endif
