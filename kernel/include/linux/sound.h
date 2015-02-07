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
 * Minor numbers for the sound driver.
 */

#include <linux/fs.h>

#define SND_DEV_CTL		0	/* Control port /dev/mixer */
#define SND_DEV_SEQ		1	/* Sequencer output /dev/sequencer (FM
						synthesizer and MIDI output) */
#define SND_DEV_MIDIN		2	/* Raw midi access */
#define SND_DEV_DSP		3	/* Digitized voice /dev/dsp */
#define SND_DEV_AUDIO		4	/* Sparc compatible /dev/audio */
#define SND_DEV_DSP16		5	/* Like /dev/dsp but 16 bits/sample */
/* #define SND_DEV_STATUS	6 */	/* /dev/sndstat (obsolete) */
#define SND_DEV_UNUSED		6
#define SND_DEV_AWFM		7	/* Reserved */
#define SND_DEV_SEQ2		8	/* /dev/sequencer, level 2 interface */
/* #define SND_DEV_SNDPROC	9 */	/* /dev/sndproc for programmable devices (not used) */
/* #define SND_DEV_DMMIDI	9 */
#define SND_DEV_SYNTH		9	/* Raw synth access /dev/synth (same as /dev/dmfm) */
#define SND_DEV_DMFM		10	/* Raw synth access /dev/dmfm */
#define SND_DEV_UNKNOWN11	11
#define SND_DEV_ADSP		12	/* Like /dev/dsp (obsolete) */
#define SND_DEV_AMIDI		13	/* Like /dev/midi (obsolete) */
#define SND_DEV_ADMMIDI		14	/* Like /dev/dmmidi (onsolete) */

#ifdef __KERNEL__
/*
 *	Sound core interface functions
 */
 
struct device;
extern int register_sound_special(const struct file_operations *fops, int unit);
extern int register_sound_special_device(const struct file_operations *fops, int unit, struct device *dev);
extern int register_sound_mixer(const struct file_operations *fops, int dev);
extern int register_sound_midi(const struct file_operations *fops, int dev);
extern int register_sound_dsp(const struct file_operations *fops, int dev);

extern void unregister_sound_special(int unit);
extern void unregister_sound_mixer(int unit);
extern void unregister_sound_midi(int unit);
extern void unregister_sound_dsp(int unit);
#endif /* __KERNEL__ */
