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
 * SpanDSP - a series of DSP components for telephony
 *
 * ec_disable_detector.h - A detector which should eventually meet the
 *                         G.164/G.165 requirements for detecting the
 *                         2100Hz echo cancellor disable tone.
 *
 * Written by Steve Underwood <steveu@coppice.org>
 *
 * Copyright (C) 2001 Steve Underwood
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
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
 *
 */

#include "dsp_biquad.h"

struct ec_disable_detector_state {
	struct biquad2_state notch;
	int notch_level;
	int channel_level;
	int tone_present;
	int tone_cycle_duration;
	int good_cycles;
	int hit;
};


#define FALSE 0
#define TRUE (!FALSE)

static inline void
echo_can_disable_detector_init(struct ec_disable_detector_state *det)
{
    /* Elliptic notch */
    /* This is actually centred at 2095Hz, but gets the balance we want, due
       to the asymmetric walls of the notch */
	biquad2_init(&det->notch,
		(int32_t) (-0.7600000*32768.0),
		(int32_t) (-0.1183852*32768.0),
		(int32_t) (-0.5104039*32768.0),
		(int32_t) (0.1567596*32768.0),
		(int32_t) (1.0000000*32768.0));

	det->channel_level = 0;
	det->notch_level = 0;
	det->tone_present = FALSE;
	det->tone_cycle_duration = 0;
	det->good_cycles = 0;
	det->hit = 0;
}
/*- End of function --------------------------------------------------------*/

static inline int
echo_can_disable_detector_update(struct ec_disable_detector_state *det,
int16_t amp)
{
	int16_t notched;

	notched = biquad2(&det->notch, amp);
	/* Estimate the overall energy in the channel, and the energy in
	   the notch (i.e. overall channel energy - tone energy => noise).
	   Use abs instead of multiply for speed (is it really faster?).
	   Damp the overall energy a little more for a stable result.
	   Damp the notch energy a little less, so we don't damp out the
	   blip every time the phase reverses */
	det->channel_level += ((abs(amp) - det->channel_level) >> 5);
	det->notch_level += ((abs(notched) - det->notch_level) >> 4);
	if (det->channel_level > 280) {
		/* There is adequate energy in the channel.
		 Is it mostly at 2100Hz? */
		if (det->notch_level*6 < det->channel_level) {
			/* The notch says yes, so we have the tone. */
			if (!det->tone_present) {
				/* Do we get a kick every 450+-25ms? */
				if (det->tone_cycle_duration >= 425*8
					&& det->tone_cycle_duration <= 475*8) {
					det->good_cycles++;
					if (det->good_cycles > 2)
					det->hit = TRUE;
				}
				det->tone_cycle_duration = 0;
			}
			det->tone_present = TRUE;
		} else
			det->tone_present = FALSE;
		det->tone_cycle_duration++;
	} else {
		det->tone_present = FALSE;
		det->tone_cycle_duration = 0;
		det->good_cycles = 0;
	}
	return det->hit;
}
/*- End of function --------------------------------------------------------*/
/*- End of file ------------------------------------------------------------*/
