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

#ifndef _ASM_PERF_H_
#define _ASM_PERF_H_

/* ioctls */
#define PA_PERF_ON	_IO('p', 1)
#define PA_PERF_OFF	_IOR('p', 2, unsigned int)
#define PA_PERF_VERSION	_IOR('p', 3, int)

#define PA_PERF_DEV	"perf"
#define PA_PERF_MINOR	146

/* Interface types */
#define UNKNOWN_INTF    255
#define ONYX_INTF         0
#define CUDA_INTF         1

/* Common Onyx and Cuda images */
#define CPI                 0
#define BUSUTIL             1
#define TLBMISS             2
#define TLBHANDMISS         3
#define PTKN                4
#define PNTKN               5
#define IMISS               6
#define DMISS               7
#define DMISS_ACCESS        8 
#define BIG_CPI 	    9
#define BIG_LS		   10  
#define BR_ABORT	   11
#define ISNT		   12 
#define QUADRANT           13
#define RW_PDFET           14
#define RW_WDFET           15
#define SHLIB_CPI          16

/* Cuda only Images */
#define FLOPS              17
#define CACHEMISS          18 
#define BRANCHES           19             
#define CRSTACK            20 
#define I_CACHE_SPEC       21 
#define MAX_CUDA_IMAGES    22 

/* Onyx only Images */
#define ADDR_INV_ABORT_ALU 17
#define BRAD_STALL	   18 
#define CNTL_IN_PIPEL	   19 
#define DSNT_XFH	   20 
#define FET_SIG1	   21 
#define FET_SIG2	   22 
#define G7_1		   23 
#define G7_2		   24 
#define G7_3 		   25
#define G7_4		   26
#define MPB_LABORT         27
#define PANIC              28
#define RARE_INST          29 
#define RW_DFET            30 
#define RW_IFET            31 
#define RW_SDFET           32 
#define SPEC_IFET          33 
#define ST_COND0           34 
#define ST_COND1           35 
#define ST_COND2           36
#define ST_COND3           37
#define ST_COND4           38
#define ST_UNPRED0         39 
#define ST_UNPRED1         40 
#define UNPRED             41 
#define GO_STORE           42
#define SHLIB_CALL         43
#define MAX_ONYX_IMAGES    44

#endif
