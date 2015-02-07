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

/***********************************
 * $Id$
 ***********************************
 *
 ***************************************
 * Simple drivers common header
 ***************************************
 */

#ifndef __SIMPLE_H
#define __SIMPLE_H

/* #include "quicc.h" */

#define GLB_SCC_0   0
#define GLB_SCC_1   1
#define GLB_SCC_2   2
#define GLB_SCC_3   3

typedef void (int_routine)(unsigned short interrupt_event);
typedef int_routine *int_routine_ptr; 
typedef void *(alloc_routine)(int length);
typedef void (free_routine)(int scc_num, int channel_num, void *buf);
typedef void (store_rx_buffer_routine)(int scc_num, int channel_num, void *buff, int length);
typedef int  (handle_tx_error_routine)(int scc_num, int channel_num, QUICC_BD *tbd);
typedef void (handle_rx_error_routine)(int scc_num, int channel_num, QUICC_BD *rbd);
typedef void (handle_lost_error_routine)(int scc_num, int channel_num);

/* user defined functions for global errors */
typedef void (handle_glob_overrun_routine)(int scc_number);
typedef void (handle_glob_underrun_routine)(int scc_number);
typedef void (glob_intr_q_overflow_routine)(int scc_number);

/*
 * General initialization and command routines
 */
void quicc_issue_cmd (unsigned short cmd, int scc_num);
void quicc_init(void);
void quicc_scc_init(int scc_number, int number_of_rx_buf, int number_of_tx_buf);
void quicc_smc_init(int smc_number, int number_of_rx_buf, int number_of_tx_buf);
void quicc_scc_start(int scc_num);
void quicc_scc_loopback(int scc_num);

/* Interrupt enable/disable routines for critical pieces of code*/
unsigned short  IntrDis(void);
void            IntrEna(unsigned short old_sr);

/* For debugging */
void print_rbd(int scc_num);
void print_tbd(int scc_num);

#endif
