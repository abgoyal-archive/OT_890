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
 * promdevmap.c:  Map device/IO areas to virtual addresses.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>

#include <asm/openprom.h>
#include <asm/oplib.h>

extern void restore_current(void);

/* Just like the routines in palloc.c, these should not be used
 * by the kernel at all.  Bootloader facility mainly.  And again,
 * this is only available on V2 proms and above.
 */

/* Map physical device address 'paddr' in IO space 'ios' of size
 * 'num_bytes' to a virtual address, with 'vhint' being a hint to
 * the prom as to where you would prefer the mapping.  We return
 * where the prom actually mapped it.
 */
char *
prom_mapio(char *vhint, int ios, unsigned int paddr, unsigned int num_bytes)
{
	unsigned long flags;
	char *ret;

	spin_lock_irqsave(&prom_lock, flags);
	if((num_bytes == 0) || (paddr == 0)) ret = (char *) 0x0;
	else
	ret = (*(romvec->pv_v2devops.v2_dumb_mmap))(vhint, ios, paddr,
						    num_bytes);
	restore_current();
	spin_unlock_irqrestore(&prom_lock, flags);
	return ret;
}

/* Unmap an IO/device area that was mapped using the above routine. */
void
prom_unmapio(char *vaddr, unsigned int num_bytes)
{
	unsigned long flags;

	if(num_bytes == 0x0) return;
	spin_lock_irqsave(&prom_lock, flags);
	(*(romvec->pv_v2devops.v2_dumb_munmap))(vaddr, num_bytes);
	restore_current();
	spin_unlock_irqrestore(&prom_lock, flags);
	return;
}
