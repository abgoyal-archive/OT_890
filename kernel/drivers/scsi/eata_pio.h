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

/********************************************************
* Header file for eata_pio.c Linux EATA-PIO SCSI driver *
* (c) 1993-96 Michael Neuffer  	                        *
*********************************************************
* last change: 2002/11/02				*
********************************************************/


#ifndef _EATA_PIO_H
#define _EATA_PIO_H

#define VER_MAJOR 0
#define VER_MINOR 0
#define VER_SUB	  "1b"

/************************************************************************
 * Here you can switch parts of the code on and of			*
 ************************************************************************/

#define VERBOSE_SETUP		/* show startup screen of 2001 */
#define ALLOW_DMA_BOARDS 1

/************************************************************************
 * Debug options.							* 
 * Enable DEBUG and whichever options you require.			*
 ************************************************************************/
#define DEBUG_EATA	1	/* Enable debug code.                       */
#define DPT_DEBUG	0	/* Bobs special                             */
#define DBG_DELAY	0	/* Build in delays so debug messages can be
				 * be read before they vanish of the top of
				 * the screen!
				 */
#define DBG_PROBE	0	/* Debug probe routines.                    */
#define DBG_ISA		0	/* Trace ISA routines                       */
#define DBG_EISA	0	/* Trace EISA routines                      */
#define DBG_PCI		0	/* Trace PCI routines                       */
#define DBG_PIO		0	/* Trace get_config_PIO                     */
#define DBG_COM		0	/* Trace command call                       */
#define DBG_QUEUE	0	/* Trace command queueing.                  */
#define DBG_INTR	0	/* Trace interrupt service routine.         */
#define DBG_INTR2	0	/* Trace interrupt service routine.         */
#define DBG_PROC	0	/* Debug proc-fs related statistics         */
#define DBG_PROC_WRITE	0
#define DBG_REGISTER	0	/* */
#define DBG_ABNORM	1	/* Debug abnormal actions (reset, abort)    */

#if DEBUG_EATA
#define DBG(x, y)   if ((x)) {y;}
#else
#define DBG(x, y)
#endif

#endif				/* _EATA_PIO_H */
