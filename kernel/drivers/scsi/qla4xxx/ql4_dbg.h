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
 * QLogic iSCSI HBA Driver
 * Copyright (c)  2003-2006 QLogic Corporation
 *
 * See LICENSE.qla4xxx for copyright and licensing details.
 */

/*
 * Driver debug definitions.
 */
/* #define QL_DEBUG  */			/* DEBUG messages */
/* #define QL_DEBUG_LEVEL_3  */		/* Output function tracing */
/* #define QL_DEBUG_LEVEL_4  */
/* #define QL_DEBUG_LEVEL_5  */
/* #define QL_DEBUG_LEVEL_9  */

#define QL_DEBUG_LEVEL_2	/* ALways enable error messagess */
#if defined(QL_DEBUG)
#define DEBUG(x)   do {x;} while (0);
#else
#define DEBUG(x)	do {} while (0);
#endif

#if defined(QL_DEBUG_LEVEL_2)
#define DEBUG2(x)      do {if(ql4xextended_error_logging == 2) x;} while (0);
#define DEBUG2_3(x)   do {x;} while (0);
#else				/*  */
#define DEBUG2(x)	do {} while (0);
#endif				/*  */

#if defined(QL_DEBUG_LEVEL_3)
#define DEBUG3(x)      do {if(ql4xextended_error_logging == 3) x;} while (0);
#else				/*  */
#define DEBUG3(x)	do {} while (0);
#if !defined(QL_DEBUG_LEVEL_2)
#define DEBUG2_3(x)	do {} while (0);
#endif				/*  */
#endif				/*  */
#if defined(QL_DEBUG_LEVEL_4)
#define DEBUG4(x)	do {x;} while (0);
#else				/*  */
#define DEBUG4(x)	do {} while (0);
#endif				/*  */

#if defined(QL_DEBUG_LEVEL_5)
#define DEBUG5(x)	do {x;} while (0);
#else				/*  */
#define DEBUG5(x)	do {} while (0);
#endif				/*  */

#if defined(QL_DEBUG_LEVEL_9)
#define DEBUG9(x)	do {x;} while (0);
#else				/*  */
#define DEBUG9(x)	do {} while (0);
#endif				/*  */
