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
 *  mxl5007t.h - driver for the MaxLinear MxL5007T silicon tuner
 *
 *  Copyright (C) 2008 Michael Krufky <mkrufky@linuxtv.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __MXL5007T_H__
#define __MXL5007T_H__

#include "dvb_frontend.h"

/* ------------------------------------------------------------------------- */

enum mxl5007t_if_freq {
	MxL_IF_4_MHZ,      /*  4000000 */
	MxL_IF_4_5_MHZ,    /*  4500000 */
	MxL_IF_4_57_MHZ,   /*  4570000 */
	MxL_IF_5_MHZ,      /*  5000000 */
	MxL_IF_5_38_MHZ,   /*  5380000 */
	MxL_IF_6_MHZ,      /*  6000000 */
	MxL_IF_6_28_MHZ,   /*  6280000 */
	MxL_IF_9_1915_MHZ, /*  9191500 */
	MxL_IF_35_25_MHZ,  /* 35250000 */
	MxL_IF_36_15_MHZ,  /* 36150000 */
	MxL_IF_44_MHZ,     /* 44000000 */
};

enum mxl5007t_xtal_freq {
	MxL_XTAL_16_MHZ,      /* 16000000 */
	MxL_XTAL_20_MHZ,      /* 20000000 */
	MxL_XTAL_20_25_MHZ,   /* 20250000 */
	MxL_XTAL_20_48_MHZ,   /* 20480000 */
	MxL_XTAL_24_MHZ,      /* 24000000 */
	MxL_XTAL_25_MHZ,      /* 25000000 */
	MxL_XTAL_25_14_MHZ,   /* 25140000 */
	MxL_XTAL_27_MHZ,      /* 27000000 */
	MxL_XTAL_28_8_MHZ,    /* 28800000 */
	MxL_XTAL_32_MHZ,      /* 32000000 */
	MxL_XTAL_40_MHZ,      /* 40000000 */
	MxL_XTAL_44_MHZ,      /* 44000000 */
	MxL_XTAL_48_MHZ,      /* 48000000 */
	MxL_XTAL_49_3811_MHZ, /* 49381100 */
};

enum mxl5007t_clkout_amp {
	MxL_CLKOUT_AMP_0_94V = 0,
	MxL_CLKOUT_AMP_0_53V = 1,
	MxL_CLKOUT_AMP_0_37V = 2,
	MxL_CLKOUT_AMP_0_28V = 3,
	MxL_CLKOUT_AMP_0_23V = 4,
	MxL_CLKOUT_AMP_0_20V = 5,
	MxL_CLKOUT_AMP_0_17V = 6,
	MxL_CLKOUT_AMP_0_15V = 7,
};

struct mxl5007t_config {
	s32 if_diff_out_level;
	enum mxl5007t_clkout_amp clk_out_amp;
	enum mxl5007t_xtal_freq xtal_freq_hz;
	enum mxl5007t_if_freq if_freq_hz;
	unsigned int invert_if:1;
	unsigned int loop_thru_enable:1;
	unsigned int clk_out_enable:1;
};

#if defined(CONFIG_MEDIA_TUNER_MXL5007T) || (defined(CONFIG_MEDIA_TUNER_MXL5007T_MODULE) && defined(MODULE))
extern struct dvb_frontend *mxl5007t_attach(struct dvb_frontend *fe,
					    struct i2c_adapter *i2c, u8 addr,
					    struct mxl5007t_config *cfg);
#else
static inline struct dvb_frontend *mxl5007t_attach(struct dvb_frontend *fe,
						   struct i2c_adapter *i2c,
						   u8 addr,
						   struct mxl5007t_config *cfg)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}
#endif

#endif /* __MXL5007T_H__ */

/*
 * Overrides for Emacs so that we follow Linus's tabbing style.
 * ---------------------------------------------------------------------------
 * Local variables:
 * c-basic-offset: 8
 * End:
 */

