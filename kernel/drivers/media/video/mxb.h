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

#ifndef __MXB__
#define __MXB__

#define BASE_VIDIOC_MXB 10

#define MXB_S_AUDIO_CD		_IOW  ('V', BASE_VIDIOC_PRIVATE+BASE_VIDIOC_MXB+0, int)
#define MXB_S_AUDIO_LINE	_IOW  ('V', BASE_VIDIOC_PRIVATE+BASE_VIDIOC_MXB+1, int)

#define MXB_IDENTIFIER "Multimedia eXtension Board"

#define MXB_AUDIOS	6

/* these are the available audio sources, which can switched
   to the line- and cd-output individually */
static struct v4l2_audio mxb_audios[MXB_AUDIOS] = {
	    {
		.index	= 0,
		.name	= "Tuner",
		.capability = V4L2_AUDCAP_STEREO,
	} , {
		.index	= 1,
		.name	= "AUX1",
		.capability = V4L2_AUDCAP_STEREO,
	} , {
		.index	= 2,
		.name	= "AUX2",
		.capability = V4L2_AUDCAP_STEREO,
	} , {
		.index	= 3,
		.name	= "AUX3",
		.capability = V4L2_AUDCAP_STEREO,
	} , {
		.index	= 4,
		.name	= "Radio (X9)",
		.capability = V4L2_AUDCAP_STEREO,
	} , {
		.index	= 5,
		.name	= "CD-ROM (X10)",
		.capability = V4L2_AUDCAP_STEREO,
	}
};
#endif
