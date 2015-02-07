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

#ifndef EXCITE_FPGA_H_INCLUDED
#define EXCITE_FPGA_H_INCLUDED


/**
 * Address alignment of the individual FPGA bytes.
 * The address arrangement of the individual bytes of the FPGA is two
 * byte aligned at the embedded MK2 platform.
 */
#ifdef EXCITE_CCI_FPGA_MK2
typedef unsigned char excite_cci_fpga_align_t __attribute__ ((aligned(2)));
#else
typedef unsigned char excite_cci_fpga_align_t;
#endif


/**
 * Size of Dual Ported RAM.
 */
#define EXCITE_DPR_SIZE 263


/**
 * Size of Reserved Status Fields in Dual Ported RAM.
 */
#define EXCITE_DPR_STATUS_SIZE 7



/**
 * FPGA.
 * Hardware register layout of the FPGA interface. The FPGA must accessed
 * byte wise solely.
 * @see EXCITE_CCI_DPR_MK2
 */
typedef struct excite_fpga {

	/**
	 * Dual Ported RAM.
	 */
	excite_cci_fpga_align_t dpr[EXCITE_DPR_SIZE];

	/**
	 * Status.
	 */
	excite_cci_fpga_align_t status[EXCITE_DPR_STATUS_SIZE];

#ifdef EXCITE_CCI_FPGA_MK2
	/**
	 * RM9000 Interrupt.
	 * Write access initiates interrupt at the RM9000 (MIPS) processor of the eXcite.
	 */
	excite_cci_fpga_align_t rm9k_int;
#else
	/**
	 * MK2 Interrupt.
	 * Write access initiates interrupt at the ARM processor of the MK2.
	 */
	excite_cci_fpga_align_t mk2_int;

	excite_cci_fpga_align_t gap[0x1000-0x10f];

	/**
	 * IRQ Source/Acknowledge.
	 */
	excite_cci_fpga_align_t rm9k_irq_src;

	/**
	 * IRQ Mask.
	 * Set bits enable the related interrupt.
	 */
	excite_cci_fpga_align_t rm9k_irq_mask;
#endif


} excite_fpga;



#endif	/* ndef EXCITE_FPGA_H_INCLUDED */
