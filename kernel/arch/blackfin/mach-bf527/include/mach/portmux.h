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

#define P_PPI0_D0	(P_DEFINED | P_IDENT(GPIO_PF0) | P_FUNCT(0))
#define P_PPI0_D1	(P_DEFINED | P_IDENT(GPIO_PF1) | P_FUNCT(0))
#define P_PPI0_D2	(P_DEFINED | P_IDENT(GPIO_PF2) | P_FUNCT(0))
#define P_PPI0_D3	(P_DEFINED | P_IDENT(GPIO_PF3) | P_FUNCT(0))
#define P_PPI0_D4	(P_DEFINED | P_IDENT(GPIO_PF4) | P_FUNCT(0))
#define P_PPI0_D5	(P_DEFINED | P_IDENT(GPIO_PF5) | P_FUNCT(0))
#define P_PPI0_D6	(P_DEFINED | P_IDENT(GPIO_PF6) | P_FUNCT(0))
#define P_PPI0_D7	(P_DEFINED | P_IDENT(GPIO_PF7) | P_FUNCT(0))
#define P_PPI0_D8	(P_DEFINED | P_IDENT(GPIO_PF8) | P_FUNCT(0))
#define P_PPI0_D9	(P_DEFINED | P_IDENT(GPIO_PF9) | P_FUNCT(0))
#define P_PPI0_D10	(P_DEFINED | P_IDENT(GPIO_PF10) | P_FUNCT(0))
#define P_PPI0_D11	(P_DEFINED | P_IDENT(GPIO_PF11) | P_FUNCT(0))
#define P_PPI0_D12	(P_DEFINED | P_IDENT(GPIO_PF12) | P_FUNCT(0))
#define P_PPI0_D13	(P_DEFINED | P_IDENT(GPIO_PF13) | P_FUNCT(0))
#define P_PPI0_D14	(P_DEFINED | P_IDENT(GPIO_PF14) | P_FUNCT(0))
#define P_PPI0_D15	(P_DEFINED | P_IDENT(GPIO_PF15) | P_FUNCT(0))

#if defined(CONFIG_BF527_SPORT0_PORTF)
#define P_SPORT0_DRPRI	(P_DEFINED | P_IDENT(GPIO_PF0) | P_FUNCT(1))
#define P_SPORT0_RFS	(P_DEFINED | P_IDENT(GPIO_PF1) | P_FUNCT(1))
#define P_SPORT0_RSCLK	(P_DEFINED | P_IDENT(GPIO_PF2) | P_FUNCT(1))
#define P_SPORT0_DTPRI	(P_DEFINED | P_IDENT(GPIO_PF3) | P_FUNCT(1))
#define P_SPORT0_TFS	(P_DEFINED | P_IDENT(GPIO_PF4) | P_FUNCT(1))
#define P_SPORT0_TSCLK	(P_DEFINED | P_IDENT(GPIO_PF5) | P_FUNCT(1))
#define P_SPORT0_DTSEC	(P_DEFINED | P_IDENT(GPIO_PF6) | P_FUNCT(1))
#define P_SPORT0_DRSEC	(P_DEFINED | P_IDENT(GPIO_PF7) | P_FUNCT(1))
#elif defined(CONFIG_BF527_SPORT0_PORTG)
#define P_SPORT0_DTPRI	(P_DEFINED | P_IDENT(GPIO_PG6) | P_FUNCT(0))
#define P_SPORT0_DRSEC	(P_DEFINED | P_IDENT(GPIO_PG3) | P_FUNCT(1))
#define P_SPORT0_DTSEC	(P_DEFINED | P_IDENT(GPIO_PG4) | P_FUNCT(1))
#define P_SPORT0_DRPRI	(P_DEFINED | P_IDENT(GPIO_PG7) | P_FUNCT(1))
#define P_SPORT0_RFS	(P_DEFINED | P_IDENT(GPIO_PG8) | P_FUNCT(1))
#define P_SPORT0_RSCLK	(P_DEFINED | P_IDENT(GPIO_PG9) | P_FUNCT(1))
#if defined(CONFIG_BF527_SPORT0_TSCLK_PG10)
#define P_SPORT0_TSCLK	(P_DEFINED | P_IDENT(GPIO_PG10) | P_FUNCT(1))
#elif defined(CONFIG_BF527_SPORT0_TSCLK_PG14)
#define P_SPORT0_TSCLK	(P_DEFINED | P_IDENT(GPIO_PG14) | P_FUNCT(0))
#endif
#define P_SPORT0_TFS	(P_DEFINED | P_IDENT(GPIO_PG15) | P_FUNCT(0))
#endif

