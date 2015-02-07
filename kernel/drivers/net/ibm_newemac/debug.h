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
 * drivers/net/ibm_newemac/debug.h
 *
 * Driver for PowerPC 4xx on-chip ethernet controller, debug print routines.
 *
 * Copyright 2007 Benjamin Herrenschmidt, IBM Corp.
 *                <benh@kernel.crashing.org>
 *
 * Based on the arch/ppc version of the driver:
 *
 * Copyright (c) 2004, 2005 Zultys Technologies
 * Eugene Surovegin <eugene.surovegin@zultys.com> or <ebs@ebshome.net>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */
#ifndef __IBM_NEWEMAC_DEBUG_H
#define __IBM_NEWEMAC_DEBUG_H

#include <linux/init.h>

#include "core.h"

#if defined(CONFIG_IBM_NEW_EMAC_DEBUG)

struct emac_instance;
struct mal_instance;

extern void emac_dbg_register(struct emac_instance *dev);
extern void emac_dbg_unregister(struct emac_instance *dev);
extern void mal_dbg_register(struct mal_instance *mal);
extern void mal_dbg_unregister(struct mal_instance *mal);
extern int emac_init_debug(void) __init;
extern void emac_fini_debug(void) __exit;
extern void emac_dbg_dump_all(void);

# define DBG_LEVEL		1

#else

# define emac_dbg_register(x)	do { } while(0)
# define emac_dbg_unregister(x)	do { } while(0)
# define mal_dbg_register(x)	do { } while(0)
# define mal_dbg_unregister(x)	do { } while(0)
# define emac_init_debug()	do { } while(0)
# define emac_fini_debug()	do { } while(0)
# define emac_dbg_dump_all()	do { } while(0)

# define DBG_LEVEL		0

#endif

#define EMAC_DBG(dev, name, fmt, arg...) \
	printk(KERN_DEBUG #name "%s: " fmt, dev->ofdev->node->full_name, ## arg)

#if DBG_LEVEL > 0
#  define DBG(d,f,x...)		EMAC_DBG(d, emac, f, ##x)
#  define MAL_DBG(d,f,x...)	EMAC_DBG(d, mal, f, ##x)
#  define ZMII_DBG(d,f,x...)	EMAC_DBG(d, zmii, f, ##x)
#  define RGMII_DBG(d,f,x...)	EMAC_DBG(d, rgmii, f, ##x)
#  define NL			"\n"
#else
#  define DBG(f,x...)		((void)0)
#  define MAL_DBG(d,f,x...)	((void)0)
#  define ZMII_DBG(d,f,x...)	((void)0)
#  define RGMII_DBG(d,f,x...)	((void)0)
#endif
#if DBG_LEVEL > 1
#  define DBG2(d,f,x...) 	DBG(d,f, ##x)
#  define MAL_DBG2(d,f,x...) 	MAL_DBG(d,f, ##x)
#  define ZMII_DBG2(d,f,x...) 	ZMII_DBG(d,f, ##x)
#  define RGMII_DBG2(d,f,x...) 	RGMII_DBG(d,f, ##x)
#else
#  define DBG2(f,x...) 		((void)0)
#  define MAL_DBG2(d,f,x...) 	((void)0)
#  define ZMII_DBG2(d,f,x...) 	((void)0)
#  define RGMII_DBG2(d,f,x...) 	((void)0)
#endif

#endif /* __IBM_NEWEMAC_DEBUG_H */
