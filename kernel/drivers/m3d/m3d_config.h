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
*  permission of MediaTek Inc. (C) 2007
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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   m3d_config.h
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   Implementation of OpenGL ES kernel driver -- The Platform dependent part.
 *
 * Author:
 * -------
 *   Robin Huang(mtk02174)/ChenHung Yang(mtk02281)
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Log$
 *
 * May 15 2009 mtk02174
 * [DUMA00005880] [OpenGLES 1.X] M3D H/W Driver first check-in
 * 
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
 
#ifndef __M3D_CONFIG_H__
#define __M3D_CONFIG_H__

/// #include "macros.h"
#define MAX_BUFFER_OBJECT   256

#define MAX_VIEW_WIDTH      240
#define MAX_VIEW_HEIGHT     320

#define MAX_LIGHT_SOURCES   8

#define MAX_MATRIX_DEPTH    32

/// #define MIN_POINT_SIZE      65536
/// #define MAX_POINT_SIZE      (65536 * 17)

/// Minimum point size
/// #define MIN_POINT_SIZE _GL_ONE
#define MIN_POINT_SIZE GL_N_ONE
/// Maximum point size
/// #define MAX_POINT_SIZE (_GL_ONE * SQRT_MAX_WIDTH)
/// #define MAX_POINT_SIZE (65536 * 17)
#define MAX_POINT_SIZE (GL_N_ONE * 17)

/// Point size granularity
#if defined(__COMMON_LITE__)
   #define POINT_SIZE_GRANULARITY (0.1F * 65536)
#else
   #define POINT_SIZE_GRANULARITY (0.1F)
#endif

#define MIN_LINE_WIDTH      GL_N_ONE       
#define MAX_LINE_WIDTH      (GL_N_ONE * 4)

#define MAX_CLIP_PLANES     1

/// Number of 2D texture mipmap levels
#define MAX_TEXTURE_LEVELS   9

#define MAX_TEXTURE_SIZE    256//(1 << (MAX_TEXTURE_LEVELS - 1))
#define MAX_TEXTURE_UNITS   3

#define SUB_PIXEL_BITS      4

/// Bit flags used for updating material values.
#define MAT_ATTRIB_FRONT_AMBIENT           0
#define MAT_ATTRIB_FRONT_DIFFUSE           1
#define MAT_ATTRIB_FRONT_SPECULAR          2
#define MAT_ATTRIB_FRONT_EMISSION          3
#define MAT_ATTRIB_FRONT_SHININESS         4
#define MAT_ATTRIB_MAX                     5

#define MAT_BIT_FRONT_AMBIENT         (1 << MAT_ATTRIB_FRONT_AMBIENT)
#define MAT_BIT_FRONT_DIFFUSE         (1 << MAT_ATTRIB_FRONT_DIFFUSE)
#define MAT_BIT_FRONT_SPECULAR        (1 << MAT_ATTRIB_FRONT_SPECULAR)
#define MAT_BIT_FRONT_EMISSION        (1 << MAT_ATTRIB_FRONT_EMISSION)
#define MAT_BIT_FRONT_SHININESS       (1 << MAT_ATTRIB_FRONT_SHININESS)
#define MAT_BIT_ALL ~0

#define GL_IMPLEMENTATION_COLOR_READ_TYPE_OES_VALUE     GL_UNSIGNED_BYTE
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES_VALUE   GL_RGBA
            
/// The number of vertex of each 3d model should be less than this value
/// to achieve maximal performance.
#ifdef __HW_C_MODEL__
   #define MAX_ARRAY_LOCK_SIZE 512
#else
   #define MAX_ARRAY_LOCK_SIZE 256
#endif


            
#endif  // __M3D_CONFIG_H__
