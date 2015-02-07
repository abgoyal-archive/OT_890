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
 * Copyright (C) 2008, 2009 Paul Pedriana <ppedriana@ea.com>. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FastAllocBase_h
#define FastAllocBase_h

// Provides customizable overrides of fastMalloc/fastFree and operator new/delete
//
// Provided functionality:
//    namespace WTF {
//        class FastAllocBase;
//
//        T*    fastNew<T>();
//        T*    fastNew<T>(arg);
//        T*    fastNew<T>(arg, arg);
//        T*    fastNewArray<T>(count);
//        void  fastDelete(T* p);
//        void  fastDeleteArray(T* p);
//        void  fastNonNullDelete(T* p);
//        void  fastNonNullDeleteArray(T* p);
//    }
//
// FastDelete assumes that the underlying
//
// Example usage:
//    class Widget : public FastAllocBase { ... };
//
//    char* charPtr = fastNew<char>();
//    fastDelete(charPtr);
//
//    char* charArrayPtr = fastNewArray<char>(37);
//    fastDeleteArray(charArrayPtr);
//
//    void** voidPtrPtr = fastNew<void*>();
//    fastDelete(voidPtrPtr);
//
//    void** voidPtrArrayPtr = fastNewArray<void*>(37);
//    fastDeleteArray(voidPtrArrayPtr);
//
//    POD* podPtr = fastNew<POD>();
//    fastDelete(podPtr);
//
//    POD* podArrayPtr = fastNewArray<POD>(37);
//    fastDeleteArray(podArrayPtr);
//
//    Object* objectPtr = fastNew<Object>();
//    fastDelete(objectPtr);
//
//    Object* objectArrayPtr = fastNewArray<Object>(37);
//    fastDeleteArray(objectArrayPtr);
//

#include <new>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "Assertions.h"
#include "FastMalloc.h"
#include "TypeTraits.h"

namespace WTF {

    class FastAllocBase {
    public:
        // Placement operator new.
        void* operator new(size_t, void* p) { return p; }
        void* operator new[](size_t, void* p) { return p; }

        void* operator new(size_t size)
        {
            void* p = fastMalloc(size);
            fastMallocMatchValidateMalloc(p, Internal::AllocTypeClassNew);
            return p;
        }

        void operator delete(void* p)
        {
            fastMallocMatchValidateFree(p, Internal::AllocTypeClassNew);
            fastFree(p);
        }

        void* operator new[](size_t size)
        {
            void* p = fastMalloc(size);
            fastMallocMatchValidateMalloc(p, Internal::AllocTypeClassNewArray);
            return p;
        }

        void operator delete[](void* p)
        {
            fastMallocMatchValidateFree(p, Internal::AllocTypeClassNewArray);
            fastFree(p);
        }
    };

    // fastNew / fastDelete

    template <typename T>
    inline T* fastNew()
    {
        void* p = fastMalloc(sizeof(T));

        if (!p)
            return 0;

        fastMallocMatchValidateMalloc(p, Internal::AllocTypeFastNew);
        return ::new(p) T;
    }

    template <typename T, typename Arg1>
    inline T* fastNew(Arg1 arg1)
    {
        void* p = fastMalloc(sizeof(T));

        if (!p)
            return 0;

        fastMallocMatchValidateMalloc(p, Internal::AllocTypeFastNew);
        return ::new(p) T(arg1);
    }

    template <typename T, typename Arg1, typename Arg2>
    inline T* fastNew(Arg1 arg1, Arg2 arg2)
    {
        void* p = fastMalloc(sizeof(T));

        if (!p)
            return 0;

        fastMallocMatchValidateMalloc(p, Internal::AllocTypeFastNew);
        return ::new(p) T(arg1, arg2);
    }

    template <typename T, typename Arg1, typename Arg2, typename Arg3>
    inline T* fastNew(Arg1 arg1, Arg2 arg2, Arg3 arg3)
    {
        void* p = fastMalloc(sizeof(T));

        if (!p)
            return 0;

        fastMallocMatchValidateMalloc(p, Internal::AllocTypeFastNew);
        return ::new(p) T(arg1, arg2, arg3);
    }

