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

#ifndef _ASM_X86_ASM_H
#define _ASM_X86_ASM_H

#ifdef __ASSEMBLY__
# define __ASM_FORM(x)	x
# define __ASM_EX_SEC	.section __ex_table
#else
# define __ASM_FORM(x)	" " #x " "
# define __ASM_EX_SEC	" .section __ex_table,\"a\"\n"
#endif

#ifdef CONFIG_X86_32
# define __ASM_SEL(a,b) __ASM_FORM(a)
#else
# define __ASM_SEL(a,b) __ASM_FORM(b)
#endif

#define __ASM_SIZE(inst)	__ASM_SEL(inst##l, inst##q)
#define __ASM_REG(reg)		__ASM_SEL(e##reg, r##reg)

#define _ASM_PTR	__ASM_SEL(.long, .quad)
#define _ASM_ALIGN	__ASM_SEL(.balign 4, .balign 8)

#define _ASM_MOV	__ASM_SIZE(mov)
#define _ASM_INC	__ASM_SIZE(inc)
#define _ASM_DEC	__ASM_SIZE(dec)
#define _ASM_ADD	__ASM_SIZE(add)
#define _ASM_SUB	__ASM_SIZE(sub)
#define _ASM_XADD	__ASM_SIZE(xadd)

#define _ASM_AX		__ASM_REG(ax)
#define _ASM_BX		__ASM_REG(bx)
#define _ASM_CX		__ASM_REG(cx)
#define _ASM_DX		__ASM_REG(dx)
#define _ASM_SP		__ASM_REG(sp)
#define _ASM_BP		__ASM_REG(bp)
#define _ASM_SI		__ASM_REG(si)
#define _ASM_DI		__ASM_REG(di)

/* Exception table entry */
# define _ASM_EXTABLE(from,to) \
	__ASM_EX_SEC	\
	_ASM_ALIGN "\n" \
	_ASM_PTR #from "," #to "\n" \
	" .previous\n"

#endif /* _ASM_X86_ASM_H */
