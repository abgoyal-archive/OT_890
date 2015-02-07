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

#ifndef _ASM_X86_FUTEX_H
#define _ASM_X86_FUTEX_H

#ifdef __KERNEL__

#include <linux/futex.h>
#include <linux/uaccess.h>

#include <asm/asm.h>
#include <asm/errno.h>
#include <asm/processor.h>
#include <asm/system.h>

#define __futex_atomic_op1(insn, ret, oldval, uaddr, oparg)	\
	asm volatile("1:\t" insn "\n"				\
		     "2:\t.section .fixup,\"ax\"\n"		\
		     "3:\tmov\t%3, %1\n"			\
		     "\tjmp\t2b\n"				\
		     "\t.previous\n"				\
		     _ASM_EXTABLE(1b, 3b)			\
		     : "=r" (oldval), "=r" (ret), "+m" (*uaddr)	\
		     : "i" (-EFAULT), "0" (oparg), "1" (0))

#define __futex_atomic_op2(insn, ret, oldval, uaddr, oparg)	\
	asm volatile("1:\tmovl	%2, %0\n"			\
		     "\tmovl\t%0, %3\n"				\
		     "\t" insn "\n"				\
		     "2:\t" LOCK_PREFIX "cmpxchgl %3, %2\n"	\
		     "\tjnz\t1b\n"				\
		     "3:\t.section .fixup,\"ax\"\n"		\
		     "4:\tmov\t%5, %1\n"			\
		     "\tjmp\t3b\n"				\
		     "\t.previous\n"				\
		     _ASM_EXTABLE(1b, 4b)			\
		     _ASM_EXTABLE(2b, 4b)			\
		     : "=&a" (oldval), "=&r" (ret),		\
		       "+m" (*uaddr), "=&r" (tem)		\
		     : "r" (oparg), "i" (-EFAULT), "1" (0))

static inline int futex_atomic_op_inuser(int encoded_op, int __user *uaddr)
{
	int op = (encoded_op >> 28) & 7;
	int cmp = (encoded_op >> 24) & 15;
	int oparg = (encoded_op << 8) >> 20;
	int cmparg = (encoded_op << 20) >> 20;
	int oldval = 0, ret, tem;

	if (encoded_op & (FUTEX_OP_OPARG_SHIFT << 28))
		oparg = 1 << oparg;

	if (!access_ok(VERIFY_WRITE, uaddr, sizeof(int)))
		return -EFAULT;

#if defined(CONFIG_X86_32) && !defined(CONFIG_X86_BSWAP)
	/* Real i386 machines can only support FUTEX_OP_SET */
	if (op != FUTEX_OP_SET && boot_cpu_data.x86 == 3)
		return -ENOSYS;
#endif

	pagefault_disable();

	switch (op) {
	case FUTEX_OP_SET:
		__futex_atomic_op1("xchgl %0, %2", ret, oldval, uaddr, oparg);
		break;
	case FUTEX_OP_ADD:
		__futex_atomic_op1(LOCK_PREFIX "xaddl %0, %2", ret, oldval,
				   uaddr, oparg);
		break;
	case FUTEX_OP_OR:
		__futex_atomic_op2("orl %4, %3", ret, oldval, uaddr, oparg);
		break;
	case FUTEX_OP_ANDN:
		__futex_atomic_op2("andl %4, %3", ret, oldval, uaddr, ~oparg);
		break;
	case FUTEX_OP_XOR:
		__futex_atomic_op2("xorl %4, %3", ret, oldval, uaddr, oparg);
		break;
	default:
		ret = -ENOSYS;
	}

	pagefault_enable();

	if (!ret) {
		switch (cmp) {
		case FUTEX_OP_CMP_EQ:
			ret = (oldval == cmparg);
			break;
		case FUTEX_OP_CMP_NE:
			ret = (oldval != cmparg);
			break;
		case FUTEX_OP_CMP_LT:
			ret = (oldval < cmparg);
			break;
		case FUTEX_OP_CMP_GE:
			ret = (oldval >= cmparg);
			break;
		case FUTEX_OP_CMP_LE:
			ret = (oldval <= cmparg);
			break;
		case FUTEX_OP_CMP_GT:
			ret = (oldval > cmparg);
			break;
		default:
			ret = -ENOSYS;
		}
	}
	return ret;
}

static inline int futex_atomic_cmpxchg_inatomic(int __user *uaddr, int oldval,
						int newval)
{

#if defined(CONFIG_X86_32) && !defined(CONFIG_X86_BSWAP)
	/* Real i386 machines have no cmpxchg instruction */
	if (boot_cpu_data.x86 == 3)
		return -ENOSYS;
#endif

	if (!access_ok(VERIFY_WRITE, uaddr, sizeof(int)))
		return -EFAULT;

	asm volatile("1:\t" LOCK_PREFIX "cmpxchgl %3, %1\n"
		     "2:\t.section .fixup, \"ax\"\n"
		     "3:\tmov     %2, %0\n"
		     "\tjmp     2b\n"
		     "\t.previous\n"
		     _ASM_EXTABLE(1b, 3b)
		     : "=a" (oldval), "+m" (*uaddr)
		     : "i" (-EFAULT), "r" (newval), "0" (oldval)
		     : "memory"
	);

	return oldval;
}

#endif
#endif /* _ASM_X86_FUTEX_H */
