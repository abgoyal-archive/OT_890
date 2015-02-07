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

#ifndef __ALPHA_PAL_H
#define __ALPHA_PAL_H

/*
 * Common PAL-code
 */
#define PAL_halt	  0
#define PAL_cflush	  1
#define PAL_draina	  2
#define PAL_bpt		128
#define PAL_bugchk	129
#define PAL_chmk	131
#define PAL_callsys	131
#define PAL_imb		134
#define PAL_rduniq	158
#define PAL_wruniq	159
#define PAL_gentrap	170
#define PAL_nphalt	190

/*
 * VMS specific PAL-code
 */
#define PAL_swppal	10
#define PAL_mfpr_vptb	41

/*
 * OSF specific PAL-code
 */
#define PAL_cserve	 9
#define PAL_wripir	13
#define PAL_rdmces	16
#define PAL_wrmces	17
#define PAL_wrfen	43
#define PAL_wrvptptr	45
#define PAL_jtopal	46
#define PAL_swpctx	48
#define PAL_wrval	49
#define PAL_rdval	50
#define PAL_tbi		51
#define PAL_wrent	52
#define PAL_swpipl	53
#define PAL_rdps	54
#define PAL_wrkgp	55
#define PAL_wrusp	56
#define PAL_wrperfmon	57
#define PAL_rdusp	58
#define PAL_whami	60
#define PAL_retsys	61
#define PAL_rti		63

#endif /* __ALPHA_PAL_H */
