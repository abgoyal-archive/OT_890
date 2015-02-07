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
 *  smpprim.h:  SMP locking primitives on the Sparc
 *
 *  God knows we won't be actually using this code for some time
 *  but I thought I'd write it since I knew how.
 *
 *  Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 */

#ifndef __SPARC_SMPPRIM_H
#define __SPARC_SMPPRIM_H

/* Test and set the unsigned byte at ADDR to 1.  Returns the previous
 * value.  On the Sparc we use the ldstub instruction since it is
 * atomic.
 */

static inline __volatile__ char test_and_set(void *addr)
{
	char state = 0;

	__asm__ __volatile__("ldstub [%0], %1         ! test_and_set\n\t"
			     "=r" (addr), "=r" (state) :
			     "0" (addr), "1" (state) : "memory");

	return state;
}

/* Initialize a spin-lock. */
static inline __volatile__ smp_initlock(void *spinlock)
{
	/* Unset the lock. */
	*((unsigned char *) spinlock) = 0;

	return;
}

/* This routine spins until it acquires the lock at ADDR. */
static inline __volatile__ smp_lock(void *addr)
{
	while(test_and_set(addr) == 0xff)
		;

	/* We now have the lock */
	return;
}

/* This routine releases the lock at ADDR. */
static inline __volatile__ smp_unlock(void *addr)
{
	*((unsigned char *) addr) = 0;
}

#endif /* !(__SPARC_SMPPRIM_H) */
