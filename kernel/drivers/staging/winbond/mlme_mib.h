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

//============================================================================
//  MLMEMIB.H -
//
//  Description:
//    Get and Set some of MLME MIB attributes.
//
//  Revision history:
//  --------------------------------------------------------------------------
//           20030117  PD43 Austin Liu
//                     Initial release
//
//  Copyright (c) 2003 Winbond Electronics Corp. All rights reserved.
//============================================================================

#ifndef _MLME_MIB_H
#define _MLME_MIB_H

//============================================================================
// MLMESetExcludeUnencrypted --
//
// Description:
//   Set the dot11ExcludeUnencrypted value.
//
// Arguments:
//   adapter        - The pointer to the miniport adapter context.
//   ExUnencrypted  - unsigned char type. The value to be set.
//
// Return values:
//   None.
//============================================================================
#define MLMESetExcludeUnencrypted(adapter, ExUnencrypted)     \
{                                                              \
    (adapter)->sLocalPara.ExcludeUnencrypted = ExUnencrypted;             \
}

//============================================================================
// MLMEGetExcludeUnencrypted --
//
// Description:
//   Get the dot11ExcludeUnencrypted value.
//
// Arguments:
//   adapter        - The pointer to the miniport adapter context.
//
// Return values:
//   unsigned char type. The current dot11ExcludeUnencrypted value.
//============================================================================
#define MLMEGetExcludeUnencrypted(adapter) ((unsigned char) (adapter)->sLocalPara.ExcludeUnencrypted)

//============================================================================
// MLMESetMaxReceiveLifeTime --
//
// Description:
//   Set the dot11MaxReceiveLifeTime value.
//
// Arguments:
//   adapter        - The pointer to the miniport adapter context.
//   ReceiveLifeTime- u32 type. The value to be set.
//
// Return values:
//   None.
//============================================================================
#define MLMESetMaxReceiveLifeTime(adapter, ReceiveLifeTime)    \
{                                                               \
    (adapter)->Mds.MaxReceiveTime = ReceiveLifeTime;                \
}

//============================================================================
// MLMESetMaxReceiveLifeTime --
//
// Description:
//   Get the dot11MaxReceiveLifeTime value.
//
// Arguments:
//   adapter        - The pointer to the miniport adapter context.
//
// Return values:
//   u32 type. The current dot11MaxReceiveLifeTime value.
//============================================================================
#define MLMEGetMaxReceiveLifeTime(adapter) ((u32) (adapter)->Mds.MaxReceiveTime)

#endif


