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

#ifndef _SPARC_TYPES_H
#define _SPARC_TYPES_H
/*
 * This file is never included by application software unless
 * explicitly requested (e.g., via linux/types.h) in which case the
 * application is Linux specific so (user-) name space pollution is
 * not a major issue.  However, for interoperability, libraries still
 * need to be careful to avoid a name clashes.
 */

#if defined(__sparc__) && defined(__arch64__)

/*** SPARC 64 bit ***/
#include <asm-generic/int-ll64.h>

#ifndef __ASSEMBLY__

typedef unsigned short umode_t;

#endif /* __ASSEMBLY__ */

#ifdef __KERNEL__

#define BITS_PER_LONG 64

#ifndef __ASSEMBLY__

/* Dma addresses come in generic and 64-bit flavours.  */

typedef u32 dma_addr_t;
typedef u64 dma64_addr_t;

#endif /* __ASSEMBLY__ */

#endif /* __KERNEL__ */
#else

/*** SPARC 32 bit ***/
#include <asm-generic/int-ll64.h>

#ifndef __ASSEMBLY__

typedef unsigned short umode_t;

#endif /* __ASSEMBLY__ */

#ifdef __KERNEL__

#define BITS_PER_LONG 32

#ifndef __ASSEMBLY__

typedef u32 dma_addr_t;
typedef u32 dma64_addr_t;

#endif /* __ASSEMBLY__ */

#endif /* __KERNEL__ */

#endif /* defined(__sparc__) && defined(__arch64__) */

#endif /* defined(_SPARC_TYPES_H) */
