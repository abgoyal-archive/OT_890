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
 * FireDTV driver (formerly known as FireSAT)
 *
 * Copyright (C) 2004 Andreas Monitzer <andy@monitzer.com>
 * Copyright (C) 2008 Henrik Kurelid <henrik@kurelid.se>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation; either version 2 of
 *	the License, or (at your option) any later version.
 */

#include <linux/device.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/types.h>

#include <dvb_frontend.h>

#include "firedtv.h"

static int fdtv_dvb_init(struct dvb_frontend *fe)
{
	struct firedtv *fdtv = fe->sec_priv;
	int err;

	/* FIXME - allocate free channel at IRM */
	fdtv->isochannel = fdtv->adapter.num;

	err = cmp_establish_pp_connection(fdtv, fdtv->subunit,
					  fdtv->isochannel);
	if (err) {
		dev_err(fdtv->device,
			"could not establish point to point connection\n");
		return err;
	}

	return fdtv->backend->start_iso(fdtv);
}

static int fdtv_sleep(struct dvb_frontend *fe)
{
	struct firedtv *fdtv = fe->sec_priv;

	fdtv->backend->stop_iso(fdtv);
	cmp_break_pp_connection(fdtv, fdtv->subunit, fdtv->isochannel);
	fdtv->isochannel = -1;
	return 0;
}

#define LNBCONTROL_DONTCARE 0xff

static int fdtv_diseqc_send_master_cmd(struct dvb_frontend *fe,
				       struct dvb_diseqc_master_cmd *cmd)
{
	struct firedtv *fdtv = fe->sec_priv;

	return avc_lnb_control(fdtv, LNBCONTROL_DONTCARE, LNBCONTROL_DONTCARE,
			       LNBCONTROL_DONTCARE, 1, cmd);
}

static int fdtv_diseqc_send_burst(struct dvb_frontend *fe,
				  fe_sec_mini_cmd_t minicmd)
{
	return 0;
}

static int fdtv_set_tone(struct dvb_frontend *fe, fe_sec_tone_mode_t tone)
{
	struct firedtv *fdtv = fe->sec_priv;

	fdtv->tone = tone;
	return 0;
}

static int fdtv_set_voltage(struct dvb_frontend *fe,
			    fe_sec_voltage_t voltage)
{
	struct firedtv *fdtv = fe->sec_priv;

	fdtv->voltage = voltage;
	return 0;
}

static int fdtv_read_status(struct dvb_frontend *fe, fe_status_t *status)
{
	struct firedtv *fdtv = fe->sec_priv;
	struct firedtv_tuner_status stat;

	if (avc_tuner_status(fdtv, &stat))
		return -EINVAL;

	if (stat.no_rf)
		*status = 0;
	else
		*status = FE_HAS_SIGNAL | FE_HAS_VITERBI | FE_HAS_SYNC |
			  FE_HAS_CARRIER | FE_HAS_LOCK;
	return 0;
}

static int fdtv_read_ber(struct dvb_frontend *fe, u32 *ber)
{
	struct firedtv *fdtv = fe->sec_priv;
	struct firedtv_tuner_status stat;

	if (avc_tuner_status(fdtv, &stat))
		return -EINVAL;

	*ber = stat.ber;
	return 0;
}

static int fdtv_read_signal_strength(struct dvb_frontend *fe, u16 *strength)
{
	struct firedtv *fdtv = fe->sec_priv;
	struct firedtv_tuner_status stat;

	if (avc_tuner_status(fdtv, &stat))
		return -EINVAL;

	*strength = stat.signal_strength << 8;
	return 0;
}

static int fdtv_read_snr(struct dvb_frontend *fe, u16 *snr)
{
	struct firedtv *fdtv = fe->sec_priv;
	struct firedtv_tuner_status stat;

	if (avc_tuner_status(fdtv, &stat))
		return -EINVAL;

	/* C/N[dB] = -10 * log10(snr / 65535) */
	*snr = stat.carrier_noise_ratio * 257;
	return 0;
}

