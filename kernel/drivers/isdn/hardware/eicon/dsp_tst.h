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

/* $Id$ */

#ifndef __DIVA_PRI_HOST_TEST_DSPS_H__
#define __DIVA_PRI_HOST_TEST_DSPS_H__

/*
   DSP registers on maestra pri
   */
#define DSP1_PORT       (0x00)
#define DSP2_PORT       (0x8)
#define DSP3_PORT       (0x800)
#define DSP4_PORT       (0x808)
#define DSP5_PORT       (0x810)
#define DSP6_PORT       (0x818)
#define DSP7_PORT       (0x820)
#define DSP8_PORT       (0x828)
#define DSP9_PORT       (0x830)
#define DSP10_PORT      (0x840)
#define DSP11_PORT      (0x848)
#define DSP12_PORT      (0x850)
#define DSP13_PORT      (0x858)
#define DSP14_PORT      (0x860)
#define DSP15_PORT      (0x868)
#define DSP16_PORT      (0x870)
#define DSP17_PORT      (0x1000)
#define DSP18_PORT      (0x1008)
#define DSP19_PORT      (0x1010)
#define DSP20_PORT      (0x1018)
#define DSP21_PORT      (0x1020)
#define DSP22_PORT      (0x1028)
#define DSP23_PORT      (0x1030)
#define DSP24_PORT      (0x1040)
#define DSP25_PORT      (0x1048)
#define DSP26_PORT      (0x1050)
#define DSP27_PORT      (0x1058)
#define DSP28_PORT      (0x1060)
#define DSP29_PORT      (0x1068)
#define DSP30_PORT      (0x1070)
#define DSP_ADR_OFFS    0x80

/*------------------------------------------------------------------
		Dsp related definitions
  ------------------------------------------------------------------ */
#define DSP_SIGNATURE_PROBE_WORD 0x5a5a
#define dsp_make_address_ex(pm,address) ((word)((pm) ? (address) : (address) + 0x4000))

#endif
