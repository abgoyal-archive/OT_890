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

#ifndef _SPARC64_PIL_H
#define _SPARC64_PIL_H

/* To avoid some locking problems, we hard allocate certain PILs
 * for SMP cross call messages that must do a etrap/rtrap.
 *
 * A local_irq_disable() does not block the cross call delivery, so
 * when SMP locking is an issue we reschedule the event into a PIL
 * interrupt which is blocked by local_irq_disable().
 *
 * In fact any XCALL which has to etrap/rtrap has a problem because
 * it is difficult to prevent rtrap from running BH's, and that would
 * need to be done if the XCALL arrived while %pil==PIL_NORMAL_MAX.
 *
 * Finally, in order to handle profiling events even when a
 * local_irq_disable() is in progress, we only disable up to level 14
 * interrupts.  Profile counter overflow interrupts arrive at level
 * 15.
 */
#define PIL_SMP_CALL_FUNC	1
#define PIL_SMP_RECEIVE_SIGNAL	2
#define PIL_SMP_CAPTURE		3
#define PIL_SMP_CTX_NEW_VERSION	4
#define PIL_DEVICE_IRQ		5
#define PIL_SMP_CALL_FUNC_SNGL	6
#define PIL_DEFERRED_PCR_WORK	7
#define PIL_KGDB_CAPTURE	8
#define PIL_NORMAL_MAX		14
#define PIL_NMI			15

#endif /* !(_SPARC64_PIL_H) */
