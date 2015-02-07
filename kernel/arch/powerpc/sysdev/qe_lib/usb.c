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
 * QE USB routines
 *
 * Copyright (c) Freescale Semicondutor, Inc. 2006.
 *               Shlomi Gridish <gridish@freescale.com>
 *               Jerry Huang <Chang-Ming.Huang@freescale.com>
 * Copyright (c) MontaVista Software, Inc. 2008.
 *               Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/io.h>
#include <asm/immap_qe.h>
#include <asm/qe.h>

int qe_usb_clock_set(enum qe_clock clk, int rate)
{
	struct qe_mux __iomem *mux = &qe_immr->qmx;
	unsigned long flags;
	u32 val;

	switch (clk) {
	case QE_CLK3:  val = QE_CMXGCR_USBCS_CLK3;  break;
	case QE_CLK5:  val = QE_CMXGCR_USBCS_CLK5;  break;
	case QE_CLK7:  val = QE_CMXGCR_USBCS_CLK7;  break;
	case QE_CLK9:  val = QE_CMXGCR_USBCS_CLK9;  break;
	case QE_CLK13: val = QE_CMXGCR_USBCS_CLK13; break;
	case QE_CLK17: val = QE_CMXGCR_USBCS_CLK17; break;
	case QE_CLK19: val = QE_CMXGCR_USBCS_CLK19; break;
	case QE_CLK21: val = QE_CMXGCR_USBCS_CLK21; break;
	case QE_BRG9:  val = QE_CMXGCR_USBCS_BRG9;  break;
	case QE_BRG10: val = QE_CMXGCR_USBCS_BRG10; break;
	default:
		pr_err("%s: requested unknown clock %d\n", __func__, clk);
		return -EINVAL;
	}

	if (qe_clock_is_brg(clk))
		qe_setbrg(clk, rate, 1);

	spin_lock_irqsave(&cmxgcr_lock, flags);

	clrsetbits_be32(&mux->cmxgcr, QE_CMXGCR_USBCS, val);

	spin_unlock_irqrestore(&cmxgcr_lock, flags);

	return 0;
}
EXPORT_SYMBOL(qe_usb_clock_set);
