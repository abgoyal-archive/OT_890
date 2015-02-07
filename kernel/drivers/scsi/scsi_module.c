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
 * Copyright (C) 2003 Christoph Hellwig.
 *	Released under GPL v2.
 *
 * Support for old-style host templates.
 *
 * NOTE:  Do not use this for new drivers ever.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <scsi/scsi_host.h>


static int __init init_this_scsi_driver(void)
{
	struct scsi_host_template *sht = &driver_template;
	struct Scsi_Host *shost;
	struct list_head *l;
	int error;

	if (!sht->release) {
		printk(KERN_ERR
		    "scsi HBA driver %s didn't set a release method.\n",
		    sht->name);
		return -EINVAL;
	}

	sht->module = THIS_MODULE;
	INIT_LIST_HEAD(&sht->legacy_hosts);

	sht->detect(sht);
	if (list_empty(&sht->legacy_hosts))
		return -ENODEV;

	list_for_each_entry(shost, &sht->legacy_hosts, sht_legacy_list) {
		error = scsi_add_host(shost, NULL);
		if (error)
			goto fail;
		scsi_scan_host(shost);
	}
	return 0;
 fail:
	l = &shost->sht_legacy_list;
	while ((l = l->prev) != &sht->legacy_hosts)
		scsi_remove_host(list_entry(l, struct Scsi_Host, sht_legacy_list));
	return error;
}

static void __exit exit_this_scsi_driver(void)
{
	struct scsi_host_template *sht = &driver_template;
	struct Scsi_Host *shost, *s;

	list_for_each_entry(shost, &sht->legacy_hosts, sht_legacy_list)
		scsi_remove_host(shost);
	list_for_each_entry_safe(shost, s, &sht->legacy_hosts, sht_legacy_list)
		sht->release(shost);

	if (list_empty(&sht->legacy_hosts))
		return;

	printk(KERN_WARNING "%s did not call scsi_unregister\n", sht->name);
	dump_stack();

	list_for_each_entry_safe(shost, s, &sht->legacy_hosts, sht_legacy_list)
		scsi_unregister(shost);
}

module_init(init_this_scsi_driver);
module_exit(exit_this_scsi_driver);
