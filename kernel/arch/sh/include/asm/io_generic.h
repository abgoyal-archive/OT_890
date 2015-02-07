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
 * Trivial I/O routine definitions, intentionally meant to be included
 * multiple times. Ugly I/O routine concatenation helpers taken from
 * alpha. Must be included _before_ io.h to avoid preprocessor-induced
 * routine mismatch.
 */
#define IO_CONCAT(a,b)	_IO_CONCAT(a,b)
#define _IO_CONCAT(a,b)	a ## _ ## b

#ifndef __IO_PREFIX
#error "Don't include this header without a valid system prefix"
#endif

u8 IO_CONCAT(__IO_PREFIX,inb)(unsigned long);
u16 IO_CONCAT(__IO_PREFIX,inw)(unsigned long);
u32 IO_CONCAT(__IO_PREFIX,inl)(unsigned long);

void IO_CONCAT(__IO_PREFIX,outb)(u8, unsigned long);
void IO_CONCAT(__IO_PREFIX,outw)(u16, unsigned long);
void IO_CONCAT(__IO_PREFIX,outl)(u32, unsigned long);

u8 IO_CONCAT(__IO_PREFIX,inb_p)(unsigned long);
u16 IO_CONCAT(__IO_PREFIX,inw_p)(unsigned long);
u32 IO_CONCAT(__IO_PREFIX,inl_p)(unsigned long);
void IO_CONCAT(__IO_PREFIX,outb_p)(u8, unsigned long);
void IO_CONCAT(__IO_PREFIX,outw_p)(u16, unsigned long);
void IO_CONCAT(__IO_PREFIX,outl_p)(u32, unsigned long);

void IO_CONCAT(__IO_PREFIX,insb)(unsigned long, void *dst, unsigned long count);
void IO_CONCAT(__IO_PREFIX,insw)(unsigned long, void *dst, unsigned long count);
void IO_CONCAT(__IO_PREFIX,insl)(unsigned long, void *dst, unsigned long count);
void IO_CONCAT(__IO_PREFIX,outsb)(unsigned long, const void *src, unsigned long count);
void IO_CONCAT(__IO_PREFIX,outsw)(unsigned long, const void *src, unsigned long count);
void IO_CONCAT(__IO_PREFIX,outsl)(unsigned long, const void *src, unsigned long count);

void *IO_CONCAT(__IO_PREFIX,ioremap)(unsigned long offset, unsigned long size);
void IO_CONCAT(__IO_PREFIX,iounmap)(void *addr);

void __iomem *IO_CONCAT(__IO_PREFIX,ioport_map)(unsigned long addr, unsigned int size);
void IO_CONCAT(__IO_PREFIX,ioport_unmap)(void __iomem *addr);

#undef __IO_PREFIX
