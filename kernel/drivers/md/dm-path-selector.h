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
 * Copyright (C) 2003 Sistina Software.
 * Copyright (C) 2004 Red Hat, Inc. All rights reserved.
 *
 * Module Author: Heinz Mauelshagen
 *
 * This file is released under the GPL.
 *
 * Path-Selector registration.
 */

#ifndef	DM_PATH_SELECTOR_H
#define	DM_PATH_SELECTOR_H

#include <linux/device-mapper.h>

#include "dm-mpath.h"

/*
 * We provide an abstraction for the code that chooses which path
 * to send some io down.
 */
struct path_selector_type;
struct path_selector {
	struct path_selector_type *type;
	void *context;
};

/* Information about a path selector type */
struct path_selector_type {
	char *name;
	struct module *module;

	unsigned int table_args;
	unsigned int info_args;

	/*
	 * Constructs a path selector object, takes custom arguments
	 */
	int (*create) (struct path_selector *ps, unsigned argc, char **argv);
	void (*destroy) (struct path_selector *ps);

	/*
	 * Add an opaque path object, along with some selector specific
	 * path args (eg, path priority).
	 */
	int (*add_path) (struct path_selector *ps, struct dm_path *path,
			 int argc, char **argv, char **error);

	/*
	 * Chooses a path for this io, if no paths are available then
	 * NULL will be returned.
	 *
	 * repeat_count is the number of times to use the path before
	 * calling the function again.  0 means don't call it again unless
	 * the path fails.
	 */
	struct dm_path *(*select_path) (struct path_selector *ps,
				     unsigned *repeat_count);

	/*
	 * Notify the selector that a path has failed.
	 */
	void (*fail_path) (struct path_selector *ps, struct dm_path *p);

	/*
	 * Ask selector to reinstate a path.
	 */
	int (*reinstate_path) (struct path_selector *ps, struct dm_path *p);

	/*
	 * Table content based on parameters added in ps_add_path_fn
	 * or path selector status
	 */
	int (*status) (struct path_selector *ps, struct dm_path *path,
		       status_type_t type, char *result, unsigned int maxlen);

	int (*end_io) (struct path_selector *ps, struct dm_path *path);
};

/* Register a path selector */
int dm_register_path_selector(struct path_selector_type *type);

/* Unregister a path selector */
int dm_unregister_path_selector(struct path_selector_type *type);

/* Returns a registered path selector type */
struct path_selector_type *dm_get_path_selector(const char *name);

/* Releases a path selector  */
void dm_put_path_selector(struct path_selector_type *pst);

#endif
