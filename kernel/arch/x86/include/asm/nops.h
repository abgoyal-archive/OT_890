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

#ifndef _ASM_X86_NOPS_H
#define _ASM_X86_NOPS_H

/* Define nops for use with alternative() */

/* generic versions from gas
   1: nop
   the following instructions are NOT nops in 64-bit mode,
   for 64-bit mode use K8 or P6 nops instead
   2: movl %esi,%esi
   3: leal 0x00(%esi),%esi
   4: leal 0x00(,%esi,1),%esi
   6: leal 0x00000000(%esi),%esi
   7: leal 0x00000000(,%esi,1),%esi
*/
#define GENERIC_NOP1 ".byte 0x90\n"
#define GENERIC_NOP2 ".byte 0x89,0xf6\n"
#define GENERIC_NOP3 ".byte 0x8d,0x76,0x00\n"
#define GENERIC_NOP4 ".byte 0x8d,0x74,0x26,0x00\n"
#define GENERIC_NOP5 GENERIC_NOP1 GENERIC_NOP4
#define GENERIC_NOP6 ".byte 0x8d,0xb6,0x00,0x00,0x00,0x00\n"
#define GENERIC_NOP7 ".byte 0x8d,0xb4,0x26,0x00,0x00,0x00,0x00\n"
#define GENERIC_NOP8 GENERIC_NOP1 GENERIC_NOP7

/* Opteron 64bit nops
   1: nop
   2: osp nop
   3: osp osp nop
   4: osp osp osp nop
*/
#define K8_NOP1 GENERIC_NOP1
#define K8_NOP2	".byte 0x66,0x90\n"
#define K8_NOP3	".byte 0x66,0x66,0x90\n"
#define K8_NOP4	".byte 0x66,0x66,0x66,0x90\n"
#define K8_NOP5	K8_NOP3 K8_NOP2
#define K8_NOP6	K8_NOP3 K8_NOP3
#define K8_NOP7	K8_NOP4 K8_NOP3
#define K8_NOP8	K8_NOP4 K8_NOP4

/* K7 nops
   uses eax dependencies (arbitary choice)
   1: nop
   2: movl %eax,%eax
   3: leal (,%eax,1),%eax
   4: leal 0x00(,%eax,1),%eax
   6: leal 0x00000000(%eax),%eax
   7: leal 0x00000000(,%eax,1),%eax
*/
#define K7_NOP1	GENERIC_NOP1
#define K7_NOP2	".byte 0x8b,0xc0\n"
#define K7_NOP3	".byte 0x8d,0x04,0x20\n"
#define K7_NOP4	".byte 0x8d,0x44,0x20,0x00\n"
#define K7_NOP5	K7_NOP4 ASM_NOP1
#define K7_NOP6	".byte 0x8d,0x80,0,0,0,0\n"
#define K7_NOP7	".byte 0x8D,0x04,0x05,0,0,0,0\n"
#define K7_NOP8	K7_NOP7 ASM_NOP1

/* P6 nops
   uses eax dependencies (Intel-recommended choice)
   1: nop
   2: osp nop
   3: nopl (%eax)
   4: nopl 0x00(%eax)
   5: nopl 0x00(%eax,%eax,1)
   6: osp nopl 0x00(%eax,%eax,1)
   7: nopl 0x00000000(%eax)
   8: nopl 0x00000000(%eax,%eax,1)
*/
#define P6_NOP1	GENERIC_NOP1
#define P6_NOP2	".byte 0x66,0x90\n"
#define P6_NOP3	".byte 0x0f,0x1f,0x00\n"
#define P6_NOP4	".byte 0x0f,0x1f,0x40,0\n"
#define P6_NOP5	".byte 0x0f,0x1f,0x44,0x00,0\n"
#define P6_NOP6	".byte 0x66,0x0f,0x1f,0x44,0x00,0\n"
#define P6_NOP7	".byte 0x0f,0x1f,0x80,0,0,0,0\n"
#define P6_NOP8	".byte 0x0f,0x1f,0x84,0x00,0,0,0,0\n"

#if defined(CONFIG_MK7)
#define ASM_NOP1 K7_NOP1
#define ASM_NOP2 K7_NOP2
#define ASM_NOP3 K7_NOP3
#define ASM_NOP4 K7_NOP4
#define ASM_NOP5 K7_NOP5
#define ASM_NOP6 K7_NOP6
#define ASM_NOP7 K7_NOP7
#define ASM_NOP8 K7_NOP8
#elif defined(CONFIG_X86_P6_NOP)
#define ASM_NOP1 P6_NOP1
#define ASM_NOP2 P6_NOP2
#define ASM_NOP3 P6_NOP3
#define ASM_NOP4 P6_NOP4
#define ASM_NOP5 P6_NOP5
#define ASM_NOP6 P6_NOP6
#define ASM_NOP7 P6_NOP7
#define ASM_NOP8 P6_NOP8
#elif defined(CONFIG_X86_64)
#define ASM_NOP1 K8_NOP1
#define ASM_NOP2 K8_NOP2
#define ASM_NOP3 K8_NOP3
#define ASM_NOP4 K8_NOP4
#define ASM_NOP5 K8_NOP5
#define ASM_NOP6 K8_NOP6
#define ASM_NOP7 K8_NOP7
#define ASM_NOP8 K8_NOP8
#else
#define ASM_NOP1 GENERIC_NOP1
#define ASM_NOP2 GENERIC_NOP2
#define ASM_NOP3 GENERIC_NOP3
#define ASM_NOP4 GENERIC_NOP4
#define ASM_NOP5 GENERIC_NOP5
#define ASM_NOP6 GENERIC_NOP6
#define ASM_NOP7 GENERIC_NOP7
#define ASM_NOP8 GENERIC_NOP8
#endif

#define ASM_NOP_MAX 8

#endif /* _ASM_X86_NOPS_H */
