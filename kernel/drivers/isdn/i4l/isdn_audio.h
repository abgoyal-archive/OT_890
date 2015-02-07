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

/* $Id$
 *
 * Linux ISDN subsystem, audio conversion and compression (linklevel).
 *
 * Copyright 1994-1999 by Fritz Elfert (fritz@isdn4linux.de)
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#define DTMF_NPOINTS 205        /* Number of samples for DTMF recognition */
typedef struct adpcm_state {
	int a;
	int d;
	int word;
	int nleft;
	int nbits;
} adpcm_state;

typedef struct dtmf_state {
	char last;
	char llast;
	int idx;
	int buf[DTMF_NPOINTS];
} dtmf_state;

typedef struct silence_state {
	int state;
	unsigned int idx;
} silence_state;

extern void isdn_audio_ulaw2alaw(unsigned char *, unsigned long);
extern void isdn_audio_alaw2ulaw(unsigned char *, unsigned long);
extern adpcm_state *isdn_audio_adpcm_init(adpcm_state *, int);
extern int isdn_audio_adpcm2xlaw(adpcm_state *, int, unsigned char *, unsigned char *, int);
extern int isdn_audio_xlaw2adpcm(adpcm_state *, int, unsigned char *, unsigned char *, int);
extern void isdn_audio_calc_dtmf(modem_info *, unsigned char *, int, int);
extern void isdn_audio_eval_dtmf(modem_info *);
dtmf_state *isdn_audio_dtmf_init(dtmf_state *);
extern void isdn_audio_calc_silence(modem_info *, unsigned char *, int, int);
extern void isdn_audio_eval_silence(modem_info *);
silence_state *isdn_audio_silence_init(silence_state *);
extern void isdn_audio_put_dle_code(modem_info *, u_char);
