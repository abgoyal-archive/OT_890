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
 * Copyright (C) 2008-2009 Torch Mobile Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#pragma once

#include <winbase.h>

typedef struct HBITMAP__* HBITMAP;
typedef struct HDC__* HDC;
typedef void *HANDLE;
typedef struct tagBITMAPINFO BITMAPINFO;

namespace WTF {

    class MemoryManager {
    public:
        MemoryManager();
        ~MemoryManager();

        bool allocationCanFail() const { return m_allocationCanFail; }
        void setAllocationCanFail(bool c) { m_allocationCanFail = c; }

        static HBITMAP createCompatibleBitmap(HDC hdc, int width, int height);
        static HBITMAP createDIBSection(const BITMAPINFO* pbmi, void** ppvBits);
        static void* m_malloc(size_t size);
        static void* m_calloc(size_t num, size_t size);
        static void* m_realloc(void* p, size_t size);
        static void m_free(void*);
        static bool resizeMemory(void* p, size_t newSize);
        static void* allocate64kBlock();
        static void free64kBlock(void*);
        static bool onIdle(DWORD& timeLimitMs);
        static LPVOID virtualAlloc(LPVOID lpAddress, DWORD dwSize, DWORD flAllocationType, DWORD flProtect);
        static BOOL virtualFree(LPVOID lpAddress, DWORD dwSize, DWORD dwFreeType);

    private:
        friend MemoryManager* memoryManager();

        bool m_allocationCanFail;
    };

    MemoryManager* memoryManager();

    class MemoryAllocationCanFail {
    public:
        MemoryAllocationCanFail() : m_old(memoryManager()->allocationCanFail()) { memoryManager()->setAllocationCanFail(true); }
        ~MemoryAllocationCanFail() { memoryManager()->setAllocationCanFail(m_old); }
    private:
        bool m_old;
    };

    class MemoryAllocationCannotFail {
    public:
        MemoryAllocationCannotFail() : m_old(memoryManager()->allocationCanFail()) { memoryManager()->setAllocationCanFail(false); }
        ~MemoryAllocationCannotFail() { memoryManager()->setAllocationCanFail(m_old); }
    private:
        bool m_old;
    };
}

using WTF::MemoryManager;
using WTF::memoryManager;
using WTF::MemoryAllocationCanFail;
using WTF::MemoryAllocationCannotFail;
