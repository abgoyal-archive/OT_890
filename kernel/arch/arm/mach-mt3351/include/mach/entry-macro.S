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
 * include/asm-arm/arch-mt5351/entry-macro.S
 *
 * Low-level IRQ helper macros for MT5351 platforms
 *
 */


#include "mt3351.h"

/* leave disable_fiq as an empty macro */
.macro disable_fiq
.endm

.macro  get_irqnr_preamble, base, tmp
.endm

.macro	arch_ret_to_user, tmp1, tmp2
.endm

.macro get_irqnr_and_base, irqnr, irqstat, base, tmp
    @ set bim base register
    ldr \base, =MT3351_INTH_VIRT 
    ldr \irqnr, [\base, #MT3351_REG_IRQ_STA2]
  
 /* 
  * 2008 09 19
  * Irene Liao
  * give an offset to irqnr by 63 if it is the external interrupt
  */

    mov \irqstat, \irqnr
    teq \irqstat, #MT3351_EIT_IRQ_LINE  @EINT
    bne 8f
    ldr \irqstat, [\base, #MT3351_REG_EINT_STA]
    teq \irqstat, #0
    bne 4f
    mov \irqstat, \irqnr  @invalid External IRQ .
    b 8f

/*
 * irqstat = the content of the register EINT_STA of interrupt contorller
 * and rega, reg1 , #0xffff is not ok...should be #0xff or 0xff00
 */ 
4:  mov \tmp, \irqstat
    and \tmp,\tmp, #0xFF00
    and \irqstat, \irqstat, #0xFF
    orr \irqstat, \irqstat, \tmp
    
    mov \irqnr, #0x3f
3:  mov \irqstat, \irqstat, LSR #1
    add \irqnr, \irqnr, #1
    teq \irqstat,#0
    bne 3b
    
    mov \irqstat, #MT3351_EIT_IRQ_LINE @ EINT 

    @ test irqstat
8: 
    bic \irqstat, \irqstat, #0xFF  
    teq \irqstat, #0x100
    .endm


