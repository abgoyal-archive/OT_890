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
 * Definitions for the KLSI KL5KUSB105 serial port adapter
 */

/* vendor/product pairs that are known to contain this chipset */
#define PALMCONNECT_VID		0x0830
#define PALMCONNECT_PID		0x0080

#define KLSI_VID		0x05e9
#define KLSI_KL5KUSB105D_PID	0x00c0

/* Vendor commands: */


/* port table -- the chip supports up to 4 channels */

/* baud rates */

enum {
  kl5kusb105a_sio_b115200 = 0,
  kl5kusb105a_sio_b57600  = 1,
  kl5kusb105a_sio_b38400  = 2,
  kl5kusb105a_sio_b19200  = 4,
  kl5kusb105a_sio_b14400  = 5,
  kl5kusb105a_sio_b9600   = 6,
  kl5kusb105a_sio_b4800   = 8,   /* unchecked */
  kl5kusb105a_sio_b2400   = 9,   /* unchecked */
  kl5kusb105a_sio_b1200   = 0xa,  /* unchecked */
  kl5kusb105a_sio_b600    = 0xb   /* unchecked */
};

/* data bits */
#define kl5kusb105a_dtb_7   7
#define kl5kusb105a_dtb_8   8



/* requests: */
#define KL5KUSB105A_SIO_SET_DATA  1
#define KL5KUSB105A_SIO_POLL      2
#define KL5KUSB105A_SIO_CONFIGURE      3
/* values used for request KL5KUSB105A_SIO_CONFIGURE */
#define KL5KUSB105A_SIO_CONFIGURE_READ_ON      3
#define KL5KUSB105A_SIO_CONFIGURE_READ_OFF     2

/* Interpretation of modem status lines */
/* These need sorting out by individually connecting pins and checking
 * results. FIXME!
 * When data is being sent we see 0x30 in the lower byte; this must
 * contain DSR and CTS ...
 */
#define KL5KUSB105A_DSR			((1<<4) | (1<<5))
#define KL5KUSB105A_CTS			((1<<5) | (1<<4))

#define KL5KUSB105A_WANTS_TO_SEND	0x30
//#define KL5KUSB105A_DTR			/* Data Terminal Ready */
//#define KL5KUSB105A_CTS			/* Clear To Send */
//#define KL5KUSB105A_CD			/* Carrier Detect */
//#define KL5KUSB105A_DSR			/* Data Set Ready */
//#define KL5KUSB105A_RxD			/* Receive pin */

//#define KL5KUSB105A_LE
//#define KL5KUSB105A_RTS		
//#define KL5KUSB105A_ST		
//#define KL5KUSB105A_SR		
//#define KL5KUSB105A_RI			/* Ring Indicator */

/* vim: set ts=8 sts=8: */

