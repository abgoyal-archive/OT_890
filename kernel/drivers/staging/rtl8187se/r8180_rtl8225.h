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
  This is part of the rtl8180-sa2400 driver
  released under the GPL (See file COPYING for details).
  Copyright (c) 2005 Andrea Merello <andreamrl@tiscali.it>

  This files contains programming code for the rtl8225
  radio frontend.

  *Many* thanks to Realtek Corp. for their great support!

*/

#include "r8180.h"

#define RTL8225_ANAPARAM_ON  0xa0000b59
#define RTL8225_ANAPARAM_OFF 0xa00beb59
#define RTL8225_ANAPARAM2_OFF 0x840dec11
#define RTL8225_ANAPARAM2_ON  0x860dec11
#define RTL8225_ANAPARAM_SLEEP 0xa00bab59
#define RTL8225_ANAPARAM2_SLEEP 0x840dec11

#ifdef CONFIG_RTL8185B
void rtl8225z2_rf_init(struct net_device *dev);
void rtl8225z2_rf_set_chan(struct net_device *dev,short ch);
void rtl8225z2_rf_close(struct net_device *dev);

void rtl8225_host_pci_init(struct net_device *dev);
void rtl8225_host_usb_init(struct net_device *dev);

void write_rtl8225(struct net_device *dev, u8 adr, u16 data);
void RF_WriteReg(struct net_device *dev, u8 offset, u32	data);
u32 RF_ReadReg(struct net_device *dev, u8 offset);
#endif
void rtl8225_rf_init(struct net_device *dev);
void rtl8225_rf_set_chan(struct net_device *dev,short ch);
void rtl8225_rf_close(struct net_device *dev);
void rtl8225_rf_sleep(struct net_device *dev);
void rtl8225_rf_wakeup(struct net_device *dev);
void rtl8180_set_mode(struct net_device *dev,int mode);
void rtl8180_set_mode(struct net_device *dev,int mode);
bool SetZebraRFPowerState8185(struct net_device *dev,RT_RF_POWER_STATE  eRFPowerState);
void rtl8225z4_rf_sleep(struct net_device *dev);
void rtl8225z4_rf_wakeup(struct net_device *dev);

