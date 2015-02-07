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

#ifndef __ASM_ARM_CACHETYPE_H
#define __ASM_ARM_CACHETYPE_H

#define CACHEID_VIVT			(1 << 0)
#define CACHEID_VIPT_NONALIASING	(1 << 1)
#define CACHEID_VIPT_ALIASING		(1 << 2)
#define CACHEID_VIPT			(CACHEID_VIPT_ALIASING|CACHEID_VIPT_NONALIASING)
#define CACHEID_ASID_TAGGED		(1 << 3)

extern unsigned int cacheid;

#define cache_is_vivt()			cacheid_is(CACHEID_VIVT)
#define cache_is_vipt()			cacheid_is(CACHEID_VIPT)
#define cache_is_vipt_nonaliasing()	cacheid_is(CACHEID_VIPT_NONALIASING)
#define cache_is_vipt_aliasing()	cacheid_is(CACHEID_VIPT_ALIASING)
#define icache_is_vivt_asid_tagged()	cacheid_is(CACHEID_ASID_TAGGED)

/*
 * __LINUX_ARM_ARCH__ is the minimum supported CPU architecture
 * Mask out support which will never be present on newer CPUs.
 * - v6+ is never VIVT
 * - v7+ VIPT never aliases
 */
#if __LINUX_ARM_ARCH__ >= 7
#define __CACHEID_ARCH_MIN	(CACHEID_VIPT_NONALIASING | CACHEID_ASID_TAGGED)
#elif __LINUX_ARM_ARCH__ >= 6
#define	__CACHEID_ARCH_MIN	(~CACHEID_VIVT)
#else
#define __CACHEID_ARCH_MIN	(~0)
#endif

/*
 * Mask out support which isn't configured
 */
#if defined(CONFIG_CPU_CACHE_VIVT) && !defined(CONFIG_CPU_CACHE_VIPT)
#define __CACHEID_ALWAYS	(CACHEID_VIVT)
#define __CACHEID_NEVER		(~CACHEID_VIVT)
#elif !defined(CONFIG_CPU_CACHE_VIVT) && defined(CONFIG_CPU_CACHE_VIPT)
#define __CACHEID_ALWAYS	(0)
#define __CACHEID_NEVER		(CACHEID_VIVT)
#else
#define __CACHEID_ALWAYS	(0)
#define __CACHEID_NEVER		(0)
#endif

static inline unsigned int __attribute__((pure)) cacheid_is(unsigned int mask)
{
	return (__CACHEID_ALWAYS & mask) |
	       (~__CACHEID_NEVER & __CACHEID_ARCH_MIN & mask & cacheid);
}

#endif
