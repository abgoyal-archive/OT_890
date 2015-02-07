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

#ifndef __LCD_REG_H__
#define __LCD_REG_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    unsigned RUN       : 1;
    unsigned DATA_PEND : 1;
    unsigned CMD_PEND  : 1;
    unsigned TE_PEND   : 1;
    unsigned rsv_4     : 12;
} LCD_REG_STATUS, *PLCD_REG_STATUS;


typedef struct
{
    unsigned COMPLETED : 1;
    unsigned DATA_PEND : 1;
    unsigned CMD_PEND  : 1;
    unsigned TE_PEND   : 1;
    unsigned rsv_4     : 12;
} LCD_REG_INTERRUPT, *PLCD_REG_INTERRUPT;


typedef struct
{
    unsigned rsv_0     : 15;
    unsigned START     : 1;
    unsigned rsv_16    : 16;
} LCD_REG_START, *PLCD_REG_START;


typedef struct
{
    unsigned SPO      : 1;
    unsigned SPH      : 1;
    unsigned DIV      : 2;
    unsigned IF_9_BIT : 1;
    unsigned CMB      : 1;
    unsigned CMG      : 1;
    unsigned CMR      : 1;
    unsigned CSP0     : 1;
    unsigned CSP1     : 1;
    unsigned MODE     : 1;
    unsigned rsv_11   : 1;
    unsigned GAMMA_ID : 2;
    unsigned CLK_26M  : 1;
    unsigned CLK_52M  : 1;
    unsigned rsv_16   : 16;
} LCD_REG_SCNF, *PLCD_REG_SCNF;


typedef struct
{
    unsigned RLT        : 5;
    unsigned CMB        : 1;
    unsigned CMG        : 1;
    unsigned CMR        : 1;
    unsigned WST        : 5;
    unsigned rsv_13     : 1;
    unsigned CLK_26M    : 1;
    unsigned CLK_52M    : 1;
    unsigned rsv_16     : 2;
    unsigned GAMMA_ID_B : 2;
    unsigned GAMMA_ID_G : 2;
    unsigned GAMMA_ID_R : 2;
    unsigned C2RS       : 4;
    unsigned C2WH       : 2;
    unsigned C2WS       : 2;
} LCD_REG_PCNF, *PLCD_REG_PCNF;


typedef struct
{
    unsigned ENABLE     : 1;
    unsigned EDGE_SEL   : 1;
    unsigned MODE       : 1;
    unsigned rsv_3      : 1;
    unsigned HS_MCH_CNT : 11;
    unsigned rsv_15     : 1;
    unsigned VS_CNT_DIV : 2;
    unsigned rsv_18     : 2;
    unsigned VS_WLMT    : 12;
} LCD_REG_TECON, *PLCD_REG_TECON;


typedef struct
{
    unsigned PCNF0_DW   : 3;
    unsigned rsv_3      : 5;
    unsigned PCNF1_DW   : 3;
    unsigned rsv_11     : 5;
    unsigned PCNF2_DW   : 3;
    unsigned rsv_19     : 13;
} LCD_REG_PCNFDW, *PLCD_REG_PCNFDW;


typedef struct
{
    UINT16 WIDTH;
    UINT16 HEIGHT;
} LCD_REG_SIZE, *PLCD_REG_SIZE;


typedef struct
{
    UINT16 X;
    UINT16 Y;
} LCD_REG_COORD, *PLCD_REG_COORD;


typedef struct
{
    unsigned W2LCM      : 1;
    unsigned W2M_FMT    : 2;
    unsigned DISCON     : 1;
    unsigned ADDINC_DIS : 1;
    unsigned DC_OUTEN   : 1;
    unsigned rsv_6      : 2;
    unsigned OUT_ALPHA  : 8;
    unsigned FB1_EN     : 1;
    unsigned FB2_EN     : 1;
    unsigned rsv_18     : 2;
    unsigned DLY_EN     : 1;
    unsigned rsv_21     : 2;
    unsigned WB_DIS     : 1;
    unsigned rsv_24     : 7;
    unsigned FBSEQ_RST  : 1;
} LCD_REG_WROI_W2MCON, *PLCD_REG_WROI_W2MCON;


typedef struct
{
    unsigned RGB_ORDER  : 1;
    unsigned BYTE_ORDER : 1;
    unsigned PADDING    : 1;
    unsigned DATA_FMT   : 3;
    unsigned INTF_FMT   : 3;
    unsigned rsv_9      : 4;
    unsigned DBS_EN     : 1;
    unsigned rsv_14     : 18;
} LCD_REG_WROI_FMT, *PLCD_REG_WROI_FMT;


