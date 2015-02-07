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
 * Copyright (C) 2003 Christophe Saout <christophe@saout.de>
 *
 * This file is released under the GPL.
 */

#include <linux/device-mapper.h>

#include <linux/module.h>
#include <linux/init.h>
#include <linux/bio.h>

#define DM_MSG_PREFIX "zero"

/*
 * Construct a dummy mapping that only returns zeros
 */
static int zero_ctr(struct dm_target *ti, unsigned int argc, char **argv)
{
	if (argc != 0) {
		ti->error = "No arguments required";
		return -EINVAL;
	}

	return 0;
}

/*
 * Return zeros only on reads
 */
static int zero_map(struct dm_target *ti, struct bio *bio,
		      union map_info *map_context)
{
	switch(bio_rw(bio)) {
	case READ:
		zero_fill_bio(bio);
		break;
	case READA:
		/* readahead of null bytes only wastes buffer cache */
		return -EIO;
	case WRITE:
		/* writes get silently dropped */
		break;
	}

	bio_endio(bio, 0);

	/* accepted bio, don't make new request */
	return DM_MAPIO_SUBMITTED;
}

static struct target_type zero_target = {
	.name   = "zero",
	.version = {1, 0, 0},
	.module = THIS_MODULE,
	.ctr    = zero_ctr,
	.map    = zero_map,
};

static int __init dm_zero_init(void)
{
	int r = dm_register_target(&zero_target);

	if (r < 0)
		DMERR("register failed %d", r);

	return r;
}

static void __exit dm_zero_exit(void)
{
	dm_unregister_target(&zero_target);
}

module_init(dm_zero_init)
module_exit(dm_zero_exit)

MODULE_AUTHOR("Christophe Saout <christophe@saout.de>");
MODULE_DESCRIPTION(DM_NAME " dummy target returning zeros");
MODULE_LICENSE("GPL");
