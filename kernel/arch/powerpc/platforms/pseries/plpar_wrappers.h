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

#ifndef _PSERIES_PLPAR_WRAPPERS_H
#define _PSERIES_PLPAR_WRAPPERS_H

#include <asm/hvcall.h>
#include <asm/page.h>

static inline long poll_pending(void)
{
	return plpar_hcall_norets(H_POLL_PENDING);
}

static inline long cede_processor(void)
{
	return plpar_hcall_norets(H_CEDE);
}

static inline long vpa_call(unsigned long flags, unsigned long cpu,
		unsigned long vpa)
{
	/* flags are in bits 16-18 (counting from most significant bit) */
	flags = flags << (63 - 18);

	return plpar_hcall_norets(H_REGISTER_VPA, flags, cpu, vpa);
}

static inline long unregister_vpa(unsigned long cpu, unsigned long vpa)
{
	return vpa_call(0x5, cpu, vpa);
}

static inline long register_vpa(unsigned long cpu, unsigned long vpa)
{
	return vpa_call(0x1, cpu, vpa);
}

static inline long unregister_slb_shadow(unsigned long cpu, unsigned long vpa)
{
	return vpa_call(0x7, cpu, vpa);
}

static inline long register_slb_shadow(unsigned long cpu, unsigned long vpa)
{
	return vpa_call(0x3, cpu, vpa);
}

static inline long plpar_page_set_loaned(unsigned long vpa)
{
	unsigned long cmo_page_sz = cmo_get_page_size();
	long rc = 0;
	int i;

	for (i = 0; !rc && i < PAGE_SIZE; i += cmo_page_sz)
		rc = plpar_hcall_norets(H_PAGE_INIT, H_PAGE_SET_LOANED, vpa + i, 0);

	for (i -= cmo_page_sz; rc && i != 0; i -= cmo_page_sz)
		plpar_hcall_norets(H_PAGE_INIT, H_PAGE_SET_ACTIVE,
				   vpa + i - cmo_page_sz, 0);

	return rc;
}

static inline long plpar_page_set_active(unsigned long vpa)
{
	unsigned long cmo_page_sz = cmo_get_page_size();
	long rc = 0;
	int i;

	for (i = 0; !rc && i < PAGE_SIZE; i += cmo_page_sz)
		rc = plpar_hcall_norets(H_PAGE_INIT, H_PAGE_SET_ACTIVE, vpa + i, 0);

	for (i -= cmo_page_sz; rc && i != 0; i -= cmo_page_sz)
		plpar_hcall_norets(H_PAGE_INIT, H_PAGE_SET_LOANED,
				   vpa + i - cmo_page_sz, 0);

	return rc;
}

extern void vpa_init(int cpu);

static inline long plpar_pte_enter(unsigned long flags,
		unsigned long hpte_group, unsigned long hpte_v,
		unsigned long hpte_r, unsigned long *slot)
{
	long rc;
	unsigned long retbuf[PLPAR_HCALL_BUFSIZE];

	rc = plpar_hcall(H_ENTER, retbuf, flags, hpte_group, hpte_v, hpte_r);

	*slot = retbuf[0];

	return rc;
}

static inline long plpar_pte_remove(unsigned long flags, unsigned long ptex,
		unsigned long avpn, unsigned long *old_pteh_ret,
		unsigned long *old_ptel_ret)
{
	long rc;
	unsigned long retbuf[PLPAR_HCALL_BUFSIZE];

	rc = plpar_hcall(H_REMOVE, retbuf, flags, ptex, avpn);

	*old_pteh_ret = retbuf[0];
	*old_ptel_ret = retbuf[1];

	return rc;
}

/* plpar_pte_remove_raw can be called in real mode. It calls plpar_hcall_raw */
static inline long plpar_pte_remove_raw(unsigned long flags, unsigned long ptex,
		unsigned long avpn, unsigned long *old_pteh_ret,
		unsigned long *old_ptel_ret)
{
	long rc;
	unsigned long retbuf[PLPAR_HCALL_BUFSIZE];

	rc = plpar_hcall_raw(H_REMOVE, retbuf, flags, ptex, avpn);

	*old_pteh_ret = retbuf[0];
	*old_ptel_ret = retbuf[1];

	return rc;
}

