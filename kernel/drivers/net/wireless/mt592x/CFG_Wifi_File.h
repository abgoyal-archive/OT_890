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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
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


/*******************************************************************************
 *
 * Filename:
 * ---------
 *   cfg_wifi_file.h
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *    header file of main function
 *
 * Author:
 * -------
 *   Ning.F (MTK08139) 09/11/2008
 *
 *------------------------------------------------------------------------------
 * $Revision:$
 * $Modtime:$
 * $Log:$
 *
 * 07 10 2010 renbang.jiang
 * [ALPS00121785][Need Patch] [Volunteer Patch] use NVRAM to save Wi-Fi custom data 
 * .
 *
 * 05 27 2010 renbang.jiang
 * [ALPS00007371][Need Patch] [Volunteer Patch] ALPS.10X.W10.11 Volunteer patch for WiFi Un-GPL license 
 * .
 *
 * 05 27 2010 renbang.jiang
 * [ALPS00007371][Need Patch] [Volunteer Patch] ALPS.10X.W10.11 Volunteer patch for WiFi Un-GPL license 
 * .
 *
 * Jul 9 2009 mtk80306
 * [DUMA00122953] optimize nvram and change meta clean boot flag.
 * modify wifi str
 *
 * Mar 21 2009 mtk80306
 * [DUMA00112158] fix the code convention.
 * fix the codeing convention.
 *
 * Mar 9 2009 mtk80306
 * [DUMA00111088] nvram customization
 * change wifi cmd structure.
 *
 * Dec 17 2008 mbj08139
 * [DUMA00105099] create meta code
 *
 *
 * Dec 8 2008 mbj08139
 * [DUMA00105099] create meta code
 *
 *
 * Nov 24 2008 mbj08139
 * [DUMA00105099] create meta code
 *
 *
 * Oct 29 2008 mbj08139
 * [DUMA00105099] create meta code
 *
 *
 *
 *
 *******************************************************************************/



#ifndef _CFG_WIFI_FILE_H
#define _CFG_WIFI_FILE_H

// the record structure define of wifi nvram file
typedef struct
{
    unsigned short usSignature;
    unsigned short usSdioCccr;
    unsigned short usSdioSrw;
    unsigned short usSdioIoCode;
    unsigned short usSdioSps;
    unsigned short usSdioOcrL;
    unsigned short usSdioOcrH;
    unsigned short usDbg;
    unsigned short usSdramCfg;
    unsigned short usHifCtl;
    unsigned short usCisStart;
    unsigned short usCisLen;
    unsigned short usChkSum;
    unsigned short usRsv_D;
    unsigned short usRsv_E;
    unsigned short usRegDomain;
    unsigned short usOscStableTime;
    unsigned short usXtalFreqTrim;
    unsigned short usVersion;
    unsigned short usMacAddr[3];
    unsigned short usThermoUsageBandSel;
    unsigned short usVgaThermoSlope;
    unsigned short us2GCckTxPwrTable[21];
    unsigned short us2GOfdm0TxPwrTable[21];
    unsigned short us2GOfdm1TxPwrTable[21];
    unsigned short us2GOfdm2TxPwrTable[21];
    unsigned short us2GOfdm3TxPwrTable[21];
    unsigned short usThermoInfo;
    unsigned short usDaisyChainCfg;
    unsigned short us2GRcpiOffsetTable[7];
    unsigned short usRsv_8A[21];
    unsigned short usNICSettingCheckSum;
    unsigned short usSdioCisContent[96];
} MT5921_CFG_PARAM_STRUCT, WIFI_CFG_PARAM_STRUCT;


#define CFG_FILE_WIFI_REC_SIZE    sizeof(WIFI_CFG_PARAM_STRUCT)
#define CFG_FILE_WIFI_WORD_SIZE   (CFG_FILE_WIFI_REC_SIZE/sizeof(WORD))
#define CFG_FILE_WIFI_REC_TOTAL   1


// the record structure define of wifi nvram file
typedef struct
{
    unsigned char mac[6];
} WIFI_Addr_Struct;


#define CFG_FILE_WIFI_ADDR_REC_SIZE    sizeof(WIFI_Addr_Struct)
#define CFG_FILE_WIFI_ADDR_REC_TOTAL   1

