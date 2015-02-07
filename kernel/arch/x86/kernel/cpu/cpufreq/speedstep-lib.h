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
 * (C) 2002 - 2003 Dominik Brodowski <linux@brodo.de>
 *
 *  Licensed under the terms of the GNU GPL License version 2.
 *
 *  Library for common functions for Intel SpeedStep v.1 and v.2 support
 *
 *  BIG FAT DISCLAIMER: Work in progress code. Possibly *dangerous*
 */



/* processors */

#define SPEEDSTEP_PROCESSOR_PIII_C_EARLY	0x00000001  /* Coppermine core */
#define SPEEDSTEP_PROCESSOR_PIII_C		0x00000002  /* Coppermine core */
#define SPEEDSTEP_PROCESSOR_PIII_T		0x00000003  /* Tualatin core */
#define SPEEDSTEP_PROCESSOR_P4M			0x00000004  /* P4-M  */

/* the following processors are not speedstep-capable and are not auto-detected
 * in speedstep_detect_processor(). However, their speed can be detected using
 * the speedstep_get_processor_frequency() call. */
#define SPEEDSTEP_PROCESSOR_PM			0xFFFFFF03  /* Pentium M  */
#define SPEEDSTEP_PROCESSOR_P4D			0xFFFFFF04  /* desktop P4  */
#define SPEEDSTEP_PROCESSOR_PCORE		0xFFFFFF05  /* Core */

/* speedstep states -- only two of them */

#define SPEEDSTEP_HIGH	0x00000000
#define SPEEDSTEP_LOW	0x00000001


/* detect a speedstep-capable processor */
extern unsigned int speedstep_detect_processor (void);

/* detect the current speed (in khz) of the processor */
extern unsigned int speedstep_get_processor_frequency(unsigned int processor);


/* detect the low and high speeds of the processor. The callback
 * set_state"'s first argument is either SPEEDSTEP_HIGH or
 * SPEEDSTEP_LOW; the second argument is zero so that no
 * cpufreq_notify_transition calls are initiated.
 */
extern unsigned int speedstep_get_freqs(unsigned int processor,
	unsigned int *low_speed,
	unsigned int *high_speed,
	unsigned int *transition_latency,
	void (*set_state) (unsigned int state));
