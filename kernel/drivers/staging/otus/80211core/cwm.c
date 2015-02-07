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

/*
 * Copyright (c) 2007-2008 Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/*                                                                      */
/*  Module Name : cwm.c                                                 */
/*                                                                      */
/*  Abstract                                                            */
/*      This module contains channel width related functions.           */
/*                                                                      */
/*  NOTES                                                               */
/*      None                                                            */
/*                                                                      */
/************************************************************************/

#include "cprecomp.h"



void zfCwmInit(zdev_t* dev) {
    //u16_t i;
    zmw_get_wlan_dev(dev);

    switch (wd->wlanMode) {
    case ZM_MODE_AP:
        wd->cwm.cw_mode = CWM_MODE2040;
        wd->cwm.cw_width = CWM_WIDTH40;
        wd->cwm.cw_enable = 1;
        break;
    case ZM_MODE_INFRASTRUCTURE:
    case ZM_MODE_PSEUDO:
    case ZM_MODE_IBSS:
    default:
        wd->cwm.cw_mode = CWM_MODE2040;
        wd->cwm.cw_width = CWM_WIDTH20;
        wd->cwm.cw_enable = 1;
        break;
    }
}


void zfCoreCwmBusy(zdev_t* dev, u16_t busy)
{

    zmw_get_wlan_dev(dev);

    zm_msg1_mm(ZM_LV_0, "CwmBusy=", busy);

    if(wd->cwm.cw_mode == CWM_MODE20) {
        wd->cwm.cw_width = CWM_WIDTH20;
        return;
    }

    if(wd->cwm.cw_mode == CWM_MODE40) {
        wd->cwm.cw_width = CWM_WIDTH40;
        return;
    }

    if (busy) {
        wd->cwm.cw_width = CWM_WIDTH20;
        return;
    }


    if((wd->wlanMode == ZM_MODE_INFRASTRUCTURE || wd->wlanMode == ZM_MODE_PSEUDO ||
        wd->wlanMode == ZM_MODE_IBSS)) {
        if (wd->sta.ie.HtCap.HtCapInfo && HTCAP_SupChannelWidthSet != 0 &&
            wd->sta.ie.HtInfo.ChannelInfo && ExtHtCap_RecomTxWidthSet != 0 &&
            (wd->sta.ie.HtInfo.ChannelInfo && ExtHtCap_ExtChannelOffsetAbove) == 1) {

            wd->cwm.cw_width = CWM_WIDTH40;
        }
        else {
            wd->cwm.cw_width = CWM_WIDTH20;
        }

        return;
    }

    if(wd->wlanMode == ZM_MODE_AP) {
        wd->cwm.cw_width = CWM_WIDTH40;
    }

}




u16_t zfCwmIsExtChanBusy(u32_t ctlBusy, u32_t extBusy)
{
    u32_t busy; /* percentage */
    u32_t cycleTime, ctlClear;

    cycleTime = 1280000; //1.28 seconds

    if (cycleTime > ctlBusy) {
        ctlClear = cycleTime - ctlBusy;
    }
    else
    {
        ctlClear = 0;
    }

    /* Compute ratio of extension channel busy to control channel clear
     * as an approximation to extension channel cleanliness.
     *
     * According to the hardware folks, ext rxclear is undefined
     * if the ctrl rxclear is de-asserted (i.e. busy)
     */
    if (ctlClear) {
        busy = (extBusy * 100) / ctlClear;
    } else {
        busy = 0;
    }
    if (busy > ATH_CWM_EXTCH_BUSY_THRESHOLD) {
        return TRUE;
    }

    return FALSE;
}
