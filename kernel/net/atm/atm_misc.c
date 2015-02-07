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

/* net/atm/atm_misc.c - Various functions for use by ATM drivers */

/* Written 1995-2000 by Werner Almesberger, EPFL ICA */


#include <linux/module.h>
#include <linux/atm.h>
#include <linux/atmdev.h>
#include <linux/skbuff.h>
#include <linux/sonet.h>
#include <linux/bitops.h>
#include <asm/atomic.h>
#include <asm/errno.h>


int atm_charge(struct atm_vcc *vcc,int truesize)
{
	atm_force_charge(vcc,truesize);
	if (atomic_read(&sk_atm(vcc)->sk_rmem_alloc) <= sk_atm(vcc)->sk_rcvbuf)
		return 1;
	atm_return(vcc,truesize);
	atomic_inc(&vcc->stats->rx_drop);
	return 0;
}


struct sk_buff *atm_alloc_charge(struct atm_vcc *vcc,int pdu_size,
    gfp_t gfp_flags)
{
	struct sock *sk = sk_atm(vcc);
	int guess = atm_guess_pdu2truesize(pdu_size);

	atm_force_charge(vcc,guess);
	if (atomic_read(&sk->sk_rmem_alloc) <= sk->sk_rcvbuf) {
		struct sk_buff *skb = alloc_skb(pdu_size,gfp_flags);

		if (skb) {
			atomic_add(skb->truesize-guess,
				   &sk->sk_rmem_alloc);
			return skb;
		}
	}
	atm_return(vcc,guess);
	atomic_inc(&vcc->stats->rx_drop);
	return NULL;
}


/*
 * atm_pcr_goal returns the positive PCR if it should be rounded up, the
 * negative PCR if it should be rounded down, and zero if the maximum available
 * bandwidth should be used.
 *
 * The rules are as follows (* = maximum, - = absent (0), x = value "x",
 * (x+ = x or next value above x, x- = x or next value below):
 *
 *	min max pcr	result		min max pcr	result
 *	-   -   -	* (UBR only)	x   -   -	x+
 *	-   -   *	*		x   -   *	*
 *	-   -   z	z-		x   -   z	z-
 *	-   *   -	*		x   *   -	x+
 *	-   *   *	*		x   *   *	*
 *	-   *   z	z-		x   *   z	z-
 *	-   y   -	y-		x   y   -	x+
 *	-   y   *	y-		x   y   *	y-
 *	-   y   z	z-		x   y   z	z-
 *
 * All non-error cases can be converted with the following simple set of rules:
 *
 *   if pcr == z then z-
 *   else if min == x && pcr == - then x+
 *     else if max == y then y-
 *	 else *
 */


int atm_pcr_goal(const struct atm_trafprm *tp)
{
	if (tp->pcr && tp->pcr != ATM_MAX_PCR)
		return -tp->pcr;
	if (tp->min_pcr && !tp->pcr)
		return tp->min_pcr;
	if (tp->max_pcr != ATM_MAX_PCR)
		return -tp->max_pcr;
	return 0;
}


void sonet_copy_stats(struct k_sonet_stats *from,struct sonet_stats *to)
{
#define __HANDLE_ITEM(i) to->i = atomic_read(&from->i)
	__SONET_ITEMS
#undef __HANDLE_ITEM
}


void sonet_subtract_stats(struct k_sonet_stats *from,struct sonet_stats *to)
{
#define __HANDLE_ITEM(i) atomic_sub(to->i,&from->i)
	__SONET_ITEMS
#undef __HANDLE_ITEM
}


EXPORT_SYMBOL(atm_charge);
EXPORT_SYMBOL(atm_alloc_charge);
EXPORT_SYMBOL(atm_pcr_goal);
EXPORT_SYMBOL(sonet_copy_stats);
EXPORT_SYMBOL(sonet_subtract_stats);
