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
 * Copyright (C) 2002 Jeff Dike (jdike@karaya.com)
 * Licensed under the GPL
 */

#ifndef __UM_UACCESS_H
#define __UM_UACCESS_H

#include <asm/errno.h>
#include <asm/processor.h>

/* thread_info has a mm_segment_t in it, so put the definition up here */
typedef struct {
	unsigned long seg;
} mm_segment_t;

#include "linux/thread_info.h"

#define VERIFY_READ 0
#define VERIFY_WRITE 1

/*
 * The fs value determines whether argument validity checking should be
 * performed or not.  If get_fs() == USER_DS, checking is performed, with
 * get_fs() == KERNEL_DS, checking is bypassed.
 *
 * For historical reasons, these macros are grossly misnamed.
 */

#define MAKE_MM_SEG(s)	((mm_segment_t) { (s) })

#define KERNEL_DS	MAKE_MM_SEG(0xFFFFFFFF)
#define USER_DS		MAKE_MM_SEG(TASK_SIZE)

#define get_ds()	(KERNEL_DS)
#define get_fs()	(current_thread_info()->addr_limit)
#define set_fs(x)	(current_thread_info()->addr_limit = (x))

#define segment_eq(a, b) ((a).seg == (b).seg)

#include "um_uaccess.h"

#define __copy_from_user(to, from, n) copy_from_user(to, from, n)

#define __copy_to_user(to, from, n) copy_to_user(to, from, n)

#define __copy_to_user_inatomic __copy_to_user
#define __copy_from_user_inatomic __copy_from_user

#define __get_user(x, ptr) \
({ \
	const __typeof__(*(ptr)) __user *__private_ptr = (ptr);	\
	__typeof__(x) __private_val;			\
	int __private_ret = -EFAULT;			\
	(x) = (__typeof__(*(__private_ptr)))0;				\
	if (__copy_from_user((__force void *)&__private_val, (__private_ptr),\
			     sizeof(*(__private_ptr))) == 0) {		\
		(x) = (__typeof__(*(__private_ptr))) __private_val;	\
		__private_ret = 0;					\
	}								\
	__private_ret;							\
}) 

#define get_user(x, ptr) \
({ \
        const __typeof__((*(ptr))) __user *private_ptr = (ptr); \
        (access_ok(VERIFY_READ, private_ptr, sizeof(*private_ptr)) ? \
	 __get_user(x, private_ptr) : ((x) = (__typeof__(*ptr))0, -EFAULT)); \
})

#define __put_user(x, ptr) \
({ \
        __typeof__(*(ptr)) __user *__private_ptr = ptr; \
        __typeof__(*(__private_ptr)) __private_val; \
        int __private_ret = -EFAULT; \
        __private_val = (__typeof__(*(__private_ptr))) (x); \
        if (__copy_to_user((__private_ptr), &__private_val, \
			   sizeof(*(__private_ptr))) == 0) { \
		__private_ret = 0; \
	} \
        __private_ret; \
})

#define put_user(x, ptr) \
({ \
        __typeof__(*(ptr)) __user *private_ptr = (ptr); \
        (access_ok(VERIFY_WRITE, private_ptr, sizeof(*private_ptr)) ? \
	 __put_user(x, private_ptr) : -EFAULT); \
})

#define strlen_user(str) strnlen_user(str, ~0U >> 1)

struct exception_table_entry
{
        unsigned long insn;
	unsigned long fixup;
};

#endif
