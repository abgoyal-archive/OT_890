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

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/


 
#ifndef _MTK_DVT_TEST_ADC_SW_H
#define _MTK_DVT_TEST_ADC_SW_H

//-----------------------------------------------------------------------------
//  IMM and TRI modes
typedef enum 
{
    NotLegalMode = -1,
    ModeNON   = 0x00,
    ModeIMM   = 0x01, //Immediate mode
    ModeTRI   = 0x10,  //Time-trigger mode
    ModeChaos = 0x11
} AdcModeType;

typedef enum 
{
    NotAdcChannel = -1,
        
    AdcChannel0 = 0,
    AdcChannel1 = 1,
    AdcChannel2 = 2,
    AdcChannel3 = 3,
    AdcChannel4 = 4,
    AdcChannel5 = 5,    
    AdcChannel6 = 6,    // VBAT, Voltage Battery
    AdcChannel7 = 7,    // ISENSE
    AdcChannel8 = 8,    // CHRIN, Charger IN
    
    AdcChannel9_0 = 9,
    _AdcChannel_MAX = AdcChannel9_0
} AdcChannelType;


typedef enum 
{
    _NON_BASE = 0,
    NON0 = _NON_BASE + 0,    
    NON1 = _NON_BASE + 1,        
    NON2 = _NON_BASE + 2,    
    NON3 = _NON_BASE + 3,        
    NON4 = _NON_BASE + 4,    
    NON5 = _NON_BASE + 5,        
    NON6 = _NON_BASE + 6,    
    NON7 = _NON_BASE + 7,        
    NON8 = _NON_BASE + 8,    

    _IMM_BASE = NON8 + 1,
    IMM0 = _IMM_BASE + 0,    
    IMM1 = _IMM_BASE + 1,        
    IMM2 = _IMM_BASE + 2,    
    IMM3 = _IMM_BASE + 3,        
    IMM4 = _IMM_BASE + 4,    
    IMM5 = _IMM_BASE + 5,        
    IMM6 = _IMM_BASE + 6,    
    IMM7 = _IMM_BASE + 7,        
    IMM8 = _IMM_BASE + 8,

    _TRI_BASE = IMM8 + 1,
    TRI0 = _TRI_BASE + 0,    
    TRI1 = _TRI_BASE + 1,        
    TRI2 = _TRI_BASE + 2,    
    TRI3 = _TRI_BASE + 3,        
    TRI4 = _TRI_BASE + 4,    
    TRI5 = _TRI_BASE + 5,        
    TRI6 = _TRI_BASE + 6,    
    TRI7 = _TRI_BASE + 7,        
    TRI8 = _TRI_BASE + 8,    

    _SYN_9_BASE = TRI8 + 1,
    SYN9_0 = _SYN_9_BASE + 0,  //_TRI_BASE + 9      

    _ChannelMode_MAX = SYN9_0 + 1
} ADC_CHANNEL_MODE;

typedef enum
{
    ADC_IDLE = 0,
    ADC_BUSY = 1,
} ADC_STATUS;


// AUXADC_QBIT
typedef enum
{
    ADC_SPLRATE1 = 1,
    ADC_SPLRATE2 = 2,
    ADC_SPLRATE3 = 3    
} ADC_SAMPLERATE_TYPE;

#if 0
// AUXADC_CON0, AUXADC_CON1, AUXADC_CON2
BOOL   Adc_Init();
void   AdcChannelMode_Reset();
BOOL   AdcChannelMode_Select(ADC_CHANNEL_MODE chlMde);
BOOL   AdcChannelMode_Set();

UINT16 AdcGetData(AdcChannelType channelNo);	
UINT16 AdcGetData0();    // TDMA event0
BOOL   Adc_Uninit();


// AUXADC_CON3
void AdcAutoSampleMode(BOOL bEnable);   // AUTOSET
void AdcAutoClearMode(BOOL bEnable);    // event1, default 1 (enable)
void AdcAutoClearMode0();                   // event0, default 1 (enable)

ADC_STATUS AdcGetStatus();    // default 0 (idle status)

// AUXADC_DTMA1_CNT
void     AdcTriggerTimer(kal_uint8 nTime);          // default 0: 0~7 bit
void     AdcTriggerEnable(BOOL bEnable);        // default 0: disable
void     AdcTriggerForce(BOOL bEnable);         // default 0: disable

// AUXADC_DTMA0_CNT
void     AdcTrigger0Timer(kal_uint8 nTime);         // default 0: 0~7 bit
void     AdcTrigger0Enable(BOOL bEnable);       // default 0: disable
void     AdcTrigger0Force(BOOL bEnable);        // default 0: disable


//-----------------------------------------------------------------------------
//  Background
BOOL AdcVoltage_Init(void (*callbackFunc) ());
BOOL AdcVoltage_UnInit();

  
// AUXADC_DET_VOLT
void     AdcVoltageSetTriggerWay(BOOL bHighTrigger);    // default 0: bit 15
BOOL AdcVoltageSetThreshold(kal_int16 nVolt);           // default 0: 0~9 bit

// AUXADC_DET_SEL
BOOL AdcVoltageSetChannel(AdcChannelType channelNo);

// AUXADC_DET_PERIOD
BOOL AdcVoltageDetectPeriod(UINT16 nPeriod);        // default 0: 0~13 bit

// AUXADC_DET_DEBT
BOOL AdcVoltageDebounceTimes(UINT16 nTimes);          // default 0: 0~13 bit

void     AdcSetupSampleRate(ADC_SAMPLERATE_TYPE splRate);

// PMU
BOOL AdcCheckValid();


//-----------------------------------------------------------------------------
// API
//-----------------------------------------------------------------------------
UINT16     ADC_GET_SAMPLE(ADC_CHANNEL_MODE channelMode);
double         ADC_GET_VOLTAGE(ADC_CHANNEL_MODE channelMode);
UINT16     ADC_GET_mVOLTAGE(ADC_CHANNEL_MODE channelMode);            

UINT16     ADC_GET_VBAT();
UINT16     ADC_GET_ISENSE();
UINT16     ADC_GET_CHRIN();

// Background
BOOL       ADC_SET_THRESHOLD(BOOL bLowerTrigger, UINT16 nDeciVoltThreshold/*DeciVoltage*/, 
                                 AdcChannelType noChannel, void (*callbackFunc)(), 
                                 UINT16* pnMiSecDetPeriod/*milliSecond, NULL=default*/, 
                                 UINT16* pnDetDebounceTimes);

BOOL       ADC_LOWBAT_SETTHRESHOLD(UINT16 nDeciVoltThreshold, void (*callbackFunc)());
#endif

#endif   /*_MTK_DVT_TEST_ADC_SW_H*/

