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
 * linux/arch/frv/mm/extable.c
 */

#include <linux/module.h>
#include <linux/spinlock.h>
#include <asm/uaccess.h>

extern const struct exception_table_entry __attribute__((aligned(8))) __start___ex_table[];
extern const struct exception_table_entry __attribute__((aligned(8))) __stop___ex_table[];
extern const void __memset_end, __memset_user_error_lr, __memset_user_error_handler;
extern const void __memcpy_end, __memcpy_user_error_lr, __memcpy_user_error_handler;
extern spinlock_t modlist_lock;

/*****************************************************************************/
/*
 *
 */
static inline unsigned long search_one_table(const struct exception_table_entry *first,
					     const struct exception_table_entry *last,
					     unsigned long value)
{
        while (first <= last) {
		const struct exception_table_entry __attribute__((aligned(8))) *mid;
		long diff;

		mid = (last - first) / 2 + first;
		diff = mid->insn - value;
                if (diff == 0)
                        return mid->fixup;
                else if (diff < 0)
                        first = mid + 1;
                else
                        last = mid - 1;
        }
        return 0;
} /* end search_one_table() */

/*****************************************************************************/
/*
 * see if there's a fixup handler available to deal with a kernel fault
 */
unsigned long search_exception_table(unsigned long pc)
{
	const struct exception_table_entry *extab;

	/* determine if the fault lay during a memcpy_user or a memset_user */
	if (__frame->lr == (unsigned long) &__memset_user_error_lr &&
	    (unsigned long) &memset <= pc && pc < (unsigned long) &__memset_end
	    ) {
		/* the fault occurred in a protected memset
		 * - we search for the return address (in LR) instead of the program counter
		 * - it was probably during a clear_user()
		 */
		return (unsigned long) &__memset_user_error_handler;
	}

	if (__frame->lr == (unsigned long) &__memcpy_user_error_lr &&
	    (unsigned long) &memcpy <= pc && pc < (unsigned long) &__memcpy_end
	    ) {
		/* the fault occurred in a protected memset
		 * - we search for the return address (in LR) instead of the program counter
		 * - it was probably during a copy_to/from_user()
		 */
		return (unsigned long) &__memcpy_user_error_handler;
	}

	extab = search_exception_tables(pc);
	if (extab)
		return extab->fixup;

	return 0;

} /* end search_exception_table() */
