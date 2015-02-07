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

/* $Id$
 *
 * ISDN interface module for Eicon active cards DIVA.
 * CAPI Interface
 * 
 * Copyright 2000-2003 by Armin Schindler (mac@melware.de) 
 * Copyright 2000-2003 Cytronics & Melware (info@melware.de)
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/skbuff.h>

#include "os_capi.h"

#include "platform.h"
#include "di_defs.h"
#include "capi20.h"
#include "divacapi.h"
#include "cp_vers.h"
#include "capifunc.h"

static char *main_revision = "$Revision$";
static char *DRIVERNAME =
    "Eicon DIVA - CAPI Interface driver (http://www.melware.net)";
static char *DRIVERLNAME = "divacapi";

MODULE_DESCRIPTION("CAPI driver for Eicon DIVA cards");
MODULE_AUTHOR("Cytronics & Melware, Eicon Networks");
MODULE_SUPPORTED_DEVICE("CAPI and DIVA card drivers");
MODULE_LICENSE("GPL");

/*
 * get revision number from revision string
 */
static char *getrev(const char *revision)
{
	char *rev;
	char *p;
	if ((p = strchr(revision, ':'))) {
		rev = p + 2;
		p = strchr(rev, '$');
		*--p = 0;
	} else
		rev = "1.0";
	return rev;

}

/*
 * alloc a message buffer
 */
diva_os_message_buffer_s *diva_os_alloc_message_buffer(unsigned long size,
						       void **data_buf)
{
	diva_os_message_buffer_s *dmb = alloc_skb(size, GFP_ATOMIC);
	if (dmb) {
		*data_buf = skb_put(dmb, size);
	}
	return (dmb);
}

/*
 * free a message buffer
 */
void diva_os_free_message_buffer(diva_os_message_buffer_s * dmb)
{
	kfree_skb(dmb);
}

/*
 * proc function for controller info
 */
static int diva_ctl_read_proc(char *page, char **start, off_t off,
			      int count, int *eof, struct capi_ctr *ctrl)
{
	diva_card *card = (diva_card *) ctrl->driverdata;
	int len = 0;

	len += sprintf(page + len, "%s\n", ctrl->name);
	len += sprintf(page + len, "Serial No. : %s\n", ctrl->serial);
	len += sprintf(page + len, "Id         : %d\n", card->Id);
	len += sprintf(page + len, "Channels   : %d\n", card->d.channels);

	if (off + count >= len)
		*eof = 1;
	if (len < off)
		return 0;
	*start = page + off;
	return ((count < len - off) ? count : len - off);
}

/*
 * set additional os settings in capi_ctr struct
 */
void diva_os_set_controller_struct(struct capi_ctr *ctrl)
{
	ctrl->driver_name = DRIVERLNAME;
	ctrl->load_firmware = NULL;
	ctrl->reset_ctr = NULL;
	ctrl->ctr_read_proc = diva_ctl_read_proc;
	ctrl->owner = THIS_MODULE;
}

/*
 * module init
 */
static int DIVA_INIT_FUNCTION divacapi_init(void)
{
	char tmprev[32];
	int ret = 0;

	sprintf(DRIVERRELEASE_CAPI, "%d.%d%s", DRRELMAJOR, DRRELMINOR,
		DRRELEXTRA);

	printk(KERN_INFO "%s\n", DRIVERNAME);
	printk(KERN_INFO "%s: Rel:%s  Rev:", DRIVERLNAME, DRIVERRELEASE_CAPI);
	strcpy(tmprev, main_revision);
	printk("%s  Build: %s(%s)\n", getrev(tmprev),
	       diva_capi_common_code_build, DIVA_BUILD);

	if (!(init_capifunc())) {
		printk(KERN_ERR "%s: failed init capi_driver.\n",
		       DRIVERLNAME);
		ret = -EIO;
	}

	return ret;
}

/*
 * module exit
 */
static void DIVA_EXIT_FUNCTION divacapi_exit(void)
{
	finit_capifunc();
	printk(KERN_INFO "%s: module unloaded.\n", DRIVERLNAME);
}

module_init(divacapi_init);
module_exit(divacapi_exit);
