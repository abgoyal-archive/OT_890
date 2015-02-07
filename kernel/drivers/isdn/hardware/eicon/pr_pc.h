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
struct pr_ram {
  word NextReq;         /* pointer to next Req Buffer               */
  word NextRc;          /* pointer to next Rc Buffer                */
  word NextInd;         /* pointer to next Ind Buffer               */
  byte ReqInput;        /* number of Req Buffers sent               */
  byte ReqOutput;       /* number of Req Buffers returned           */
  byte ReqReserved;     /* number of Req Buffers reserved           */
  byte Int;             /* ISDN-P interrupt                         */
  byte XLock;           /* Lock field for arbitration               */
  byte RcOutput;        /* number of Rc buffers received            */
  byte IndOutput;       /* number of Ind buffers received           */
  byte IMask;           /* Interrupt Mask Flag                      */
  byte Reserved1[2];    /* reserved field, do not use               */
  byte ReadyInt;        /* request field for ready interrupt        */
  byte Reserved2[12];   /* reserved field, do not use               */
  byte InterfaceType;   /* interface type 1=16K interface           */
  word Signature;       /* ISDN-P initialized indication            */
  byte B[1];            /* buffer space for Req,Ind and Rc          */
};
typedef struct {
  word next;
  byte Req;
  byte ReqId;
  byte ReqCh;
  byte Reserved1;
  word Reference;
  byte Reserved[8];
  PBUFFER XBuffer;
} REQ;
typedef struct {
  word next;
  byte Rc;
  byte RcId;
  byte RcCh;
  byte Reserved1;
  word Reference;
  byte Reserved2[8];
} RC;
typedef struct {
  word next;
  byte Ind;
  byte IndId;
  byte IndCh;
  byte MInd;
  word MLength;
  word Reference;
  byte RNR;
  byte Reserved;
  dword Ack;
  PBUFFER RBuffer;
} IND;
