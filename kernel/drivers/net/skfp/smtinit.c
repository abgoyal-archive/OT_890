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
	Init SMT
	call all module level initialization routines
*/

#include "h/types.h"
#include "h/fddi.h"
#include "h/smc.h"

#ifndef	lint
static const char ID_sccs[] = "@(#)smtinit.c	1.15 97/05/06 (C) SK " ;
#endif

void init_fddi_driver(struct s_smc *smc, u_char *mac_addr);

/* define global debug variable */
#if defined(DEBUG) && !defined(DEBUG_BRD)
struct smt_debug debug;
#endif

#ifndef MULT_OEM
#define OEMID(smc,i)	oem_id[i]
	extern u_char	oem_id[] ;
#else	/* MULT_OEM */
#define OEMID(smc,i)	smc->hw.oem_id->oi_mark[i]
	extern struct s_oem_ids	oem_ids[] ;
#endif	/* MULT_OEM */

/*
 * Set OEM specific values
 *
 * Can not be called in smt_reset_defaults, because it is not sure that
 * the OEM ID is already defined.
 */
static void set_oem_spec_val(struct s_smc *smc)
{
	struct fddi_mib *mib ;

	mib = &smc->mib ;

	/*
	 * set IBM specific values
	 */
	if (OEMID(smc,0) == 'I') {
		mib->fddiSMTConnectionPolicy = POLICY_MM ;
	}
}

/*
 * Init SMT
 */
int init_smt(struct s_smc *smc, u_char *mac_addr)
/* u_char *mac_addr;	canonical address or NULL */
{
	int	p ;

#if defined(DEBUG) && !defined(DEBUG_BRD)
	debug.d_smt = 0 ;
	debug.d_smtf = 0 ;
	debug.d_rmt = 0 ;
	debug.d_ecm = 0 ;
	debug.d_pcm = 0 ;
	debug.d_cfm = 0 ;

	debug.d_plc = 0 ;
#ifdef	ESS
	debug.d_ess = 0 ;
#endif
#ifdef	SBA
	debug.d_sba = 0 ;
#endif
#endif	/* DEBUG && !DEBUG_BRD */

	/* First initialize the ports mib->pointers */
	for ( p = 0; p < NUMPHYS; p ++ ) {
		smc->y[p].mib = & smc->mib.p[p] ;
	}

	set_oem_spec_val(smc) ;	
	(void) smt_set_mac_opvalues(smc) ;
	init_fddi_driver(smc,mac_addr) ;	/* HW driver */
	smt_fixup_mib(smc) ;		/* update values that depend on s.sas */

	ev_init(smc) ;			/* event queue */
#ifndef	SLIM_SMT
	smt_init_evc(smc) ;		/* evcs in MIB */
#endif	/* no SLIM_SMT */
	smt_timer_init(smc) ;		/* timer package */
	smt_agent_init(smc) ;		/* SMT frame manager */

	pcm_init(smc) ;			/* PCM state machine */
	ecm_init(smc) ;			/* ECM state machine */
	cfm_init(smc) ;			/* CFM state machine */
	rmt_init(smc) ;			/* RMT state machine */

	for (p = 0 ; p < NUMPHYS ; p++) {
		pcm(smc,p,0) ;		/* PCM A state machine */
	}
	ecm(smc,0) ;			/* ECM state machine */
	cfm(smc,0) ;			/* CFM state machine */
	rmt(smc,0) ;			/* RMT state machine */

	smt_agent_task(smc) ;		/* NIF FSM etc */

        PNMI_INIT(smc) ;                /* PNMI initialization */

	return(0) ;
}

