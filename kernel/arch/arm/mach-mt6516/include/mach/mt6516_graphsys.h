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
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   mt6516_apmcusys.h
 *
 * Project:
 * --------
 *   Andes
 *
 * Description:
 * ------------
 *   MT6516 GRAPHSYS (0x80092000) and (0x800A7000) power control
 *
 * Author:
 * -------
 *   Kelvin Yang   (mtk01638)
 *
 ****************************************************************************/

#ifndef __MT6516_GRAPHSYS__
#define __MT6516_GRAPHSYS__

#include    <mach/mt6516_typedefs.h>
#include    <mach/mt6516_reg_base.h>


/*******************************************************************************
 * Constant definition
 *******************************************************************************/

#define GRAPH1SYS_CG_CON        (GRAPH1SYS_CONFG_BASE + 0x300)
#define GRAPH1SYS_CG_SET        (GRAPH1SYS_CONFG_BASE + 0x320)
#define GRAPH1SYS_CG_CLR        (GRAPH1SYS_CONFG_BASE + 0x340)
#define GRAPH1SYS_LCD_IO_SEL    (GRAPH1SYS_CONFG_BASE + 0x400)
#define GRAPH1SYS_DELSEL0       (GRAPH1SYS_CONFG_BASE + 0x600)
#define GRAPH1SYS_DELSEL1       (GRAPH1SYS_CONFG_BASE + 0x604)
#define GRAPH1SYS_DELSEL2       (GRAPH1SYS_CONFG_BASE + 0x608)
#define GRAPH1SYS_DELSEL3       (GRAPH1SYS_CONFG_BASE + 0x60C)

#define GRAPH2SYS_CG_CON        (GRAPH2SYS_BASE + 0x0)
#define GRAPH2SYS_CG_SET        (GRAPH2SYS_BASE + 0x4)
#define GRAPH2SYS_CG_CLR        (GRAPH2SYS_BASE + 0x8)
#define GRAPH2SYS_DESEL0        (GRAPH2SYS_BASE + 0x10)
#define GRAPH2SYS_DESEL1        (GRAPH2SYS_BASE + 0x14)
#define GRAPH2SYS_DESEL2        (GRAPH2SYS_BASE + 0x18)


/*******************************************************************************
 * offset definition
 *******************************************************************************/

typedef enum
{
    PDN_MM_GMC1	    =	0,
    PDN_MM_G2D	    =	1,
    PDN_MM_GCMQ	    =	2,
    PDN_MM_BLS	    =	3,
    PDN_MM_IMGDMA0	=	4,
    PDN_MM_PNG	    =	5,
    PDN_MM_DSI	    =	6,
    PDN_MM_TVE	    =	8,
    PDN_MM_TVC	    =	9,
    PDN_MM_ISP	    =	10,
    PDN_MM_IPP	    =	11,
    PDN_MM_PRZ	    =	12,
    PDN_MM_CRZ	    =	13,
    PDN_MM_DRZ	    =	14,
    PDN_MM_WT	    =	16,
    PDN_MM_AFE	    =	17,
    PDN_MM_SPI	    =	19,
    PDN_MM_ASM	    =	20,                    
    PDN_MM_RESZLB	=	22,                    
    PDN_MM_LCD	    =	23,                    
    PDN_MM_DPI	    =	24,                    
    PDN_MM_G1FAKE	=	25,                                 
} GRAPH1SYS_PDN_MODE;


typedef enum
{
    //GRAPH2SYS Clock Gating    
    PDN_MM_GMC2	    =	0,
    PDN_MM_IMGDMA1	=	1,
    PDN_MM_PRZ2	    =	2,
    PDN_MM_M3D	    =	3,
    PDN_MM_H264	    =	4,
    PDN_MM_DCT	    =	5,
    PDN_MM_JPEG	    =	6,
    PDN_MM_MP4	    =	7,
    PDN_MM_MP4DBLK	=	8,       
} GRAPH2SYS_PDN_MODE;


typedef enum
{
    GRAPH1SYS_LCD_IO_SEL_CPU_IF_ONLY = 0,
    GRAPH1SYS_LCD_IO_SEL_8CPU_18RGB  = 1,
    GRAPH1SYS_LCD_IO_SEL_9CPU_16RGB  = 2,
    GRAPH1SYS_LCD_IO_SEL_18CPU_8RGB  = 3,
    GRAPH1SYS_LCD_IO_SEL_MASK        = 0x3,
} GRAPH1SYS_LCD_IO_SEL_MODE;


#endif 



