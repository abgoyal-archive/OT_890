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
/*  Module Name : wrap_dbg.c                                            */
/*                                                                      */
/*  Abstract                                                            */
/*     This module contains wrapper functions for debug functions       */
/*                                                                      */
/*  NOTES                                                               */
/*     Platform dependent.                                              */
/*                                                                      */
/************************************************************************/

#include "oal_dt.h"
#include "usbdrv.h"

#include <linux/netlink.h>

#if WIRELESS_EXT > 12
#include <net/iw_handler.h>
#endif

void zfwDumpBuf(zdev_t* dev, zbuf_t* buf)
{
    u16_t i;

    for (i=0; i<buf->len; i++)
    {
        printk("%02x ", *(((u8_t*)buf->data)+i));
        if ((i&0xf)==0xf)
        {
            printk("\n");
        }
    }
    printk("\n");
}


void zfwDbgReadRegDone(zdev_t* dev, u32_t addr, u32_t val)
{
    printk("Read addr:%x = %x\n", addr, val);
}

void zfwDbgWriteRegDone(zdev_t* dev, u32_t addr, u32_t val)
{
    printk("Write addr:%x = %x\n", addr, val);
}

void zfwDbgReadTallyDone(zdev_t* dev)
{
    //printk("Read Tall Done\n");
}

void zfwDbgWriteEepromDone(zdev_t* dev, u32_t addr, u32_t val)
{
}

void zfwDbgQueryHwTxBusyDone(zdev_t* dev, u32_t val)
{
}

//For Evl ++
void zfwDbgReadFlashDone(zdev_t* dev, u32_t addr, u32_t* rspdata, u32_t datalen)
{
    printk("Read Flash addr:%x length:%x\n", addr, datalen);
}

void zfwDbgProgrameFlashDone(zdev_t* dev)
{
    printk("Program Flash Done\n");
}

void zfwDbgProgrameFlashChkDone(zdev_t* dev)
{
    printk("Program Flash Done\n");
}

void zfwDbgGetFlashChkSumDone(zdev_t* dev, u32_t* rspdata)
{
    printk("Get Flash ChkSum Done\n");
}

void zfwDbgDownloadFwInitDone(zdev_t* dev)
{
    printk("Download FW Init Done\n");
}
//For Evl --

/* Leave an empty line below to remove warning message on some compiler */
