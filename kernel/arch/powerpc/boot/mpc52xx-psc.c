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
 * MPC5200 PSC serial console support.
 *
 * Author: Grant Likely <grant.likely@secretlab.ca>
 *
 * Copyright (c) 2007 Secret Lab Technologies Ltd.
 * Copyright (c) 2007 Freescale Semiconductor, Inc.
 *
 * It is assumed that the firmware (or the platform file) has already set
 * up the port.
 */

#include "types.h"
#include "io.h"
#include "ops.h"

/* Programmable Serial Controller (PSC) status register bits */
#define MPC52xx_PSC_SR		0x04
#define MPC52xx_PSC_SR_RXRDY		0x0100
#define MPC52xx_PSC_SR_RXFULL		0x0200
#define MPC52xx_PSC_SR_TXRDY		0x0400
#define MPC52xx_PSC_SR_TXEMP		0x0800

#define MPC52xx_PSC_BUFFER	0x0C

static void *psc;

static int psc_open(void)
{
	/* Assume the firmware has already configured the PSC into
	 * uart mode */
	return 0;
}

static void psc_putc(unsigned char c)
{
	while (!(in_be16(psc + MPC52xx_PSC_SR) & MPC52xx_PSC_SR_TXRDY)) ;
	out_8(psc + MPC52xx_PSC_BUFFER, c);
}

static unsigned char psc_tstc(void)
{
	return (in_be16(psc + MPC52xx_PSC_SR) & MPC52xx_PSC_SR_RXRDY) != 0;
}

static unsigned char psc_getc(void)
{
	while (!(in_be16(psc + MPC52xx_PSC_SR) & MPC52xx_PSC_SR_RXRDY)) ;
	return in_8(psc + MPC52xx_PSC_BUFFER);
}

int mpc5200_psc_console_init(void *devp, struct serial_console_data *scdp)
{
	/* Get the base address of the psc registers */
	if (dt_get_virtual_reg(devp, &psc, 1) < 1)
		return -1;

	scdp->open = psc_open;
	scdp->putc = psc_putc;
	scdp->getc = psc_getc;
	scdp->tstc = psc_tstc;

	return 0;
}
