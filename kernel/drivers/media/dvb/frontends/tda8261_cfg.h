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
	TDA8261 8PSK/QPSK tuner driver
	Copyright (C) Manu Abraham (abraham.manu@gmail.com)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

static int tda8261_get_frequency(struct dvb_frontend *fe, u32 *frequency)
{
	struct dvb_frontend_ops	*frontend_ops = NULL;
	struct dvb_tuner_ops	*tuner_ops = NULL;
	struct tuner_state	t_state;
	int err = 0;

	if (&fe->ops)
		frontend_ops = &fe->ops;
	if (&frontend_ops->tuner_ops)
		tuner_ops = &frontend_ops->tuner_ops;
	if (tuner_ops->get_state) {
		if ((err = tuner_ops->get_state(fe, DVBFE_TUNER_FREQUENCY, &t_state)) < 0) {
			printk("%s: Invalid parameter\n", __func__);
			return err;
		}
		*frequency = t_state.frequency;
		printk("%s: Frequency=%d\n", __func__, t_state.frequency);
	}
	return 0;
}

static int tda8261_set_frequency(struct dvb_frontend *fe, u32 frequency)
{
	struct dvb_frontend_ops	*frontend_ops = NULL;
	struct dvb_tuner_ops	*tuner_ops = NULL;
	struct tuner_state	t_state;
	int err = 0;

	t_state.frequency = frequency;
	if (&fe->ops)
		frontend_ops = &fe->ops;
	if (&frontend_ops->tuner_ops)
		tuner_ops = &frontend_ops->tuner_ops;
	if (tuner_ops->set_state) {
		if ((err = tuner_ops->set_state(fe, DVBFE_TUNER_FREQUENCY, &t_state)) < 0) {
			printk("%s: Invalid parameter\n", __func__);
			return err;
		}
	}
	printk("%s: Frequency=%d\n", __func__, t_state.frequency);
	return 0;
}

static int tda8261_get_bandwidth(struct dvb_frontend *fe, u32 *bandwidth)
{
	struct dvb_frontend_ops	*frontend_ops = &fe->ops;
	struct dvb_tuner_ops	*tuner_ops = &frontend_ops->tuner_ops;
	struct tuner_state	t_state;
	int err = 0;

	if (&fe->ops)
		frontend_ops = &fe->ops;
	if (&frontend_ops->tuner_ops)
		tuner_ops = &frontend_ops->tuner_ops;
	if (tuner_ops->get_state) {
		if ((err = tuner_ops->get_state(fe, DVBFE_TUNER_BANDWIDTH, &t_state)) < 0) {
			printk("%s: Invalid parameter\n", __func__);
			return err;
		}
		*bandwidth = t_state.bandwidth;
	}
	printk("%s: Bandwidth=%d\n", __func__, t_state.bandwidth);
	return 0;
}
