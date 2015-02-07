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

/**************************************************************************
 *
 * Copyright (c)2000-2002 Alacritech, Inc.  All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY ALACRITECH, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL ALACRITECH, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation
 * are those of the authors and should not be interpreted as representing
 * official policies, either expressed or implied, of Alacritech, Inc.
 *
 **************************************************************************/

/*
 * FILENAME: slic_os.h
 *
 * These are the Linux-specific definitions required for the SLICOSS
 * driver, which should allow for greater portability to other OSes.
 */
#ifndef _SLIC_OS_SPECIFIC_H_
#define _SLIC_OS_SPECIFIC_H_

#define FALSE               (0)
#define TRUE                (1)

#define SLIC_SECS_TO_JIFFS(x)  ((x) * HZ)
#define SLIC_MS_TO_JIFFIES(x)  (SLIC_SECS_TO_JIFFS((x)) / 1000)

#ifdef DEBUG_REGISTER_TRACE
#define WRITE_REG(reg, value, flush)                                      \
	{                                                           \
		adapter->card->reg_type[adapter->card->debug_ix] = 0;   \
		adapter->card->reg_offset[adapter->card->debug_ix] = \
		((unsigned char *)(&reg)) - \
		((unsigned char *)adapter->slic_regs); \
		adapter->card->reg_value[adapter->card->debug_ix++] = value;  \
		if (adapter->card->debug_ix == 32) \
			adapter->card->debug_ix = 0;                      \
		slic_reg32_write((&reg), (value), (flush));            \
	}
#define WRITE_REG64(a, reg, value, regh, valh, flush)                        \
	{                                                           \
		adapter->card->reg_type[adapter->card->debug_ix] = 1;        \
		adapter->card->reg_offset[adapter->card->debug_ix] = \
		((unsigned char *)(&reg)) - \
		((unsigned char *)adapter->slic_regs); \
		adapter->card->reg_value[adapter->card->debug_ix] = value;   \
		adapter->card->reg_valueh[adapter->card->debug_ix++] = valh;  \
		if (adapter->card->debug_ix == 32) \
			adapter->card->debug_ix = 0;                      \
		slic_reg64_write((a), (&reg), (value), (&regh), (valh), \
				(flush));\
	}
#else
#define WRITE_REG(reg, value, flush) \
	slic_reg32_write((&reg), (value), (flush))
#define WRITE_REG64(a, reg, value, regh, valh, flush) \
	slic_reg64_write((a), (&reg), (value), (&regh), (valh), (flush))
#endif

#endif  /* _SLIC_OS_SPECIFIC_H_  */

