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
 * Copyright (c) 2004 Mellanox Technologies Ltd.  All rights reserved.
 * Copyright (c) 2004 Infinicon Corporation.  All rights reserved.
 * Copyright (c) 2004 Intel Corporation.  All rights reserved.
 * Copyright (c) 2004 Topspin Corporation.  All rights reserved.
 * Copyright (c) 2004-2007 Voltaire Corporation.  All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
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

#ifndef __SMI_H_
#define __SMI_H_

#include <rdma/ib_smi.h>

enum smi_action {
	IB_SMI_DISCARD,
	IB_SMI_HANDLE
};

enum smi_forward_action {
	IB_SMI_LOCAL,	/* SMP should be completed up the stack */
	IB_SMI_SEND,	/* received DR SMP should be forwarded to the send queue */
	IB_SMI_FORWARD	/* SMP should be forwarded (for switches only) */
};

enum smi_action smi_handle_dr_smp_recv(struct ib_smp *smp, u8 node_type,
				       int port_num, int phys_port_cnt);
int smi_get_fwd_port(struct ib_smp *smp);
extern enum smi_forward_action smi_check_forward_dr_smp(struct ib_smp *smp);
extern enum smi_action smi_handle_dr_smp_send(struct ib_smp *smp,
					      u8 node_type, int port_num);

/*
 * Return IB_SMI_HANDLE if the SMP should be handled by the local SMA/SM
 * via process_mad
 */
static inline enum smi_action smi_check_local_smp(struct ib_smp *smp,
						  struct ib_device *device)
{
	/* C14-9:3 -- We're at the end of the DR segment of path */
	/* C14-9:4 -- Hop Pointer = Hop Count + 1 -> give to SMA/SM */
	return ((device->process_mad &&
		!ib_get_smp_direction(smp) &&
		(smp->hop_ptr == smp->hop_cnt + 1)) ?
		IB_SMI_HANDLE : IB_SMI_DISCARD);
}

/*
 * Return IB_SMI_HANDLE if the SMP should be handled by the local SMA/SM
 * via process_mad
 */
static inline enum smi_action smi_check_local_returning_smp(struct ib_smp *smp,
						   struct ib_device *device)
{
	/* C14-13:3 -- We're at the end of the DR segment of path */
	/* C14-13:4 -- Hop Pointer == 0 -> give to SM */
	return ((device->process_mad &&
		ib_get_smp_direction(smp) &&
		!smp->hop_ptr) ? IB_SMI_HANDLE : IB_SMI_DISCARD);
}

#endif	/* __SMI_H_ */
