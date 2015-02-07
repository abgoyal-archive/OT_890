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

#ifndef LLC_C_ST_H
#define LLC_C_ST_H
/*
 * Copyright (c) 1997 by Procom Technology,Inc.
 *		2001 by Arnaldo Carvalho de Melo <acme@conectiva.com.br>
 *
 * This program can be redistributed or modified under the terms of the
 * GNU General Public License as published by the Free Software Foundation.
 * This program is distributed without any warranty or implied warranty
 * of merchantability or fitness for a particular purpose.
 *
 * See the GNU General Public License for more details.
 */
/* Connection component state management */
/* connection states */
#define LLC_CONN_OUT_OF_SVC		 0	/* prior to allocation */
 
#define LLC_CONN_STATE_ADM		 1	/* disc, initial state */
#define LLC_CONN_STATE_SETUP		 2	/* disconnected state */
#define LLC_CONN_STATE_NORMAL		 3	/* connected state */
#define LLC_CONN_STATE_BUSY		 4	/* connected state */
#define LLC_CONN_STATE_REJ		 5	/* connected state */
#define LLC_CONN_STATE_AWAIT		 6	/* connected state */
#define LLC_CONN_STATE_AWAIT_BUSY	 7	/* connected state */
#define LLC_CONN_STATE_AWAIT_REJ	 8	/* connected state */
#define LLC_CONN_STATE_D_CONN		 9	/* disconnected state */
#define LLC_CONN_STATE_RESET		10	/* disconnected state */
#define LLC_CONN_STATE_ERROR		11	/* disconnected state */
#define LLC_CONN_STATE_TEMP		12	/* disconnected state */

#define NBR_CONN_STATES			12	/* size of state table */
#define NO_STATE_CHANGE			100

/* Connection state table structure */
struct llc_conn_state_trans {
	llc_conn_ev_t	   ev;
	u8		   next_state;
	llc_conn_ev_qfyr_t *ev_qualifiers;
	llc_conn_action_t  *ev_actions;
};

struct llc_conn_state {
	u8			    current_state;
	struct llc_conn_state_trans **transitions;
};

extern struct llc_conn_state llc_conn_state_table[];
#endif /* LLC_C_ST_H */
