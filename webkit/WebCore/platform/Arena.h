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
 * Copyright (C) 1998-2000 Netscape Communications Corporation.
 * Copyright (C) 2003-6 Apple Computer
 *
 * Other contributors:
 *   Nick Blievers <nickb@adacel.com.au>
 *   Jeff Hostetler <jeff@nerdone.com>
 *   Tom Rini <trini@kernel.crashing.org>
 *   Raffaele Sena <raff@netwinder.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Alternatively, the contents of this file may be used under the terms
 * of either the Mozilla Public License Version 1.1, found at
 * http://www.mozilla.org/MPL/ (the "MPL") or the GNU General Public
 * License Version 2.0, found at http://www.fsf.org/copyleft/gpl.html
 * (the "GPL"), in which case the provisions of the MPL or the GPL are
 * applicable instead of those above.  If you wish to allow use of your
 * version of this file only under the terms of one of those two
 * licenses (the MPL or the GPL) and not to allow others to use your
 * version of this file under the LGPL, indicate your decision by
 * deletingthe provisions above and replace them with the notice and
 * other provisions required by the MPL or the GPL, as the case may be.
 * If you do not delete the provisions above, a recipient may use your
 * version of this file under any of the LGPL, the MPL or the GPL.
 */

#ifndef Arena_h
#define Arena_h

#define ARENA_ALIGN_MASK 3

namespace WebCore {

typedef unsigned long uword;

struct Arena {
    Arena* next;        // next arena
    uword base;         // aligned base address
    uword limit;        // end of arena (1+last byte)
    uword avail;        // points to next available byte in arena
};

struct ArenaPool {
    Arena first;        // first arena in pool list.
    Arena* current;     // current arena.
    unsigned int arenasize;
    uword mask;         // Mask (power-of-2 - 1)
};

void InitArenaPool(ArenaPool *pool, const char *name, 
                   unsigned int size, unsigned int align);
void FinishArenaPool(ArenaPool *pool);
void FreeArenaPool(ArenaPool *pool);
void* ArenaAllocate(ArenaPool *pool, unsigned int nb);

#define ARENA_ALIGN(pool, n) (((uword)(n) + ARENA_ALIGN_MASK) & ~ARENA_ALIGN_MASK)
#define INIT_ARENA_POOL(pool, name, size) \
        InitArenaPool(pool, name, size, ARENA_ALIGN_MASK + 1)

#define ARENA_ALLOCATE(p, pool, nb) \
        Arena *_a = (pool)->current; \
        unsigned int _nb = ARENA_ALIGN(pool, nb); \
        uword _p = _a->avail; \
        uword _q = _p + _nb; \
        if (_q > _a->limit) \
            _p = (uword)ArenaAllocate(pool, _nb); \
        else \
            _a->avail = _q; \
        p = (void *)_p;

#define ARENA_GROW(p, pool, size, incr) \
        Arena *_a = (pool)->current; \
        unsigned int _incr = ARENA_ALIGN(pool, incr); \
        uword _p = _a->avail; \
        uword _q = _p + _incr; \
        if (_p == (uword)(p) + ARENA_ALIGN(pool, size) && \
            _q <= _a->limit) { \
            _a->avail = _q; \
        } else { \
            p = ArenaGrow(pool, p, size, incr); \
        }

#define ARENA_MARK(pool) ((void *) (pool)->current->avail)
#define UPTRDIFF(p,q) ((uword)(p) - (uword)(q))     

#ifdef DEBUG
#define FREE_PATTERN 0xDA
#define CLEAR_UNUSED(a) ASSERT((a)->avail <= (a)->limit); \
                        memset((void*)(a)->avail, FREE_PATTERN, \
                            (a)->limit - (a)->avail)
#define CLEAR_ARENA(a)  memset((void*)(a), FREE_PATTERN, \
                            (a)->limit - (uword)(a))
#else
#define CLEAR_UNUSED(a)
#define CLEAR_ARENA(a)
#endif

#define ARENA_RELEASE(pool, mark) \
         char *_m = (char *)(mark); \
         Arena *_a = (pool)->current; \
         if (UPTRDIFF(_m, _a->base) <= UPTRDIFF(_a->avail, _a->base)) { \
             _a->avail = (uword)ARENA_ALIGN(pool, _m); \
             CLEAR_UNUSED(_a); \
         } else { \
             ArenaRelease(pool, _m); \
         }

#define ARENA_DESTROY(pool, a, pnext) \
         if ((pool)->current == (a)) (pool)->current = &(pool)->first; \
         *(pnext) = (a)->next; \
         CLEAR_ARENA(a); \
         fastFree(a); \
         (a) = 0;

#ifdef ANDROID_INSTRUMENT
size_t ReportPoolSize(const ArenaPool* pool);
#endif

}

#endif
