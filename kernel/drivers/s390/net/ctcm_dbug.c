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
 *	drivers/s390/net/ctcm_dbug.c
 *
 *	Copyright IBM Corp. 2001, 2007
 *	Authors:	Peter Tiedemann (ptiedem@de.ibm.com)
 *
 */

#include <linux/stddef.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/ctype.h>
#include <linux/sysctl.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include "ctcm_dbug.h"

/*
 * Debug Facility Stuff
 */

struct ctcm_dbf_info ctcm_dbf[CTCM_DBF_INFOS] = {
	[CTCM_DBF_SETUP]     = {"ctc_setup", 8, 1, 64, CTC_DBF_INFO, NULL},
	[CTCM_DBF_ERROR]     = {"ctc_error", 8, 1, 64, CTC_DBF_ERROR, NULL},
	[CTCM_DBF_TRACE]     = {"ctc_trace", 8, 1, 64, CTC_DBF_ERROR, NULL},
	[CTCM_DBF_MPC_SETUP] = {"mpc_setup", 8, 1, 80, CTC_DBF_INFO, NULL},
	[CTCM_DBF_MPC_ERROR] = {"mpc_error", 8, 1, 80, CTC_DBF_ERROR, NULL},
	[CTCM_DBF_MPC_TRACE] = {"mpc_trace", 8, 1, 80, CTC_DBF_ERROR, NULL},
};

void ctcm_unregister_dbf_views(void)
{
	int x;
	for (x = 0; x < CTCM_DBF_INFOS; x++) {
		debug_unregister(ctcm_dbf[x].id);
		ctcm_dbf[x].id = NULL;
	}
}

int ctcm_register_dbf_views(void)
{
	int x;
	for (x = 0; x < CTCM_DBF_INFOS; x++) {
		/* register the areas */
		ctcm_dbf[x].id = debug_register(ctcm_dbf[x].name,
						ctcm_dbf[x].pages,
						ctcm_dbf[x].areas,
						ctcm_dbf[x].len);
		if (ctcm_dbf[x].id == NULL) {
			ctcm_unregister_dbf_views();
			return -ENOMEM;
		}

		/* register a view */
		debug_register_view(ctcm_dbf[x].id, &debug_hex_ascii_view);
		/* set a passing level */
		debug_set_level(ctcm_dbf[x].id, ctcm_dbf[x].level);
	}

	return 0;
}

void ctcm_dbf_longtext(enum ctcm_dbf_names dbf_nix, int level, char *fmt, ...)
{
	char dbf_txt_buf[64];
	va_list args;

	if (level > (ctcm_dbf[dbf_nix].id)->level)
		return;
	va_start(args, fmt);
	vsnprintf(dbf_txt_buf, sizeof(dbf_txt_buf), fmt, args);
	va_end(args);

	debug_text_event(ctcm_dbf[dbf_nix].id, level, dbf_txt_buf);
}

