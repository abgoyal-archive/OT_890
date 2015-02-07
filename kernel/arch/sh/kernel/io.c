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
 * linux/arch/sh/kernel/io.c
 *
 * Copyright (C) 2000  Stuart Menefy
 * Copyright (C) 2005  Paul Mundt
 *
 * Provide real functions which expand to whatever the header file defined.
 * Also definitions of machine independent IO functions.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/module.h>
#include <asm/machvec.h>
#include <asm/io.h>

/*
 * Copy data from IO memory space to "real" memory space.
 * This needs to be optimized.
 */
void memcpy_fromio(void *to, const volatile void __iomem *from, unsigned long count)
{
	unsigned char *p = to;
        while (count) {
                count--;
                *p = readb(from);
                p++;
                from++;
        }
}
EXPORT_SYMBOL(memcpy_fromio);

/*
 * Copy data from "real" memory space to IO memory space.
 * This needs to be optimized.
 */
void memcpy_toio(volatile void __iomem *to, const void *from, unsigned long count)
{
	const unsigned char *p = from;
        while (count) {
                count--;
                writeb(*p, to);
                p++;
                to++;
        }
}
EXPORT_SYMBOL(memcpy_toio);

/*
 * "memset" on IO memory space.
 * This needs to be optimized.
 */
void memset_io(volatile void __iomem *dst, int c, unsigned long count)
{
        while (count) {
                count--;
                writeb(c, dst);
                dst++;
        }
}
EXPORT_SYMBOL(memset_io);

void __iomem *ioport_map(unsigned long port, unsigned int nr)
{
	void __iomem *ret;

	ret = __ioport_map_trapped(port, nr);
	if (ret)
		return ret;

	return __ioport_map(port, nr);
}
EXPORT_SYMBOL(ioport_map);

void ioport_unmap(void __iomem *addr)
{
	sh_mv.mv_ioport_unmap(addr);
}
EXPORT_SYMBOL(ioport_unmap);