#define P_SPORT1_DRPRI	(P_DEFINED | P_IDENT(GPIO_PF8) | P_FUNCT(1))
#define P_SPORT1_RSCLK	(P_DEFINED | P_IDENT(GPIO_PF9) | P_FUNCT(1))
#define P_SPORT1_RFS	(P_DEFINED | P_IDENT(GPIO_PF10) | P_FUNCT(1))
#define P_SPORT1_TFS	(P_DEFINED | P_IDENT(GPIO_PF11) | P_FUNCT(1))
#define P_SPORT1_DTPRI	(P_DEFINED | P_IDENT(GPIO_PF12) | P_FUNCT(1))
#define P_SPORT1_TSCLK	(P_DEFINED | P_IDENT(GPIO_PF13) | P_FUNCT(1))
#define P_SPORT1_DTSEC	(P_DEFINED | P_IDENT(GPIO_PF14) | P_FUNCT(1))
#define P_SPORT1_DRSEC	(P_DEFINED | P_IDENT(GPIO_PF15) | P_FUNCT(1))

#define P_SPI0_SSEL6	(P_DEFINED | P_IDENT(GPIO_PF9) | P_FUNCT(2))
#define P_SPI0_SSEL7	(P_DEFINED | P_IDENT(GPIO_PF10) | P_FUNCT(2))

#define P_SPI0_SSEL2	(P_DEFINED | P_IDENT(GPIO_PF12) | P_FUNCT(2))
#define P_SPI0_SSEL3	(P_DEFINED | P_IDENT(GPIO_PF13) | P_FUNCT(2))

#if defined(CONFIG_BF527_UART1_PORTF)
#define P_UART1_TX	(P_DEFINED | P_IDENT(GPIO_PF14) | P_FUNCT(2))
#define P_UART1_RX	(P_DEFINED | P_IDENT(GPIO_PF15) | P_FUNCT(2))
#elif defined(CONFIG_BF527_UART1_PORTG)
#define P_UART1_TX	(P_DEFINED | P_IDENT(GPIO_PG12) | P_FUNCT(1))
#define P_UART1_RX	(P_DEFINED | P_IDENT(GPIO_PG13) | P_FUNCT(1))
#endif

#define P_CNT_CZM	(P_DEFINED | P_IDENT(GPIO_PF11) | P_FUNCT(3))
#define P_CNT_CDG	(P_DEFINED | P_IDENT(GPIO_PF12) | P_FUNCT(3))
#define P_CNT_CUD	(P_DEFINED | P_IDENT(GPIO_PF13) | P_FUNCT(3))

#define P_HWAIT		(P_DONTCARE)

#define P_DEFAULT_BOOT_SPI_CS P_SPI0_SSEL1

