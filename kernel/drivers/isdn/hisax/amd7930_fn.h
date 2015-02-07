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

/* drivers/isdn/hisax/amd7930_fn.h
 *
 * gerdes_amd7930.h     Header-file included by
 *                      gerdes_amd7930.c
 *
 * Author               Christoph Ersfeld <info@formula-n.de>
 *                      Formula-n Europe AG (www.formula-n.com)
 *                      previously Gerdes AG
 *
 *
 *                      This file is (c) under GNU PUBLIC LICENSE
 */




#define BYTE							unsigned char
#define WORD							unsigned int
#define rByteAMD(cs, reg)					cs->readisac(cs, reg)
#define wByteAMD(cs, reg, val)					cs->writeisac(cs, reg, val)
#define rWordAMD(cs, reg)					ReadWordAmd7930(cs, reg)
#define wWordAMD(cs, reg, val)					WriteWordAmd7930(cs, reg, val)
#define HIBYTE(w)						((unsigned char)((w & 0xff00) / 256))
#define LOBYTE(w)						((unsigned char)(w & 0x00ff))

#define AmdIrqOff(cs)						cs->dc.amd7930.setIrqMask(cs, 0)
#define AmdIrqOn(cs)						cs->dc.amd7930.setIrqMask(cs, 1)

#define AMD_CR		0x00
#define AMD_DR		0x01


#define DBUSY_TIMER_VALUE 80

extern void Amd7930_interrupt(struct IsdnCardState *, unsigned char);
extern void Amd7930_init(struct IsdnCardState *);
extern void setup_Amd7930(struct IsdnCardState *);
