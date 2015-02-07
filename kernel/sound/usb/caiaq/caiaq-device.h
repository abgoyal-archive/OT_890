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

#ifndef CAIAQ_DEVICE_H
#define CAIAQ_DEVICE_H

#include "../usbaudio.h"

#define USB_VID_NATIVEINSTRUMENTS 0x17cc

#define USB_PID_RIGKONTROL2	0x1969
#define USB_PID_RIGKONTROL3	0x1940
#define USB_PID_KORECONTROLLER	0x4711
#define USB_PID_KORECONTROLLER2	0x4712
#define USB_PID_AK1		0x0815
#define USB_PID_AUDIO8DJ	0x1978
#define USB_PID_SESSIONIO	0x1915

#define EP1_BUFSIZE 64
#define CAIAQ_USB_STR_LEN 0xff
#define MAX_STREAMS 32

//#define	SND_USB_CAIAQ_DEBUG

#define MODNAME "snd-usb-caiaq"
#define log(x...) snd_printk(KERN_WARNING MODNAME" log: " x)

#ifdef SND_USB_CAIAQ_DEBUG
#define debug(x...) snd_printk(KERN_WARNING MODNAME " debug: " x)
#else
#define debug(x...) do { } while(0)
#endif

#define EP1_CMD_GET_DEVICE_INFO	0x1
#define EP1_CMD_READ_ERP	0x2
#define EP1_CMD_READ_ANALOG	0x3
#define EP1_CMD_READ_IO		0x4
#define EP1_CMD_WRITE_IO	0x5
#define EP1_CMD_MIDI_READ	0x6
#define EP1_CMD_MIDI_WRITE	0x7
#define EP1_CMD_AUDIO_PARAMS	0x9
#define EP1_CMD_AUTO_MSG	0xb
#define EP1_CMD_DIMM_LEDS       0xc

struct caiaq_device_spec {
	unsigned short fw_version;
	unsigned char hw_subtype;
	unsigned char num_erp;
	unsigned char num_analog_in;
	unsigned char num_digital_in;
	unsigned char num_digital_out;
	unsigned char num_analog_audio_out;
	unsigned char num_analog_audio_in;
	unsigned char num_digital_audio_out;
	unsigned char num_digital_audio_in;
	unsigned char num_midi_out;
	unsigned char num_midi_in;
	unsigned char data_alignment;
} __attribute__ ((packed));

struct snd_usb_caiaq_cb_info;

struct snd_usb_caiaqdev {
	struct snd_usb_audio chip;

	struct urb ep1_in_urb;
	struct urb midi_out_urb;
	struct urb **data_urbs_in;
	struct urb **data_urbs_out;
	struct snd_usb_caiaq_cb_info *data_cb_info;

	unsigned char ep1_in_buf[EP1_BUFSIZE];
	unsigned char ep1_out_buf[EP1_BUFSIZE];
	unsigned char midi_out_buf[EP1_BUFSIZE];

	struct caiaq_device_spec spec;
	spinlock_t spinlock;
	wait_queue_head_t ep1_wait_queue;
	wait_queue_head_t prepare_wait_queue;
	int spec_received, audio_parm_answer;
	int midi_out_active;

	char vendor_name[CAIAQ_USB_STR_LEN];
	char product_name[CAIAQ_USB_STR_LEN];
	char serial[CAIAQ_USB_STR_LEN];

	int n_streams, n_audio_in, n_audio_out;
	int streaming, first_packet, output_running;
	int audio_in_buf_pos[MAX_STREAMS];
	int audio_out_buf_pos[MAX_STREAMS];
	int period_in_count[MAX_STREAMS];
	int period_out_count[MAX_STREAMS];
	int input_panic, output_panic;
	char *audio_in_buf, *audio_out_buf;
	unsigned int samplerates;

	struct snd_pcm_substream *sub_playback[MAX_STREAMS];
	struct snd_pcm_substream *sub_capture[MAX_STREAMS];

	/* Controls */
	unsigned char control_state[64];

	/* Linux input */
#ifdef CONFIG_SND_USB_CAIAQ_INPUT
	struct input_dev *input_dev;
	char phys[64];			/* physical device path */
	unsigned short keycode[64];
#endif

	/* ALSA */
	struct snd_pcm *pcm;
	struct snd_pcm_hardware pcm_info;
	struct snd_rawmidi *rmidi;
	struct snd_rawmidi_substream *midi_receive_substream;
	struct snd_rawmidi_substream *midi_out_substream;
};

struct snd_usb_caiaq_cb_info {
	struct snd_usb_caiaqdev *dev;
	int index;
};

#define caiaqdev(c) ((struct snd_usb_caiaqdev*)(c)->private_data)

int snd_usb_caiaq_set_audio_params (struct snd_usb_caiaqdev *dev, int rate, int depth, int bbp);
int snd_usb_caiaq_set_auto_msg (struct snd_usb_caiaqdev *dev, int digital, int analog, int erp);
int snd_usb_caiaq_send_command(struct snd_usb_caiaqdev *dev,
			       unsigned char command,
			       const unsigned char *buffer,
			       int len);

#endif /* CAIAQ_DEVICE_H */
