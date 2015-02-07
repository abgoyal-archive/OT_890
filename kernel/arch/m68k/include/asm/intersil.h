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

#ifndef _SUN3_INTERSIL_H
#define _SUN3_INTERSIL_H
/* bits 0 and 1 */
#define INTERSIL_FREQ_32K        0x00
#define INTERSIL_FREQ_1M         0x01
#define INTERSIL_FREQ_2M         0x02
#define INTERSIL_FREQ_4M         0x03

/* bit 2 */
#define INTERSIL_12H_MODE   0x00
#define INTERSIL_24H_MODE   0x04

/* bit 3 */
#define INTERSIL_STOP            0x00
#define INTERSIL_RUN             0x08

/* bit 4 */
#define INTERSIL_INT_ENABLE     0x10
#define INTERSIL_INT_DISABLE    0x00

/* bit 5 */
#define INTERSIL_MODE_NORMAL     0x00
#define INTERSIL_MODE_TEST       0x20

#define INTERSIL_HZ_100_MASK	 0x02

struct intersil_dt {
	unsigned char	csec;
	unsigned char	hour;
	unsigned char	minute;
	unsigned char	second;
	unsigned char	month;
	unsigned char	day;
	unsigned char	year;
	unsigned char	weekday;
};

struct intersil_7170 {
	struct intersil_dt counter;
	struct intersil_dt alarm;
	unsigned char	int_reg;
	unsigned char	cmd_reg;
};

extern volatile char* clock_va;
#define intersil_clock ((volatile struct intersil_7170 *) clock_va)
#define intersil_clear() (void)intersil_clock->int_reg
#endif
