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

#ifndef _ASM_X86_UACCESS_32_H
#define _ASM_X86_UACCESS_32_H

/*
 * User space memory access functions
 */
#include <linux/errno.h>
#include <linux/thread_info.h>
#include <linux/prefetch.h>
#include <linux/string.h>
#include <asm/asm.h>
#include <asm/page.h>

unsigned long __must_check __copy_to_user_ll
		(void __user *to, const void *from, unsigned long n);
unsigned long __must_check __copy_from_user_ll
		(void *to, const void __user *from, unsigned long n);
unsigned long __must_check __copy_from_user_ll_nozero
		(void *to, const void __user *from, unsigned long n);
unsigned long __must_check __copy_from_user_ll_nocache
		(void *to, const void __user *from, unsigned long n);
unsigned long __must_check __copy_from_user_ll_nocache_nozero
		(void *to, const void __user *from, unsigned long n);

/**
 * __copy_to_user_inatomic: - Copy a block of data into user space, with less checking.
 * @to:   Destination address, in user space.
 * @from: Source address, in kernel space.
 * @n:    Number of bytes to copy.
 *
 * Context: User context only.
 *
 * Copy data from kernel space to user space.  Caller must check
 * the specified block with access_ok() before calling this function.
 * The caller should also make sure he pins the user space address
 * so that the we don't result in page fault and sleep.
 *
 * Here we special-case 1, 2 and 4-byte copy_*_user invocations.  On a fault
 * we return the initial request size (1, 2 or 4), as copy_*_user should do.
 * If a store crosses a page boundary and gets a fault, the x86 will not write
 * anything, so this is accurate.
 */

static __always_inline unsigned long __must_check
__copy_to_user_inatomic(void __user *to, const void *from, unsigned long n)
{
	if (__builtin_constant_p(n)) {
		unsigned long ret;

		switch (n) {
		case 1:
			__put_user_size(*(u8 *)from, (u8 __user *)to,
					1, ret, 1);
			return ret;
		case 2:
			__put_user_size(*(u16 *)from, (u16 __user *)to,
					2, ret, 2);
			return ret;
		case 4:
			__put_user_size(*(u32 *)from, (u32 __user *)to,
					4, ret, 4);
			return ret;
		}
	}
	return __copy_to_user_ll(to, from, n);
}

/**
 * __copy_to_user: - Copy a block of data into user space, with less checking.
 * @to:   Destination address, in user space.
 * @from: Source address, in kernel space.
 * @n:    Number of bytes to copy.
 *
 * Context: User context only.  This function may sleep.
 *
 * Copy data from kernel space to user space.  Caller must check
 * the specified block with access_ok() before calling this function.
 *
 * Returns number of bytes that could not be copied.
 * On success, this will be zero.
 */
static __always_inline unsigned long __must_check
__copy_to_user(void __user *to, const void *from, unsigned long n)
{
	might_fault();
	return __copy_to_user_inatomic(to, from, n);
}

static __always_inline unsigned long
__copy_from_user_inatomic(void *to, const void __user *from, unsigned long n)
{
	/* Avoid zeroing the tail if the copy fails..
	 * If 'n' is constant and 1, 2, or 4, we do still zero on a failure,
	 * but as the zeroing behaviour is only significant when n is not
	 * constant, that shouldn't be a problem.
	 */
	if (__builtin_constant_p(n)) {
		unsigned long ret;

		switch (n) {
		case 1:
			__get_user_size(*(u8 *)to, from, 1, ret, 1);
			return ret;
		case 2:
			__get_user_size(*(u16 *)to, from, 2, ret, 2);
			return ret;
		case 4:
			__get_user_size(*(u32 *)to, from, 4, ret, 4);
			return ret;
		}
	}
	return __copy_from_user_ll_nozero(to, from, n);
}

/**
 * __copy_from_user: - Copy a block of data from user space, with less checking.
 * @to:   Destination address, in kernel space.
 * @from: Source address, in user space.
 * @n:    Number of bytes to copy.
 *
 * Context: User context only.  This function may sleep.
 *
 * Copy data from user space to kernel space.  Caller must check
 * the specified block with access_ok() before calling this function.
 *
 * Returns number of bytes that could not be copied.
 * On success, this will be zero.
 *
 * If some data could not be copied, this function will pad the copied
 * data to the requested size using zero bytes.
 *
 * An alternate version - __copy_from_user_inatomic() - may be called from
 * atomic context and will fail rather than sleep.  In this case the
 * uncopied bytes will *NOT* be padded with zeros.  See fs/filemap.h
 * for explanation of why this is needed.
 */
static __always_inline unsigned long
__copy_from_user(void *to, const void __user *from, unsigned long n)
{
	might_fault();
	if (__builtin_constant_p(n)) {
		unsigned long ret;

		switch (n) {
		case 1:
			__get_user_size(*(u8 *)to, from, 1, ret, 1);
			return ret;
		case 2:
			__get_user_size(*(u16 *)to, from, 2, ret, 2);
			return ret;
		case 4:
			__get_user_size(*(u32 *)to, from, 4, ret, 4);
			return ret;
		}
	}
	return __copy_from_user_ll(to, from, n);
}

static __always_inline unsigned long __copy_from_user_nocache(void *to,
				const void __user *from, unsigned long n)
{
	might_fault();
	if (__builtin_constant_p(n)) {
		unsigned long ret;

		switch (n) {
		case 1:
			__get_user_size(*(u8 *)to, from, 1, ret, 1);
			return ret;
		case 2:
			__get_user_size(*(u16 *)to, from, 2, ret, 2);
			return ret;
		case 4:
			__get_user_size(*(u32 *)to, from, 4, ret, 4);
			return ret;
		}
	}
	return __copy_from_user_ll_nocache(to, from, n);
}

static __always_inline unsigned long
__copy_from_user_inatomic_nocache(void *to, const void __user *from,
				  unsigned long n)
{
       return __copy_from_user_ll_nocache_nozero(to, from, n);
}

unsigned long __must_check copy_to_user(void __user *to,
					const void *from, unsigned long n);
unsigned long __must_check copy_from_user(void *to,
					  const void __user *from,
					  unsigned long n);
long __must_check strncpy_from_user(char *dst, const char __user *src,
				    long count);
long __must_check __strncpy_from_user(char *dst,
				      const char __user *src, long count);

/**
 * strlen_user: - Get the size of a string in user space.
 * @str: The string to measure.
 *
 * Context: User context only.  This function may sleep.
 *
 * Get the size of a NUL-terminated string in user space.
 *
 * Returns the size of the string INCLUDING the terminating NUL.
 * On exception, returns 0.
 *
 * If there is a limit on the length of a valid string, you may wish to
 * consider using strnlen_user() instead.
 */
#define strlen_user(str) strnlen_user(str, LONG_MAX)

long strnlen_user(const char __user *str, long n);
unsigned long __must_check clear_user(void __user *mem, unsigned long len);
unsigned long __must_check __clear_user(void __user *mem, unsigned long len);

#endif /* _ASM_X86_UACCESS_32_H */
