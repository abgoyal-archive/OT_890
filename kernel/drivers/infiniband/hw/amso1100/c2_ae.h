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
 * Copyright (c) 2005 Ammasso, Inc. All rights reserved.
 * Copyright (c) 2005 Open Grid Computing, Inc. All rights reserved.
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
 */
#ifndef _C2_AE_H_
#define _C2_AE_H_

/*
 * WARNING: If you change this file, also bump C2_IVN_BASE
 * in common/include/clustercore/c2_ivn.h.
 */

/*
 * Asynchronous Event Identifiers
 *
 * These start at 0x80 only so it's obvious from inspection that
 * they are not work-request statuses.  This isn't critical.
 *
 * NOTE: these event id's must fit in eight bits.
 */
enum c2_event_id {
	CCAE_REMOTE_SHUTDOWN = 0x80,
	CCAE_ACTIVE_CONNECT_RESULTS,
	CCAE_CONNECTION_REQUEST,
	CCAE_LLP_CLOSE_COMPLETE,
	CCAE_TERMINATE_MESSAGE_RECEIVED,
	CCAE_LLP_CONNECTION_RESET,
	CCAE_LLP_CONNECTION_LOST,
	CCAE_LLP_SEGMENT_SIZE_INVALID,
	CCAE_LLP_INVALID_CRC,
	CCAE_LLP_BAD_FPDU,
	CCAE_INVALID_DDP_VERSION,
	CCAE_INVALID_RDMA_VERSION,
	CCAE_UNEXPECTED_OPCODE,
	CCAE_INVALID_DDP_QUEUE_NUMBER,
	CCAE_RDMA_READ_NOT_ENABLED,
	CCAE_RDMA_WRITE_NOT_ENABLED,
	CCAE_RDMA_READ_TOO_SMALL,
	CCAE_NO_L_BIT,
	CCAE_TAGGED_INVALID_STAG,
	CCAE_TAGGED_BASE_BOUNDS_VIOLATION,
	CCAE_TAGGED_ACCESS_RIGHTS_VIOLATION,
	CCAE_TAGGED_INVALID_PD,
	CCAE_WRAP_ERROR,
	CCAE_BAD_CLOSE,
	CCAE_BAD_LLP_CLOSE,
	CCAE_INVALID_MSN_RANGE,
	CCAE_INVALID_MSN_GAP,
	CCAE_IRRQ_OVERFLOW,
	CCAE_IRRQ_MSN_GAP,
	CCAE_IRRQ_MSN_RANGE,
	CCAE_IRRQ_INVALID_STAG,
	CCAE_IRRQ_BASE_BOUNDS_VIOLATION,
	CCAE_IRRQ_ACCESS_RIGHTS_VIOLATION,
	CCAE_IRRQ_INVALID_PD,
	CCAE_IRRQ_WRAP_ERROR,
	CCAE_CQ_SQ_COMPLETION_OVERFLOW,
	CCAE_CQ_RQ_COMPLETION_ERROR,
	CCAE_QP_SRQ_WQE_ERROR,
	CCAE_QP_LOCAL_CATASTROPHIC_ERROR,
	CCAE_CQ_OVERFLOW,
	CCAE_CQ_OPERATION_ERROR,
	CCAE_SRQ_LIMIT_REACHED,
	CCAE_QP_RQ_LIMIT_REACHED,
	CCAE_SRQ_CATASTROPHIC_ERROR,
	CCAE_RNIC_CATASTROPHIC_ERROR
/* WARNING If you add more id's, make sure their values fit in eight bits. */
};

/*
 * Resource Indicators and Identifiers
 */
enum c2_resource_indicator {
	C2_RES_IND_QP = 1,
	C2_RES_IND_EP,
	C2_RES_IND_CQ,
	C2_RES_IND_SRQ,
};

#endif /* _C2_AE_H_ */
