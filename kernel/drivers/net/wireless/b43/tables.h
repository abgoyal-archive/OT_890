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

#ifndef B43_TABLES_H_
#define B43_TABLES_H_

#define B43_TAB_ROTOR_SIZE	53
extern const u32 b43_tab_rotor[];
#define B43_TAB_RETARD_SIZE	53
extern const u32 b43_tab_retard[];
#define B43_TAB_FINEFREQA_SIZE	256
extern const u16 b43_tab_finefreqa[];
#define B43_TAB_FINEFREQG_SIZE	256
extern const u16 b43_tab_finefreqg[];
#define B43_TAB_NOISEA2_SIZE	8
extern const u16 b43_tab_noisea2[];
#define B43_TAB_NOISEA3_SIZE	8
extern const u16 b43_tab_noisea3[];
#define B43_TAB_NOISEG1_SIZE	8
extern const u16 b43_tab_noiseg1[];
#define B43_TAB_NOISEG2_SIZE	8
extern const u16 b43_tab_noiseg2[];
#define B43_TAB_NOISESCALE_SIZE	27
extern const u16 b43_tab_noisescalea2[];
extern const u16 b43_tab_noisescalea3[];
extern const u16 b43_tab_noisescaleg1[];
extern const u16 b43_tab_noisescaleg2[];
extern const u16 b43_tab_noisescaleg3[];
#define B43_TAB_SIGMASQR_SIZE	53
extern const u16 b43_tab_sigmasqr1[];
extern const u16 b43_tab_sigmasqr2[];
#define B43_TAB_RSSIAGC1_SIZE	16
extern const u16 b43_tab_rssiagc1[];
#define B43_TAB_RSSIAGC2_SIZE	48
extern const u16 b43_tab_rssiagc2[];

#endif /* B43_TABLES_H_ */
