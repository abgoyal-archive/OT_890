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

/**
 * @file mesubdevice.h
 *
 * @brief Provides the subdevice base class.
 * @note Copyright (C) 2007 Meilhaus Electronic GmbH (support@meilhaus.de)
 * @author Guenter Gebhardt
 */

#ifndef _MESUBDEVICE_H_
#define _MESUBDEVICE_H_

#include <linux/list.h>

#include "metypes.h"
#include "meioctl.h"
#include "meslock.h"

# include <linux/workqueue.h>

#ifdef __KERNEL__

/**
 * @brief Macro used to enter a subdevice.
 */
#define ME_SUBDEVICE_ENTER	\
{ \
	int err; \
	err = me_slock_enter(&instance->base.lock, filep); \
	if(err){ \
		PERROR("Cannot enter subdevice.\n"); \
		return err; \
	} \
}

/**
 * @brief Macro used to exit a subdevice.
 */
#define ME_SUBDEVICE_EXIT	\
{\
	int err; \
	err = me_slock_exit(&instance->base.lock, filep); \
	if(err){ \
		PERROR("Cannot exit subdevice.\n"); \
		return err; \
	} \
}

/**
 * @brief The subdevice base class.
 */
typedef struct me_subdevice {
	/* Attributes */
	struct list_head list;		/**< Enables the subdevice to be added to a dynamic list. */
	me_slock_t lock;		/**< Used by user application in order to lock the subdevice for exclusive usage. */

	/* Methods */
	int (*me_subdevice_io_irq_start) (struct me_subdevice * subdevice,
					  struct file * filep,
					  int channel,
					  int irq_source,
					  int irq_edge, int irq_arg, int flags);

	int (*me_subdevice_io_irq_wait) (struct me_subdevice * subdevice,
					 struct file * filep,
					 int channel,
					 int *irq_count,
					 int *value, int time_out, int flags);

	int (*me_subdevice_io_irq_stop) (struct me_subdevice * subdevice,
					 struct file * filep,
					 int channel, int flags);

	int (*me_subdevice_io_reset_subdevice) (struct me_subdevice * subdevice,
						struct file * filep, int flags);

	int (*me_subdevice_io_single_config) (struct me_subdevice * subdevice,
					      struct file * filep,
					      int channel,
					      int single_config,
					      int ref,
					      int trig_chan,
					      int trig_type,
					      int trig_edge, int flags);

	int (*me_subdevice_io_single_read) (struct me_subdevice * subdevice,
					    struct file * filep,
					    int channel,
					    int *value,
					    int time_out, int flags);

	int (*me_subdevice_io_single_write) (struct me_subdevice * subdevice,
					     struct file * filep,
					     int channel,
					     int value,
					     int time_out, int flags);

	int (*me_subdevice_io_stream_config) (struct me_subdevice * subdevice,
					      struct file * filep,
					      meIOStreamConfig_t * config_list,
					      int count,
					      meIOStreamTrigger_t * trigger,
					      int fifo_irq_threshold,
					      int flags);

	int (*me_subdevice_io_stream_new_values) (struct me_subdevice *
						  subdevice,
						  struct file * filep,
						  int time_out, int *count,
						  int flags);

	int (*me_subdevice_io_stream_read) (struct me_subdevice * subdevice,
					    struct file * filep,
					    int read_mode,
					    int *values, int *count, int flags);

	int (*me_subdevice_io_stream_start) (struct me_subdevice * subdevice,
					     struct file * filep,
					     int start_mode,
					     int time_out, int flags);

	int (*me_subdevice_io_stream_status) (struct me_subdevice * subdevice,
					      struct file * filep,
					      int wait,
					      int *status,
					      int *count, int flags);

	int (*me_subdevice_io_stream_stop) (struct me_subdevice * subdevice,
					    struct file * filep,
					    int stop_mode, int flags);

	int (*me_subdevice_io_stream_write) (struct me_subdevice * subdevice,
					     struct file * filep,
					     int write_mode,
					     int *values,
					     int *count, int flags);

	int (*me_subdevice_lock_subdevice) (struct me_subdevice * subdevice,
					    struct file * filep,
					    int lock, int flags);

	int (*me_subdevice_query_number_channels) (struct me_subdevice *
						   subdevice, int *number);

	int (*me_subdevice_query_number_ranges) (struct me_subdevice *
						 subdevice, int unit,
						 int *count);

	int (*me_subdevice_query_range_by_min_max) (struct me_subdevice *
						    subdevice, int unit,
						    int *min, int *max,
						    int *maxdata, int *range);

	int (*me_subdevice_query_range_info) (struct me_subdevice * subdevice,
					      int range,
					      int *unit,
					      int *min, int *max, int *maxdata);

	int (*me_subdevice_query_subdevice_type) (struct me_subdevice *
						  subdevice, int *type,
						  int *subtype);

	int (*me_subdevice_query_subdevice_caps) (struct me_subdevice *
						  subdevice, int *caps);

	int (*me_subdevice_query_subdevice_caps_args) (struct me_subdevice *
						       subdevice, int cap,
						       int *args, int count);

	int (*me_subdevice_query_timer) (struct me_subdevice * subdevice,
					 int timer,
					 int *base_frequency,
					 long long *min_ticks,
					 long long *max_ticks);

	int (*me_subdevice_config_load) (struct me_subdevice * subdevice,
					 me_cfg_device_entry_t * config);

	void (*me_subdevice_destructor) (struct me_subdevice * subdevice);
} me_subdevice_t;

/**
 * @brief Initializes a subdevice structure.
 *
 * @param subdevice The subdevice structure to initialize.
 * @return 0 on success.
 */
int me_subdevice_init(me_subdevice_t * subdevice);

/**
 * @brief Deinitializes a subdevice structure.
 *
 * @param subdevice The subdevice structure to initialize.
 */
void me_subdevice_deinit(me_subdevice_t * subdevice);

#endif
#endif
