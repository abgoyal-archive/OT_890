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
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef	__XFS_SUPPORT_DEBUG_H__
#define	__XFS_SUPPORT_DEBUG_H__

#include <stdarg.h>

#define CE_DEBUG        7               /* debug        */
#define CE_CONT         6               /* continuation */
#define CE_NOTE         5               /* notice       */
#define CE_WARN         4               /* warning      */
#define CE_ALERT        1               /* alert        */
#define CE_PANIC        0               /* panic        */

extern void cmn_err(int, char *, ...)
	__attribute__ ((format (printf, 2, 3)));
extern void assfail(char *expr, char *f, int l);

#define ASSERT_ALWAYS(expr)	\
	(unlikely(expr) ? (void)0 : assfail(#expr, __FILE__, __LINE__))

#ifndef DEBUG
#define ASSERT(expr)	((void)0)

#ifndef STATIC
# define STATIC static noinline
#endif

#ifndef STATIC_INLINE
# define STATIC_INLINE static inline
#endif

#else /* DEBUG */

#define ASSERT(expr)	\
	(unlikely(expr) ? (void)0 : assfail(#expr, __FILE__, __LINE__))

#ifndef STATIC
# define STATIC noinline
#endif

/*
 * We stop inlining of inline functions in debug mode.
 * Unfortunately, this means static inline in header files
 * get multiple definitions, so they need to remain static.
 * This then gives tonnes of warnings about unused but defined
 * functions, so we need to add the unused attribute to prevent
 * these spurious warnings.
 */
#ifndef STATIC_INLINE
# define STATIC_INLINE static __attribute__ ((unused)) noinline
#endif

#endif /* DEBUG */


#endif  /* __XFS_SUPPORT_DEBUG_H__ */
