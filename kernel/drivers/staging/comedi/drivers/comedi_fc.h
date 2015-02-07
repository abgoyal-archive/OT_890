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
    comedi_fc.h

    This is a place for code driver writers wish to share between
    two or more drivers. These functions are meant to be used only
    by drivers, they are NOT part of the kcomedilib API!

    Author:  Frank Mori Hess <fmhess@users.sourceforge.net>
    Copyright (C) 2002 Frank Mori Hess

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

************************************************************************/

#ifndef _COMEDI_FC_H
#define _COMEDI_FC_H

#include "../comedidev.h"

/* Writes an array of data points to comedi's buffer */
extern unsigned int cfc_write_array_to_buffer(comedi_subdevice *subd,
					      void *data,
					      unsigned int num_bytes);

static inline unsigned int cfc_write_to_buffer(comedi_subdevice *subd,
					       sampl_t data)
{
	return cfc_write_array_to_buffer(subd, &data, sizeof(data));
};

static inline unsigned int cfc_write_long_to_buffer(comedi_subdevice *subd,
						    lsampl_t data)
{
	return cfc_write_array_to_buffer(subd, &data, sizeof(data));
};

extern unsigned int cfc_read_array_from_buffer(comedi_subdevice *subd,
					       void *data,
					       unsigned int num_bytes);

extern unsigned int cfc_handle_events(comedi_device *dev,
				      comedi_subdevice *subd);

static inline unsigned int cfc_bytes_per_scan(comedi_subdevice *subd)
{
	int num_samples;
	int bits_per_sample;

	switch (subd->type) {
	case COMEDI_SUBD_DI:
	case COMEDI_SUBD_DO:
	case COMEDI_SUBD_DIO:
		bits_per_sample = 8 * bytes_per_sample(subd);
		num_samples = (subd->async->cmd.chanlist_len +
			       bits_per_sample - 1) / bits_per_sample;
		break;
	default:
		num_samples = subd->async->cmd.chanlist_len;
		break;
	}
	return num_samples * bytes_per_sample(subd);
}

#endif /* _COMEDI_FC_H */
