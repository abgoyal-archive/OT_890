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
 * test_rodata.c: functional test for mark_rodata_ro function
 *
 * (C) Copyright 2008 Intel Corporation
 * Author: Arjan van de Ven <arjan@linux.intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */
#include <linux/module.h>
#include <asm/cacheflush.h>
#include <asm/sections.h>

int rodata_test(void)
{
	unsigned long result;
	unsigned long start, end;

	/* test 1: read the value */
	/* If this test fails, some previous testrun has clobbered the state */
	if (!rodata_test_data) {
		printk(KERN_ERR "rodata_test: test 1 fails (start data)\n");
		return -ENODEV;
	}

	/* test 2: write to the variable; this should fault */
	/*
	 * If this test fails, we managed to overwrite the data
	 *
	 * This is written in assembly to be able to catch the
	 * exception that is supposed to happen in the correct
	 * case
	 */

	result = 1;
	asm volatile(
		"0:	mov %[zero],(%[rodata_test])\n"
		"	mov %[zero], %[rslt]\n"
		"1:\n"
		".section .fixup,\"ax\"\n"
		"2:	jmp 1b\n"
		".previous\n"
		".section __ex_table,\"a\"\n"
		"       .align 16\n"
#ifdef CONFIG_X86_32
		"	.long 0b,2b\n"
#else
		"	.quad 0b,2b\n"
#endif
		".previous"
		: [rslt] "=r" (result)
		: [rodata_test] "r" (&rodata_test_data), [zero] "r" (0UL)
	);


	if (!result) {
		printk(KERN_ERR "rodata_test: test data was not read only\n");
		return -ENODEV;
	}

	/* test 3: check the value hasn't changed */
	/* If this test fails, we managed to overwrite the data */
	if (!rodata_test_data) {
		printk(KERN_ERR "rodata_test: Test 3 failes (end data)\n");
		return -ENODEV;
	}
	/* test 4: check if the rodata section is 4Kb aligned */
	start = (unsigned long)__start_rodata;
	end = (unsigned long)__end_rodata;
	if (start & (PAGE_SIZE - 1)) {
		printk(KERN_ERR "rodata_test: .rodata is not 4k aligned\n");
		return -ENODEV;
	}
	if (end & (PAGE_SIZE - 1)) {
		printk(KERN_ERR "rodata_test: .rodata end is not 4k aligned\n");
		return -ENODEV;
	}

	return 0;
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Testcase for the DEBUG_RODATA infrastructure");
MODULE_AUTHOR("Arjan van de Ven <arjan@linux.intel.com>");
