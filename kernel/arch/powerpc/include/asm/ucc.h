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
 * Copyright (C) 2006 Freescale Semicondutor, Inc. All rights reserved.
 *
 * Authors: 	Shlomi Gridish <gridish@freescale.com>
 * 		Li Yang <leoli@freescale.com>
 *
 * Description:
 * Internal header file for UCC unit routines.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */
#ifndef __UCC_H__
#define __UCC_H__

#include <asm/immap_qe.h>
#include <asm/qe.h>

#define STATISTICS

#define UCC_MAX_NUM	8

/* Slow or fast type for UCCs.
*/
enum ucc_speed_type {
	UCC_SPEED_TYPE_FAST = UCC_GUEMR_MODE_FAST_RX | UCC_GUEMR_MODE_FAST_TX,
	UCC_SPEED_TYPE_SLOW = UCC_GUEMR_MODE_SLOW_RX | UCC_GUEMR_MODE_SLOW_TX
};

/* ucc_set_type
 * Sets UCC to slow or fast mode.
 *
 * ucc_num - (In) number of UCC (0-7).
 * speed   - (In) slow or fast mode for UCC.
 */
int ucc_set_type(unsigned int ucc_num, enum ucc_speed_type speed);

int ucc_set_qe_mux_mii_mng(unsigned int ucc_num);

int ucc_set_qe_mux_rxtx(unsigned int ucc_num, enum qe_clock clock,
	enum comm_dir mode);

int ucc_mux_set_grant_tsa_bkpt(unsigned int ucc_num, int set, u32 mask);

/* QE MUX clock routing for UCC
*/
static inline int ucc_set_qe_mux_grant(unsigned int ucc_num, int set)
{
	return ucc_mux_set_grant_tsa_bkpt(ucc_num, set, QE_CMXUCR_GRANT);
}

static inline int ucc_set_qe_mux_tsa(unsigned int ucc_num, int set)
{
	return ucc_mux_set_grant_tsa_bkpt(ucc_num, set, QE_CMXUCR_TSA);
}

static inline int ucc_set_qe_mux_bkpt(unsigned int ucc_num, int set)
{
	return ucc_mux_set_grant_tsa_bkpt(ucc_num, set, QE_CMXUCR_BKPT);
}

#endif				/* __UCC_H__ */
