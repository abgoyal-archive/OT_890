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

/*	BSDI osd_defs.h,v 1.4 1998/06/03 19:14:58 karels Exp	*/
/*
 * Copyright (c) 1996-1999 Distributed Processing Technology Corporation
 * All rights reserved.
 *
 * Redistribution and use in source form, with or without modification, are
 * permitted provided that redistributions of source code must retain the
 * above copyright notice, this list of conditions and the following disclaimer.
 *
 * This software is provided `as is' by Distributed Processing Technology and
 * any express or implied warranties, including, but not limited to, the
 * implied warranties of merchantability and fitness for a particular purpose,
 * are disclaimed. In no event shall Distributed Processing Technology be
 * liable for any direct, indirect, incidental, special, exemplary or
 * consequential damages (including, but not limited to, procurement of
 * substitute goods or services; loss of use, data, or profits; or business
 * interruptions) however caused and on any theory of liability, whether in
 * contract, strict liability, or tort (including negligence or otherwise)
 * arising in any way out of the use of this driver software, even if advised
 * of the possibility of such damage.
 *
 */

#ifndef		_OSD_DEFS_H
#define		_OSD_DEFS_H

/*File - OSD_DEFS.H
 ****************************************************************************
 *
 *Description:
 *
 *	This file contains the OS dependent defines.  This file is included
 *in osd_util.h and provides the OS specific defines for that file.
 *
 *Copyright Distributed Processing Technology, Corp.
 *	  140 Candace Dr.
 *	  Maitland, Fl.	32751   USA
 *	  Phone: (407) 830-5522  Fax: (407) 260-5366
 *	  All Rights Reserved
 *
 *Author:	Doug Anderson
 *Date:		1/31/94
 *
 *Editors:
 *
 *Remarks:
 *
 *
 *****************************************************************************/


/*Definitions - Defines & Constants ----------------------------------------- */

  /* Define the operating system */
#if (defined(__linux__))
# define _DPT_LINUX
#elif (defined(__bsdi__))
# define _DPT_BSDI
#elif (defined(__FreeBSD__))
# define _DPT_FREE_BSD
#else
# define _DPT_SCO
#endif

#if defined (ZIL_CURSES)
#define		_DPT_CURSES
#else
#define         _DPT_MOTIF
#endif

  /* Redefine 'far' to nothing - no far pointer type required in UNIX */
#define		far

  /* Define the mutually exclusive semaphore type */
#define		SEMAPHORE_T	unsigned int *
  /* Define a handle to a DLL */
#define		DLL_HANDLE_T	unsigned int *

#endif
