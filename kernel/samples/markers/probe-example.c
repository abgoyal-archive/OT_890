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

/* probe-example.c
 *
 * Connects two functions to marker call sites.
 *
 * (C) Copyright 2007 Mathieu Desnoyers <mathieu.desnoyers@polymtl.ca>
 *
 * This file is released under the GPLv2.
 * See the file COPYING for more details.
 */

#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/marker.h>
#include <asm/atomic.h>

struct probe_data {
	const char *name;
	const char *format;
	marker_probe_func *probe_func;
};

void probe_subsystem_event(void *probe_data, void *call_data,
	const char *format, va_list *args)
{
	/* Declare args */
	unsigned int value;
	const char *mystr;

	/* Assign args */
	value = va_arg(*args, typeof(value));
	mystr = va_arg(*args, typeof(mystr));

	/* Call printk */
	printk(KERN_INFO "Value %u, string %s\n", value, mystr);

	/* or count, check rights, serialize data in a buffer */
}

atomic_t eventb_count = ATOMIC_INIT(0);

void probe_subsystem_eventb(void *probe_data, void *call_data,
	const char *format, va_list *args)
{
	/* Increment counter */
	atomic_inc(&eventb_count);
}

static struct probe_data probe_array[] =
{
	{	.name = "subsystem_event",
		.format = "integer %d string %s",
		.probe_func = probe_subsystem_event },
	{	.name = "subsystem_eventb",
		.format = MARK_NOARGS,
		.probe_func = probe_subsystem_eventb },
};

static int __init probe_init(void)
{
	int result;
	int i;

	for (i = 0; i < ARRAY_SIZE(probe_array); i++) {
		result = marker_probe_register(probe_array[i].name,
				probe_array[i].format,
				probe_array[i].probe_func, &probe_array[i]);
		if (result)
			printk(KERN_INFO "Unable to register probe %s\n",
				probe_array[i].name);
	}
	return 0;
}

static void __exit probe_fini(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(probe_array); i++)
		marker_probe_unregister(probe_array[i].name,
			probe_array[i].probe_func, &probe_array[i]);
	printk(KERN_INFO "Number of event b : %u\n",
			atomic_read(&eventb_count));
	marker_synchronize_unregister();
}

module_init(probe_init);
module_exit(probe_fini);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mathieu Desnoyers");
MODULE_DESCRIPTION("SUBSYSTEM Probe");
