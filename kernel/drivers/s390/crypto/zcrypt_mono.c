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
 *  linux/drivers/s390/crypto/zcrypt_mono.c
 *
 *  zcrypt 2.1.0
 *
 *  Copyright (C)  2001, 2006 IBM Corporation
 *  Author(s): Robert Burroughs
 *	       Eric Rossman (edrossma@us.ibm.com)
 *
 *  Hotplug & misc device support: Jochen Roehrig (roehrig@de.ibm.com)
 *  Major cleanup & driver split: Martin Schwidefsky <schwidefsky@de.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/compat.h>
#include <asm/atomic.h>
#include <asm/uaccess.h>

#include "ap_bus.h"
#include "zcrypt_api.h"
#include "zcrypt_pcica.h"
#include "zcrypt_pcicc.h"
#include "zcrypt_pcixcc.h"
#include "zcrypt_cex2a.h"

/**
 * The module initialization code.
 */
static int __init zcrypt_init(void)
{
	int rc;

	rc = ap_module_init();
	if (rc)
		goto out;
	rc = zcrypt_api_init();
	if (rc)
		goto out_ap;
	rc = zcrypt_pcica_init();
	if (rc)
		goto out_api;
	rc = zcrypt_pcicc_init();
	if (rc)
		goto out_pcica;
	rc = zcrypt_pcixcc_init();
	if (rc)
		goto out_pcicc;
	rc = zcrypt_cex2a_init();
	if (rc)
		goto out_pcixcc;
	return 0;

out_pcixcc:
	zcrypt_pcixcc_exit();
out_pcicc:
	zcrypt_pcicc_exit();
out_pcica:
	zcrypt_pcica_exit();
out_api:
	zcrypt_api_exit();
out_ap:
	ap_module_exit();
out:
	return rc;
}

/**
 * The module termination code.
 */
static void __exit zcrypt_exit(void)
{
	zcrypt_cex2a_exit();
	zcrypt_pcixcc_exit();
	zcrypt_pcicc_exit();
	zcrypt_pcica_exit();
	zcrypt_api_exit();
	ap_module_exit();
}

module_init(zcrypt_init);
module_exit(zcrypt_exit);
