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
 * OHCI HCD (Host Controller Driver) for USB.
 *
 * (C) Copyright 1999 Roman Weissgaerber <weissg@vienna.at>
 * (C) Copyright 2000-2002 David Brownell <dbrownell@users.sourceforge.net>
 *
 * This file is licenced under the GPL.
 */

/*-------------------------------------------------------------------------*/

/*
 * OHCI deals with three types of memory:
 *	- data used only by the HCD ... kmalloc is fine
 *	- async and periodic schedules, shared by HC and HCD ... these
 *	  need to use dma_pool or dma_alloc_coherent
 *	- driver buffers, read/written by HC ... the hcd glue or the
 *	  device driver provides us with dma addresses
 *
 * There's also "register" data, which is memory mapped.
 * No memory seen by this driver (or any HCD) may be paged out.
 */

/*-------------------------------------------------------------------------*/

static void ohci_hcd_init (struct ohci_hcd *ohci)
{
	ohci->next_statechange = jiffies;
	spin_lock_init (&ohci->lock);
	INIT_LIST_HEAD (&ohci->pending);
}

/*-------------------------------------------------------------------------*/

static int ohci_mem_init (struct ohci_hcd *ohci)
{
	ohci->td_cache = dma_pool_create ("ohci_td",
		ohci_to_hcd(ohci)->self.controller,
		sizeof (struct td),
		32 /* byte alignment */,
		0 /* no page-crossing issues */);
	if (!ohci->td_cache)
		return -ENOMEM;
	ohci->ed_cache = dma_pool_create ("ohci_ed",
		ohci_to_hcd(ohci)->self.controller,
		sizeof (struct ed),
		16 /* byte alignment */,
		0 /* no page-crossing issues */);
	if (!ohci->ed_cache) {
		dma_pool_destroy (ohci->td_cache);
		return -ENOMEM;
	}
	return 0;
}

static void ohci_mem_cleanup (struct ohci_hcd *ohci)
{
	if (ohci->td_cache) {
		dma_pool_destroy (ohci->td_cache);
		ohci->td_cache = NULL;
	}
	if (ohci->ed_cache) {
		dma_pool_destroy (ohci->ed_cache);
		ohci->ed_cache = NULL;
	}
}

/*-------------------------------------------------------------------------*/

/* ohci "done list" processing needs this mapping */
static inline struct td *
dma_to_td (struct ohci_hcd *hc, dma_addr_t td_dma)
{
	struct td *td;

	td_dma &= TD_MASK;
	td = hc->td_hash [TD_HASH_FUNC(td_dma)];
	while (td && td->td_dma != td_dma)
		td = td->td_hash;
	return td;
}

/* TDs ... */
static struct td *
td_alloc (struct ohci_hcd *hc, gfp_t mem_flags)
{
	dma_addr_t	dma;
	struct td	*td;

	td = dma_pool_alloc (hc->td_cache, mem_flags, &dma);
	if (td) {
		/* in case hc fetches it, make it look dead */
		memset (td, 0, sizeof *td);
		td->hwNextTD = cpu_to_hc32 (hc, dma);
		td->td_dma = dma;
		/* hashed in td_fill */
	}
	return td;
}

static void
td_free (struct ohci_hcd *hc, struct td *td)
{
	struct td	**prev = &hc->td_hash [TD_HASH_FUNC (td->td_dma)];

	while (*prev && *prev != td)
		prev = &(*prev)->td_hash;
	if (*prev)
		*prev = td->td_hash;
	else if ((td->hwINFO & cpu_to_hc32(hc, TD_DONE)) != 0)
		ohci_dbg (hc, "no hash for td %p\n", td);
	dma_pool_free (hc->td_cache, td, td->td_dma);
}

/*-------------------------------------------------------------------------*/

/* EDs ... */
static struct ed *
ed_alloc (struct ohci_hcd *hc, gfp_t mem_flags)
{
	dma_addr_t	dma;
	struct ed	*ed;

	ed = dma_pool_alloc (hc->ed_cache, mem_flags, &dma);
	if (ed) {
		memset (ed, 0, sizeof (*ed));
		INIT_LIST_HEAD (&ed->td_list);
		ed->dma = dma;
	}
	return ed;
}

static void
ed_free (struct ohci_hcd *hc, struct ed *ed)
{
	dma_pool_free (hc->ed_cache, ed, ed->dma);
}