#define P_SPI0_SS	(P_DEFINED | P_IDENT(GPIO_PG1) | P_FUNCT(0))
#define P_SPI0_SSEL1	(P_DEFINED | P_IDENT(GPIO_PG1) | P_FUNCT(2))
#define P_SPI0_SCK	(P_DEFINED | P_IDENT(GPIO_PG2) | P_FUNCT(2))
#define P_SPI0_MISO	(P_DEFINED | P_IDENT(GPIO_PG3) | P_FUNCT(2))
#define P_SPI0_MOSI	(P_DEFINED | P_IDENT(GPIO_PG4) | P_FUNCT(2))
#define P_TMR1		(P_DEFINED | P_IDENT(GPIO_PG5) | P_FUNCT(0))
#define P_PPI0_FS2	(P_DEFINED | P_IDENT(GPIO_PG5) | P_FUNCT(0))
#define P_TMR3		(P_DEFINED | P_IDENT(GPIO_PG7) | P_FUNCT(0))
#define P_TMR4		(P_DEFINED | P_IDENT(GPIO_PG8) | P_FUNCT(0))
#define P_TMR5		(P_DEFINED | P_IDENT(GPIO_PG9) | P_FUNCT(0))
#define P_TMR6		(P_DEFINED | P_IDENT(GPIO_PG10) | P_FUNCT(0))
/* #define P_TMR7		(P_DEFINED | P_IDENT(GPIO_PG11) | P_FUNCT(0)) */
#define P_DMAR1		(P_DEFINED | P_IDENT(GPIO_PG12) | P_FUNCT(0))
#define P_DMAR0		(P_DEFINED | P_IDENT(GPIO_PG13) | P_FUNCT(0))
#define P_TMR2		(P_DEFINED | P_IDENT(GPIO_PG6) | P_FUNCT(1))
#define P_TMR7		(P_DEFINED | P_IDENT(GPIO_PG11) | P_FUNCT(1))
#define P_MDC		(P_DEFINED | P_IDENT(GPIO_PG14) | P_FUNCT(1))
#define P_RMII0_MDINT	(P_DEFINED | P_IDENT(GPIO_PG15) | P_FUNCT(1))
#define P_MII0_PHYINT	(P_DEFINED | P_IDENT(GPIO_PG15) | P_FUNCT(1))

#define P_PPI0_FS3	(P_DEFINED | P_IDENT(GPIO_PG6) | P_FUNCT(2))
#define P_UART0_TX	(P_DEFINED | P_IDENT(GPIO_PG7) | P_FUNCT(2))
#define P_UART0_RX	(P_DEFINED | P_IDENT(GPIO_PG8) | P_FUNCT(2))

#define P_HOST_WR	(P_DEFINED | P_IDENT(GPIO_PG11) | P_FUNCT(2))
#define P_HOST_ACK	(P_DEFINED | P_IDENT(GPIO_PG12) | P_FUNCT(2))
#define P_HOST_ADDR	(P_DEFINED | P_IDENT(GPIO_PG13) | P_FUNCT(2))
#define P_HOST_RD	(P_DEFINED | P_IDENT(GPIO_PG14) | P_FUNCT(2))
#define P_HOST_CE	(P_DEFINED | P_IDENT(GPIO_PG15) | P_FUNCT(2))

