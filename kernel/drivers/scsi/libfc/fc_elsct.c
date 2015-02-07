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
 * Copyright(c) 2008 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Maintained at www.Open-FCoE.org
 */

/*
 * Provide interface to send ELS/CT FC frames
 */

#include <asm/unaligned.h>
#include <scsi/fc/fc_gs.h>
#include <scsi/fc/fc_ns.h>
#include <scsi/fc/fc_els.h>
#include <scsi/libfc.h>
#include <scsi/fc_encode.h>

/*
 * fc_elsct_send - sends ELS/CT frame
 */
static struct fc_seq *fc_elsct_send(struct fc_lport *lport,
				    struct fc_rport *rport,
				    struct fc_frame *fp,
				    unsigned int op,
				    void (*resp)(struct fc_seq *,
						 struct fc_frame *fp,
						 void *arg),
				    void *arg, u32 timer_msec)
{
	enum fc_rctl r_ctl;
	u32 did;
	enum fc_fh_type fh_type;
	int rc;

	/* ELS requests */
	if ((op >= ELS_LS_RJT) && (op <= ELS_AUTH_ELS))
		rc = fc_els_fill(lport, rport, fp, op, &r_ctl, &did, &fh_type);
	else
		/* CT requests */
		rc = fc_ct_fill(lport, fp, op, &r_ctl, &did, &fh_type);

	if (rc)
		return NULL;

	fc_fill_fc_hdr(fp, r_ctl, did, fc_host_port_id(lport->host), fh_type,
		       FC_FC_FIRST_SEQ | FC_FC_END_SEQ | FC_FC_SEQ_INIT, 0);

	return lport->tt.exch_seq_send(lport, fp, resp, NULL, arg, timer_msec);
}

int fc_elsct_init(struct fc_lport *lport)
{
	if (!lport->tt.elsct_send)
		lport->tt.elsct_send = fc_elsct_send;

	return 0;
}
EXPORT_SYMBOL(fc_elsct_init);
