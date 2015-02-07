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
 * drivers/net/ibm_newemac/zmii.h
 *
 * Driver for PowerPC 4xx on-chip ethernet controller, ZMII bridge support.
 *
 * Copyright 2007 Benjamin Herrenschmidt, IBM Corp.
 *                <benh@kernel.crashing.org>
 *
 * Based on the arch/ppc version of the driver:
 *
 * Copyright (c) 2004, 2005 Zultys Technologies.
 * Eugene Surovegin <eugene.surovegin@zultys.com> or <ebs@ebshome.net>
 *
 * Based on original work by
 *      Armin Kuster <akuster@mvista.com>
 * 	Copyright 2001 MontaVista Softare Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */
#ifndef __IBM_NEWEMAC_ZMII_H
#define __IBM_NEWEMAC_ZMII_H

/* ZMII bridge registers */
struct zmii_regs {
	u32 fer;		/* Function enable reg */
	u32 ssr;		/* Speed select reg */
	u32 smiirs;		/* SMII status reg */
};

/* ZMII device */
struct zmii_instance {
	struct zmii_regs __iomem	*base;

	/* Only one EMAC whacks us at a time */
	struct mutex			lock;

	/* subset of PHY_MODE_XXXX */
	int				mode;

	/* number of EMACs using this ZMII bridge */
	int				users;

	/* FER value left by firmware */
	u32				fer_save;

	/* OF device instance */
	struct of_device		*ofdev;
};

#ifdef CONFIG_IBM_NEW_EMAC_ZMII

extern int zmii_init(void);
extern void zmii_exit(void);
extern int zmii_attach(struct of_device *ofdev, int input, int *mode);
extern void zmii_detach(struct of_device *ofdev, int input);
extern void zmii_get_mdio(struct of_device *ofdev, int input);
extern void zmii_put_mdio(struct of_device *ofdev, int input);
extern void zmii_set_speed(struct of_device *ofdev, int input, int speed);
extern int zmii_get_regs_len(struct of_device *ocpdev);
extern void *zmii_dump_regs(struct of_device *ofdev, void *buf);

#else
# define zmii_init()		0
# define zmii_exit()		do { } while(0)
# define zmii_attach(x,y,z)	(-ENXIO)
# define zmii_detach(x,y)	do { } while(0)
# define zmii_get_mdio(x,y)	do { } while(0)
# define zmii_put_mdio(x,y)	do { } while(0)
# define zmii_set_speed(x,y,z)	do { } while(0)
# define zmii_get_regs_len(x)	0
# define zmii_dump_regs(x,buf)	(buf)
#endif				/* !CONFIG_IBM_NEW_EMAC_ZMII */

#endif /* __IBM_NEWEMAC_ZMII_H */
