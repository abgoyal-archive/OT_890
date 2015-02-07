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
 *  Definitions for the SCOOP interface found on various Sharp PDAs
 *
 *  Copyright (c) 2004 Richard Purdie
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 */

#define SCOOP_MCR  0x00
#define SCOOP_CDR  0x04
#define SCOOP_CSR  0x08
#define SCOOP_CPR  0x0C
#define SCOOP_CCR  0x10
#define SCOOP_IRR  0x14
#define SCOOP_IRM  0x14
#define SCOOP_IMR  0x18
#define SCOOP_ISR  0x1C
#define SCOOP_GPCR 0x20
#define SCOOP_GPWR 0x24
#define SCOOP_GPRR 0x28

#define SCOOP_GPCR_PA22	( 1 << 12 )
#define SCOOP_GPCR_PA21	( 1 << 11 )
#define SCOOP_GPCR_PA20	( 1 << 10 )
#define SCOOP_GPCR_PA19	( 1 << 9 )
#define SCOOP_GPCR_PA18	( 1 << 8 )
#define SCOOP_GPCR_PA17	( 1 << 7 )
#define SCOOP_GPCR_PA16	( 1 << 6 )
#define SCOOP_GPCR_PA15	( 1 << 5 )
#define SCOOP_GPCR_PA14	( 1 << 4 )
#define SCOOP_GPCR_PA13	( 1 << 3 )
#define SCOOP_GPCR_PA12	( 1 << 2 )
#define SCOOP_GPCR_PA11	( 1 << 1 )

struct scoop_config {
	unsigned short io_out;
	unsigned short io_dir;
	unsigned short suspend_clr;
	unsigned short suspend_set;
	int gpio_base;
};

/* Structure for linking scoop devices to PCMCIA sockets */
struct scoop_pcmcia_dev {
	struct device *dev;     /* Pointer to this socket's scoop device */
	int	irq;                /* irq for socket */
	int cd_irq;
	const char *cd_irq_str;
	unsigned char keep_vs;
	unsigned char keep_rd;
};

struct scoop_pcmcia_config {
	struct scoop_pcmcia_dev *devs;
	int num_devs;
	void (*pcmcia_init)(void);
	void (*power_ctrl)(struct device *scoop, unsigned short cpr, int nr);
};

extern struct scoop_pcmcia_config *platform_scoop_config;

void reset_scoop(struct device *dev);
unsigned short __deprecated set_scoop_gpio(struct device *dev, unsigned short bit);
unsigned short __deprecated reset_scoop_gpio(struct device *dev, unsigned short bit);
unsigned short read_scoop_reg(struct device *dev, unsigned short reg);
void write_scoop_reg(struct device *dev, unsigned short reg, unsigned short data);
