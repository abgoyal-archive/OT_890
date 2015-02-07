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
 * drivers/i2c/busses/i2c-ibm_iic.h
 *
 * Support for the IIC peripheral on IBM PPC 4xx
 *
 * Copyright (c) 2003 Zultys Technologies.
 * Eugene Surovegin <eugene.surovegin@zultys.com> or <ebs@ebshome.net>
 *
 * Based on original work by
 * 	Ian DaSilva  <idasilva@mvista.com>
 *      Armin Kuster <akuster@mvista.com>
 * 	Matt Porter  <mporter@mvista.com>
 *
 *      Copyright 2000-2003 MontaVista Software Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */
#ifndef __I2C_IBM_IIC_H_
#define __I2C_IBM_IIC_H_

#include <linux/i2c.h>

struct iic_regs {
	u16 mdbuf;
	u16 sbbuf;
	u8 lmadr;
	u8 hmadr;
	u8 cntl;
	u8 mdcntl;
	u8 sts;
	u8 extsts;
	u8 lsadr;
	u8 hsadr;
	u8 clkdiv;
	u8 intmsk;
	u8 xfrcnt;
	u8 xtcntlss;
	u8 directcntl;
};

struct ibm_iic_private {
	struct i2c_adapter adap;
	volatile struct iic_regs __iomem *vaddr;
	wait_queue_head_t wq;
	int idx;
	int irq;
	int fast_mode;
	u8  clckdiv;
};

/* IICx_CNTL register */
#define CNTL_HMT	0x80
#define CNTL_AMD	0x40
#define CNTL_TCT_MASK	0x30
#define CNTL_TCT_SHIFT	4
#define CNTL_RPST	0x08
#define CNTL_CHT	0x04
#define CNTL_RW		0x02
#define CNTL_PT		0x01

/* IICx_MDCNTL register */
#define MDCNTL_FSDB	0x80
#define MDCNTL_FMDB	0x40
#define MDCNTL_EGC	0x20
#define MDCNTL_FSM	0x10
#define MDCNTL_ESM	0x08
#define MDCNTL_EINT	0x04
#define MDCNTL_EUBS	0x02
#define MDCNTL_HSCL	0x01

/* IICx_STS register */
#define STS_SSS		0x80
#define STS_SLPR	0x40
#define STS_MDBS	0x20
#define STS_MDBF	0x10
#define STS_SCMP	0x08
#define STS_ERR		0x04
#define STS_IRQA	0x02
#define STS_PT		0x01

/* IICx_EXTSTS register */
#define EXTSTS_IRQP	0x80
#define EXTSTS_BCS_MASK	0x70
#define   EXTSTS_BCS_FREE  0x40
#define EXTSTS_IRQD	0x08
#define EXTSTS_LA	0x04
#define EXTSTS_ICT	0x02
#define EXTSTS_XFRA	0x01

/* IICx_INTRMSK register */
#define INTRMSK_EIRC	0x80
#define INTRMSK_EIRS	0x40
#define INTRMSK_EIWC	0x20
#define INTRMSK_EIWS	0x10
#define INTRMSK_EIHE	0x08
#define INTRMSK_EIIC	0x04
#define INTRMSK_EITA	0x02
#define INTRMSK_EIMTC	0x01

/* IICx_XFRCNT register */
#define XFRCNT_MTC_MASK	0x07

/* IICx_XTCNTLSS register */
#define XTCNTLSS_SRC	0x80
#define XTCNTLSS_SRS	0x40
#define XTCNTLSS_SWC	0x20
#define XTCNTLSS_SWS	0x10
#define XTCNTLSS_SRST	0x01

/* IICx_DIRECTCNTL register */
#define DIRCNTL_SDAC	0x08
#define DIRCNTL_SCC	0x04
#define DIRCNTL_MSDA	0x02
#define DIRCNTL_MSC	0x01

/* Check if we really control the I2C bus and bus is free */
#define DIRCTNL_FREE(v)	(((v) & 0x0f) == 0x0f)

#endif /* __I2C_IBM_IIC_H_ */
