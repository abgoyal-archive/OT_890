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

/**
 * @file mecirc_buf.h
 *
 * @brief Meilhaus circular buffer implementation.
 * @note Copyright (C) 2007 Meilhaus Electronic GmbH (support@meilhaus.de)
 * @author Guenter Gebhardt
 * @author Krzysztof Gantzke  (k.gantzke@meilhaus.de)
 */

/*
 * Copyright (C) 2007 Meilhaus Electronic GmbH (support@meilhaus.de)
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _MECIRC_BUF_H_
#define _MECIRC_BUF_H_

# ifdef __KERNEL__

#  ifdef BOSCH

typedef struct me_circ_buf {
	unsigned int mask;
//      unsigned int count;
	uint32_t *buf;
	int volatile head;
	int volatile tail;
} me_circ_buf_t;

static int inline me_circ_buf_values(me_circ_buf_t * buf)
{
//      return ((buf->head - buf->tail) & (buf->count - 1));
	return ((buf->head - buf->tail) & (buf->mask));
}

static int inline me_circ_buf_space(me_circ_buf_t * buf)
{
//      return ((buf->tail - (buf->head + 1)) & (buf->count - 1));
	return ((buf->tail - (buf->head + 1)) & (buf->mask));
}

static int inline me_circ_buf_values_to_end(me_circ_buf_t * buf)
{
	int end;
	int n;
//      end = buf->count - buf->tail;
//      n = (buf->head + end) & (buf->count - 1);
	end = buf->mask + 1 - buf->tail;
	n = (buf->head + end) & (buf->mask);
	return (n < end) ? n : end;
}

static int inline me_circ_buf_space_to_end(me_circ_buf_t * buf)
{
	int end;
	int n;

//      end = buf->count - 1 - buf->head;
//      n = (end + buf->tail) & (buf->count - 1);
	end = buf->mask - buf->head;
	n = (end + buf->tail) & (buf->mask);
	return (n <= end) ? n : (end + 1);
}

#define _CBUFF_32b_t

#  else	//~BOSCH
/// @note buf->mask = buf->count-1 = ME4600_AI_CIRC_BUF_COUNT-1

#   ifdef _CBUFF_32b_t
	//32 bit
typedef struct me_circ_buf_32b {
	int volatile head;
	int volatile tail;
	unsigned int mask;	//buffor size-1 must be 2^n-1 to work
	uint32_t *buf;
} me_circ_buf_t;
#   else
	//16 bit
typedef struct me_circ_buf_16b {
	int volatile head;
	int volatile tail;
	unsigned int mask;	//buffor size-1 must be 2^n-1 to work
	uint16_t *buf;
} me_circ_buf_t;
#   endif //_CBUFF_32b_t

/** How many values is in buffer */
static int inline me_circ_buf_values(me_circ_buf_t * buf)
{
	return ((buf->head - buf->tail) & (buf->mask));
}

/** How many space left */
static int inline me_circ_buf_space(me_circ_buf_t * buf)
{
	return ((buf->tail - (buf->head + 1)) & (buf->mask));
}

/** How many values can be read from buffor in one chunck. */
static int inline me_circ_buf_values_to_end(me_circ_buf_t * buf)
{
	return (buf->tail <=
		buf->head) ? (buf->head - buf->tail) : (buf->mask - buf->tail +
							1);
}

/** How many values can be write to buffer in one chunck. */
static int inline me_circ_buf_space_to_end(me_circ_buf_t * buf)
{
	return (buf->tail <=
		buf->head) ? (buf->mask - buf->head + 1) : (buf->tail -
							    buf->head - 1);
}

#  endif //BOSCH
# endif	//__KERNEL__
#endif //_MECIRC_BUF_H_
