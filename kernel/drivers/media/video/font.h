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

static unsigned char rom8x16_bits[] = {
/* Character 0 (0x30):
   ht=16, width=8
   +--------+
   |        |
   |        |
   | *****  |
   |**   ** |
   |**   ** |
   |**  *** |
   |** **** |
   |**** ** |
   |***  ** |
   |**   ** |
   |**   ** |
   | *****  |
   |        |
   |        |
   |        |
   |        |
   +--------+ */
0x00,
0x00,
0x7c,
0xc6,
0xc6,
0xce,
0xde,
0xf6,
0xe6,
0xc6,
0xc6,
0x7c,
0x00,
0x00,
0x00,
0x00,

/* Character 1 (0x31):
   ht=16, width=8
   +--------+
   |        |
   |        |
   |   **   |
   | ****   |
   |   **   |
   |   **   |
   |   **   |
   |   **   |
   |   **   |
   |   **   |
   |   **   |
   | ****** |
   |        |
   |        |
   |        |
   |        |
   +--------+ */
0x00,
0x00,
0x18,
0x78,
0x18,
0x18,
0x18,
0x18,
0x18,
0x18,
0x18,
0x7e,
0x00,
0x00,
0x00,
0x00,

/* Character 2 (0x32):
   ht=16, width=8
   +--------+
   |        |
   |        |
   | *****  |
   |**   ** |
   |**   ** |
   |     ** |
   |    **  |
   |   **   |
   |  **    |
   | **     |
   |**   ** |
   |******* |
   |        |
   |        |
   |        |
   |        |
   +--------+ */
0x00,
0x00,
0x7c,
0xc6,
0xc6,
0x06,
0x0c,
0x18,
0x30,
0x60,
0xc6,
0xfe,
0x00,
0x00,
0x00,
0x00,

/* Character 3 (0x33):
   ht=16, width=8
   +--------+
   |        |
   |        |
   | *****  |
   |**   ** |
   |     ** |
   |     ** |
   |  ****  |
   |     ** |
   |     ** |
   |     ** |
   |**   ** |
   | *****  |
   |        |
   |        |
   |        |
   |        |
   +--------+ */
0x00,
0x00,
0x7c,
0xc6,
0x06,
0x06,
0x3c,
0x06,
0x06,
0x06,
0xc6,
0x7c,
0x00,
0x00,
0x00,
0x00,

/* Character 4 (0x34):
   ht=16, width=8
   +--------+
   |        |
   |        |
   |    **  |
   |   ***  |
   |  ****  |
   | ** **  |
   |**  **  |
   |**  **  |
   |******* |
   |    **  |
   |    **  |
   |   **** |
   |        |
   |        |
   |        |
   |        |
   +--------+ */
0x00,
0x00,
0x0c,
0x1c,
0x3c,
0x6c,
0xcc,
0xcc,
0xfe,
0x0c,
0x0c,
0x1e,
0x00,
0x00,
0x00,
0x00,

/* Character 5 (0x35):
   ht=16, width=8
   +--------+
   |        |
   |        |
   |******* |
   |**      |
   |**      |
   |**      |
   |******  |
   |     ** |
   |     ** |
   |     ** |
   |**   ** |
   | *****  |
   |        |
   |        |
   |        |
   |        |
   +--------+ */
0x00,
0x00,
0xfe,
0xc0,
0xc0,
0xc0,
0xfc,
0x06,
0x06,
0x06,
0xc6,
0x7c,
0x00,
0x00,
0x00,
0x00,

/* Character 6 (0x36):
   ht=16, width=8
   +--------+
   |        |
   |        |
   | *****  |
   |**   ** |
   |**      |
   |**      |
   |******  |
   |**   ** |
   |**   ** |
   |**   ** |
   |**   ** |
   | *****  |
   |        |
   |        |
   |        |
   |        |
   +--------+ */
0x00,
0x00,
0x7c,
0xc6,
0xc0,
0xc0,
0xfc,
0xc6,
0xc6,
0xc6,
0xc6,
0x7c,
0x00,
0x00,
0x00,
0x00,

/* Character 7 (0x37):
   ht=16, width=8
   +--------+
   |        |
   |        |
   |******* |
   |**   ** |
   |     ** |
   |    **  |
   |   **   |
   |  **    |
   |  **    |
   |  **    |
   |  **    |
   |  **    |
   |        |
   |        |
   |        |
   |        |
   +--------+ */
0x00,
0x00,
0xfe,
0xc6,
0x06,
0x0c,
0x18,
0x30,
0x30,
0x30,
0x30,
0x30,
0x00,
0x00,
0x00,
0x00,

/* Character 8 (0x38):
   ht=16, width=8
   +--------+
   |        |
   |        |
   | *****  |
   |**   ** |
   |**   ** |
   |**   ** |
   | *****  |
   |**   ** |
   |**   ** |
   |**   ** |
   |**   ** |
   | *****  |
   |        |
   |        |
   |        |
   |        |
   +--------+ */
0x00,
0x00,
0x7c,
0xc6,
0xc6,
0xc6,
0x7c,
0xc6,
0xc6,
0xc6,
0xc6,
0x7c,
0x00,
0x00,
0x00,
0x00,

/* Character 9 (0x39):
   ht=16, width=8
   +--------+
   |        |
   |        |
   | *****  |
   |**   ** |
   |**   ** |
   |**   ** |
   |**   ** |
   | ****** |
   |     ** |
   |     ** |
   |**   ** |
   | *****  |
   |        |
   |        |
   |        |
   |        |
   +--------+ */
0x00,
0x00,
0x7c,
0xc6,
0xc6,
0xc6,
0xc6,
0x7e,
0x06,
0x06,
0xc6,
0x7c,
0x00,
0x00,
0x00,
0x00,
/* Character : (0x3a):
   ht=16, width=8
   +--------+
   |        |
   |        |
   |        |
   |        |
   |        |
   |    **  |
   |    **  |
   |        |
   |        |
   |    **  |
   |    **  |
   |        |
   |        |
   |        |
   |        |
   |        |
   +--------+ */
0x00,
0x00,
0x00,
0x00,
0x00,
0x0c,
0x0c,
0x00,
0x00,
0x0c,
0x0c,
0x00,
0x00,
0x00,
0x00,
0x00,
};
