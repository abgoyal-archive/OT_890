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
 *  include/asm-s390/types.h
 *
 *  S390 version
 *
 *  Derived from "include/asm-i386/types.h"
 */

#ifndef _S390_TYPES_H
#define _S390_TYPES_H

#include <asm-generic/int-ll64.h>

#ifndef __ASSEMBLY__

typedef unsigned short umode_t;

/* A address type so that arithmetic can be done on it & it can be upgraded to
   64 bit when necessary 
*/
typedef unsigned long addr_t; 
typedef __signed__ long saddr_t;

#endif /* __ASSEMBLY__ */

/*
 * These aren't exported outside the kernel to avoid name space clashes
 */
#ifdef __KERNEL__

#ifndef __s390x__
#define BITS_PER_LONG 32
#else
#define BITS_PER_LONG 64
#endif

#ifndef __ASSEMBLY__

typedef u64 dma64_addr_t;
#ifdef __s390x__
/* DMA addresses come in 32-bit and 64-bit flavours. */
typedef u64 dma_addr_t;
#else
typedef u32 dma_addr_t;
#endif

#ifndef __s390x__
typedef union {
	unsigned long long pair;
	struct {
		unsigned long even;
		unsigned long odd;
	} subreg;
} register_pair;

#endif /* ! __s390x__   */
#endif /* __ASSEMBLY__  */
#endif /* __KERNEL__    */
#endif /* _S390_TYPES_H */
