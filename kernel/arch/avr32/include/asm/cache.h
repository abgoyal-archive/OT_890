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

#ifndef __ASM_AVR32_CACHE_H
#define __ASM_AVR32_CACHE_H

#define L1_CACHE_SHIFT 5
#define L1_CACHE_BYTES (1 << L1_CACHE_SHIFT)

/*
 * Memory returned by kmalloc() may be used for DMA, so we must make
 * sure that all such allocations are cache aligned. Otherwise,
 * unrelated code may cause parts of the buffer to be read into the
 * cache before the transfer is done, causing old data to be seen by
 * the CPU.
 */
#define ARCH_KMALLOC_MINALIGN	L1_CACHE_BYTES

#ifndef __ASSEMBLER__
struct cache_info {
	unsigned int ways;
	unsigned int sets;
	unsigned int linesz;
};
#endif /* __ASSEMBLER */

/* Cache operation constants */
#define ICACHE_FLUSH		0x00
#define ICACHE_INVALIDATE	0x01
#define ICACHE_LOCK		0x02
#define ICACHE_UNLOCK		0x03
#define ICACHE_PREFETCH		0x04

#define DCACHE_FLUSH		0x08
#define DCACHE_LOCK		0x09
#define DCACHE_UNLOCK		0x0a
#define DCACHE_INVALIDATE	0x0b
#define DCACHE_CLEAN		0x0c
#define DCACHE_CLEAN_INVAL	0x0d

#endif /* __ASM_AVR32_CACHE_H */
