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

#include "tpd.h"
#define AUX_TP_DEBT           (AUXADC_BASE + 0x0050)
#define AUX_TP_CMD            (AUXADC_BASE + 0x0054)
#define AUX_TP_CON            (AUXADC_BASE + 0x0058)
#define AUX_TP_DATA0          (AUXADC_BASE + 0x005C)
#define AUXADC_CON3           (AUXADC_BASE + 0x000C)
#define AUXADC_CON3_STA_MASK  0x0001
#define TP_DEBT_MASK          0x3fff
#define TP_CMD_PD_MASK        0x0003
#define TP_CMD_PD_YDRV_SH     0x0000
#define TP_CMD_PD_IRQ_SH      0x0001
#define TP_CMD_PD_IRQ         0x0003
#define TP_CMD_SE_DF_MASK     0x0004
#define TP_CMD_DIFFERENTIAL   0x0000
#define TP_CMD_SINGLE_END     0x0004
#define TP_CMD_MODE_MASK      0x0008
#define TP_CMD_MODE_10BIT     0x0000
#define TP_CMD_MODE_8BIT      0x0008
#define TP_CMD_ADDR_MASK      0x0070
#define TP_CMD_ADDR_Y         0x0010
#define TP_CMD_ADDR_Z1        0x0030
#define TP_CMD_ADDR_Z2        0x0040
#define TP_CMD_ADDR_X         0x0050
#define TP_CON_SPL_MASK       0x0001
#define TP_CON_SPL_TRIGGER    0x0001
#define TP_CON_STATUS_MASK    0x0002
#define TP_DAT0_DAT_MASK      0x03ff
#define TP_DEBOUNCE_TIME      (1*32) /* 20ms */
#define TP_AUXADC_POWER_UP    0x0c000c00

/* DIFFERENTIAL | MODE_10BIT | PD_YDRV_SH */
#define TP_SAMPLE_SETTING     0x0000 

void tpd_adc_init();
u16 tpd_read(int position);
u16 tpd_read_adc(u16 pos);

void tpd_adc_init() {
    __raw_writew(TP_DEBOUNCE_TIME, AUX_TP_DEBT);
}

u16 tpd_read(int position) {
    switch(position) {
        default:
        case TPD_X:  return tpd_read_adc(TP_CMD_ADDR_X);
        case TPD_Y:  return tpd_read_adc(TP_CMD_ADDR_Y);
        case TPD_Z1: return tpd_read_adc(TP_CMD_ADDR_Z1);
        case TPD_Z2: return tpd_read_adc(TP_CMD_ADDR_Z2);
    } return 0;
}

/* pass command, return sampled data */
u16 tpd_read_adc(u16 pos) {
   __raw_writew(pos | TP_SAMPLE_SETTING, AUX_TP_CMD);
   __raw_writew(TP_CON_SPL_TRIGGER, AUX_TP_CON);
   while(TP_CON_SPL_MASK & __raw_readw(AUX_TP_CON)) { ; }
   return __raw_readw(AUX_TP_DATA0); 
}

u16 tpd_read_status() {
    return __raw_readw(AUX_TP_CON) & 2;
}
