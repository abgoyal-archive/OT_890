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

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include "longjmp.h"
#include "kern_constants.h"

static jmp_buf buf;

static void segfault(int sig)
{
	longjmp(buf, 1);
}

static int page_ok(unsigned long page)
{
	unsigned long *address = (unsigned long *) (page << UM_KERN_PAGE_SHIFT);
	unsigned long n = ~0UL;
	void *mapped = NULL;
	int ok = 0;

	/*
	 * First see if the page is readable.  If it is, it may still
	 * be a VDSO, so we go on to see if it's writable.  If not
	 * then try mapping memory there.  If that fails, then we're
	 * still in the kernel area.  As a sanity check, we'll fail if
	 * the mmap succeeds, but gives us an address different from
	 * what we wanted.
	 */
	if (setjmp(buf) == 0)
		n = *address;
	else {
		mapped = mmap(address, UM_KERN_PAGE_SIZE,
			      PROT_READ | PROT_WRITE,
			      MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (mapped == MAP_FAILED)
			return 0;
		if (mapped != address)
			goto out;
	}

	/*
	 * Now, is it writeable?  If so, then we're in user address
	 * space.  If not, then try mprotecting it and try the write
	 * again.
	 */
	if (setjmp(buf) == 0) {
		*address = n;
		ok = 1;
		goto out;
	} else if (mprotect(address, UM_KERN_PAGE_SIZE,
			    PROT_READ | PROT_WRITE) != 0)
		goto out;

	if (setjmp(buf) == 0) {
		*address = n;
		ok = 1;
	}

 out:
	if (mapped != NULL)
		munmap(mapped, UM_KERN_PAGE_SIZE);
	return ok;
}

unsigned long os_get_top_address(void)
{
	struct sigaction sa, old;
	unsigned long bottom = 0;
	/*
	 * A 32-bit UML on a 64-bit host gets confused about the VDSO at
	 * 0xffffe000.  It is mapped, is readable, can be reprotected writeable
	 * and written.  However, exec discovers later that it can't be
	 * unmapped.  So, just set the highest address to be checked to just
	 * below it.  This might waste some address space on 4G/4G 32-bit
	 * hosts, but shouldn't hurt otherwise.
	 */
	unsigned long top = 0xffffd000 >> UM_KERN_PAGE_SHIFT;
	unsigned long test, original;

	printf("Locating the bottom of the address space ... ");
	fflush(stdout);

	/*
	 * We're going to be longjmping out of the signal handler, so
	 * SA_DEFER needs to be set.
	 */
	sa.sa_handler = segfault;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_NODEFER;
	if (sigaction(SIGSEGV, &sa, &old)) {
		perror("os_get_top_address");
		exit(1);
	}

	/* Manually scan the address space, bottom-up, until we find
	 * the first valid page (or run out of them).
	 */
	for (bottom = 0; bottom < top; bottom++) {
		if (page_ok(bottom))
			break;
	}

	/* If we've got this far, we ran out of pages. */
	if (bottom == top) {
		fprintf(stderr, "Unable to determine bottom of address "
			"space.\n");
		exit(1);
	}

	printf("0x%x\n", bottom << UM_KERN_PAGE_SHIFT);
	printf("Locating the top of the address space ... ");
	fflush(stdout);

	original = bottom;

	/* This could happen with a 4G/4G split */
	if (page_ok(top))
		goto out;

	do {
		test = bottom + (top - bottom) / 2;
		if (page_ok(test))
			bottom = test;
		else
			top = test;
	} while (top - bottom > 1);

out:
	/* Restore the old SIGSEGV handling */
	if (sigaction(SIGSEGV, &old, NULL)) {
		perror("os_get_top_address");
		exit(1);
	}
	top <<= UM_KERN_PAGE_SHIFT;
	printf("0x%x\n", top);

	return top;
}
