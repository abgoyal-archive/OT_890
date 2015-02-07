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
 *	include/linux/vt_buffer.h -- Access to VT screen buffer
 *
 *	(c) 1998 Martin Mares <mj@ucw.cz>
 *
 *	This is a set of macros and functions which are used in the
 *	console driver and related code to access the screen buffer.
 *	In most cases the console works with simple in-memory buffer,
 *	but when handling hardware text mode consoles, we store
 *	the foreground console directly in video memory.
 */

#ifndef _LINUX_VT_BUFFER_H_
#define _LINUX_VT_BUFFER_H_


#if defined(CONFIG_VGA_CONSOLE) || defined(CONFIG_MDA_CONSOLE)
#include <asm/vga.h>
#endif

#ifndef VT_BUF_HAVE_RW
#define scr_writew(val, addr) (*(addr) = (val))
#define scr_readw(addr) (*(addr))
#define scr_memcpyw(d, s, c) memcpy(d, s, c)
#define scr_memmovew(d, s, c) memmove(d, s, c)
#define VT_BUF_HAVE_MEMCPYW
#define VT_BUF_HAVE_MEMMOVEW
#endif

#ifndef VT_BUF_HAVE_MEMSETW
static inline void scr_memsetw(u16 *s, u16 c, unsigned int count)
{
	count /= 2;
	while (count--)
		scr_writew(c, s++);
}
#endif

#ifndef VT_BUF_HAVE_MEMCPYW
static inline void scr_memcpyw(u16 *d, const u16 *s, unsigned int count)
{
	count /= 2;
	while (count--)
		scr_writew(scr_readw(s++), d++);
}
#endif

#ifndef VT_BUF_HAVE_MEMMOVEW
static inline void scr_memmovew(u16 *d, const u16 *s, unsigned int count)
{
	if (d < s)
		scr_memcpyw(d, s, count);
	else {
		count /= 2;
		d += count;
		s += count;
		while (count--)
			scr_writew(scr_readw(--s), --d);
	}
}
#endif

#endif
