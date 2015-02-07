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
 * arch/xtensa/io.c
 *
 * IO primitives
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * Copied from sparc.
 *
 * Chris Zankel <chris@zankel.net>
 *
 */

#include <asm/io.h>
#include <asm/byteorder.h>

void outsb(unsigned long addr, const void *src, unsigned long count) {
        while (count) {
                count -= 1;
                writeb(*(const char *)src, addr);
                src += 1;
                addr += 1;
        }
}

void outsw(unsigned long addr, const void *src, unsigned long count) {
        while (count) {
                count -= 2;
                writew(*(const short *)src, addr);
                src += 2;
                addr += 2;
        }
}

void outsl(unsigned long addr, const void *src, unsigned long count) {
        while (count) {
                count -= 4;
                writel(*(const long *)src, addr);
                src += 4;
                addr += 4;
        }
}

void insb(unsigned long addr, void *dst, unsigned long count) {
        while (count) {
                count -= 1;
                *(unsigned char *)dst = readb(addr);
                dst += 1;
                addr += 1;
        }
}

void insw(unsigned long addr, void *dst, unsigned long count) {
        while (count) {
                count -= 2;
                *(unsigned short *)dst = readw(addr);
                dst += 2;
                addr += 2;
        }
}

void insl(unsigned long addr, void *dst, unsigned long count) {
        while (count) {
                count -= 4;
                /*
                 * XXX I am sure we are in for an unaligned trap here.
                 */
                *(unsigned long *)dst = readl(addr);
                dst += 4;
                addr += 4;
        }
}
