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

#ifndef _ASM_PARISC_FUTEX_H
#define _ASM_PARISC_FUTEX_H

#ifdef __KERNEL__

#include <linux/futex.h>
#include <linux/uaccess.h>
#include <asm/errno.h>

static inline int
futex_atomic_op_inuser (int encoded_op, int __user *uaddr)
{
	int op = (encoded_op >> 28) & 7;
	int cmp = (encoded_op >> 24) & 15;
	int oparg = (encoded_op << 8) >> 20;
	int cmparg = (encoded_op << 20) >> 20;
	int oldval = 0, ret;
	if (encoded_op & (FUTEX_OP_OPARG_SHIFT << 28))
		oparg = 1 << oparg;

	if (! access_ok (VERIFY_WRITE, uaddr, sizeof(int)))
		return -EFAULT;

	pagefault_disable();

	switch (op) {
	case FUTEX_OP_SET:
	case FUTEX_OP_ADD:
	case FUTEX_OP_OR:
	case FUTEX_OP_ANDN:
	case FUTEX_OP_XOR:
	default:
		ret = -ENOSYS;
	}

	pagefault_enable();

	if (!ret) {
		switch (cmp) {
		case FUTEX_OP_CMP_EQ: ret = (oldval == cmparg); break;
		case FUTEX_OP_CMP_NE: ret = (oldval != cmparg); break;
		case FUTEX_OP_CMP_LT: ret = (oldval < cmparg); break;
		case FUTEX_OP_CMP_GE: ret = (oldval >= cmparg); break;
		case FUTEX_OP_CMP_LE: ret = (oldval <= cmparg); break;
		case FUTEX_OP_CMP_GT: ret = (oldval > cmparg); break;
		default: ret = -ENOSYS;
		}
	}
	return ret;
}

/* Non-atomic version */
static inline int
futex_atomic_cmpxchg_inatomic(int __user *uaddr, int oldval, int newval)
{
	int err = 0;
	int uval;

	/* futex.c wants to do a cmpxchg_inatomic on kernel NULL, which is
	 * our gateway page, and causes no end of trouble...
	 */
	if (segment_eq(KERNEL_DS, get_fs()) && !uaddr)
		return -EFAULT;

	if (!access_ok(VERIFY_WRITE, uaddr, sizeof(int)))
		return -EFAULT;

	err = get_user(uval, uaddr);
	if (err) return -EFAULT;
	if (uval == oldval)
		err = put_user(newval, uaddr);
	if (err) return -EFAULT;
	return uval;
}

#endif /*__KERNEL__*/
#endif /*_ASM_PARISC_FUTEX_H*/
