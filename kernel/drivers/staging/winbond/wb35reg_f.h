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

#ifndef __WINBOND_WB35REG_F_H
#define __WINBOND_WB35REG_F_H

#include "wbhal_s.h"

//====================================
// Interface function declare
//====================================
unsigned char Wb35Reg_initial(  phw_data_t pHwData );
void Uxx_power_on_procedure(  phw_data_t pHwData );
void Uxx_power_off_procedure(  phw_data_t pHwData );
void Uxx_ReadEthernetAddress(  phw_data_t pHwData );
void Dxx_initial(  phw_data_t pHwData );
void Mxx_initial(  phw_data_t pHwData );
void RFSynthesizer_initial(  phw_data_t pHwData );
//void RFSynthesizer_SwitchingChannel(  phw_data_t pHwData,  s8 Channel );
void RFSynthesizer_SwitchingChannel(  phw_data_t pHwData,  ChanInfo Channel );
void BBProcessor_initial(  phw_data_t pHwData );
void BBProcessor_RateChanging(  phw_data_t pHwData,  u8 rate ); // 20060613.1
//void RF_RateChanging(  phw_data_t pHwData,  u8 rate ); // 20060626.5.c Add
u8 RFSynthesizer_SetPowerIndex(  phw_data_t pHwData,  u8 PowerIndex );
u8 RFSynthesizer_SetMaxim2828_24Power(  phw_data_t,  u8 index );
u8 RFSynthesizer_SetMaxim2828_50Power(  phw_data_t,  u8 index );
u8 RFSynthesizer_SetMaxim2827_24Power(  phw_data_t,  u8 index );
u8 RFSynthesizer_SetMaxim2827_50Power(  phw_data_t,  u8 index );
u8 RFSynthesizer_SetMaxim2825Power(  phw_data_t,  u8 index );
u8 RFSynthesizer_SetAiroha2230Power(  phw_data_t,  u8 index );
u8 RFSynthesizer_SetAiroha7230Power(  phw_data_t,  u8 index );
u8 RFSynthesizer_SetWinbond242Power(  phw_data_t,  u8 index );
void GetTxVgaFromEEPROM(  phw_data_t pHwData );
void EEPROMTxVgaAdjust(  phw_data_t pHwData ); // 20060619.5 Add

#define RFWriteControlData( _A, _V ) Wb35Reg_Write( _A, 0x0864, _V )

void Wb35Reg_destroy(  phw_data_t pHwData );

unsigned char Wb35Reg_Read(  phw_data_t pHwData,  u16 RegisterNo,   u32 * pRegisterValue );
unsigned char Wb35Reg_ReadSync(  phw_data_t pHwData,  u16 RegisterNo,   u32 * pRegisterValue );
unsigned char Wb35Reg_Write(  phw_data_t pHwData,  u16 RegisterNo,  u32 RegisterValue );
unsigned char Wb35Reg_WriteSync(  phw_data_t pHwData,  u16 RegisterNo,  u32 RegisterValue );
unsigned char Wb35Reg_WriteWithCallbackValue(  phw_data_t pHwData,
								 u16 RegisterNo,
								 u32 RegisterValue,
								 s8 *pValue,
								 s8 Len);
unsigned char Wb35Reg_BurstWrite(  phw_data_t pHwData,  u16 RegisterNo,  u32 * pRegisterData,  u8 NumberOfData,  u8 Flag );

void Wb35Reg_EP0VM(  phw_data_t pHwData );
void Wb35Reg_EP0VM_start(  phw_data_t pHwData );
void Wb35Reg_EP0VM_complete(struct urb *urb);

u32 BitReverse( u32 dwData, u32 DataLength);

void CardGetMulticastBit(   u8 Address[MAC_ADDR_LENGTH],  u8 *Byte,  u8 *Value );
u32 CardComputeCrc(  u8 * Buffer,  u32 Length );

void Wb35Reg_phy_calibration(  phw_data_t pHwData );
void Wb35Reg_Update(  phw_data_t pHwData,  u16 RegisterNo,  u32 RegisterValue );
unsigned char adjust_TXVGA_for_iq_mag(  phw_data_t pHwData );

#endif
