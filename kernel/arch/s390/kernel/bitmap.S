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
 *  arch/s390/kernel/bitmap.S
 *    Bitmaps for set_bit, clear_bit, test_and_set_bit, ...
 *    See include/asm-s390/{bitops.h|posix_types.h} for details
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com),
 */

         .globl _oi_bitmap
_oi_bitmap:
         .byte  0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80

         .globl _ni_bitmap
_ni_bitmap:
         .byte  0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F

         .globl _zb_findmap
_zb_findmap:
         .byte  0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4
         .byte  0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5
         .byte  0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4 
         .byte  0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,6
         .byte  0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4
         .byte  0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5
         .byte  0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4
         .byte  0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,7
         .byte  0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4
         .byte  0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5
         .byte  0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4
         .byte  0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,6
         .byte  0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4
         .byte  0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,5
         .byte  0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4
         .byte  0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,8

         .globl _sb_findmap
_sb_findmap:
         .byte  8,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
         .byte  4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
         .byte  5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
         .byte  4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
         .byte  6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
         .byte  4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
         .byte  5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
         .byte  4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
         .byte  7,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
         .byte  4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
         .byte  5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
         .byte  4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
         .byte  6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
         .byte  4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
         .byte  5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0
         .byte  4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0

