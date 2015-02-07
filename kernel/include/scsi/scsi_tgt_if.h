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
 * SCSI target kernel/user interface
 *
 * Copyright (C) 2005 FUJITA Tomonori <tomof@acm.org>
 * Copyright (C) 2005 Mike Christie <michaelc@cs.wisc.edu>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */
#ifndef __SCSI_TARGET_IF_H
#define __SCSI_TARGET_IF_H

/* user -> kernel */
#define	TGT_UEVENT_CMD_RSP		0x0001
#define	TGT_UEVENT_IT_NEXUS_RSP		0x0002
#define	TGT_UEVENT_TSK_MGMT_RSP		0x0003

/* kernel -> user */
#define	TGT_KEVENT_CMD_REQ		0x1001
#define	TGT_KEVENT_CMD_DONE		0x1002
#define	TGT_KEVENT_IT_NEXUS_REQ		0x1003
#define	TGT_KEVENT_TSK_MGMT_REQ		0x1004

struct tgt_event_hdr {
	uint16_t version;
	uint16_t status;
	uint16_t type;
	uint16_t len;
} __attribute__ ((aligned (sizeof(uint64_t))));

struct tgt_event {
	struct tgt_event_hdr hdr;

	union {
		/* user-> kernel */
		struct {
			int host_no;
			int result;
			aligned_u64 itn_id;
			aligned_u64 tag;
			aligned_u64 uaddr;
			aligned_u64 sense_uaddr;
			uint32_t len;
			uint32_t sense_len;
			uint8_t rw;
		} cmd_rsp;
		struct {
			int host_no;
			int result;
			aligned_u64 itn_id;
			aligned_u64 mid;
		} tsk_mgmt_rsp;
		struct {
			__s32 host_no;
			__s32 result;
			aligned_u64 itn_id;
			__u32 function;
		} it_nexus_rsp;

		/* kernel -> user */
		struct {
			int host_no;
			uint32_t data_len;
			aligned_u64 itn_id;
			uint8_t scb[16];
			uint8_t lun[8];
			int attribute;
			aligned_u64 tag;
		} cmd_req;
		struct {
			int host_no;
			int result;
			aligned_u64 itn_id;
			aligned_u64 tag;
		} cmd_done;
		struct {
			int host_no;
			int function;
			aligned_u64 itn_id;
			aligned_u64 tag;
			uint8_t lun[8];
			aligned_u64 mid;
		} tsk_mgmt_req;
		struct {
			__s32 host_no;
			__u32 function;
			aligned_u64 itn_id;
			__u32 max_cmds;
			__u8 initiator_id[16];
		} it_nexus_req;
	} p;
} __attribute__ ((aligned (sizeof(uint64_t))));

#define TGT_RING_SIZE (1UL << 16)

#endif
