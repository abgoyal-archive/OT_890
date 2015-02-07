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
 * arch/arm/mach-sa1100/include/mach/collie.h
 *
 * This file contains the hardware specific definitions for Assabet
 * Only include this file from SA1100-specific files.
 *
 * ChangeLog:
 *   04-06-2001 Lineo Japan, Inc.
 *   04-16-2001 SHARP Corporation
 *   07-07-2002 Chris Larson <clarson@digi.com>
 *
 */
#ifndef __ASM_ARCH_COLLIE_H
#define __ASM_ARCH_COLLIE_H


#define COLLIE_SCP_CHARGE_ON	SCOOP_GPCR_PA11
#define COLLIE_SCP_DIAG_BOOT1	SCOOP_GPCR_PA12
#define COLLIE_SCP_DIAG_BOOT2	SCOOP_GPCR_PA13
#define COLLIE_SCP_MUTE_L	SCOOP_GPCR_PA14
#define COLLIE_SCP_MUTE_R	SCOOP_GPCR_PA15
#define COLLIE_SCP_5VON		SCOOP_GPCR_PA16
#define COLLIE_SCP_AMP_ON	SCOOP_GPCR_PA17
#define COLLIE_SCP_VPEN		SCOOP_GPCR_PA18
#define COLLIE_SCP_LB_VOL_CHG	SCOOP_GPCR_PA19

#define COLLIE_SCOOP_IO_DIR	( COLLIE_SCP_CHARGE_ON | COLLIE_SCP_MUTE_L | COLLIE_SCP_MUTE_R | \
				COLLIE_SCP_5VON | COLLIE_SCP_AMP_ON | COLLIE_SCP_VPEN | \
				COLLIE_SCP_LB_VOL_CHG )
#define COLLIE_SCOOP_IO_OUT	( COLLIE_SCP_MUTE_L | COLLIE_SCP_MUTE_R | COLLIE_SCP_VPEN | \
				COLLIE_SCP_CHARGE_ON )

/* GPIOs for which the generic definition doesn't say much */

#define COLLIE_GPIO_ON_KEY		GPIO_GPIO (0)
#define COLLIE_GPIO_AC_IN		GPIO_GPIO (1)
#define COLLIE_GPIO_SDIO_INT		GPIO_GPIO (11)
#define COLLIE_GPIO_CF_IRQ		GPIO_GPIO (14)
#define COLLIE_GPIO_nREMOCON_INT	GPIO_GPIO (15)
#define COLLIE_GPIO_UCB1x00_RESET	GPIO_GPIO (16)
#define COLLIE_GPIO_nMIC_ON		GPIO_GPIO (17)
#define COLLIE_GPIO_nREMOCON_ON		GPIO_GPIO (18)
#define COLLIE_GPIO_CO			GPIO_GPIO (20)
#define COLLIE_GPIO_MCP_CLK		GPIO_GPIO (21)
#define COLLIE_GPIO_CF_CD		GPIO_GPIO (22)
#define COLLIE_GPIO_UCB1x00_IRQ		GPIO_GPIO (23)
#define COLLIE_GPIO_WAKEUP		GPIO_GPIO (24)
#define COLLIE_GPIO_GA_INT		GPIO_GPIO (25)
#define COLLIE_GPIO_MAIN_BAT_LOW	GPIO_GPIO (26)

/* Interrupts */

#define COLLIE_IRQ_GPIO_ON_KEY		IRQ_GPIO0
#define COLLIE_IRQ_GPIO_AC_IN		IRQ_GPIO1
#define COLLIE_IRQ_GPIO_SDIO_IRQ	IRQ_GPIO11
#define COLLIE_IRQ_GPIO_CF_IRQ		IRQ_GPIO14
#define COLLIE_IRQ_GPIO_nREMOCON_INT	IRQ_GPIO15
#define COLLIE_IRQ_GPIO_CO		IRQ_GPIO20
#define COLLIE_IRQ_GPIO_CF_CD		IRQ_GPIO22
#define COLLIE_IRQ_GPIO_UCB1x00_IRQ	IRQ_GPIO23
#define COLLIE_IRQ_GPIO_WAKEUP		IRQ_GPIO24
#define COLLIE_IRQ_GPIO_GA_INT		IRQ_GPIO25
#define COLLIE_IRQ_GPIO_MAIN_BAT_LOW	IRQ_GPIO26

#define COLLIE_LCM_IRQ_GPIO_RTS		IRQ_LOCOMO_GPIO0
#define COLLIE_LCM_IRQ_GPIO_CTS		IRQ_LOCOMO_GPIO1
#define COLLIE_LCM_IRQ_GPIO_DSR		IRQ_LOCOMO_GPIO2
#define COLLIE_LCM_IRQ_GPIO_DTR		IRQ_LOCOMO_GPIO3
#define COLLIE_LCM_IRQ_GPIO_nSD_DETECT	IRQ_LOCOMO_GPIO13
#define COLLIE_LCM_IRQ_GPIO_nSD_WP	IRQ_LOCOMO_GPIO14

/* GPIO's on the TC35143AF (Toshiba Analog Frontend) */
#define COLLIE_TC35143_GPIO_VERSION0    UCB_IO_0	/* GPIO0=Version                 */
#define COLLIE_TC35143_GPIO_TBL_CHK     UCB_IO_1	/* GPIO1=TBL_CHK                 */
#define COLLIE_TC35143_GPIO_VPEN_ON     UCB_IO_2	/* GPIO2=VPNE_ON                 */
#define COLLIE_TC35143_GPIO_IR_ON       UCB_IO_3	/* GPIO3=IR_ON                   */
#define COLLIE_TC35143_GPIO_AMP_ON      UCB_IO_4	/* GPIO4=AMP_ON                  */
#define COLLIE_TC35143_GPIO_VERSION1    UCB_IO_5	/* GPIO5=Version                 */
#define COLLIE_TC35143_GPIO_FS8KLPF     UCB_IO_5	/* GPIO5=fs 8k LPF               */
#define COLLIE_TC35143_GPIO_BUZZER_BIAS UCB_IO_6	/* GPIO6=BUZZER BIAS             */
#define COLLIE_TC35143_GPIO_MBAT_ON     UCB_IO_7	/* GPIO7=MBAT_ON                 */
#define COLLIE_TC35143_GPIO_BBAT_ON     UCB_IO_8	/* GPIO8=BBAT_ON                 */
#define COLLIE_TC35143_GPIO_TMP_ON      UCB_IO_9	/* GPIO9=TMP_ON                  */
#define COLLIE_TC35143_GPIO_IN		( UCB_IO_0 | UCB_IO_2 | UCB_IO_5 )
#define COLLIE_TC35143_GPIO_OUT		( UCB_IO_1 | UCB_IO_3 | UCB_IO_4 | UCB_IO_6 | \
					UCB_IO_7 | UCB_IO_8 | UCB_IO_9 )

#endif