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
 * Copyright 2008 Cisco Systems, Inc.  All rights reserved.
 * Copyright 2007 Nuova Systems, Inc.  All rights reserved.
 *
 * This program is free software; you may redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef _CQ_DESC_H_
#define _CQ_DESC_H_

/*
 * Completion queue descriptor types
 */
enum cq_desc_types {
	CQ_DESC_TYPE_WQ_ENET = 0,
	CQ_DESC_TYPE_DESC_COPY = 1,
	CQ_DESC_TYPE_WQ_EXCH = 2,
	CQ_DESC_TYPE_RQ_ENET = 3,
	CQ_DESC_TYPE_RQ_FCP = 4,
};

/* Completion queue descriptor: 16B
 *
 * All completion queues have this basic layout.  The
 * type_specfic area is unique for each completion
 * queue type.
 */
struct cq_desc {
	__le16 completed_index;
	__le16 q_number;
	u8 type_specfic[11];
	u8 type_color;
};

#define CQ_DESC_TYPE_BITS        4
#define CQ_DESC_TYPE_MASK        ((1 << CQ_DESC_TYPE_BITS) - 1)
#define CQ_DESC_COLOR_MASK       1
#define CQ_DESC_COLOR_SHIFT      7
#define CQ_DESC_Q_NUM_BITS       10
#define CQ_DESC_Q_NUM_MASK       ((1 << CQ_DESC_Q_NUM_BITS) - 1)
#define CQ_DESC_COMP_NDX_BITS    12
#define CQ_DESC_COMP_NDX_MASK    ((1 << CQ_DESC_COMP_NDX_BITS) - 1)

static inline void cq_desc_dec(const struct cq_desc *desc_arg,
	u8 *type, u8 *color, u16 *q_number, u16 *completed_index)
{
	const struct cq_desc *desc = desc_arg;
	const u8 type_color = desc->type_color;

	*color = (type_color >> CQ_DESC_COLOR_SHIFT) & CQ_DESC_COLOR_MASK;

	/*
	 * Make sure color bit is read from desc *before* other fields
	 * are read from desc.  Hardware guarantees color bit is last
	 * bit (byte) written.  Adding the rmb() prevents the compiler
	 * and/or CPU from reordering the reads which would potentially
	 * result in reading stale values.
	 */

	rmb();

	*type = type_color & CQ_DESC_TYPE_MASK;
	*q_number = le16_to_cpu(desc->q_number) & CQ_DESC_Q_NUM_MASK;
	*completed_index = le16_to_cpu(desc->completed_index) &
		CQ_DESC_COMP_NDX_MASK;
}

#endif /* _CQ_DESC_H_ */
