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
 * __put_user functions.
 *
 * (C) Copyright 2005 Linus Torvalds
 * (C) Copyright 2005 Andi Kleen
 * (C) Copyright 2008 Glauber Costa
 *
 * These functions have a non-standard call interface
 * to make them more efficient, especially as they
 * return an error value in addition to the "real"
 * return value.
 */
#include <linux/linkage.h>
#include <asm/dwarf2.h>
#include <asm/thread_info.h>
#include <asm/errno.h>
#include <asm/asm.h>


/*
 * __put_user_X
 *
 * Inputs:	%eax[:%edx] contains the data
 *		%ecx contains the address
 *
 * Outputs:	%eax is error code (0 or -EFAULT)
 *
 * These functions should not modify any other registers,
 * as they get called from within inline assembly.
 */

#define ENTER	CFI_STARTPROC ; \
		GET_THREAD_INFO(%_ASM_BX)
#define EXIT	ret ; \
		CFI_ENDPROC

.text
ENTRY(__put_user_1)
	ENTER
	cmp TI_addr_limit(%_ASM_BX),%_ASM_CX
	jae bad_put_user
1:	movb %al,(%_ASM_CX)
	xor %eax,%eax
	EXIT
ENDPROC(__put_user_1)

ENTRY(__put_user_2)
	ENTER
	mov TI_addr_limit(%_ASM_BX),%_ASM_BX
	sub $1,%_ASM_BX
	cmp %_ASM_BX,%_ASM_CX
	jae bad_put_user
2:	movw %ax,(%_ASM_CX)
	xor %eax,%eax
	EXIT
ENDPROC(__put_user_2)

ENTRY(__put_user_4)
	ENTER
	mov TI_addr_limit(%_ASM_BX),%_ASM_BX
	sub $3,%_ASM_BX
	cmp %_ASM_BX,%_ASM_CX
	jae bad_put_user
3:	movl %eax,(%_ASM_CX)
	xor %eax,%eax
	EXIT
ENDPROC(__put_user_4)

ENTRY(__put_user_8)
	ENTER
	mov TI_addr_limit(%_ASM_BX),%_ASM_BX
	sub $7,%_ASM_BX
	cmp %_ASM_BX,%_ASM_CX
	jae bad_put_user
4:	mov %_ASM_AX,(%_ASM_CX)
#ifdef CONFIG_X86_32
5:	movl %edx,4(%_ASM_CX)
#endif
	xor %eax,%eax
	EXIT
ENDPROC(__put_user_8)

bad_put_user:
	CFI_STARTPROC
	movl $-EFAULT,%eax
	EXIT
END(bad_put_user)

.section __ex_table,"a"
	_ASM_PTR 1b,bad_put_user
	_ASM_PTR 2b,bad_put_user
	_ASM_PTR 3b,bad_put_user
	_ASM_PTR 4b,bad_put_user
#ifdef CONFIG_X86_32
	_ASM_PTR 5b,bad_put_user
#endif
.previous
