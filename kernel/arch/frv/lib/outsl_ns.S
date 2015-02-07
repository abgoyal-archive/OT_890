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

/* outsl_ns.S: output array of 4b words to device without byte swapping
 *
 * Copyright (C) 2003 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */


        .text
        .p2align	4

###############################################################################
#
# void __outsl_ns(unsigned int port, const void *buf, int n)
#
###############################################################################
        .globl		__outsl_ns
        .type		__outsl_ns,@function
__outsl_ns:
	andicc.p	gr9,#3,gr0,icc0
	setlos		#4,gr4
	bne		icc0,#0,__outsl_ns_misaligned
	subi		gr9,#4,gr9
0:
	ldu.p		@(gr9,gr4),gr5
	subicc		gr10,#1,gr10,icc0
	sti.p		gr5,@(gr8,#0)
	bhi		icc0,#2,0b

	membar
	bralr

__outsl_ns_misaligned:
	subi.p		gr9,#1,gr9
	setlos		#1,gr4
0:
	ldubu		@(gr9,gr4),gr5
	ldubu.p		@(gr9,gr4),gr6
	slli		gr5,#8,gr5
	ldubu.p		@(gr9,gr4),gr7
	or		gr5,gr6,gr5
	ldubu.p		@(gr9,gr4),gr6
	slli		gr5,#16,gr5
	slli.p		gr7,#8,gr7
	or		gr5,gr6,gr5
	subicc.p	gr10,#1,gr10,icc0
	or		gr5,gr7,gr5

	sti.p		gr5,@(gr8,#0)
	bhi		icc0,#2,0b

	membar
	bralr

	.size		__outsl_ns, .-__outsl_ns
