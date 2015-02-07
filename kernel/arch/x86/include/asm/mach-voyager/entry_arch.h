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

/* -*- mode: c; c-basic-offset: 8 -*- */

/* Copyright (C) 2002
 *
 * Author: James.Bottomley@HansenPartnership.com
 *
 * linux/arch/i386/voyager/entry_arch.h
 *
 * This file builds the VIC and QIC CPI gates
 */

/* initialise the voyager interrupt gates 
 *
 * This uses the macros in irq.h to set up assembly jump gates.  The
 * calls are then redirected to the same routine with smp_ prefixed */
BUILD_INTERRUPT(vic_sys_interrupt, VIC_SYS_INT)
BUILD_INTERRUPT(vic_cmn_interrupt, VIC_CMN_INT)
BUILD_INTERRUPT(vic_cpi_interrupt, VIC_CPI_LEVEL0);

/* do all the QIC interrupts */
BUILD_INTERRUPT(qic_timer_interrupt, QIC_TIMER_CPI);
BUILD_INTERRUPT(qic_invalidate_interrupt, QIC_INVALIDATE_CPI);
BUILD_INTERRUPT(qic_reschedule_interrupt, QIC_RESCHEDULE_CPI);
BUILD_INTERRUPT(qic_enable_irq_interrupt, QIC_ENABLE_IRQ_CPI);
BUILD_INTERRUPT(qic_call_function_interrupt, QIC_CALL_FUNCTION_CPI);
BUILD_INTERRUPT(qic_call_function_single_interrupt, QIC_CALL_FUNCTION_SINGLE_CPI);
