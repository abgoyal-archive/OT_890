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

#ifndef  __MOVS_H__
#define __MOVS_H__

/*
** movs.h
**
** Inline assembly macros to generate movs & related instructions
*/

/* Set DFC register value */

#define SET_DFC(x) \
        __asm__ __volatile__ (" movec %0,%/dfc" : : "d" (x));

/* Get DFC register value */

#define GET_DFC(x) \
        __asm__ __volatile__ (" movec %/dfc, %0" : "=d" (x) : );

/* Set SFC register value */

#define SET_SFC(x) \
        __asm__ __volatile__ (" movec %0,%/sfc" : : "d" (x));

/* Get SFC register value */

#define GET_SFC(x) \
        __asm__ __volatile__ (" movec %/sfc, %0" : "=d" (x) : );

#define SET_VBR(x) \
        __asm__ __volatile__ (" movec %0,%/vbr" : : "r" (x));

#define GET_VBR(x) \
        __asm__ __volatile__ (" movec %/vbr, %0" : "=g" (x) : );

/* Set a byte using the "movs" instruction */

#define SET_CONTROL_BYTE(addr,value) \
        __asm__ __volatile__ (" movsb %0, %1@" : : "d" (value), "a" (addr));

/* Get a byte using the "movs" instruction */

#define GET_CONTROL_BYTE(addr,value) \
        __asm__ __volatile__ (" movsb %1@, %0" : "=d" (value) : "a" (addr));

/* Set a (long)word using the "movs" instruction */

#define SET_CONTROL_WORD(addr,value) \
        __asm__ __volatile__ (" movsl %0, %1@" : : "d" (value), "a" (addr));

/* Get a (long)word using the "movs" instruction */

#define GET_CONTROL_WORD(addr,value) \
        __asm__ __volatile__ (" movsl %1@, %0" : "=d" (value) : "a" (addr));
#endif
