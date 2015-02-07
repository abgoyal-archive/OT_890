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
 *
  Copyright (c) Eicon Networks, 2002.
 *
  This source file is supplied for the use with
  Eicon Networks range of DIVA Server Adapters.
 *
  Eicon File Revision :    2.1
 *
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.
 *
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY OF ANY KIND WHATSOEVER INCLUDING ANY
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU General Public License for more details.
 *
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#ifndef __DIVA_IDI_DFIFO_INC__
#define __DIVA_IDI_DFIFO_INC__
#define DIVA_DFIFO_CACHE_SZ   64 /* Used to isolate pipe from
                    rest of the world
                   should be divisible by 4
                   */
#define DIVA_DFIFO_RAW_SZ    (2512*8)
#define DIVA_DFIFO_DATA_SZ   68
#define DIVA_DFIFO_HDR_SZ    4
#define DIVA_DFIFO_SEGMENT_SZ  (DIVA_DFIFO_DATA_SZ+DIVA_DFIFO_HDR_SZ)
#define DIVA_DFIFO_SEGMENTS   ((DIVA_DFIFO_RAW_SZ)/(DIVA_DFIFO_SEGMENT_SZ)+1)
#define DIVA_DFIFO_MEM_SZ (\
        (DIVA_DFIFO_SEGMENT_SZ)*(DIVA_DFIFO_SEGMENTS)+\
        (DIVA_DFIFO_CACHE_SZ)*2\
             )
#define DIVA_DFIFO_STEP DIVA_DFIFO_SEGMENT_SZ
/* -------------------------------------------------------------------------
  Block header layout is:
   byte[0] -> flags
   byte[1] -> length of data in block
   byte[2] -> reserved
   byte[4] -> reserved
  ------------------------------------------------------------------------- */
#define DIVA_DFIFO_WRAP   0x80 /* This is the last block in fifo   */
#define DIVA_DFIFO_READY  0x40 /* This block is ready for processing */
#define DIVA_DFIFO_LAST   0x20 /* This block is last in message      */
#define DIVA_DFIFO_AUTO   0x10 /* Don't look for 'ready', don't ack */
int diva_dfifo_create (void* start, int length);
#endif
