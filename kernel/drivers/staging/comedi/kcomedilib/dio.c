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
    kcomedilib/dio.c
    implements comedi_dio_*() functions

    COMEDI - Linux Control and Measurement Device Interface
    Copyright (C) 2000 David A. Schleef <ds@schleef.org>

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

*/

#include "../comedi.h"
#include "../comedilib.h"

#include <linux/string.h>

int comedi_dio_config(comedi_t * dev, unsigned int subdev, unsigned int chan,
	unsigned int io)
{
	comedi_insn insn;

	memset(&insn, 0, sizeof(insn));
	insn.insn = INSN_CONFIG;
	insn.n = 1;
	insn.data = &io;
	insn.subdev = subdev;
	insn.chanspec = CR_PACK(chan, 0, 0);

	return comedi_do_insn(dev, &insn);
}

int comedi_dio_read(comedi_t * dev, unsigned int subdev, unsigned int chan,
	unsigned int *val)
{
	comedi_insn insn;

	memset(&insn, 0, sizeof(insn));
	insn.insn = INSN_READ;
	insn.n = 1;
	insn.data = val;
	insn.subdev = subdev;
	insn.chanspec = CR_PACK(chan, 0, 0);

	return comedi_do_insn(dev, &insn);
}

int comedi_dio_write(comedi_t * dev, unsigned int subdev, unsigned int chan,
	unsigned int val)
{
	comedi_insn insn;

	memset(&insn, 0, sizeof(insn));
	insn.insn = INSN_WRITE;
	insn.n = 1;
	insn.data = &val;
	insn.subdev = subdev;
	insn.chanspec = CR_PACK(chan, 0, 0);

	return comedi_do_insn(dev, &insn);
}

int comedi_dio_bitfield(comedi_t * dev, unsigned int subdev, unsigned int mask,
	unsigned int *bits)
{
	comedi_insn insn;
	lsampl_t data[2];
	int ret;

	memset(&insn, 0, sizeof(insn));
	insn.insn = INSN_BITS;
	insn.n = 2;
	insn.data = data;
	insn.subdev = subdev;

	data[0] = mask;
	data[1] = *bits;

	ret = comedi_do_insn(dev, &insn);

	*bits = data[1];

	return ret;
}
