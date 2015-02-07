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

#ifndef __BFIN_MMAN_H__
#define __BFIN_MMAN_H__

#define PROT_READ	0x1	/* page can be read */
#define PROT_WRITE	0x2	/* page can be written */
#define PROT_EXEC	0x4	/* page can be executed */
#define PROT_SEM	0x8	/* page may be used for atomic ops */
#define PROT_NONE	0x0	/* page can not be accessed */
#define PROT_GROWSDOWN	0x01000000	/* mprotect flag: extend change to start of growsdown vma */
#define PROT_GROWSUP	0x02000000	/* mprotect flag: extend change to end of growsup vma */

#define MAP_SHARED	0x01	/* Share changes */
#define MAP_PRIVATE	0x02	/* Changes are private */
#define MAP_TYPE	0x0f	/* Mask for type of mapping */
#define MAP_FIXED	0x10	/* Interpret addr exactly */
#define MAP_ANONYMOUS	0x20	/* don't use a file */

#define MAP_GROWSDOWN	0x0100	/* stack-like segment */
#define MAP_DENYWRITE	0x0800	/* ETXTBSY */
#define MAP_EXECUTABLE	0x1000	/* mark it as an executable */
#define MAP_LOCKED	0x2000	/* pages are locked */
#define MAP_NORESERVE	0x4000	/* don't check for reservations */
#define MAP_POPULATE	0x8000	/* populate (prefault) pagetables */
#define MAP_NONBLOCK	0x10000	/* do not block on IO */

#define MS_ASYNC	1	/* sync memory asynchronously */
#define MS_INVALIDATE	2	/* invalidate the caches */
#define MS_SYNC		4	/* synchronous memory sync */

#define MCL_CURRENT	1	/* lock all current mappings */
#define MCL_FUTURE	2	/* lock all future mappings */

#define MADV_NORMAL	0x0	/* default page-in behavior */
#define MADV_RANDOM	0x1	/* page-in minimum required */
#define MADV_SEQUENTIAL	0x2	/* read-ahead aggressively */
#define MADV_WILLNEED	0x3	/* pre-fault pages */
#define MADV_DONTNEED	0x4	/* discard these pages */

/* compatibility flags */
#define MAP_ANON	MAP_ANONYMOUS
#define MAP_FILE	0

#endif				/* __BFIN_MMAN_H__ */
