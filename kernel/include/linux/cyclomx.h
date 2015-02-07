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

#ifndef	_CYCLOMX_H
#define	_CYCLOMX_H
/*
* cyclomx.h	Cyclom 2X WAN Link Driver.
*		User-level API definitions.
*
* Author:	Arnaldo Carvalho de Melo <acme@conectiva.com.br>
*
* Copyright:	(c) 1998-2003 Arnaldo Carvalho de Melo
*
* Based on wanpipe.h by Gene Kozin <genek@compuserve.com>
*
*		This program is free software; you can redistribute it and/or
*		modify it under the terms of the GNU General Public License
*		as published by the Free Software Foundation; either version
*		2 of the License, or (at your option) any later version.
* ============================================================================
* 2000/07/13    acme		remove crap #if KERNEL_VERSION > blah
* 2000/01/21    acme            rename cyclomx_open to cyclomx_mod_inc_use_count
*                               and cyclomx_close to cyclomx_mod_dec_use_count
* 1999/05/19	acme		wait_queue_head_t wait_stats(support for 2.3.*)
* 1999/01/03	acme		judicious use of data types
* 1998/12/27	acme		cleanup: PACKED not needed
* 1998/08/08	acme		Version 0.0.1
*/

#include <linux/wanrouter.h>
#include <linux/spinlock.h>

#ifdef	__KERNEL__
/* Kernel Interface */

#include <linux/cycx_drv.h>	/* Cyclom 2X support module API definitions */
#include <linux/cycx_cfm.h>	/* Cyclom 2X firmware module definitions */
#ifdef CONFIG_CYCLOMX_X25
#include <linux/cycx_x25.h>
#endif

/* Adapter Data Space.
 * This structure is needed because we handle multiple cards, otherwise
 * static data would do it.
 */
struct cycx_device {
	char devname[WAN_DRVNAME_SZ + 1];/* card name */
	struct cycx_hw hw;		/* hardware configuration */
	struct wan_device wandev;	/* WAN device data space */
	u32 state_tick;			/* link state timestamp */
	spinlock_t lock;
	char in_isr;			/* interrupt-in-service flag */
	char buff_int_mode_unbusy;      /* flag for carrying out dev_tint */
	wait_queue_head_t wait_stats;  /* to wait for the STATS indication */
	void __iomem *mbox;			/* -> mailbox */
	void (*isr)(struct cycx_device* card);	/* interrupt service routine */
	int (*exec)(struct cycx_device* card, void* u_cmd, void* u_data);
	union {
#ifdef CONFIG_CYCLOMX_X25
		struct { /* X.25 specific data */
			u32 lo_pvc;
			u32 hi_pvc;
			u32 lo_svc;
			u32 hi_svc;
			struct cycx_x25_stats stats;
			spinlock_t lock;
			u32 connection_keys;
		} x;
#endif
	} u;
};

/* Public Functions */
void cycx_set_state(struct cycx_device *card, int state);

#ifdef CONFIG_CYCLOMX_X25
int cycx_x25_wan_init(struct cycx_device *card, wandev_conf_t *conf);
#endif
#endif	/* __KERNEL__ */
#endif	/* _CYCLOMX_H */