static int fdtv_read_uncorrected_blocks(struct dvb_frontend *fe, u32 *ucblocks)
{
	return -EOPNOTSUPP;
}

#define ACCEPTED 0x9

static int fdtv_set_frontend(struct dvb_frontend *fe,
			     struct dvb_frontend_parameters *params)
{
	struct firedtv *fdtv = fe->sec_priv;

	/* FIXME: avc_tuner_dsd never returns ACCEPTED. Check status? */
	if (avc_tuner_dsd(fdtv, params) != ACCEPTED)
		return -EINVAL;
	else
		return 0; /* not sure of this... */
}

static int fdtv_get_frontend(struct dvb_frontend *fe,
			     struct dvb_frontend_parameters *params)
{
	return -EOPNOTSUPP;
}

void fdtv_frontend_init(struct firedtv *fdtv)
{
	struct dvb_frontend_ops *ops = &fdtv->fe.ops;
	struct dvb_frontend_info *fi = &ops->info;

	ops->init			= fdtv_dvb_init;
	ops->sleep			= fdtv_sleep;

	ops->set_frontend		= fdtv_set_frontend;
	ops->get_frontend		= fdtv_get_frontend;

	ops->read_status		= fdtv_read_status;
	ops->read_ber			= fdtv_read_ber;
	ops->read_signal_strength	= fdtv_read_signal_strength;
	ops->read_snr			= fdtv_read_snr;
	ops->read_ucblocks		= fdtv_read_uncorrected_blocks;

	ops->diseqc_send_master_cmd 	= fdtv_diseqc_send_master_cmd;
	ops->diseqc_send_burst		= fdtv_diseqc_send_burst;
	ops->set_tone			= fdtv_set_tone;
	ops->set_voltage		= fdtv_set_voltage;

	switch (fdtv->type) {
	case FIREDTV_DVB_S:
	case FIREDTV_DVB_S2:
		fi->type		= FE_QPSK;

		fi->frequency_min	= 950000;
		fi->frequency_max	= 2150000;
		fi->frequency_stepsize	= 125;
		fi->symbol_rate_min	= 1000000;
		fi->symbol_rate_max	= 40000000;

		fi->caps 		= FE_CAN_INVERSION_AUTO	|
					  FE_CAN_FEC_1_2	|
					  FE_CAN_FEC_2_3	|
					  FE_CAN_FEC_3_4	|
					  FE_CAN_FEC_5_6	|
					  FE_CAN_FEC_7_8	|
					  FE_CAN_FEC_AUTO	|
					  FE_CAN_QPSK;
		break;

	case FIREDTV_DVB_C:
		fi->type		= FE_QAM;

		fi->frequency_min	= 47000000;
		fi->frequency_max	= 866000000;
		fi->frequency_stepsize	= 62500;
		fi->symbol_rate_min	= 870000;
		fi->symbol_rate_max	= 6900000;

		fi->caps 		= FE_CAN_INVERSION_AUTO |
					  FE_CAN_QAM_16		|
					  FE_CAN_QAM_32		|
					  FE_CAN_QAM_64		|
					  FE_CAN_QAM_128	|
					  FE_CAN_QAM_256	|
					  FE_CAN_QAM_AUTO;
		break;

	case FIREDTV_DVB_T:
		fi->type		= FE_OFDM;

		fi->frequency_min	= 49000000;
		fi->frequency_max	= 861000000;
		fi->frequency_stepsize	= 62500;

		fi->caps 		= FE_CAN_INVERSION_AUTO		|
					  FE_CAN_FEC_2_3		|
					  FE_CAN_TRANSMISSION_MODE_AUTO |
					  FE_CAN_GUARD_INTERVAL_AUTO	|
					  FE_CAN_HIERARCHY_AUTO;
		break;

	default:
		dev_err(fdtv->device, "no frontend for model type %d\n",
			fdtv->type);
	}
	strcpy(fi->name, fdtv_model_names[fdtv->type]);

	fdtv->fe.dvb = &fdtv->adapter;
	fdtv->fe.sec_priv = fdtv;
}
