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
 */

#ifndef MSP3400_DRIVER_H
#define MSP3400_DRIVER_H

#include <media/msp3400.h>
#include <media/v4l2-device.h>

/* ---------------------------------------------------------------------- */

/* This macro is allowed for *constants* only, gcc must calculate it
   at compile time.  Remember -- no floats in kernel mode */
#define MSP_CARRIER(freq) ((int)((float)(freq / 18.432) * (1 << 24)))

#define MSP_MODE_AM_DETECT   0
#define MSP_MODE_FM_RADIO    2
#define MSP_MODE_FM_TERRA    3
#define MSP_MODE_FM_SAT      4
#define MSP_MODE_FM_NICAM1   5
#define MSP_MODE_FM_NICAM2   6
#define MSP_MODE_AM_NICAM    7
#define MSP_MODE_BTSC        8
#define MSP_MODE_EXTERN      9

#define SCART_IN1     0
#define SCART_IN2     1
#define SCART_IN3     2
#define SCART_IN4     3
#define SCART_IN1_DA  4
#define SCART_IN2_DA  5
#define SCART_MONO    6
#define SCART_MUTE    7

#define SCART_DSP_IN  0
#define SCART1_OUT    1
#define SCART2_OUT    2

#define OPMODE_AUTO       -1
#define OPMODE_MANUAL      0
#define OPMODE_AUTODETECT  1   /* use autodetect (>= msp3410 only) */
#define OPMODE_AUTOSELECT  2   /* use autodetect & autoselect (>= msp34xxG)   */

/* module parameters */
extern int msp_debug;
extern int msp_once;
extern int msp_amsound;
extern int msp_standard;
extern int msp_dolby;
extern int msp_stereo_thresh;

struct msp_state {
	struct v4l2_subdev sd;
	int rev1, rev2;
	int ident;
	u8 has_nicam;
	u8 has_radio;
	u8 has_headphones;
	u8 has_ntsc_jp_d_k3;
	u8 has_scart2;
	u8 has_scart3;
	u8 has_scart4;
	u8 has_scart2_out;
	u8 has_scart2_out_volume;
	u8 has_i2s_conf;
	u8 has_subwoofer;
	u8 has_sound_processing;
	u8 has_virtual_dolby_surround;
	u8 has_dolby_pro_logic;
	u8 force_btsc;

	int radio;
	int opmode;
	int std;
	int mode;
	v4l2_std_id v4l2_std;
	int nicam_on;
	int acb;
	int in_scart;
	int i2s_mode;
	int main, second;	/* sound carrier */
	int input;
	struct v4l2_routing routing;

	/* v4l2 */
	int audmode;
	int rxsubchans;

	int volume, muted;
	int balance, loudness;
	int bass, treble;
	int scan_in_progress;

	/* thread */
	struct task_struct   *kthread;
	wait_queue_head_t    wq;
	unsigned int         restart:1;
	unsigned int         watch_stereo:1;
};

static inline struct msp_state *to_state(struct v4l2_subdev *sd)
{
	return container_of(sd, struct msp_state, sd);
}

/* msp3400-driver.c */
int msp_write_dem(struct i2c_client *client, int addr, int val);
int msp_write_dsp(struct i2c_client *client, int addr, int val);
int msp_read_dem(struct i2c_client *client, int addr);
int msp_read_dsp(struct i2c_client *client, int addr);
int msp_reset(struct i2c_client *client);
void msp_set_scart(struct i2c_client *client, int in, int out);
void msp_set_audio(struct i2c_client *client);
int msp_sleep(struct msp_state *state, int timeout);

/* msp3400-kthreads.c */
const char *msp_standard_std_name(int std);
void msp_set_audmode(struct i2c_client *client);
int msp_detect_stereo(struct i2c_client *client);
int msp3400c_thread(void *data);
int msp3410d_thread(void *data);
int msp34xxg_thread(void *data);
void msp3400c_set_mode(struct i2c_client *client, int mode);
void msp3400c_set_carrier(struct i2c_client *client, int cdo1, int cdo2);

#endif /* MSP3400_DRIVER_H */
