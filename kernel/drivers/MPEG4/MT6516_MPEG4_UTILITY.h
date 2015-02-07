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
 * mp4_utility.h
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   mp4 codec utility header file
 *
 * Author:
 * -------
 * mtk01845 (Port from Nucleus, provided by mtk00796)
 *
 *------------------------------------------------------------------------------
 * $Revision$
 * $Modtime:$
 * $Log:$
 *
 * Feb 10 2009 mtk01845
 * [DUMA00001897] [Drv][Video] Video driver revise for easily debugging
 * 
 *
 * Dec 10 2008 mtk01845
 * [DUMA00001084] First Check in of MT6516 multimedia drivers
 * add to source control recursely
 *
 * Apr 14 2008 mtk01845
 * [DUMA00000073] [Drv][Video] Video driver first check-in
 * add to source control recursely
 *
 *******************************************************************************/
/** 
 * @file 
 *   MT6516_MPEG4_UTILITY.h
 *
 * @par Project:
 *   Yusu 
 *
 * @par Description::
 *   mp4 codec utility header file
 *
 * @par Author:
 *   TeChien Chen (mtk01790)
 *
 * @par $Revision$
 * @par $Modtime:$
 * @par $Log:$
 *
 */
#ifndef __MT6516_MPEG4_UTILITY_H__
#define __MT6516_MPEG4_UTILITY_H__
#include <mach/mt6516_typedefs.h> 
/*
#ifndef _NUCLEUS_TYPE_ALIAS_
#define _NUCLEUS_TYPE_ALIAS_
#include <windows.h>
// define these to get type alias for compatible with Nucleus
#define kal_bool	BOOL
#define KAL_TRUE	TRUE
#define KAL_FALSE	FALSE
#define kal_uint32	DWORD
#define kal_int32	INT32
#define kal_uint16	WORD
#define kal_uint8	UINT8

#endif //_NUCLEUS_TYPE_ALIAS_

#define DRV_WriteReg32(addr,data)     ((*(volatile UINT32 *)(addr)) = (UINT32)(data))

// The following DRV_WriteReg32 is debug version
//#define DRV_WriteReg32(addr,data){\
//	((*(volatile UINT32 *)(addr)) = (UINT32)(data));\
//	RETAILMSG(1, (TEXT("Write addr(0x%08x), value: 0x%08x\r\n"), addr, data));\
//	RETAILMSG(1, (TEXT("Read back value: 0x%08x\r\n"), (*(volatile UINT32 *)(addr))));\
//	};

#define DRV_Reg32(addr)               (*(volatile UINT32 *)(addr))
*/


/////////////////////////////////////////
//  spec related
#define I_VOP		               0
#define P_VOP		               1
#define B_VOP		               2
#define S_VOP		               3

#define VO_START_CODE		      0x8
#define VO_START_CODE_MIN	      0x100
#define VO_START_CODE_MAX	      0x11f

#define VOL_START_CODE		      0x12
#define VOL_START_CODE_MIN	      0x120
#define VOL_START_CODE_MAX	      0x12f

#define VOS_START_CODE           0x1b0
#define USR_START_CODE           0x1b2
#define GOP_START_CODE           0x1b3
#define VSO_START_CODE           0x1b5
#define VOP_START_CODE	         0x1b6
#define STF_START_CODE           0x1c3 // stuffing_start_code
#define SHV_START_CODE           0x020
#define SHV_END_MARKER           0x03f

///////////////////////////
//   utility definition

#define MP4_MAX(a,b)                 ( ((a)>(b))? a : b )
#define MP4_MIN(a,b)                 ( ((a)<(b))? a : b )
#define MP4_ENFORCE_MINMAX(a,b,c)    MP4_MAX(a,(MP4_MIN(b,c)))

#define VIDEO_MAX(a,b)                 ( ((a)>(b))? a : b )
#define VIDEO_MIN(a,b)                 ( ((a)<(b))? a : b )
#define ENFORCE_MINMAX(a,b,c)    VIDEO_MAX(a,(VIDEO_MIN(b,c)))

//#define MP4_ANYBASE_TO_ANYBASE(_TIME_SRC, _TIME_BASE_SRC, _TIME_BASE_DST)    ((kal_uint64)_TIME_SRC*(kal_uint64)_TIME_BASE_DST/(kal_uint64)_TIME_BASE_SRC)


/////////////////////////////
// functions
/////////////////////////////

/**
 * @par Function:
 *  mp4_warning
 * @par Description:
 *    - debug function, to save warning line
 *
 * @param line            [IN]        source line
 *
 * @par Returns:
 *  none 
 */
extern void mp4_warning(UINT32 line);

/**
 * @par Function:
 *  mp4_util_show_bits
 * @par Description:
 *    - to get some bits
 *
 * @param *data             [IN]        data
 * @param bitcnt            [IN]        current bits position
 * @param num               [IN]        number of bits to get
 *
 * @par Returns:
 *  bits we get
 */
extern INT32 mp4_util_show_bits(UINT8 * data, INT32 bitcnt, INT32 num);
/**
 * @par Function:
 *  mp4_util_get_bits
 * @par Description:
 *    - wrapper for mp4_util_show_bits
 *
 * @param *data              [IN]        data
 * @param *bitcnt            [IN]        current bits position
 * @param num                [IN]        number of bits to get
 *
 * @par Returns:
 *  bits we get
 */
extern INT32 mp4_util_get_bits(UINT8 * data, INT32 *bitcnt, INT32 num);
/**
 * @par Function:
 *  mp4_util_show_word
 * @par Description:
 *    - get word
 *
 * @param *a              [IN]        data

 * @par Returns:
 *  word we get
 */
extern INT32 mp4_util_show_word(UINT8 * a);
/**
 * @par Function:
 *  mp4_util_log2ceil
 * @par Description:
 *    - get log2 result
 *
 * @param arg              [IN]        data

 * @par Returns:
 *  result
 */
extern INT32 mp4_util_log2ceil(INT32 arg);

extern INT32 mp4_util_user_data(UINT8 * data, INT32 * bitcnt, UINT32 max_parse_data_size);

/**
 * @par Function:
 *  mp4_putbits
 * @par Description:
 *    - put bits in destination memory
 *
 * @param *in               [IN]        destination
 * @param *bitcnt           [IN]        current bits position
 * @param data              [IN]        data to put
 * @param data_length       [IN]        data length
 *
 * @par Returns:
 *  none
 */
extern void mp4_putbits(UINT8 * in, INT32 * bitcnt, INT32 data, INT32 data_length);


#endif /*__MT6516_MPEG4_UTILITY_H__*/


