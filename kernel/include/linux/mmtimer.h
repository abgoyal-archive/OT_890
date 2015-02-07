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
 * Intel Multimedia Timer device interface
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 2001-2004 Silicon Graphics, Inc.  All rights reserved.
 *
 * This file should define an interface compatible with the IA-PC Multimedia
 * Timers Draft Specification (rev. 0.97) from Intel.  Note that some
 * hardware may not be able to safely export its registers to userspace,
 * so the ioctl interface should support all necessary functionality.
 *
 * 11/01/01 - jbarnes - initial revision
 * 9/10/04 - Christoph Lameter - remove interrupt support
 * 9/17/04 - jbarnes - remove test program, move some #defines to the driver
 */

#ifndef _LINUX_MMTIMER_H
#define _LINUX_MMTIMER_H

/*
 * Breakdown of the ioctl's available.  An 'optional' next to the command
 * indicates that supporting this command is optional, while 'required'
 * commands must be implemented if conformance is desired.
 *
 * MMTIMER_GETOFFSET - optional
 *   Should return the offset (relative to the start of the page where the
 *   registers are mapped) for the counter in question.
 *
 * MMTIMER_GETRES - required
 *   The resolution of the clock in femto (10^-15) seconds
 *
 * MMTIMER_GETFREQ - required
 *   Frequency of the clock in Hz
 *
 * MMTIMER_GETBITS - required
 *   Number of bits in the clock's counter
 *
 * MMTIMER_MMAPAVAIL - required
 *   Returns nonzero if the registers can be mmap'd into userspace, 0 otherwise
 *
 * MMTIMER_GETCOUNTER - required
 *   Gets the current value in the counter
 */
#define MMTIMER_IOCTL_BASE 'm'

#define MMTIMER_GETOFFSET _IO(MMTIMER_IOCTL_BASE, 0)
#define MMTIMER_GETRES _IOR(MMTIMER_IOCTL_BASE, 1, unsigned long)
#define MMTIMER_GETFREQ _IOR(MMTIMER_IOCTL_BASE, 2, unsigned long)
#define MMTIMER_GETBITS _IO(MMTIMER_IOCTL_BASE, 4)
#define MMTIMER_MMAPAVAIL _IO(MMTIMER_IOCTL_BASE, 6)
#define MMTIMER_GETCOUNTER _IOR(MMTIMER_IOCTL_BASE, 9, unsigned long)

#endif /* _LINUX_MMTIMER_H */