typedef struct
{
    unsigned rsv_0   : 8;
    unsigned COMMAND : 5;
    unsigned COM_SEL : 1;
    unsigned W2M     : 1;
    unsigned ENC     : 1;
    unsigned PERIOD  : 10;
    unsigned EN5     : 1;
    unsigned EN4     : 1;
    unsigned EN3     : 1;
    unsigned EN2     : 1;
    unsigned EN1     : 1;
    unsigned EN0     : 1;
} LCD_REG_WROI_CON, *PLCD_REG_WROI_CON;


typedef struct
{
    unsigned HWREF    : 1;
    unsigned rsv_1    : 6;
    unsigned HWEN     : 1;
    unsigned rsv_9    : 8;
    unsigned HREF_SEL : 2;
    unsigned rsv_18   : 8;
    unsigned EN5      : 1;
    unsigned EN4      : 1;
    unsigned EN3      : 1;
    unsigned EN2      : 1;
    unsigned EN1      : 1;
    unsigned EN0      : 1;
} LCD_WROI_HWREF, *PLCD_WROI_HWREF;


typedef struct
{
    unsigned rsv_0   : 4;
    unsigned DC_SEL5 : 2;
    unsigned DC_SEL4 : 2;
    unsigned DC_SEL3 : 2;
    unsigned DC_SEL2 : 2;
    unsigned DC_SEL1 : 2;
    unsigned DC_SEL0 : 2;
    unsigned rsv_16  : 10;
    unsigned EN5     : 1;
    unsigned EN4     : 1;
    unsigned EN3     : 1;
    unsigned EN2     : 1;
    unsigned EN1     : 1;
    unsigned EN0     : 1;
} LCD_REG_WROI_DC, *PLCD_REG_WROI_DC;


typedef struct
{
    unsigned OPA        : 8;
    unsigned OPAEN      : 1;
    unsigned CLRDPT     : 2;
    unsigned ROTATE     : 3;
    unsigned KEY_EN     : 1;
    unsigned SRC        : 1;
    unsigned SWP        : 1;
    unsigned SRCL_EN    : 1;
    unsigned GMA_EN     : 1;
    unsigned rsv_19     : 1;
    unsigned RD_BUF_DIS : 1;
    unsigned rsv_21     : 11;
} LCD_REG_LAYER_CON, *PLCD_REG_LAYER_CON;


typedef struct
{
    LCD_REG_LAYER_CON CONTROL;
    UINT32            SRCKEY;
    LCD_REG_COORD     OFFSET;
    UINT32            ADDRESS;
    LCD_REG_SIZE      SIZE;
    LCD_REG_COORD     SCRL_OFFSET;
    UINT32            rsv_18[2];
} LCD_REG_LAYER, *PLCD_REG_LAYER;


typedef struct
{
    unsigned COL2   : 10;
    unsigned COL1   : 10;
    unsigned COL0   : 10;
    unsigned rsv_30 : 2;
} LCD_REG_COEF_ROW, *PLCD_REG_COEF_ROW;


typedef struct
{
    UINT8  XOFF;
    UINT8  YOFF;
    UINT16 SLOPE;
} LCD_REG_GAMMA, *PLCD_REG_GAMMA;


typedef struct
{
    unsigned TARGET_RATE : 4;
    unsigned rsv_4       : 12;
    unsigned LINE_LENGTH : 12;
    unsigned rsv_28      : 4;
} FBCE_REG_TARGET_RATE, *PFBCE_REG_TARGET_RATE;


typedef struct
{
    unsigned FLAG0 : 1;
    unsigned FLAG1 : 1;
    unsigned FLAG2 : 1;
    unsigned FLAG3 : 1;
    unsigned FLAG4 : 1;
    unsigned rsv_5 : 27;
} FBCE_REG_STATUS, *PFBCE_REG_STATUS;


typedef struct
{
    unsigned LINE_LENGTH : 12;
    unsigned rsv_12      : 4;
    unsigned IM          : 2;
    unsigned rsv_18      : 8;
    unsigned EN5         : 1;
    unsigned EN4         : 1;
    unsigned EN3         : 1;
    unsigned EN2         : 1;
    unsigned EN1         : 1;
    unsigned EN0         : 1;
} FBCD_REG_CON, *PFBCD_REG_CON;


