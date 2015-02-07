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
/*  Module Name : wrap_buf.c                                            */
/*                                                                      */
/*  Abstract                                                            */
/*     This module contains wrapper functions for buffer management     */
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



/* Called to allocate buffer, must return a continue buffer space */
zbuf_t* zfwBufAllocate(zdev_t* dev, u16_t len)
{
    zbuf_t* buf;

    /* Allocate SKB for packet*/
    buf = dev_alloc_skb(len);

    return buf;
}


/* Called to free buffer, replace below 3 functions */
void zfwBufFree(zdev_t* dev, zbuf_t* buf, u16_t status)
{
    dev_kfree_skb_any(buf);
}

/* Called to adjust buffer size and head pointer */
u16_t zfwBufRemoveHead(zdev_t* dev, zbuf_t* buf, u16_t size)
{
    //zm_assert(buf->len > size);

    buf->data += size;
    buf->len -= size;
    return 0;
}




/* return tail if head==NULL, called to chain multiple buffer together */
/* Used to chain Rx buffer to form a frame. if the prepared Rx buffer  */
/* is greater than an ethernet frame(1518+32 byte), then this function    */
/* will only be called with head=NULL.                                 */
u16_t zfwBufChain(zdev_t* dev, zbuf_t** head, zbuf_t* tail)
{

    *head = tail;
    return 0;
}


/* Called when doing infra-bss forwarding */
u16_t zfwBufCopy(zdev_t* dev, zbuf_t* dst, zbuf_t* src)
{
    memcpy(dst->data, src->data, src->len);
    dst->tail = dst->data;
    skb_put(dst, src->len);
    return 0;
}


/* Called to adjust buffer size and tail pointer */
u16_t zfwBufSetSize(zdev_t* dev, zbuf_t* buf, u16_t size)
{
#ifdef NET_SKBUFF_DATA_USES_OFFSET
    buf->tail = 0;
    buf->len = 0;
#else
    buf->tail = buf->data;
    buf->len = 0;
#endif

    skb_put(buf, size);
    return 0;
}

u16_t zfwBufGetSize(zdev_t* dev, zbuf_t* buf)
{
    return buf->len;
}

void zfwCopyBufContext(zdev_t* dev, zbuf_t* source, zbuf_t* dst)
{
}