#if defined(CONFIG_BF527_NAND_D_PORTF)
#define P_NAND_D0	(P_DEFINED | P_IDENT(GPIO_PF0) | P_FUNCT(2))
#define P_NAND_D1	(P_DEFINED | P_IDENT(GPIO_PF1) | P_FUNCT(2))
#define P_NAND_D2	(P_DEFINED | P_IDENT(GPIO_PF2) | P_FUNCT(2))
#define P_NAND_D3	(P_DEFINED | P_IDENT(GPIO_PF3) | P_FUNCT(2))
#define P_NAND_D4	(P_DEFINED | P_IDENT(GPIO_PF4) | P_FUNCT(2))
#define P_NAND_D5	(P_DEFINED | P_IDENT(GPIO_PF5) | P_FUNCT(2))
#define P_NAND_D6	(P_DEFINED | P_IDENT(GPIO_PF6) | P_FUNCT(2))
#define P_NAND_D7	(P_DEFINED | P_IDENT(GPIO_PF7) | P_FUNCT(2))
#elif defined(CONFIG_BF527_NAND_D_PORTH)
#define P_NAND_D0	(P_DEFINED | P_IDENT(GPIO_PH0) | P_FUNCT(0))
#define P_NAND_D1	(P_DEFINED | P_IDENT(GPIO_PH1) | P_FUNCT(0))
#define P_NAND_D2	(P_DEFINED | P_IDENT(GPIO_PH2) | P_FUNCT(0))
#define P_NAND_D3	(P_DEFINED | P_IDENT(GPIO_PH3) | P_FUNCT(0))
#define P_NAND_D4	(P_DEFINED | P_IDENT(GPIO_PH4) | P_FUNCT(0))
#define P_NAND_D5	(P_DEFINED | P_IDENT(GPIO_PH5) | P_FUNCT(0))
#define P_NAND_D6	(P_DEFINED | P_IDENT(GPIO_PH6) | P_FUNCT(0))
#define P_NAND_D7	(P_DEFINED | P_IDENT(GPIO_PH7) | P_FUNCT(0))
#endif

#define P_SPI0_SSEL4	(P_DEFINED | P_IDENT(GPIO_PH8) | P_FUNCT(0))
#define P_SPI0_SSEL5	(P_DEFINED | P_IDENT(GPIO_PH9) | P_FUNCT(0))
#define P_NAND_CE	(P_DEFINED | P_IDENT(GPIO_PH10) | P_FUNCT(0))
#define P_NAND_WE	(P_DEFINED | P_IDENT(GPIO_PH11) | P_FUNCT(0))
#define P_NAND_RE	(P_DEFINED | P_IDENT(GPIO_PH12) | P_FUNCT(0))
#define P_NAND_RB	(P_DEFINED | P_IDENT(GPIO_PH13) | P_FUNCT(0))
#define P_NAND_CLE	(P_DEFINED | P_IDENT(GPIO_PH14) | P_FUNCT(0))
#define P_NAND_ALE	(P_DEFINED | P_IDENT(GPIO_PH15) | P_FUNCT(0))

#define P_HOST_D0	(P_DEFINED | P_IDENT(GPIO_PH0) | P_FUNCT(2))
#define P_HOST_D1	(P_DEFINED | P_IDENT(GPIO_PH1) | P_FUNCT(2))
#define P_HOST_D2	(P_DEFINED | P_IDENT(GPIO_PH2) | P_FUNCT(2))
#define P_HOST_D3	(P_DEFINED | P_IDENT(GPIO_PH3) | P_FUNCT(2))
#define P_HOST_D4	(P_DEFINED | P_IDENT(GPIO_PH4) | P_FUNCT(2))
#define P_HOST_D5	(P_DEFINED | P_IDENT(GPIO_PH5) | P_FUNCT(2))
#define P_HOST_D6	(P_DEFINED | P_IDENT(GPIO_PH6) | P_FUNCT(2))
#define P_HOST_D7	(P_DEFINED | P_IDENT(GPIO_PH7) | P_FUNCT(2))
#define P_HOST_D8	(P_DEFINED | P_IDENT(GPIO_PH8) | P_FUNCT(2))
#define P_HOST_D9	(P_DEFINED | P_IDENT(GPIO_PH9) | P_FUNCT(2))
#define P_HOST_D10	(P_DEFINED | P_IDENT(GPIO_PH10) | P_FUNCT(2))
#define P_HOST_D11	(P_DEFINED | P_IDENT(GPIO_PH11) | P_FUNCT(2))
#define P_HOST_D12	(P_DEFINED | P_IDENT(GPIO_PH12) | P_FUNCT(2))
#define P_HOST_D13	(P_DEFINED | P_IDENT(GPIO_PH13) | P_FUNCT(2))
#define P_HOST_D14	(P_DEFINED | P_IDENT(GPIO_PH14) | P_FUNCT(2))
#define P_HOST_D15	(P_DEFINED | P_IDENT(GPIO_PH15) | P_FUNCT(2))

