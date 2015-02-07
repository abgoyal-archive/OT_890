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

#ifndef _MIRO_H_
#define _MIRO_H_

#define ACI_REG_COMMAND		0	/* write register offset */
#define ACI_REG_STATUS		1	/* read register offset */
#define ACI_REG_BUSY		2	/* busy register offset */
#define ACI_REG_RDS		2	/* PCM20: RDS register offset */
#define ACI_MINTIME		500	/* ACI time out limit */

#define ACI_SET_MUTE		0x0d
#define ACI_SET_POWERAMP	0x0f
#define ACI_SET_TUNERMUTE	0xa3
#define ACI_SET_TUNERMONO	0xa4
#define ACI_SET_IDE		0xd0
#define ACI_SET_WSS		0xd1
#define ACI_SET_SOLOMODE	0xd2
#define ACI_SET_PREAMP		0x03
#define ACI_GET_PREAMP		0x21
#define ACI_WRITE_TUNE		0xa7
#define ACI_READ_TUNERSTEREO	0xa8
#define ACI_READ_TUNERSTATION	0xa9
#define ACI_READ_VERSION	0xf1
#define ACI_READ_IDCODE		0xf2
#define ACI_INIT		0xff
#define ACI_STATUS		0xf0
#define ACI_S_GENERAL		0x00
#define ACI_ERROR_OP		0xdf

/* ACI Mixer */

/* These are the values for the right channel GET registers.
   Add an offset of 0x01 for the left channel register.
   (left=right+0x01) */

#define ACI_GET_MASTER		0x03
#define ACI_GET_MIC		0x05
#define ACI_GET_LINE		0x07
#define ACI_GET_CD		0x09
#define ACI_GET_SYNTH		0x0b
#define ACI_GET_PCM		0x0d
#define ACI_GET_LINE1		0x10	/* Radio on PCM20 */
#define ACI_GET_LINE2		0x12

#define ACI_GET_EQ1		0x22	/* from Bass ... */
#define ACI_GET_EQ2		0x24
#define ACI_GET_EQ3		0x26
#define ACI_GET_EQ4		0x28
#define ACI_GET_EQ5		0x2a
#define ACI_GET_EQ6		0x2c
#define ACI_GET_EQ7		0x2e	/* ... to Treble */

/* And these are the values for the right channel SET registers.
   For left channel access you have to add an offset of 0x08.
   MASTER is an exception, which needs an offset of 0x01 */

#define ACI_SET_MASTER		0x00
#define ACI_SET_MIC		0x30
#define ACI_SET_LINE		0x31
#define ACI_SET_CD		0x34
#define ACI_SET_SYNTH		0x33
#define ACI_SET_PCM		0x32
#define ACI_SET_LINE1		0x35	/* Radio on PCM20 */
#define ACI_SET_LINE2		0x36

#define ACI_SET_EQ1		0x40	/* from Bass ... */
#define ACI_SET_EQ2		0x41
#define ACI_SET_EQ3		0x42
#define ACI_SET_EQ4		0x43
#define ACI_SET_EQ5		0x44
#define ACI_SET_EQ6		0x45
#define ACI_SET_EQ7		0x46	/* ... to Treble */

#endif  /* _MIRO_H_ */
