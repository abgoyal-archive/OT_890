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
*  This software is protected by copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (c) 2008 MediaTek Inc.
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
#ifndef __MT6516_MONITOR_H__
#define __MT6516_MONITOR_H__

#include <mach/mt6516.h>

//----------------------------------------------------------------------------
// MT6516 performance monitor register

#define MT6516_MONITOR_VIRT          	 (IO_VIRT + 0x00039700)

#define MON_CON	     	 (MT6516_MONITOR_VIRT+0x00)
#define MON_SET      	 (MT6516_MONITOR_VIRT+0x04)
#define MON_CLR      	 (MT6516_MONITOR_VIRT+0x08)
#define MON_PERF1        (MT6516_MONITOR_VIRT+0x0c)
#define MON_PERF2        (MT6516_MONITOR_VIRT+0x10)
#define MON_PERF3		 (MT6516_MONITOR_VIRT+0x14)
#define MON_PERF4	     (MT6516_MONITOR_VIRT+0x18)
#define MON_PERF5	     (MT6516_MONITOR_VIRT+0x1c)
#define MON_PERF6        (MT6516_MONITOR_VIRT+0x20)
#define MON_PERF7   	 (MT6516_MONITOR_VIRT+0x24)
#define MON_PERF8        (MT6516_MONITOR_VIRT+0x28)
#define MON_PERF9	     (MT6516_MONITOR_VIRT+0x2c)
#define MON_PERF10       (MT6516_MONITOR_VIRT+0x30)
#define MON_PERF11	     (MT6516_MONITOR_VIRT+0x34)
#define MON_PERF12       (MT6516_MONITOR_VIRT+0x38)
#define MON_PERF13       (MT6516_MONITOR_VIRT+0x3c)
#define MON_PERF14       (MT6516_MONITOR_VIRT+0x40)
#define MON_PERF15       (MT6516_MONITOR_VIRT+0x44)
#define MON_PERF16	     (MT6516_MONITOR_VIRT+0x48)
#define MON_PERF17       (MT6516_MONITOR_VIRT+0x4c)
#define MON_PERF18       (MT6516_MONITOR_VIRT+0x50)
#define MON_PERF19	     (MT6516_MONITOR_VIRT+0x54)
#define MON_PERF20       (MT6516_MONITOR_VIRT+0x58)
#define MON_PERF21	     (MT6516_MONITOR_VIRT+0x5c)
#define MON_PERF22       (MT6516_MONITOR_VIRT+0x60)

//----------------------------------------------------------------------------
// MT6516 performance register offset
typedef enum
{
    DCM_EN          = 0,
    ICM_EN 	      = 1, 
    ACTIVE_EN     = 2, 
    DTLB_EN	      = 3, 
    ITLB_EN 	      = 4, 
    DCP_EN 	      = 5, 
    ICP_EN 	      = 6, 
    DEXT_EN 	      = 7, 
    IEXT_EN 	      = 8,
    DAHB_EN 	      = 9,
    IAHB_EN 	      = 10,
    DCM_CLR 	      = 11,
    ICM_CLR 	      = 12,
    ACTIVE_CLR    = 13,
    DTLB_CLR 	      = 14,
    ITLB_CLR 	      = 15,
    DCP_CLR 	      = 16,
    ICP_CLR 		= 17,
    DEXT_CLR 	= 18,
    IEXT_CLR 		= 19,
    DAHB_CLR 	= 20,
    IAHB_CLR 		= 21,
    DAHB_SEL_EXT_MEM = 22,
    DAHB_SEL_INT_MEM = 23,    
    DAHB_SEL_APB_REG = 24        
} MT6516_MON_MODE;


//----------------------------------------------------------------------------
// MT6516 performance monitor command

typedef enum
{
    EXT_MEM = 0,
    INT_MEM = 1,    
    REG = 2        
} MT6516_DAHB_SEL;

//----------------------------------------------------------------------------
// MT6516 performance monitor api

void Monitor_EnableMonControl (MT6516_MON_MODE mode);
void Monitor_DisableMonControl (MT6516_MON_MODE mode);
unsigned char Monitor_GetMonControlStatus (MT6516_MON_MODE mode);
void Monitor_SetDAHB (MT6516_DAHB_SEL sel);
void Monitor_DcacheMissBegin(void);
void Monitor_DcacheMissEnd(void);
void Monitor_IcacheMissBegin(void);
void Monitor_IcacheMissEnd(void);
void Monitor_ARMActiveBegin(void);
void Monitor_ARMActiveEnd(void);
void Monitor_DTLBPenaltyBegin(void);
void Monitor_DTLBPenaltyEnd(void);
void Monitor_ITLBPenaltyBegin(void);
void Monitor_ITLBPenaltyEnd(void);
void Monitor_DcachePenaltyBegin(void);
void Monitor_DcachePenaltyEnd(void);
void Monitor_IcachePenaltyBegin(void);
void Monitor_IcachePenaltyEnd(void);
void Monitor_DEXTPenaltyBegin(void);
void Monitor_DEXTPenaltyEnd(void);
void Monitor_IEXTPenaltyBegin(void);
void Monitor_IEXTPenaltyEnd(void);
void Monitor_DAHBBegin(MT6516_DAHB_SEL sel);
void Monitor_DAHBEnd(void);
void Monitor_IAHBBegin(void);
void Monitor_IAHBEnd(void);


#endif  /* __MT6516_MONITOR_H__ */

