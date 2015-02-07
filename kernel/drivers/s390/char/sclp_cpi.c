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
 *  drivers/s390/char/sclp_cpi.c
 *    SCLP control programm identification
 *
 *    Copyright IBM Corp. 2001, 2007
 *    Author(s): Martin Peschke <mpeschke@de.ibm.com>
 *		 Michael Ernst <mernst@de.ibm.com>
 */

#include <linux/kmod.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/version.h>
#include "sclp_cpi_sys.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Identify this operating system instance "
		   "to the System z hardware");
MODULE_AUTHOR("Martin Peschke <mpeschke@de.ibm.com>, "
	      "Michael Ernst <mernst@de.ibm.com>");

static char *system_name = "";
static char *sysplex_name = "";

module_param(system_name, charp, 0);
MODULE_PARM_DESC(system_name, "e.g. hostname - max. 8 characters");
module_param(sysplex_name, charp, 0);
MODULE_PARM_DESC(sysplex_name, "if applicable - max. 8 characters");

static int __init cpi_module_init(void)
{
	return sclp_cpi_set_data(system_name, sysplex_name, "LINUX",
				 LINUX_VERSION_CODE);
}

static void __exit cpi_module_exit(void)
{
}

module_init(cpi_module_init);
module_exit(cpi_module_exit);
