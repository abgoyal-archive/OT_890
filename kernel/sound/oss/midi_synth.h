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

int midi_synth_ioctl (int dev,
	    unsigned int cmd, void __user * arg);
int midi_synth_kill_note (int dev, int channel, int note, int velocity);
int midi_synth_set_instr (int dev, int channel, int instr_no);
int midi_synth_start_note (int dev, int channel, int note, int volume);
void midi_synth_reset (int dev);
int midi_synth_open (int dev, int mode);
void midi_synth_close (int dev);
void midi_synth_hw_control (int dev, unsigned char *event);
int midi_synth_load_patch (int dev, int format, const char __user * addr,
		 int offs, int count, int pmgr_flag);
void midi_synth_panning (int dev, int channel, int pressure);
void midi_synth_aftertouch (int dev, int channel, int pressure);
void midi_synth_controller (int dev, int channel, int ctrl_num, int value);
void midi_synth_bender (int dev, int chn, int value);
void midi_synth_setup_voice (int dev, int voice, int chn);
int midi_synth_send_sysex(int dev, unsigned char *bytes,int len);

#ifndef _MIDI_SYNTH_C_
static struct synth_info std_synth_info =
{MIDI_SYNTH_NAME, 0, SYNTH_TYPE_MIDI, 0, 0, 128, 0, 128, MIDI_SYNTH_CAPS};

static struct synth_operations std_midi_synth =
{
	.owner		= THIS_MODULE,
	.id		= "MIDI",
	.info		= &std_synth_info,
	.midi_dev	= 0,
	.synth_type	= SYNTH_TYPE_MIDI,
	.synth_subtype	= 0,
	.open		= midi_synth_open,
	.close		= midi_synth_close,
	.ioctl		= midi_synth_ioctl,
	.kill_note	= midi_synth_kill_note,
	.start_note	= midi_synth_start_note,
	.set_instr	= midi_synth_set_instr,
	.reset		= midi_synth_reset,
	.hw_control	= midi_synth_hw_control,
	.load_patch	= midi_synth_load_patch,
	.aftertouch	= midi_synth_aftertouch,
	.controller	= midi_synth_controller,
	.panning		= midi_synth_panning,
	.bender		= midi_synth_bender,
	.setup_voice	= midi_synth_setup_voice,
	.send_sysex	= midi_synth_send_sysex
};
#endif