typedef struct
{
    unsigned long     u4SdBlockSize;      /* SDIO block size */
    unsigned long     u4SdBusWidth;       /* SDIO bus width. 1 or 4 */
    unsigned long     u4SdClockRate;      /* SDIO clock rate. (in unit of HZ) */

    unsigned long     u4BTCoexistWindowType; /*  */
    unsigned long     u4EnableTxAutoFragmentForBT; /*  */
    unsigned long     u4BtCR0;            /* BTCR0 value (correcspondent to register setting) */
    unsigned long     u4BtCR1;            /* BTCR1 value (correcspondent to register setting) */
    unsigned long     u4BtCR2;            /* BTCR2 value (correcspondent to register setting) */
    unsigned long     u4BtCR3;            /* BTCR3 value (correcspondent to register setting) */

    unsigned long     u4SingleAclBtCR0;            /* BTCR0 value (correcspondent to register setting) */
    unsigned long     u4SingleAclBtCR1;            /* BTCR1 value (correcspondent to register setting) */
    unsigned long     u4SingleAclBtCR2;            /* BTCR2 value (correcspondent to register setting) */
    unsigned long     u4SingleAclBtCR3;            /* BTCR3 value (correcspondent to register setting) */

    unsigned long     u4SingleMixBtCR0;            /* BTCR0 value (correcspondent to register setting) */
    unsigned long     u4SingleMixBtCR1;            /* BTCR1 value (correcspondent to register setting) */
    unsigned long     u4SingleMixBtCR2;            /* BTCR2 value (correcspondent to register setting) */
    unsigned long     u4SingleMixBtCR3;            /* BTCR3 value (correcspondent to register setting) */

    unsigned long     u4DualAclBtCR0;            /* BTCR0 value (correcspondent to register setting) */
    unsigned long     u4DualAclBtCR1;            /* BTCR1 value (correcspondent to register setting) */
    unsigned long     u4DualAclBtCR2;            /* BTCR2 value (correcspondent to register setting) */
    unsigned long     u4DualAclBtCR3;            /* BTCR3 value (correcspondent to register setting) */

    unsigned long     u4DualMixBtCR0;            /* BTCR0 value (correcspondent to register setting) */
    unsigned long     u4DualMixBtCR1;            /* BTCR1 value (correcspondent to register setting) */
    unsigned long     u4DualMixBtCR2;            /* BTCR2 value (correcspondent to register setting) */
    unsigned long     u4DualMixBtCR3;            /* BTCR3 value (correcspondent to register setting) */

    unsigned long     u4BTSetting;        /* useless for BT only */
    unsigned long     u4SingleAntenna;    /* 0: Not single antenna, 1: Single antenna */
    unsigned long     u4BtProfile;        /* 0: No PTA, 1: SCO, 2: A2DP(ACL), 3: See btcr0-3 */
    unsigned long     u4PtaEnabled;

    unsigned char     aucMacAddr[6];      /* MAC address */
    unsigned short    aucCountryCode[4];  /* Country code (in ISO 3166-1 expression, ex: "US", "TW")  */
    unsigned long     u4UapsdAc;          /* UAPSD AC (in bitwise OR of: a. 0x11-AC0, b. 0x22-AC1, a. 0x44-AC0, a. 0x88-AC0) */
    unsigned long     u4PowerMode;        /* Power mode (0: CAM, 1: MAX, 2: FAST) */
    unsigned long     u4AtimWindow;       /* ATIM Window value (in unit of TU) */
    unsigned long     u4VoipInterval;     /* VoIP interval (in unit of ms) */
    unsigned long     u4PollInterval;     /* Poll interval (in unit of ms) */
    unsigned long     u4PollIntervalB;    /* Poll interval, if Wi-Fi no traffic (in unit of ms) */
    unsigned long     u4L3PktFilterEn;    /* Enable pattern search for broadcast ARP filter */
    unsigned long     u4AdHocMode;        /* Adhoc mode */
    unsigned long     u4RoamingEn;        /* Enable Roaming function */

    unsigned long     u4MultiDomainCap;   /* Multi-domain capability */

    unsigned long     u4GPIO2_Mode;       /* GPIO2 mode (0: GPI, 1: INT, 2: Daisy) */

    unsigned long     u4ViAifsnBias;      /* Define VI AIFSN BIAS (Default: 0, 0~15) */
                                        /* Actual AIFSN = AIFSN from AP + AIFSN Bias */
    unsigned long     u4ViMaxTxopLimit;   /* Define VI MAX TXOP LIMIT(A upper bound) (Default: 0xFFFF, 0~0xFFFF) */

    unsigned long     u4InitDelayInd;     /* Define how many MS to indicate DISCONNECTION after initialized */


    unsigned long     u4UseWapi;          /* Define use at wapi mode, control by wapi ui (Default: 0 disable, 1~0xFFFFFFFF: enable) */

    unsigned long     u4DaisyChainEnable; /* whether Daisy Chain is enabled: 0: disabled, 1: enabled */

    unsigned short    u2LedBlinkMode;
    unsigned long     u4LedBlinkOnTime;
    unsigned long     u4LedBlinkOffTime;
	
    unsigned char     ucWmmPsEnable;
    
    unsigned long     u4Resv[8];         /* Reserved */
}MT5921_CUSTOM_PARAM_STRUCT, WIFI_CUSTOM_PARAM_STRUCT, REG_INFO_T, *P_REG_INFO_T;


#define CFG_FILE_WIFI_CUSTOM_REC_SIZE    sizeof(MT5921_CUSTOM_PARAM_STRUCT)
#define CFG_FILE_WIFI_CUSTOM_REC_TOTAL   1


#endif


