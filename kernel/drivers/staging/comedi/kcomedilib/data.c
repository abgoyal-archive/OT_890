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
    kcomedilib/data.c
    implements comedi_data_*() functions

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
#include "../comedidev.h"	/* for comedi_udelay() */

#include <linux/string.h>

int comedi_data_write(comedi_t * dev, unsigned int subdev, unsigned int chan,
	unsigned int range, unsigned int aref, lsampl_t data)
{
	comedi_insn insn;

	memset(&insn, 0, sizeof(insn));
	insn.insn = INSN_WRITE;
	insn.n = 1;
	insn.data = &data;
	insn.subdev = subdev;
	insn.chanspec = CR_PACK(chan, range, aref);

	return comedi_do_insn(dev, &insn);
}

int comedi_data_read(comedi_t * dev, unsigned int subdev, unsigned int chan,
	unsigned int range, unsigned int aref, lsampl_t * data)
{
	comedi_insn insn;

	memset(&insn, 0, sizeof(insn));
	insn.insn = INSN_READ;
	insn.n = 1;
	insn.data = data;
	insn.subdev = subdev;
	insn.chanspec = CR_PACK(chan, range, aref);

	return comedi_do_insn(dev, &insn);
}

int comedi_data_read_hint(comedi_t * dev, unsigned int subdev,
	unsigned int chan, unsigned int range, unsigned int aref)
{
	comedi_insn insn;
	lsampl_t dummy_data;

	memset(&insn, 0, sizeof(insn));
	insn.insn = INSN_READ;
	insn.n = 0;
	insn.data = &dummy_data;
	insn.subdev = subdev;
	insn.chanspec = CR_PACK(chan, range, aref);

	return comedi_do_insn(dev, &insn);
}

int comedi_data_read_delayed(comedi_t * dev, unsigned int subdev,
	unsigned int chan, unsigned int range, unsigned int aref,
	lsampl_t * data, unsigned int nano_sec)
{
	int retval;

	retval = comedi_data_read_hint(dev, subdev, chan, range, aref);
	if (retval < 0)
		return retval;

	comedi_udelay((nano_sec + 999) / 1000);

	return comedi_data_read(dev, subdev, chan, range, aref, data);
}
