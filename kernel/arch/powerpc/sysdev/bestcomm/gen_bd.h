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
 * Header for Bestcomm General Buffer Descriptor tasks driver
 *
 *
 * Copyright (C) 2007 Sylvain Munaut <tnt@246tNt.com>
 * Copyright (C) 2006 AppSpec Computer Technologies Corp.
 *                    Jeff Gibbons <jeff.gibbons@appspec.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 *
 */

#ifndef __BESTCOMM_GEN_BD_H__
#define __BESTCOMM_GEN_BD_H__

struct bcom_gen_bd {
	u32	status;
	u32	buf_pa;
};


extern struct bcom_task *
bcom_gen_bd_rx_init(int queue_len, phys_addr_t fifo,
			int initiator, int ipr, int maxbufsize);

extern int
bcom_gen_bd_rx_reset(struct bcom_task *tsk);

extern void
bcom_gen_bd_rx_release(struct bcom_task *tsk);


extern struct bcom_task *
bcom_gen_bd_tx_init(int queue_len, phys_addr_t fifo,
			int initiator, int ipr);

extern int
bcom_gen_bd_tx_reset(struct bcom_task *tsk);

extern void
bcom_gen_bd_tx_release(struct bcom_task *tsk);


/* PSC support utility wrappers */
struct bcom_task * bcom_psc_gen_bd_rx_init(unsigned psc_num, int queue_len,
					   phys_addr_t fifo, int maxbufsize);
struct bcom_task * bcom_psc_gen_bd_tx_init(unsigned psc_num, int queue_len,
					   phys_addr_t fifo);
#endif  /* __BESTCOMM_GEN_BD_H__ */