    template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    inline T* fastNew(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
    {
        void* p = fastMalloc(sizeof(T));

        if (!p)
            return 0;

        fastMallocMatchValidateMalloc(p, Internal::AllocTypeFastNew);
        return ::new(p) T(arg1, arg2, arg3, arg4);
    }

    template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    inline T* fastNew(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
    {
        void* p = fastMalloc(sizeof(T));

        if (!p)
            return 0;

        fastMallocMatchValidateMalloc(p, Internal::AllocTypeFastNew);
        return ::new(p) T(arg1, arg2, arg3, arg4, arg5);
    }

    namespace Internal {

        // We define a union of pointer to an integer and pointer to T.
        // When non-POD arrays are allocated we add a few leading bytes to tell what
        // the size of the array is. We return to the user the pointer to T.
        // The way to think of it is as if we allocate a struct like so:
        //    struct Array {
        //        AllocAlignmentInteger m_size;
        //        T m_T[array count];
        //    };

        template <typename T>
        union ArraySize {
            AllocAlignmentInteger* size;
            T* t;
        };

        // This is a support template for fastNewArray.
        // This handles the case wherein T has a trivial ctor and a trivial dtor.
        template <typename T, bool trivialCtor, bool trivialDtor>
        struct NewArrayImpl {
            static T* fastNewArray(size_t count)
            {
                T* p = static_cast<T*>(fastMalloc(sizeof(T) * count));
                fastMallocMatchValidateMalloc(p, Internal::AllocTypeFastNewArray);
                return p;
            }
        };

        // This is a support template for fastNewArray.
        // This handles the case wherein T has a non-trivial ctor and a trivial dtor.
        template <typename T>
        struct NewArrayImpl<T, false, true> {
            static T* fastNewArray(size_t count)
            {
                T* p = static_cast<T*>(fastMalloc(sizeof(T) * count));

                if (!p)
                    return 0;

                fastMallocMatchValidateMalloc(p, Internal::AllocTypeFastNewArray);

                for (T* pObject = p, *pObjectEnd = pObject + count; pObject != pObjectEnd; ++pObject)
                    ::new(pObject) T;

                return p;
            }
        };

        // This is a support template for fastNewArray.
        // This handles the case wherein T has a trivial ctor and a non-trivial dtor.
        template <typename T>
        struct NewArrayImpl<T, true, false> {
            static T* fastNewArray(size_t count)
            {
                void* p = fastMalloc(sizeof(AllocAlignmentInteger) + (sizeof(T) * count));
                ArraySize<T> a = { static_cast<AllocAlignmentInteger*>(p) };

                if (!p)
                    return 0;

                fastMallocMatchValidateMalloc(p, Internal::AllocTypeFastNewArray);
                *a.size++ = count;
                // No need to construct the objects in this case.

                return a.t;
            }
        };

        // This is a support template for fastNewArray.
        // This handles the case wherein T has a non-trivial ctor and a non-trivial dtor.
        template <typename T>
        struct NewArrayImpl<T, false, false> {
            static T* fastNewArray(size_t count)
            {
                void* p = fastMalloc(sizeof(AllocAlignmentInteger) + (sizeof(T) * count));
                ArraySize<T> a = { static_cast<AllocAlignmentInteger*>(p) };

                if (!p)
                    return 0;

                fastMallocMatchValidateMalloc(p, Internal::AllocTypeFastNewArray);
                *a.size++ = count;

                for (T* pT = a.t, *pTEnd = pT + count; pT != pTEnd; ++pT)
                    ::new(pT) T;

                return a.t;
            }
        };
    } // namespace Internal

    template <typename T>
    inline T* fastNewArray(size_t count)
    {
        return Internal::NewArrayImpl<T, WTF::HasTrivialConstructor<T>::value, WTF::HasTrivialDestructor<T>::value>::fastNewArray(count);
    }

    template <typename T>
    inline void fastDelete(T* p)
    {
        if (!p)
            return;

        fastMallocMatchValidateFree(p, Internal::AllocTypeFastNew);
        p->~T();
        fastFree(p);
    }

    namespace Internal {
        // This is a support template for fastDeleteArray.
        // This handles the case wherein T has a trivial dtor.
        template <typename T, bool trivialDtor>
        struct DeleteArrayImpl {
            static void fastDeleteArray(void* p)
            {
                // No need to destruct the objects in this case.
                // We expect that fastFree checks for null.
                fastMallocMatchValidateFree(p, Internal::AllocTypeFastNewArray);
                fastFree(p);
            }
        };

        // This is a support template for fastDeleteArray.
        // This handles the case wherein T has a non-trivial dtor.
        template <typename T>
        struct DeleteArrayImpl<T, false> {
            static void fastDeleteArray(T* p)
            {
                if (!p)
                    return;

                ArraySize<T> a;
                a.t = p;
                a.size--; // Decrement size pointer

                T* pEnd = p + *a.size;
                while (pEnd-- != p)
                    pEnd->~T();

                fastMallocMatchValidateFree(a.size, Internal::AllocTypeFastNewArray);
                fastFree(a.size);
            }
        };

    } // namespace Internal

    template <typename T>
    void fastDeleteArray(T* p)
    {
        Internal::DeleteArrayImpl<T, WTF::HasTrivialDestructor<T>::value>::fastDeleteArray(p);
    }


    template <typename T>
    inline void fastNonNullDelete(T* p)
    {
        fastMallocMatchValidateFree(p, Internal::AllocTypeFastNew);
        p->~T();
        fastFree(p);
    }

    namespace Internal {
        // This is a support template for fastDeleteArray.
        // This handles the case wherein T has a trivial dtor.
        template <typename T, bool trivialDtor>
        struct NonNullDeleteArrayImpl {
            static void fastNonNullDeleteArray(void* p)
            {
                fastMallocMatchValidateFree(p, Internal::AllocTypeFastNewArray);
                // No need to destruct the objects in this case.
                fastFree(p);
            }
        };

        // This is a support template for fastDeleteArray.
        // This handles the case wherein T has a non-trivial dtor.
        template <typename T>
        struct NonNullDeleteArrayImpl<T, false> {
            static void fastNonNullDeleteArray(T* p)
            {
                ArraySize<T> a;
                a.t = p;
                a.size--;

                T* pEnd = p + *a.size;
                while (pEnd-- != p)
                    pEnd->~T();

                fastMallocMatchValidateFree(a.size, Internal::AllocTypeFastNewArray);
                fastFree(a.size);
            }
        };

    } // namespace Internal

    template <typename T>
    void fastNonNullDeleteArray(T* p)
    {
        Internal::NonNullDeleteArrayImpl<T, WTF::HasTrivialDestructor<T>::value>::fastNonNullDeleteArray(p);
    }


} // namespace WTF

// Using WTF::FastAllocBase to avoid using FastAllocBase's explicit qualification by WTF::.
using WTF::FastAllocBase;

#endif // FastAllocBase_h
