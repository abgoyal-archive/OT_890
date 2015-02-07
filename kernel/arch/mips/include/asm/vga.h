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
 *	Access to VGA videoram
 *
 *	(c) 1998 Martin Mares <mj@ucw.cz>
 */
#ifndef _ASM_VGA_H
#define _ASM_VGA_H

#include <asm/byteorder.h>

/*
 *	On the PC, we can just recalculate addresses and then
 *	access the videoram directly without any black magic.
 */

#define VGA_MAP_MEM(x, s)	(0xb0000000L + (unsigned long)(x))

#define vga_readb(x)	(*(x))
#define vga_writeb(x, y)	(*(y) = (x))

#define VT_BUF_HAVE_RW
/*
 *  These are only needed for supporting VGA or MDA text mode, which use little
 *  endian byte ordering.
 *  In other cases, we can optimize by using native byte ordering and
 *  <linux/vt_buffer.h> has already done the right job for us.
 */

#undef scr_writew
#undef scr_readw

static inline void scr_writew(u16 val, volatile u16 *addr)
{
	*addr = cpu_to_le16(val);
}

static inline u16 scr_readw(volatile const u16 *addr)
{
	return le16_to_cpu(*addr);
}

#define scr_memcpyw(d, s, c) memcpy(d, s, c)
#define scr_memmovew(d, s, c) memmove(d, s, c)
#define VT_BUF_HAVE_MEMCPYW
#define VT_BUF_HAVE_MEMMOVEW

#endif /* _ASM_VGA_H */
