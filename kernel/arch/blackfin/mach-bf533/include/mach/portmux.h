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

#ifndef _MACH_PORTMUX_H_
#define _MACH_PORTMUX_H_

#define MAX_RESOURCES 	MAX_BLACKFIN_GPIOS

#define P_PPI0_CLK	(P_DONTCARE)
#define P_PPI0_FS1	(P_DONTCARE)
#define P_PPI0_FS2	(P_DONTCARE)
#define P_PPI0_FS3	(P_DEFINED | P_IDENT(GPIO_PF3))
#define P_PPI0_D15	(P_DEFINED | P_IDENT(GPIO_PF4))
#define P_PPI0_D14	(P_DEFINED | P_IDENT(GPIO_PF5))
#define P_PPI0_D13	(P_DEFINED | P_IDENT(GPIO_PF6))
#define P_PPI0_D12	(P_DEFINED | P_IDENT(GPIO_PF7))
#define P_PPI0_D11	(P_DEFINED | P_IDENT(GPIO_PF8))
#define P_PPI0_D10	(P_DEFINED | P_IDENT(GPIO_PF9))
#define P_PPI0_D9	(P_DEFINED | P_IDENT(GPIO_PF10))
#define P_PPI0_D8	(P_DEFINED | P_IDENT(GPIO_PF11))
#define P_PPI0_D0	(P_DONTCARE)
#define P_PPI0_D1	(P_DONTCARE)
#define P_PPI0_D2	(P_DONTCARE)
#define P_PPI0_D3	(P_DONTCARE)
#define P_PPI0_D4	(P_DEFINED | P_IDENT(GPIO_PF15))
#define P_PPI0_D5	(P_DEFINED | P_IDENT(GPIO_PF14))
#define P_PPI0_D6	(P_DEFINED | P_IDENT(GPIO_PF13))
#define P_PPI0_D7	(P_DEFINED | P_IDENT(GPIO_PF12))

#define P_SPORT1_TSCLK	(P_DONTCARE)
#define P_SPORT1_RSCLK	(P_DONTCARE)
#define P_SPORT0_TSCLK	(P_DONTCARE)
#define P_SPORT0_RSCLK	(P_DONTCARE)
#define P_UART0_RX	(P_DONTCARE)
#define P_UART0_TX	(P_DONTCARE)
#define P_SPORT1_DRSEC	(P_DONTCARE)
#define P_SPORT1_RFS	(P_DONTCARE)
#define P_SPORT1_DTPRI	(P_DONTCARE)
#define P_SPORT1_DTSEC	(P_DONTCARE)
#define P_SPORT1_TFS	(P_DONTCARE)
#define P_SPORT1_DRPRI	(P_DONTCARE)
#define P_SPORT0_DRSEC	(P_DONTCARE)
#define P_SPORT0_RFS	(P_DONTCARE)
#define P_SPORT0_DTPRI	(P_DONTCARE)
#define P_SPORT0_DTSEC	(P_DONTCARE)
#define P_SPORT0_TFS	(P_DONTCARE)
#define P_SPORT0_DRPRI	(P_DONTCARE)

#define P_SPI0_MOSI	(P_DONTCARE)
#define P_SPI0_MISO	(P_DONTCARE)
#define P_SPI0_SCK	(P_DONTCARE)
#define P_SPI0_SSEL7	(P_DEFINED | P_IDENT(GPIO_PF7))
#define P_SPI0_SSEL6	(P_DEFINED | P_IDENT(GPIO_PF6))
#define P_SPI0_SSEL5	(P_DEFINED | P_IDENT(GPIO_PF5))
#define P_SPI0_SSEL4	(P_DEFINED | P_IDENT(GPIO_PF4))
#define P_SPI0_SSEL3	(P_DEFINED | P_IDENT(GPIO_PF3))
#define P_SPI0_SSEL2	(P_DEFINED | P_IDENT(GPIO_PF2))
#define P_SPI0_SSEL1	(P_DEFINED | P_IDENT(GPIO_PF1))
#define P_SPI0_SS	(P_DEFINED | P_IDENT(GPIO_PF0))
#define P_DEFAULT_BOOT_SPI_CS P_SPI0_SSEL2

#define P_TMR2		(P_DONTCARE)
#define P_TMR1		(P_DONTCARE)
#define P_TMR0		(P_DONTCARE)
#define P_TMRCLK	(P_DEFINED | P_IDENT(GPIO_PF1))

#endif /* _MACH_PORTMUX_H_ */