static inline long plpar_pte_read(unsigned long flags, unsigned long ptex,
		unsigned long *old_pteh_ret, unsigned long *old_ptel_ret)
{
	long rc;
	unsigned long retbuf[PLPAR_HCALL_BUFSIZE];

	rc = plpar_hcall(H_READ, retbuf, flags, ptex);

	*old_pteh_ret = retbuf[0];
	*old_ptel_ret = retbuf[1];

	return rc;
}

/* plpar_pte_read_raw can be called in real mode. It calls plpar_hcall_raw */
static inline long plpar_pte_read_raw(unsigned long flags, unsigned long ptex,
		unsigned long *old_pteh_ret, unsigned long *old_ptel_ret)
{
	long rc;
	unsigned long retbuf[PLPAR_HCALL_BUFSIZE];

	rc = plpar_hcall_raw(H_READ, retbuf, flags, ptex);

	*old_pteh_ret = retbuf[0];
	*old_ptel_ret = retbuf[1];

	return rc;
}

static inline long plpar_pte_protect(unsigned long flags, unsigned long ptex,
		unsigned long avpn)
{
	return plpar_hcall_norets(H_PROTECT, flags, ptex, avpn);
}

static inline long plpar_tce_get(unsigned long liobn, unsigned long ioba,
		unsigned long *tce_ret)
{
	long rc;
	unsigned long retbuf[PLPAR_HCALL_BUFSIZE];

	rc = plpar_hcall(H_GET_TCE, retbuf, liobn, ioba);

	*tce_ret = retbuf[0];

	return rc;
}

static inline long plpar_tce_put(unsigned long liobn, unsigned long ioba,
		unsigned long tceval)
{
	return plpar_hcall_norets(H_PUT_TCE, liobn, ioba, tceval);
}

static inline long plpar_tce_put_indirect(unsigned long liobn,
		unsigned long ioba, unsigned long page, unsigned long count)
{
	return plpar_hcall_norets(H_PUT_TCE_INDIRECT, liobn, ioba, page, count);
}

static inline long plpar_tce_stuff(unsigned long liobn, unsigned long ioba,
		unsigned long tceval, unsigned long count)
{
	return plpar_hcall_norets(H_STUFF_TCE, liobn, ioba, tceval, count);
}

static inline long plpar_get_term_char(unsigned long termno,
		unsigned long *len_ret, char *buf_ret)
{
	long rc;
	unsigned long retbuf[PLPAR_HCALL_BUFSIZE];
	unsigned long *lbuf = (unsigned long *)buf_ret;	/* TODO: alignment? */

	rc = plpar_hcall(H_GET_TERM_CHAR, retbuf, termno);

	*len_ret = retbuf[0];
	lbuf[0] = retbuf[1];
	lbuf[1] = retbuf[2];

	return rc;
}

static inline long plpar_put_term_char(unsigned long termno, unsigned long len,
		const char *buffer)
{
	unsigned long *lbuf = (unsigned long *)buffer;	/* TODO: alignment? */
	return plpar_hcall_norets(H_PUT_TERM_CHAR, termno, len, lbuf[0],
			lbuf[1]);
}

static inline long plpar_eoi(unsigned long xirr)
{
	return plpar_hcall_norets(H_EOI, xirr);
}

static inline long plpar_cppr(unsigned long cppr)
{
	return plpar_hcall_norets(H_CPPR, cppr);
}

static inline long plpar_ipi(unsigned long servernum, unsigned long mfrr)
{
	return plpar_hcall_norets(H_IPI, servernum, mfrr);
}

static inline long plpar_xirr(unsigned long *xirr_ret)
{
	long rc;
	unsigned long retbuf[PLPAR_HCALL_BUFSIZE];

	rc = plpar_hcall(H_XIRR, retbuf);

	*xirr_ret = retbuf[0];

	return rc;
}

#endif /* _PSERIES_PLPAR_WRAPPERS_H */
