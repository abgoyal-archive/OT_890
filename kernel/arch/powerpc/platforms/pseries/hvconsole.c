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
 * hvconsole.c
 * Copyright (C) 2004 Hollis Blanchard, IBM Corporation
 * Copyright (C) 2004 IBM Corporation
 *
 * Additional Author(s):
 *  Ryan S. Arnold <rsa@us.ibm.com>
 *
 * LPAR console support.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/hvcall.h>
#include <asm/hvconsole.h>
#include "plpar_wrappers.h"

/**
 * hvc_get_chars - retrieve characters from firmware for denoted vterm adatper
 * @vtermno: The vtermno or unit_address of the adapter from which to fetch the
 *	data.
 * @buf: The character buffer into which to put the character data fetched from
 *	firmware.
 * @count: not used?
 */
int hvc_get_chars(uint32_t vtermno, char *buf, int count)
{
	unsigned long got;

	if (plpar_get_term_char(vtermno, &got, buf) == H_SUCCESS)
		return got;

	return 0;
}

EXPORT_SYMBOL(hvc_get_chars);


/**
 * hvc_put_chars: send characters to firmware for denoted vterm adapter
 * @vtermno: The vtermno or unit_address of the adapter from which the data
 *	originated.
 * @buf: The character buffer that contains the character data to send to
 *	firmware.
 * @count: Send this number of characters.
 */
int hvc_put_chars(uint32_t vtermno, const char *buf, int count)
{
	unsigned long *lbuf = (unsigned long *) buf;
	long ret;


	/* hcall will ret H_PARAMETER if 'count' exceeds firmware max.*/
	if (count > MAX_VIO_PUT_CHARS)
		count = MAX_VIO_PUT_CHARS;

	ret = plpar_hcall_norets(H_PUT_TERM_CHAR, vtermno, count, lbuf[0],
				 lbuf[1]);
	if (ret == H_SUCCESS)
		return count;
	if (ret == H_BUSY)
		return 0;
	return -EIO;
}

EXPORT_SYMBOL(hvc_put_chars);
