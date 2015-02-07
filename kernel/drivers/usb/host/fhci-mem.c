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
 * Freescale QUICC Engine USB Host Controller Driver
 *
 * Copyright (c) Freescale Semicondutor, Inc. 2006.
 *               Shlomi Gridish <gridish@freescale.com>
 *               Jerry Huang <Chang-Ming.Huang@freescale.com>
 * Copyright (c) Logic Product Development, Inc. 2007
 *               Peter Barada <peterb@logicpd.com>
 * Copyright (c) MontaVista Software, Inc. 2008.
 *               Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/list.h>
#include <linux/usb.h>
#include "../core/hcd.h"
#include "fhci.h"

static void init_td(struct td *td)
{
	memset(td, 0, sizeof(*td));
	INIT_LIST_HEAD(&td->node);
	INIT_LIST_HEAD(&td->frame_lh);
}

static void init_ed(struct ed *ed)
{
	memset(ed, 0, sizeof(*ed));
	INIT_LIST_HEAD(&ed->td_list);
	INIT_LIST_HEAD(&ed->node);
}

static struct td *get_empty_td(struct fhci_hcd *fhci)
{
	struct td *td;

	if (!list_empty(&fhci->empty_tds)) {
		td = list_entry(fhci->empty_tds.next, struct td, node);
		list_del(fhci->empty_tds.next);
	} else {
		td = kmalloc(sizeof(*td), GFP_ATOMIC);
		if (!td)
			fhci_err(fhci, "No memory to allocate to TD\n");
		else
			init_td(td);
	}

	return td;
}

void fhci_recycle_empty_td(struct fhci_hcd *fhci, struct td *td)
{
	init_td(td);
	list_add(&td->node, &fhci->empty_tds);
}

struct ed *fhci_get_empty_ed(struct fhci_hcd *fhci)
{
	struct ed *ed;

	if (!list_empty(&fhci->empty_eds)) {
		ed = list_entry(fhci->empty_eds.next, struct ed, node);
		list_del(fhci->empty_eds.next);
	} else {
		ed = kmalloc(sizeof(*ed), GFP_ATOMIC);
		if (!ed)
			fhci_err(fhci, "No memory to allocate to ED\n");
		else
			init_ed(ed);
	}

	return ed;
}

void fhci_recycle_empty_ed(struct fhci_hcd *fhci, struct ed *ed)
{
	init_ed(ed);
	list_add(&ed->node, &fhci->empty_eds);
}

struct td *fhci_td_fill(struct fhci_hcd *fhci, struct urb *urb,
			struct urb_priv *urb_priv, struct ed *ed, u16 index,
			enum fhci_ta_type type, int toggle, u8 *data, u32 len,
			u16 interval, u16 start_frame, bool ioc)
{
	struct td *td = get_empty_td(fhci);

	if (!td)
		return NULL;

	td->urb = urb;
	td->ed = ed;
	td->type = type;
	td->toggle = toggle;
	td->data = data;
	td->len = len;
	td->iso_index = index;
	td->interval = interval;
	td->start_frame = start_frame;
	td->ioc = ioc;
	td->status = USB_TD_OK;

	urb_priv->tds[index] = td;

	return td;
}
