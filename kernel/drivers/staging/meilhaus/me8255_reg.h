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

/**
 * @file me8255_reg.h
 *
 * @brief 8255 counter register definitions.
 * @note Copyright (C) 2006 Meilhaus Electronic GmbH (support@meilhaus.de)
 * @author Guenter Gebhardt
 */

#ifndef _ME8255_REG_H_
#define _ME8255_REG_H_

#ifdef __KERNEL__

#define ME8255_NUMBER_CHANNELS		8		/**< The number of channels per 8255 port. */

#define ME1400AB_PORT_A_0			0x0000	/**< Port 0 offset. */
#define ME1400AB_PORT_A_1			0x0001	/**< Port 1 offset. */
#define ME1400AB_PORT_A_2			0x0002	/**< Port 2 offset. */
#define ME1400AB_PORT_A_CTRL		0x0003	/**< Control register for 8255 A. */

#define ME1400AB_PORT_B_0			0x0008	/**< Port 0 offset. */
#define ME1400AB_PORT_B_1			0x0009	/**< Port 1 offset. */
#define ME1400AB_PORT_B_2			0x000A	/**< Port 2 offset. */
#define ME1400AB_PORT_B_CTRL		0x000B	/**< Control register for 8255 B. */

#define ME1400CD_PORT_A_0			0x0000	/**< Port 0 offset. */
#define ME1400CD_PORT_A_1			0x0001	/**< Port 1 offset. */
#define ME1400CD_PORT_A_2			0x0002	/**< Port 2 offset. */
#define ME1400CD_PORT_A_CTRL		0x0003	/**< Control register for 8255 A. */

#define ME1400CD_PORT_B_0			0x0040	/**< Port 0 offset. */
#define ME1400CD_PORT_B_1			0x0041	/**< Port 1 offset. */
#define ME1400CD_PORT_B_2			0x0042	/**< Port 2 offset. */
#define ME1400CD_PORT_B_CTRL		0x0043	/**< Control register for 8255 B. */

#define ME8255_MODE_OOO				0x80	/**< Port 2 = Output, Port 1 = Output, Port 0 = Output */
#define ME8255_MODE_IOO				0x89	/**< Port 2 = Input,  Port 1 = Output, Port 0 = Output */
#define ME8255_MODE_OIO				0x82	/**< Port 2 = Output, Port 1 = Input,  Port 0 = Output */
#define ME8255_MODE_IIO				0x8B	/**< Port 2 = Input,  Port 1 = Input,  Port 0 = Output */
#define ME8255_MODE_OOI				0x90	/**< Port 2 = Output, Port 1 = Output, Port 0 = Input */
#define ME8255_MODE_IOI				0x99	/**< Port 2 = Input,  Port 1 = Output, Port 0 = Input */
#define ME8255_MODE_OII				0x92	/**< Port 2 = Output, Port 1 = Input,  Port 0 = Input */
#define ME8255_MODE_III				0x9B	/**< Port 2 = Input,  Port 1 = Input,  Port 0 = Input */

#define ME8255_PORT_0_OUTPUT		0x1		/**< If set in mirror then port 0 is in output mode. */
#define ME8255_PORT_1_OUTPUT		0x2		/**< If set in mirror then port 1 is in output mode. */
#define ME8255_PORT_2_OUTPUT		0x4		/**< If set in mirror then port 2 is in output mode. */

#endif
#endif