#define P_MII0_ETxD0	(P_DEFINED | P_IDENT(GPIO_PH5) | P_FUNCT(1))
#define P_MII0_ETxD1	(P_DEFINED | P_IDENT(GPIO_PH7) | P_FUNCT(1))
#define P_MII0_ETxD2	(P_DEFINED | P_IDENT(GPIO_PH9) | P_FUNCT(1))
#define P_MII0_ETxD3	(P_DEFINED | P_IDENT(GPIO_PH11) | P_FUNCT(1))
#define P_MII0_ETxEN	(P_DEFINED | P_IDENT(GPIO_PH3) | P_FUNCT(1))
#define P_MII0_TxCLK	(P_DEFINED | P_IDENT(GPIO_PH4) | P_FUNCT(1))
#define P_MII0_COL	(P_DEFINED | P_IDENT(GPIO_PH15) | P_FUNCT(1))
#define P_MII0_ERxD0	(P_DEFINED | P_IDENT(GPIO_PH6) | P_FUNCT(1))
#define P_MII0_ERxD1	(P_DEFINED | P_IDENT(GPIO_PH8) | P_FUNCT(1))
#define P_MII0_ERxD2	(P_DEFINED | P_IDENT(GPIO_PH10) | P_FUNCT(1))
#define P_MII0_ERxD3	(P_DEFINED | P_IDENT(GPIO_PH12) | P_FUNCT(1))
#define P_MII0_ERxDV	(P_DEFINED | P_IDENT(GPIO_PH14) | P_FUNCT(1))
#define P_MII0_ERxCLK	(P_DEFINED | P_IDENT(GPIO_PH13) | P_FUNCT(1))
#define P_MII0_ERxER	(P_DEFINED | P_IDENT(GPIO_PH1) | P_FUNCT(1))
#define P_MII0_CRS	(P_DEFINED | P_IDENT(GPIO_PH0) | P_FUNCT(1))
#define P_RMII0_REF_CLK	(P_DEFINED | P_IDENT(GPIO_PH4) | P_FUNCT(1))
#define P_RMII0_CRS_DV	(P_DEFINED | P_IDENT(GPIO_PH0) | P_FUNCT(1))
#define P_MDIO		(P_DEFINED | P_IDENT(GPIO_PH2) | P_FUNCT(1))

#define P_TWI0_SCL	(P_DONTCARE)
#define P_TWI0_SDA	(P_DONTCARE)
#define P_PPI0_FS1	(P_DONTCARE)
#define P_TMR0		(P_DONTCARE)
#define P_TMRCLK	(P_DONTCARE)
#define P_PPI0_CLK	(P_DONTCARE)

#define P_MII0 {\
	P_MII0_ETxD0, \
	P_MII0_ETxD1, \
	P_MII0_ETxD2, \
	P_MII0_ETxD3, \
	P_MII0_ETxEN, \
	P_MII0_TxCLK, \
	P_MII0_PHYINT, \
	P_MII0_COL, \
	P_MII0_ERxD0, \
	P_MII0_ERxD1, \
	P_MII0_ERxD2, \
	P_MII0_ERxD3, \
	P_MII0_ERxDV, \
	P_MII0_ERxCLK, \
	P_MII0_ERxER, \
	P_MII0_CRS, \
	P_MDC, \
	P_MDIO, 0}

#define P_RMII0 {\
	P_MII0_ETxD0, \
	P_MII0_ETxD1, \
	P_MII0_ETxEN, \
	P_MII0_ERxD0, \
	P_MII0_ERxD1, \
	P_MII0_ERxER, \
	P_RMII0_REF_CLK, \
	P_RMII0_MDINT, \
	P_RMII0_CRS_DV, \
	P_MDC, \
	P_MDIO, 0}

#endif				/* _MACH_PORTMUX_H_ */