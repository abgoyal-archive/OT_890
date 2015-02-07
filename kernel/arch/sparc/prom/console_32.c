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
 * console.c: Routines that deal with sending and receiving IO
 *            to/from the current console device using the PROM.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 * Copyright (C) 1998 Pete Zaitcev <zaitcev@yahoo.com>
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm/openprom.h>
#include <asm/oplib.h>
#include <asm/system.h>
#include <linux/string.h>

extern void restore_current(void);

/* Non blocking get character from console input device, returns -1
 * if no input was taken.  This can be used for polling.
 */
int
prom_nbgetchar(void)
{
	static char inc;
	int i = -1;
	unsigned long flags;

	spin_lock_irqsave(&prom_lock, flags);
	switch(prom_vers) {
	case PROM_V0:
		i = (*(romvec->pv_nbgetchar))();
		break;
	case PROM_V2:
	case PROM_V3:
		if( (*(romvec->pv_v2devops).v2_dev_read)(*romvec->pv_v2bootargs.fd_stdin , &inc, 0x1) == 1) {
			i = inc;
		} else {
			i = -1;
		}
		break;
	default:
		i = -1;
		break;
	};
	restore_current();
	spin_unlock_irqrestore(&prom_lock, flags);
	return i; /* Ugh, we could spin forever on unsupported proms ;( */
}

/* Non blocking put character to console device, returns -1 if
 * unsuccessful.
 */
int
prom_nbputchar(char c)
{
	static char outc;
	unsigned long flags;
	int i = -1;

	spin_lock_irqsave(&prom_lock, flags);
	switch(prom_vers) {
	case PROM_V0:
		i = (*(romvec->pv_nbputchar))(c);
		break;
	case PROM_V2:
	case PROM_V3:
		outc = c;
		if( (*(romvec->pv_v2devops).v2_dev_write)(*romvec->pv_v2bootargs.fd_stdout, &outc, 0x1) == 1)
			i = 0;
		else
			i = -1;
		break;
	default:
		i = -1;
		break;
	};
	restore_current();
	spin_unlock_irqrestore(&prom_lock, flags);
	return i; /* Ugh, we could spin forever on unsupported proms ;( */
}

/* Blocking version of get character routine above. */
char
prom_getchar(void)
{
	int character;
	while((character = prom_nbgetchar()) == -1) ;
	return (char) character;
}

/* Blocking version of put character routine above. */
void
prom_putchar(char c)
{
	while(prom_nbputchar(c) == -1) ;
	return;
}
