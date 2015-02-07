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

#ifndef R8180_DM_H 
#define R8180_DM_H

#include "r8180.h"
//#include "r8180_hw.h"
//#include "r8180_93cx6.h"
void SwAntennaDiversityRxOk8185(struct net_device *dev, u8 SignalStrength);
bool SetAntenna8185(struct net_device *dev,	u8 u1bAntennaIndex);
bool SwitchAntenna(	struct net_device *dev);
void SwAntennaDiversity(struct net_device *dev	);
void SwAntennaDiversityTimerCallback(struct net_device *dev);
bool CheckDig(struct net_device *dev);
bool CheckHighPower(struct net_device *dev);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void rtl8180_hw_dig_wq (struct work_struct *work);
#else
void rtl8180_hw_dig_wq(struct net_device *dev);
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void rtl8180_tx_pw_wq (struct work_struct *work);
#else
void rtl8180_tx_pw_wq(struct net_device *dev);
#endif
#if LINUX_VERSION_CODE >=KERNEL_VERSION(2,6,20)
void rtl8180_rate_adapter(struct work_struct * work);

#else
void rtl8180_rate_adapter(struct net_device *dev);

#endif
void TxPwrTracking87SE(struct net_device *dev);
bool CheckTxPwrTracking(struct net_device *dev);
#if LINUX_VERSION_CODE >=KERNEL_VERSION(2,6,20)
void rtl8180_rate_adapter(struct work_struct * work);
#else
void rtl8180_rate_adapter(struct net_device *dev);
#endif
void timer_rate_adaptive(unsigned long data);


#endif
