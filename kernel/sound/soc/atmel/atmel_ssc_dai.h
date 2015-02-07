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
 * atmel_ssc_dai.h - ALSA SSC interface for the Atmel  SoC
 *
 * Copyright (C) 2005 SAN People
 * Copyright (C) 2008 Atmel
 *
 * Author: Sedji Gaouaou <sedji.gaouaou@atmel.com>
 *         ATMEL CORP.
 *
 * Based on at91-ssc.c by
 * Frank Mandarino <fmandarino@endrelia.com>
 * Based on pxa2xx Platform drivers by
 * Liam Girdwood <lrg@slimlogic.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _ATMEL_SSC_DAI_H
#define _ATMEL_SSC_DAI_H

#include <linux/types.h>
#include <linux/atmel-ssc.h>

#include "atmel-pcm.h"

/* SSC system clock ids */
#define ATMEL_SYSCLK_MCK	0 /* SSC uses AT91 MCK as system clock */

/* SSC divider ids */
#define ATMEL_SSC_CMR_DIV	0 /* MCK divider for BCLK */
#define ATMEL_SSC_TCMR_PERIOD	1 /* BCLK divider for transmit FS */
#define ATMEL_SSC_RCMR_PERIOD	2 /* BCLK divider for receive FS */
/*
 * SSC direction masks
 */
#define SSC_DIR_MASK_UNUSED	0
#define SSC_DIR_MASK_PLAYBACK	1
#define SSC_DIR_MASK_CAPTURE	2

/*
 * SSC register values that Atmel left out of <linux/atmel-ssc.h>.  These
 * are expected to be used with SSC_BF
 */
/* START bit field values */
#define SSC_START_CONTINUOUS	0
#define SSC_START_TX_RX		1
#define SSC_START_LOW_RF	2
#define SSC_START_HIGH_RF	3
#define SSC_START_FALLING_RF	4
#define SSC_START_RISING_RF	5
#define SSC_START_LEVEL_RF	6
#define SSC_START_EDGE_RF	7
#define SSS_START_COMPARE_0	8

/* CKI bit field values */
#define SSC_CKI_FALLING		0
#define SSC_CKI_RISING		1

/* CKO bit field values */
#define SSC_CKO_NONE		0
#define SSC_CKO_CONTINUOUS	1
#define SSC_CKO_TRANSFER	2

/* CKS bit field values */
#define SSC_CKS_DIV		0
#define SSC_CKS_CLOCK		1
#define SSC_CKS_PIN		2

/* FSEDGE bit field values */
#define SSC_FSEDGE_POSITIVE	0
#define SSC_FSEDGE_NEGATIVE	1

/* FSOS bit field values */
#define SSC_FSOS_NONE		0
#define SSC_FSOS_NEGATIVE	1
#define SSC_FSOS_POSITIVE	2
#define SSC_FSOS_LOW		3
#define SSC_FSOS_HIGH		4
#define SSC_FSOS_TOGGLE		5

#define START_DELAY		1

struct atmel_ssc_state {
	u32 ssc_cmr;
	u32 ssc_rcmr;
	u32 ssc_rfmr;
	u32 ssc_tcmr;
	u32 ssc_tfmr;
	u32 ssc_sr;
	u32 ssc_imr;
};


struct atmel_ssc_info {
	char *name;
	struct ssc_device *ssc;
	spinlock_t lock;	/* lock for dir_mask */
	unsigned short dir_mask;	/* 0=unused, 1=playback, 2=capture */
	unsigned short initialized;	/* true if SSC has been initialized */
	unsigned short daifmt;
	unsigned short cmr_div;
	unsigned short tcmr_period;
	unsigned short rcmr_period;
	struct atmel_pcm_dma_params *dma_params[2];
	struct atmel_ssc_state ssc_state;
};
extern struct snd_soc_dai atmel_ssc_dai[];

#endif /* _AT91_SSC_DAI_H */
