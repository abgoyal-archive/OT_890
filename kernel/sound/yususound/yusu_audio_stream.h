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
 * Yusu_audio_stream.h
 *
 * Project:
 * --------
 *   yusu
 *
 * Description:
 * ------------
 *   dirver Audio Stream operation
 *
 * Author:
 * -------
 *   ChiPeng Chang(mtk02308)
 *
 *------------------------------------------------------------------------------
 * $Revision$
 * $Modtime:$
 * $Log:$
 *
 * 06 01 2010 chipeng.chang
 * [ALPS00002042][Patch rule]

If you fix the issue, please check-in your code and choose the correct type on the CR you use to check in BU Spec tab, thanks.

 
 * patch for I2S driver and add AudioyusuI2sTreamin for record
 *
 * 05 17 2010 chipeng.chang
 * [ALPS00006429][Call] AEE happened after save recording file.
 * when mediaserver is killed , enable CCI and recovery from previous state , previsous state in save in kernel sapce driver.
 *
 *******************************************************************************/
#ifndef _YUSU_AUDIO_STREAM_H_
#define _YUSU_AUDIO_STREAM_H_

#define ASM_SRC_BLOCK_MAX (1)
#define EDI_BLOCK_MAX (1)
#define ASM_STREAM_DIGITAL_GAIN (0x1000)

//#define CONFIG_DEBUG_MSG
//#define SOUND_FAKE_READ

typedef enum
{
    Mmap_None = -1,
    Mmap_Src_Block0,
    Mmap_Src_Block1,
    Mmap_Src_Block2
}_MMAP_BLOCK;


typedef struct
{
    kal_uint32 pucPhysBufAddr;
    kal_uint8 *pucVirtBufAddr;
    kal_int32 u4BufferSize;
    kal_int32 u4DataRemained;
    kal_uint32 u4SampleNumMask;    // sample number mask
    kal_uint32 u4SamplesPerInt;    // number of samples to play before interrupting
    kal_int32 u4WriteIdx;         // Previous Write Index.
    kal_int32 u4DMAReadIdx;       // Previous DMA Read Index.
    kal_uint32 u4InterruptLine;
    kal_uint32 u4fsyncflag;
    struct file *flip;
} ASM_SRC_BLOCK_T;


typedef struct
{

    ASM_SRC_BLOCK_T rSRCBlock[ASM_SRC_BLOCK_MAX];
    kal_uint32    u4BufferSize;
    kal_uint32    u4SRCBlockOccupiedNum;          // Occupied Block Number
    kal_bool        bRunning;
} ASM_CONTROL_T;

typedef struct
{
    ASM_SRC_BLOCK_T rSRCBlock[EDI_BLOCK_MAX];
    kal_uint32    u4BufferSize;
    kal_uint32    u4SRCBlockOccupiedNum;          // Occupied Block Number
    kal_bool        bRunning;
} EDI_CONTROL_T;

typedef struct
{
    int bSpeechFlag;
    int bBgsFlag;
    int bRecordFlag;
    int bTtyFlag;    
}Audio_Control_T;

#endif
