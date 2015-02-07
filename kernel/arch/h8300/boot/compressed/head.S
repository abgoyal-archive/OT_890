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
 *  linux/arch/h8300/boot/compressed/head.S
 *
 *  Copyright (C) 2006 Yoshinori Sato
 */

	.h8300h
#include <linux/linkage.h>

#define SRAM_START 0xff4000

	.section	.text.startup
	.global	startup
startup:
	mov.l	#SRAM_START+0x8000, sp
	mov.l	#__sbss, er0
	mov.l	#__ebss, er1
	sub.l	er0, er1
	shlr	er1
	shlr	er1
	sub.l	er2, er2
1:
	mov.l	er2, @er0
	adds	#4, er0
	dec.l	#1, er1
	bne	1b
	jsr	@_decompress_kernel
	jmp	@0x400000

	.align	9
fake_headers_as_bzImage:
	.word	0
	.ascii	"HdrS"		; header signature
	.word	0x0202		; header version number (>= 0x0105)
				; or else old loadlin-1.5 will fail)
	.word	0		; default_switch
	.word	0		; SETUPSEG
	.word	0x1000
	.word	0		; pointing to kernel version string
	.byte	0		; = 0, old one (LILO, Loadlin,
				; 0xTV: T=0 for LILO
				;       V = version
	.byte	1		; Load flags bzImage=1
	.word	0x8000		; size to move, when setup is not
	.long	0x100000	; 0x100000 = default for big kernel
	.long	0		; address of loaded ramdisk image
	.long	0		; its size in bytes
