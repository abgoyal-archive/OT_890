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

/* ------------------------------------------------------------------------ *
 * i2c-parport.h I2C bus over parallel port                                 *
 * ------------------------------------------------------------------------ *
   Copyright (C) 2003-2004 Jean Delvare <khali@linux-fr.org>
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * ------------------------------------------------------------------------ */

#ifdef DATA
#undef DATA
#endif

#define DATA	0
#define STAT	1
#define CTRL	2

struct lineop {
	u8 val;
	u8 port;
	u8 inverted;
};

struct adapter_parm {
	struct lineop setsda;
	struct lineop setscl;
	struct lineop getsda;
	struct lineop getscl;
	struct lineop init;
};

static struct adapter_parm adapter_parm[] = {
	/* type 0: Philips adapter */
	{
		.setsda	= { 0x80, DATA, 1 },
		.setscl	= { 0x08, CTRL, 0 },
		.getsda	= { 0x80, STAT, 0 },
		.getscl	= { 0x08, STAT, 0 },
	},
	/* type 1: home brew teletext adapter */
	{
		.setsda	= { 0x02, DATA, 0 },
		.setscl	= { 0x01, DATA, 0 },
		.getsda	= { 0x80, STAT, 1 },
	},
	/* type 2: Velleman K8000 adapter */
	{
		.setsda	= { 0x02, CTRL, 1 },
		.setscl	= { 0x08, CTRL, 1 },
		.getsda	= { 0x10, STAT, 0 },
	},
	/* type 3: ELV adapter */
	{
		.setsda	= { 0x02, DATA, 1 },
		.setscl	= { 0x01, DATA, 1 },
		.getsda	= { 0x40, STAT, 1 },
		.getscl	= { 0x08, STAT, 1 },
	},
	/* type 4: ADM1032 evaluation board */
	{
		.setsda	= { 0x02, DATA, 1 },
		.setscl	= { 0x01, DATA, 1 },
		.getsda	= { 0x10, STAT, 1 },
		.init	= { 0xf0, DATA, 0 },
	},
	/* type 5: ADM1025, ADM1030 and ADM1031 evaluation boards */
	{
		.setsda	= { 0x02, DATA, 1 },
		.setscl	= { 0x01, DATA, 1 },
		.getsda	= { 0x10, STAT, 1 },
	},
	/* type 6: Barco LPT->DVI (K5800236) adapter */
	{
		.setsda	= { 0x02, DATA, 1 },
		.setscl	= { 0x01, DATA, 1 },
		.getsda	= { 0x20, STAT, 0 },
		.getscl	= { 0x40, STAT, 0 },
		.init	= { 0xfc, DATA, 0 },
	},
	/* type 7: One For All JP1 parallel port adapter */
	{
		.setsda	= { 0x01, DATA, 0 },
		.setscl	= { 0x02, DATA, 0 },
		.getsda	= { 0x80, STAT, 1 },
		.init	= { 0x04, DATA, 1 },
	},
};

static int type = -1;
module_param(type, int, 0);
MODULE_PARM_DESC(type,
	"Type of adapter:\n"
	" 0 = Philips adapter\n"
	" 1 = home brew teletext adapter\n"
	" 2 = Velleman K8000 adapter\n"
	" 3 = ELV adapter\n"
	" 4 = ADM1032 evaluation board\n"
	" 5 = ADM1025, ADM1030 and ADM1031 evaluation boards\n"
	" 6 = Barco LPT->DVI (K5800236) adapter\n"
	" 7 = One For All JP1 parallel port adapter\n"
);
