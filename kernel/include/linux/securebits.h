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

#ifndef _LINUX_SECUREBITS_H
#define _LINUX_SECUREBITS_H 1

#define SECUREBITS_DEFAULT 0x00000000

/* When set UID 0 has no special privileges. When unset, we support
   inheritance of root-permissions and suid-root executable under
   compatibility mode. We raise the effective and inheritable bitmasks
   *of the executable file* if the effective uid of the new process is
   0. If the real uid is 0, we raise the effective (legacy) bit of the
   executable file. */
#define SECURE_NOROOT			0
#define SECURE_NOROOT_LOCKED		1  /* make bit-0 immutable */

/* When set, setuid to/from uid 0 does not trigger capability-"fixup".
   When unset, to provide compatiblility with old programs relying on
   set*uid to gain/lose privilege, transitions to/from uid 0 cause
   capabilities to be gained/lost. */
#define SECURE_NO_SETUID_FIXUP		2
#define SECURE_NO_SETUID_FIXUP_LOCKED	3  /* make bit-2 immutable */

/* When set, a process can retain its capabilities even after
   transitioning to a non-root user (the set-uid fixup suppressed by
   bit 2). Bit-4 is cleared when a process calls exec(); setting both
   bit 4 and 5 will create a barrier through exec that no exec()'d
   child can use this feature again. */
#define SECURE_KEEP_CAPS		4
#define SECURE_KEEP_CAPS_LOCKED		5  /* make bit-4 immutable */

/* Each securesetting is implemented using two bits. One bit specifies
   whether the setting is on or off. The other bit specify whether the
   setting is locked or not. A setting which is locked cannot be
   changed from user-level. */
#define issecure_mask(X)	(1 << (X))
#define issecure(X)		(issecure_mask(X) & current_cred_xxx(securebits))

#define SECURE_ALL_BITS		(issecure_mask(SECURE_NOROOT) | \
				 issecure_mask(SECURE_NO_SETUID_FIXUP) | \
				 issecure_mask(SECURE_KEEP_CAPS))
#define SECURE_ALL_LOCKS	(SECURE_ALL_BITS << 1)

#endif /* !_LINUX_SECUREBITS_H */
