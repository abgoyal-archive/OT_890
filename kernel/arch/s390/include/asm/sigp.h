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
 *  include/asm-s390/sigp.h
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Denis Joseph Barrow (djbarrow@de.ibm.com,barrow_dj@yahoo.com),
 *               Martin Schwidefsky (schwidefsky@de.ibm.com)
 *               Heiko Carstens (heiko.carstens@de.ibm.com)
 *
 *  sigp.h by D.J. Barrow (c) IBM 1999
 *  contains routines / structures for signalling other S/390 processors in an
 *  SMP configuration.
 */

#ifndef __SIGP__
#define __SIGP__

#include <asm/ptrace.h>
#include <asm/atomic.h>

/* get real cpu address from logical cpu number */
extern volatile int __cpu_logical_map[];

typedef enum
{
	sigp_unassigned=0x0,
	sigp_sense,
	sigp_external_call,
	sigp_emergency_signal,
	sigp_start,
	sigp_stop,
	sigp_restart,
	sigp_unassigned1,
	sigp_unassigned2,
	sigp_stop_and_store_status,
	sigp_unassigned3,
	sigp_initial_cpu_reset,
	sigp_cpu_reset,
	sigp_set_prefix,
	sigp_store_status_at_address,
	sigp_store_extended_status_at_address
} sigp_order_code;

typedef __u32 sigp_status_word;

typedef enum
{
        sigp_order_code_accepted=0,
	sigp_status_stored,
	sigp_busy,
	sigp_not_operational
} sigp_ccode;


/*
 * Definitions for the external call
 */

/* 'Bit' signals, asynchronous */
typedef enum
{
	ec_schedule=0,
	ec_call_function,
	ec_call_function_single,
	ec_bit_last
} ec_bit_sig;

/*
 * Signal processor
 */
static inline sigp_ccode
signal_processor(__u16 cpu_addr, sigp_order_code order_code)
{
	register unsigned long reg1 asm ("1") = 0;
	sigp_ccode ccode;

	asm volatile(
		"	sigp	%1,%2,0(%3)\n"
		"	ipm	%0\n"
		"	srl	%0,28\n"
		:	"=d"	(ccode)
		: "d" (reg1), "d" (__cpu_logical_map[cpu_addr]),
		  "a" (order_code) : "cc" , "memory");
	return ccode;
}

/*
 * Signal processor with parameter
 */
static inline sigp_ccode
signal_processor_p(__u32 parameter, __u16 cpu_addr, sigp_order_code order_code)
{
	register unsigned int reg1 asm ("1") = parameter;
	sigp_ccode ccode;

	asm volatile(
		"	sigp	%1,%2,0(%3)\n"
		"	ipm	%0\n"
		"	srl	%0,28\n"
		: "=d" (ccode)
		: "d" (reg1), "d" (__cpu_logical_map[cpu_addr]),
		  "a" (order_code) : "cc" , "memory");
	return ccode;
}

/*
 * Signal processor with parameter and return status
 */
static inline sigp_ccode
signal_processor_ps(__u32 *statusptr, __u32 parameter, __u16 cpu_addr,
		    sigp_order_code order_code)
{
	register unsigned int reg1 asm ("1") = parameter;
	sigp_ccode ccode;

	asm volatile(
		"	sigp	%1,%2,0(%3)\n"
		"	ipm	%0\n"
		"	srl	%0,28\n"
		: "=d" (ccode), "+d" (reg1)
		: "d" (__cpu_logical_map[cpu_addr]), "a" (order_code)
		: "cc" , "memory");
	*statusptr = reg1;
	return ccode;
}

#endif /* __SIGP__ */
