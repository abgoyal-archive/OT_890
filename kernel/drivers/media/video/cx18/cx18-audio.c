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
 *  cx18 audio-related functions
 *
 *  Derived from ivtv-audio.c
 *
 *  Copyright (C) 2007  Hans Verkuil <hverkuil@xs4all.nl>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA
 */

#include "cx18-driver.h"
#include "cx18-io.h"
#include "cx18-i2c.h"
#include "cx18-cards.h"
#include "cx18-audio.h"

#define CX18_AUDIO_ENABLE 0xc72014

/* Selects the audio input and output according to the current
   settings. */
int cx18_audio_set_io(struct cx18 *cx)
{
	struct v4l2_routing route;
	u32 audio_input;
	u32 val;
	int mux_input;
	int err;

	/* Determine which input to use */
	if (test_bit(CX18_F_I_RADIO_USER, &cx->i_flags)) {
		audio_input = cx->card->radio_input.audio_input;
		mux_input = cx->card->radio_input.muxer_input;
	} else {
		audio_input =
			cx->card->audio_inputs[cx->audio_input].audio_input;
		mux_input =
			cx->card->audio_inputs[cx->audio_input].muxer_input;
	}

	/* handle muxer chips */
	route.input = mux_input;
	route.output = 0;
	cx18_i2c_hw(cx, cx->card->hw_muxer, VIDIOC_INT_S_AUDIO_ROUTING, &route);

	route.input = audio_input;
	err = cx18_i2c_hw(cx, cx->card->hw_audio_ctrl,
			VIDIOC_INT_S_AUDIO_ROUTING, &route);
	if (err)
		return err;

	val = cx18_read_reg(cx, CX18_AUDIO_ENABLE) & ~0x30;
	val |= (audio_input > CX18_AV_AUDIO_SERIAL2) ? 0x20 :
					(audio_input << 4);
	cx18_write_reg(cx, val | 0xb00, CX18_AUDIO_ENABLE);
	cx18_vapi(cx, CX18_APU_RESETAI, 1, 0);
	return 0;
}

void cx18_audio_set_route(struct cx18 *cx, struct v4l2_routing *route)
{
	cx18_i2c_hw(cx, cx->card->hw_audio_ctrl,
			VIDIOC_INT_S_AUDIO_ROUTING, route);
}

void cx18_audio_set_audio_clock_freq(struct cx18 *cx, u8 freq)
{
	static u32 freqs[3] = { 44100, 48000, 32000 };

	/* The audio clock of the digitizer must match the codec sample
	   rate otherwise you get some very strange effects. */
	if (freq > 2)
		return;
	cx18_call_i2c_clients(cx, VIDIOC_INT_AUDIO_CLOCK_FREQ, &freqs[freq]);
}
