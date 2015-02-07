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
 * Header for Bestcomm FEC tasks driver
 *
 *
 * Copyright (C) 2006-2007 Sylvain Munaut <tnt@246tNt.com>
 * Copyright (C) 2003-2004 MontaVista, Software, Inc.
 *                         ( by Dale Farnsworth <dfarnsworth@mvista.com> )
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

#ifndef __BESTCOMM_FEC_H__
#define __BESTCOMM_FEC_H__


struct bcom_fec_bd {
	u32	status;
	u32	skb_pa;
};

#define BCOM_FEC_TX_BD_TFD	0x08000000ul	/* transmit frame done */
#define BCOM_FEC_TX_BD_TC	0x04000000ul	/* transmit CRC */
#define BCOM_FEC_TX_BD_ABC	0x02000000ul	/* append bad CRC */

#define BCOM_FEC_RX_BD_L	0x08000000ul	/* buffer is last in frame */
#define BCOM_FEC_RX_BD_BC	0x00800000ul	/* DA is broadcast */
#define BCOM_FEC_RX_BD_MC	0x00400000ul	/* DA is multicast and not broadcast */
#define BCOM_FEC_RX_BD_LG	0x00200000ul	/* Rx frame length violation */
#define BCOM_FEC_RX_BD_NO	0x00100000ul	/* Rx non-octet aligned frame */
#define BCOM_FEC_RX_BD_CR	0x00040000ul	/* Rx CRC error */
#define BCOM_FEC_RX_BD_OV	0x00020000ul	/* overrun */
#define BCOM_FEC_RX_BD_TR	0x00010000ul	/* Rx frame truncated */
#define BCOM_FEC_RX_BD_LEN_MASK	0x000007fful	/* mask for length of received frame */
#define BCOM_FEC_RX_BD_ERRORS	(BCOM_FEC_RX_BD_LG | BCOM_FEC_RX_BD_NO | \
		BCOM_FEC_RX_BD_CR | BCOM_FEC_RX_BD_OV | BCOM_FEC_RX_BD_TR)


extern struct bcom_task *
bcom_fec_rx_init(int queue_len, phys_addr_t fifo, int maxbufsize);

extern int
bcom_fec_rx_reset(struct bcom_task *tsk);

extern void
bcom_fec_rx_release(struct bcom_task *tsk);


extern struct bcom_task *
bcom_fec_tx_init(int queue_len, phys_addr_t fifo);

extern int
bcom_fec_tx_reset(struct bcom_task *tsk);

extern void
bcom_fec_tx_release(struct bcom_task *tsk);


#endif /* __BESTCOMM_FEC_H__ */

