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
 * udbg function for Beat
 *
 * (C) Copyright 2006 TOSHIBA CORPORATION
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <linux/kernel.h>
#include <linux/console.h>

#include <asm/machdep.h>
#include <asm/prom.h>
#include <asm/udbg.h>

#include "beat.h"

#define	celleb_vtermno	0

static void udbg_putc_beat(char c)
{
	unsigned long rc;

	if (c == '\n')
		udbg_putc_beat('\r');

	rc = beat_put_term_char(celleb_vtermno, 1, (uint64_t)c << 56, 0);
}

/* Buffered chars getc */
static u64 inbuflen;
static u64 inbuf[2];	/* must be 2 u64s */

static int udbg_getc_poll_beat(void)
{
	/* The interface is tricky because it may return up to 16 chars.
	 * We save them statically for future calls to udbg_getc().
	 */
	char ch, *buf = (char *)inbuf;
	int i;
	long rc;
	if (inbuflen == 0) {
		/* get some more chars. */
		inbuflen = 0;
		rc = beat_get_term_char(celleb_vtermno, &inbuflen,
					inbuf+0, inbuf+1);
		if (rc != 0)
			inbuflen = 0;	/* otherwise inbuflen is garbage */
	}
	if (inbuflen <= 0 || inbuflen > 16) {
		/* Catch error case as well as other oddities (corruption) */
		inbuflen = 0;
		return -1;
	}
	ch = buf[0];
	for (i = 1; i < inbuflen; i++)	/* shuffle them down. */
		buf[i-1] = buf[i];
	inbuflen--;
	return ch;
}

static int udbg_getc_beat(void)
{
	int ch;
	for (;;) {
		ch = udbg_getc_poll_beat();
		if (ch == -1) {
			/* This shouldn't be needed...but... */
			volatile unsigned long delay;
			for (delay = 0; delay < 2000000; delay++)
				;
		} else {
			return ch;
		}
	}
}

/* call this from early_init() for a working debug console on
 * vterm capable LPAR machines
 */
void __init udbg_init_debug_beat(void)
{
	udbg_putc = udbg_putc_beat;
	udbg_getc = udbg_getc_beat;
	udbg_getc_poll = udbg_getc_poll_beat;
}
