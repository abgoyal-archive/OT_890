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
 * Functions for registration of I/O interruption subclasses on s390.
 *
 * Copyright IBM Corp. 2008
 * Authors: Sebastian Ott <sebott@linux.vnet.ibm.com>
 */

#include <linux/spinlock.h>
#include <linux/module.h>
#include <asm/isc.h>

static unsigned int isc_refs[MAX_ISC + 1];
static DEFINE_SPINLOCK(isc_ref_lock);


/**
 * isc_register - register an I/O interruption subclass.
 * @isc: I/O interruption subclass to register
 *
 * The number of users for @isc is increased. If this is the first user to
 * register @isc, the corresponding I/O interruption subclass mask is enabled.
 *
 * Context:
 *   This function must not be called in interrupt context.
 */
void isc_register(unsigned int isc)
{
	if (isc > MAX_ISC) {
		WARN_ON(1);
		return;
	}

	spin_lock(&isc_ref_lock);
	if (isc_refs[isc] == 0)
		ctl_set_bit(6, 31 - isc);
	isc_refs[isc]++;
	spin_unlock(&isc_ref_lock);
}
EXPORT_SYMBOL_GPL(isc_register);

/**
 * isc_unregister - unregister an I/O interruption subclass.
 * @isc: I/O interruption subclass to unregister
 *
 * The number of users for @isc is decreased. If this is the last user to
 * unregister @isc, the corresponding I/O interruption subclass mask is
 * disabled.
 * Note: This function must not be called if isc_register() hasn't been called
 * before by the driver for @isc.
 *
 * Context:
 *   This function must not be called in interrupt context.
 */
void isc_unregister(unsigned int isc)
{
	spin_lock(&isc_ref_lock);
	/* check for misuse */
	if (isc > MAX_ISC || isc_refs[isc] == 0) {
		WARN_ON(1);
		goto out_unlock;
	}
	if (isc_refs[isc] == 1)
		ctl_clear_bit(6, 31 - isc);
	isc_refs[isc]--;
out_unlock:
	spin_unlock(&isc_ref_lock);
}
EXPORT_SYMBOL_GPL(isc_unregister);
