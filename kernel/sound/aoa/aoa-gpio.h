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
 * Apple Onboard Audio GPIO definitions
 *
 * Copyright 2006 Johannes Berg <johannes@sipsolutions.net>
 *
 * GPL v2, can be found in COPYING.
 */

#ifndef __AOA_GPIO_H
#define __AOA_GPIO_H
#include <linux/workqueue.h>
#include <linux/mutex.h>
#include <asm/prom.h>

typedef void (*notify_func_t)(void *data);

enum notify_type {
	AOA_NOTIFY_HEADPHONE,
	AOA_NOTIFY_LINE_IN,
	AOA_NOTIFY_LINE_OUT,
};

struct gpio_runtime;
struct gpio_methods {
	/* for initialisation/de-initialisation of the GPIO layer */
	void (*init)(struct gpio_runtime *rt);
	void (*exit)(struct gpio_runtime *rt);

	/* turn off headphone, speakers, lineout */
	void (*all_amps_off)(struct gpio_runtime *rt);
	/* turn headphone, speakers, lineout back to previous setting */
	void (*all_amps_restore)(struct gpio_runtime *rt);

	void (*set_headphone)(struct gpio_runtime *rt, int on);
	void (*set_speakers)(struct gpio_runtime *rt, int on);
	void (*set_lineout)(struct gpio_runtime *rt, int on);

	int (*get_headphone)(struct gpio_runtime *rt);
	int (*get_speakers)(struct gpio_runtime *rt);
	int (*get_lineout)(struct gpio_runtime *rt);

	void (*set_hw_reset)(struct gpio_runtime *rt, int on);

	/* use this to be notified of any events. The notification
	 * function is passed the data, and is called in process
	 * context by the use of schedule_work.
	 * The interface for it is that setting a function to NULL
	 * removes it, and they return 0 if the operation succeeded,
	 * and -EBUSY if the notification is already assigned by
	 * someone else. */
	int (*set_notify)(struct gpio_runtime *rt,
			  enum notify_type type,
			  notify_func_t notify,
			  void *data);
	/* returns 0 if not plugged in, 1 if plugged in
	 * or a negative error code */
	int (*get_detect)(struct gpio_runtime *rt,
			  enum notify_type type);
};

struct gpio_notification {
	struct delayed_work work;
	notify_func_t notify;
	void *data;
	void *gpio_private;
	struct mutex mutex;
};

struct gpio_runtime {
	/* to be assigned by fabric */
	struct device_node *node;
	/* since everyone needs this pointer anyway... */
	struct gpio_methods *methods;
	/* to be used by the gpio implementation */
	int implementation_private;
	struct gpio_notification headphone_notify;
	struct gpio_notification line_in_notify;
	struct gpio_notification line_out_notify;
};

#endif /* __AOA_GPIO_H */
