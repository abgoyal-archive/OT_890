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
 * Copyright(c) 2007 Intel Corporation. All rights reserved.
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

#ifndef _FC_GS_H_
#define	_FC_GS_H_

/*
 * Fibre Channel Services - Common Transport.
 * From T11.org FC-GS-2 Rev 5.3 November 1998.
 */

struct fc_ct_hdr {
	__u8		ct_rev;		/* revision */
	__u8		ct_in_id[3];	/* N_Port ID of original requestor */
	__u8		ct_fs_type;	/* type of fibre channel service */
	__u8		ct_fs_subtype;	/* subtype */
	__u8		ct_options;
	__u8		_ct_resvd1;
	__be16		ct_cmd;		/* command / response code */
	__be16		ct_mr_size;	/* maximum / residual size */
	__u8		_ct_resvd2;
	__u8		ct_reason;	/* reject reason */
	__u8		ct_explan;	/* reason code explanation */
	__u8		ct_vendor;	/* vendor unique data */
};

#define	FC_CT_HDR_LEN	16	/* expected sizeof (struct fc_ct_hdr) */

enum fc_ct_rev {
	FC_CT_REV = 1		/* common transport revision */
};

/*
 * ct_fs_type values.
 */
enum fc_ct_fs_type {
	FC_FST_ALIAS =	0xf8,	/* alias service */
	FC_FST_MGMT =	0xfa,	/* management service */
	FC_FST_TIME =	0xfb,	/* time service */
	FC_FST_DIR =	0xfc,	/* directory service */
};

/*
 * ct_cmd: Command / response codes
 */
enum fc_ct_cmd {
	FC_FS_RJT =	0x8001,	/* reject */
	FC_FS_ACC =	0x8002,	/* accept */
};

/*
 * FS_RJT reason codes.
 */
enum fc_ct_reason {
	FC_FS_RJT_CMD =		0x01,	/* invalid command code */
	FC_FS_RJT_VER =		0x02,	/* invalid version level */
	FC_FS_RJT_LOG =		0x03,	/* logical error */
	FC_FS_RJT_IUSIZ =	0x04,	/* invalid IU size */
	FC_FS_RJT_BSY =		0x05,	/* logical busy */
	FC_FS_RJT_PROTO =	0x07,	/* protocol error */
	FC_FS_RJT_UNABL =	0x09,	/* unable to perform command request */
	FC_FS_RJT_UNSUP =	0x0b,	/* command not supported */
};

/*
 * FS_RJT reason code explanations.
 */
enum fc_ct_explan {
	FC_FS_EXP_NONE =	0x00,	/* no additional explanation */
	FC_FS_EXP_PID =		0x01,	/* port ID not registered */
	FC_FS_EXP_PNAM =	0x02,	/* port name not registered */
	FC_FS_EXP_NNAM =	0x03,	/* node name not registered */
	FC_FS_EXP_COS =		0x04,	/* class of service not registered */
	/* definitions not complete */
};

#endif /* _FC_GS_H_ */
