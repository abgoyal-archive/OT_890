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

/* $Id$ */

#ifndef __DIVA_USER_MODE_XDI_H__
#define __DIVA_USER_MODE_XDI_H__

/*
  Contains declaratiom of structures shared between application
  and user mode idi driver
*/

typedef struct _diva_um_idi_adapter_features {
	dword type;
	dword features;
	dword channels;
	dword serial_number;
	char name[128];
} diva_um_idi_adapter_features_t;

#define DIVA_UM_IDI_REQ_MASK			0x0000FFFF
#define DIVA_UM_IDI_REQ_TYPE_MASK		(~(DIVA_UM_IDI_REQ_MASK))
#define DIVA_UM_IDI_GET_FEATURES		1	/* trigger features indication */
#define DIVA_UM_IDI_REQ				2
#define DIVA_UM_IDI_REQ_TYPE_MAN		0x10000000
#define DIVA_UM_IDI_REQ_TYPE_SIG		0x20000000
#define DIVA_UM_IDI_REQ_TYPE_NET		0x30000000
#define DIVA_UM_IDI_REQ_MAN			(DIVA_UM_IDI_REQ | DIVA_UM_IDI_REQ_TYPE_MAN)
#define DIVA_UM_IDI_REQ_SIG			(DIVA_UM_IDI_REQ | DIVA_UM_IDI_REQ_TYPE_SIG)
#define DIVA_UM_IDI_REQ_NET			(DIVA_UM_IDI_REQ | DIVA_UM_IDI_REQ_TYPE_NET)
/*
  data_length  bytes will follow this structure
*/
typedef struct _diva_um_idi_req_hdr {
	dword type;
	dword Req;
	dword ReqCh;
	dword data_length;
} diva_um_idi_req_hdr_t;

typedef struct _diva_um_idi_ind_parameters {
	dword Ind;
	dword IndCh;
} diva_um_idi_ind_parameters_t;

typedef struct _diva_um_idi_rc_parameters {
	dword Rc;
	dword RcCh;
} diva_um_idi_rc_parameters_t;

typedef union _diva_um_idi_ind {
	diva_um_idi_adapter_features_t features;
	diva_um_idi_ind_parameters_t ind;
	diva_um_idi_rc_parameters_t rc;
} diva_um_idi_ind_t;

#define DIVA_UM_IDI_IND_FEATURES  1	/* features indication */
#define DIVA_UM_IDI_IND           2
#define DIVA_UM_IDI_IND_RC        3
/*
  data_length bytes of data follow
  this structure
*/
typedef struct _diva_um_idi_ind_hdr {
	dword type;
	diva_um_idi_ind_t hdr;
	dword data_length;
} diva_um_idi_ind_hdr_t;

#endif
