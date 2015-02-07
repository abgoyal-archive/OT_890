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

#ifndef _LMC_H_
#define _LMC_H_

#include "lmc_var.h"

/*
 * prototypes for everyone
 */
int lmc_probe(struct net_device * dev);
unsigned lmc_mii_readreg(lmc_softc_t * const sc, unsigned
      			  devaddr, unsigned regno);
void lmc_mii_writereg(lmc_softc_t * const sc, unsigned devaddr,
			       unsigned regno, unsigned data);
void lmc_led_on(lmc_softc_t * const, u32);
void lmc_led_off(lmc_softc_t * const, u32);
unsigned lmc_mii_readreg(lmc_softc_t * const, unsigned, unsigned);
void lmc_mii_writereg(lmc_softc_t * const, unsigned, unsigned, unsigned);
void lmc_gpio_mkinput(lmc_softc_t * const sc, u32 bits);
void lmc_gpio_mkoutput(lmc_softc_t * const sc, u32 bits);

int lmc_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd);

extern lmc_media_t lmc_ds3_media;
extern lmc_media_t lmc_ssi_media;
extern lmc_media_t lmc_t1_media;
extern lmc_media_t lmc_hssi_media;

#ifdef _DBG_EVENTLOG
static void lmcEventLog(u32 EventNum, u32 arg2, u32 arg3);
#endif

#endif
