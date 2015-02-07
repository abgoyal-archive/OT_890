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

/******************************************************************************
 *
 *	(C)Copyright 1998,1999 SysKonnect,
 *	a business unit of Schneider & Koch & Co. Datensysteme GmbH.
 *
 *	See the file "skfddi.c" for further information.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	The information in this file is provided "AS IS" without warranty.
 *
 ******************************************************************************/

/*
	SMT timer
*/

#include "h/types.h"
#include "h/fddi.h"
#include "h/smc.h"

#ifndef	lint
static const char ID_sccs[] = "@(#)smttimer.c	2.4 97/08/04 (C) SK " ;
#endif

static void timer_done(struct s_smc *smc, int restart);

void smt_timer_init(struct s_smc *smc)
{
	smc->t.st_queue = NULL;
	smc->t.st_fast.tm_active = FALSE ;
	smc->t.st_fast.tm_next = NULL;
	hwt_init(smc) ;
}

void smt_timer_stop(struct s_smc *smc, struct smt_timer *timer)
{
	struct smt_timer	**prev ;
	struct smt_timer	*tm ;

	/*
	 * remove timer from queue
	 */
	timer->tm_active = FALSE ;
	if (smc->t.st_queue == timer && !timer->tm_next) {
		hwt_stop(smc) ;
	}
	for (prev = &smc->t.st_queue ; (tm = *prev) ; prev = &tm->tm_next ) {
		if (tm == timer) {
			*prev = tm->tm_next ;
			if (tm->tm_next) {
				tm->tm_next->tm_delta += tm->tm_delta ;
			}
			return ;
		}
	}
}

void smt_timer_start(struct s_smc *smc, struct smt_timer *timer, u_long time,
		     u_long token)
{
	struct smt_timer	**prev ;
	struct smt_timer	*tm ;
	u_long			delta = 0 ;

	time /= 16 ;		/* input is uS, clock ticks are 16uS */
	if (!time)
		time = 1 ;
	smt_timer_stop(smc,timer) ;
	timer->tm_smc = smc ;
	timer->tm_token = token ;
	timer->tm_active = TRUE ;
	if (!smc->t.st_queue) {
		smc->t.st_queue = timer ;
		timer->tm_next = NULL;
		timer->tm_delta = time ;
		hwt_start(smc,time) ;
		return ;
	}
	/*
	 * timer correction
	 */
	timer_done(smc,0) ;

	/*
	 * find position in queue
	 */
	delta = 0 ;
	for (prev = &smc->t.st_queue ; (tm = *prev) ; prev = &tm->tm_next ) {
		if (delta + tm->tm_delta > time) {
			break ;
		}
		delta += tm->tm_delta ;
	}
	/* insert in queue */
	*prev = timer ;
	timer->tm_next = tm ;
	timer->tm_delta = time - delta ;
	if (tm)
		tm->tm_delta -= timer->tm_delta ;
	/*
	 * start new with first
	 */
	hwt_start(smc,smc->t.st_queue->tm_delta) ;
}

void smt_force_irq(struct s_smc *smc)
{
	smt_timer_start(smc,&smc->t.st_fast,32L, EV_TOKEN(EVENT_SMT,SM_FAST)); 
}

void smt_timer_done(struct s_smc *smc)
{
	timer_done(smc,1) ;
}

static void timer_done(struct s_smc *smc, int restart)
{
	u_long			delta ;
	struct smt_timer	*tm ;
	struct smt_timer	*next ;
	struct smt_timer	**last ;
	int			done = 0 ;

	delta = hwt_read(smc) ;
	last = &smc->t.st_queue ;
	tm = smc->t.st_queue ;
	while (tm && !done) {
		if (delta >= tm->tm_delta) {
			tm->tm_active = FALSE ;
			delta -= tm->tm_delta ;
			last = &tm->tm_next ;
			tm = tm->tm_next ;
		}
		else {
			tm->tm_delta -= delta ;
			delta = 0 ;
			done = 1 ;
		}
	}
	*last = NULL;
	next = smc->t.st_queue ;
	smc->t.st_queue = tm ;

	for ( tm = next ; tm ; tm = next) {
		next = tm->tm_next ;
		timer_event(smc,tm->tm_token) ;
	}

	if (restart && smc->t.st_queue)
		hwt_start(smc,smc->t.st_queue->tm_delta) ;
}