typedef struct
{
    LCD_REG_STATUS      STATUS;             // 0000
    LCD_REG_INTERRUPT   INT_ENABLE;         // 0004
    LCD_REG_INTERRUPT   INT_STATUS;         // 0008
    LCD_REG_START       START;              // 000C
    UINT32              RESET;              // 0010

    LCD_REG_SCNF        SERIAL_CFG;         // 0014
    LCD_REG_PCNF        PARALLEL_CFG[3];    // 0018..0020
    LCD_REG_TECON       TEARING_CFG;        // 0024
    LCD_REG_PCNFDW      PARALLEL_DW;        // 0028
    UINT32              rsv_002C;           // 002C

    UINT32              WROI_W2M_ADDR[3];   // 0030..0038
    UINT32              rsv_003C;           // 003C
    LCD_REG_SIZE        MWIN_SIZE;          // 0040
    LCD_REG_COORD       WROI_W2M_OFFSET;    // 0044
    LCD_REG_WROI_W2MCON WROI_W2M_CONTROL;   // 0048
    LCD_REG_WROI_FMT    WROI_FORMAT;        // 004C
    LCD_REG_WROI_CON    WROI_CONTROL;       // 0050
    LCD_REG_COORD       WROI_OFFSET;        // 0054
    UINT16              WROI_CMD_ADDR;      // 0058
    UINT16              rsv_005A;           // 005A
    UINT16              WROI_DATA_ADDR;     // 005C
    UINT16              rsv_005E;           // 005E
    LCD_REG_SIZE        WROI_SIZE;          // 0060
    LCD_WROI_HWREF      WROI_HW_REFRESH;    // 0064
    LCD_REG_WROI_DC     WROI_DC;            // 0068
    UINT32              WROI_BG_COLOR;      // 006C

    LCD_REG_LAYER       LAYER[6];           // 0070..012C

    LCD_REG_COEF_ROW    COEF_ROW[3];        // 0130..0138
    UINT32              rsv_013C;           // 013C
    LCD_REG_GAMMA       GAMMA[20];          // 0140..018C
    UINT32              rsv_0190[28];       // 0190..01FC

    // Frame Buffer Compression

    FBCE_REG_TARGET_RATE FBCE_TARGET_RATE;  // 0200
    UINT32               FBCE_INPUT_MODE;   // 0204
    UINT32               FBCE_ACT_EN;       // 0208
    UINT32               FBCE_CLR_EN;       // 020C
    FBCE_REG_STATUS      FBCE_STATUS;       // 0210
    UINT32               rsv_0214[3];       // 0214..21C

    FBCD_REG_CON         FBCD_CON;          // 0220

    UINT32               rsv_0224[3959];    // 0224..3FFC

    UINT32               PDATA0;            // 4000
    UINT32               rsv_04004[1023];   // 4004..4FFC
    UINT32               PDATA1;            // 5000
    UINT32               rsv_05004[1023];   // 5004..5FFC
    UINT32               PDATA2;            // 6000
    UINT32               rsv_06004[2047];   // 6004..8FFC

    UINT32               SDATA1;            // 8000
    UINT32               rsv_08004[1023];   // 8004..8FFC
    UINT32               SDATA0;            // 9000
    UINT32               rsv_09004[3071];   // 9004..BFFC

    UINT32               GAMMA_TBL[1024];   // C000..CFFC
    UINT32               PALETTE[256];      // D000..D3FC
    UINT32               CMDQ[2][32];       // D400..D4FC
} volatile LCD_REGS, *PLCD_REGS;


STATIC_ASSERT(0x0030 == offsetof(LCD_REGS, WROI_W2M_ADDR));
STATIC_ASSERT(0x0070 == offsetof(LCD_REGS, LAYER));
STATIC_ASSERT(0x0130 == offsetof(LCD_REGS, COEF_ROW));
STATIC_ASSERT(0x0200 == offsetof(LCD_REGS, FBCE_TARGET_RATE));
STATIC_ASSERT(0x4000 == offsetof(LCD_REGS, PDATA0));
STATIC_ASSERT(0xC000 == offsetof(LCD_REGS, GAMMA_TBL));
STATIC_ASSERT(0xD500 == sizeof(LCD_REGS));

extern PLCD_REGS const LCD_REG;

#define LCD_A0_LOW_OFFSET  (0x0)
#define LCD_A0_HIGH_OFFSET (0x100)

#endif // __LCD_REG_H__
