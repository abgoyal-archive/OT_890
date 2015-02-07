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

/* $Id$
 *
 * ISDN lowlevel-module for the IBM ISDN-S0 Active 2000 (ISA-Version).
 *
 * Author       Fritz Elfert
 * Copyright    by Fritz Elfert      <fritz@isdn4linux.de>
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 * Thanks to Friedemann Baitinger and IBM Germany
 *
 */

#ifndef act2000_isa_h
#define act2000_isa_h

#define ISA_POLL_LOOP 40        /* Try to read-write before give up */

typedef enum {
        INT_NO_CHANGE = 0,      /* Do not change the Mask */
        INT_ON = 1,             /* Set to Enable */
        INT_OFF = 2,            /* Set to Disable */
} ISA_INT_T;

/**************************************************************************/
/*      Configuration Register COR (RW)                                   */
/**************************************************************************/
/*    7    |   6    |    5   |   4    |    3   |    2   |    1   |    0   */
/* Soft Res|  IRQM  |        IRQ Select        |   N/A  |  WAIT  |Proc err */
/**************************************************************************/
#define        ISA_COR             0	/* Offset for ISA config register */
#define        ISA_COR_PERR     0x01	/* Processor Error Enabled        */
#define        ISA_COR_WS       0x02	/* Insert Wait State if 1         */
#define        ISA_COR_IRQOFF   0x38	/* No Interrupt                   */
#define        ISA_COR_IRQ07    0x30	/* IRQ 7 Enable                   */
#define        ISA_COR_IRQ05    0x28	/* IRQ 5 Enable                   */
#define        ISA_COR_IRQ03    0x20	/* IRQ 3 Enable                   */
#define        ISA_COR_IRQ10    0x18	/* IRQ 10 Enable                  */
#define        ISA_COR_IRQ11    0x10	/* IRQ 11 Enable                  */
#define        ISA_COR_IRQ12    0x08	/* IRQ 12 Enable                  */
#define        ISA_COR_IRQ15    0x00	/* IRQ 15 Enable                  */
#define        ISA_COR_IRQPULSE 0x40	/* 0 = Level 1 = Pulse Interrupt  */
#define        ISA_COR_RESET    0x80	/* Soft Reset for Transputer      */

/**************************************************************************/
/*      Interrupt Source Register ISR (RO)                                */
/**************************************************************************/
/*    7    |   6    |    5   |   4    |    3   |    2   |    1   |    0   */
/*   N/A   |  N/A   |   N/A  |Err sig |Ser ID  |IN Intr |Out Intr| Error  */
/**************************************************************************/
#define        ISA_ISR             1	/* Offset for Interrupt Register  */
#define        ISA_ISR_ERR      0x01	/* Error Interrupt                */
#define        ISA_ISR_OUT      0x02	/* Output Interrupt               */
#define        ISA_ISR_INP      0x04	/* Input Interrupt                */
#define        ISA_ISR_SERIAL   0x08	/* Read out Serial ID after Reset */
#define        ISA_ISR_ERRSIG   0x10	/* Error Signal Input             */
#define        ISA_ISR_ERR_MASK 0xfe    /* Mask Error Interrupt           */
#define        ISA_ISR_OUT_MASK 0xfd    /* Mask Output Interrupt          */
#define        ISA_ISR_INP_MASK 0xfb    /* Mask Input Interrupt           */

/* Signature delivered after Reset at ISA_ISR_SERIAL (LSB first)          */
#define        ISA_SER_ID     0x0201	/* ID for ISA Card                */

/**************************************************************************/
/*      EEPROM Register EPR (RW)                                          */
/**************************************************************************/
/*    7    |   6    |    5   |   4    |    3   |    2   |    1   |    0   */
/*   N/A   |  N/A   |   N/A  |ROM Hold| ROM CS |ROM CLK | ROM IN |ROM Out */
/**************************************************************************/
#define        ISA_EPR             2	/* Offset for this Register       */
#define        ISA_EPR_OUT      0x01	/* Rome Register Out (RO)         */
#define        ISA_EPR_IN       0x02	/* Rom Register In (WR)           */
#define        ISA_EPR_CLK      0x04	/* Rom Clock (WR)                 */
#define        ISA_EPR_CS       0x08	/* Rom Cip Select (WR)            */
#define        ISA_EPR_HOLD     0x10	/* Rom Hold Signal (WR)           */

/**************************************************************************/
/*      EEPROM enable Register EER (unused)                               */
/**************************************************************************/
#define        ISA_EER             3	/* Offset for this Register       */

/**************************************************************************/
/*      SLC Data Input SDI (RO)                                           */
/**************************************************************************/
#define        ISA_SDI             4	/* Offset for this Register       */

/**************************************************************************/
/*      SLC Data Output SDO (WO)                                          */
/**************************************************************************/
#define        ISA_SDO             5	/* Offset for this Register       */

/**************************************************************************/
/*      IMS C011 Mode 2 Input Status Register for INMOS CPU SIS (RW)      */
/**************************************************************************/
/*    7    |   6    |    5   |   4    |    3   |    2   |    1   |    0   */
/*   N/A   |  N/A   |   N/A  |  N/A   |   N/A  |   N/A  |Int Ena |Data Pre */
/**************************************************************************/
#define        ISA_SIS             6	/* Offset for this Register       */
#define        ISA_SIS_READY    0x01	/* If 1 : data is available       */
#define        ISA_SIS_INT      0x02	/* Enable Interrupt for READ      */

/**************************************************************************/
/*      IMS C011 Mode 2 Output Status Register from INMOS CPU SOS (RW)    */
/**************************************************************************/
/*    7    |   6    |    5   |   4    |    3   |    2   |    1   |    0   */
/*   N/A   |  N/A   |   N/A  |  N/A   |   N/A  |   N/A  |Int Ena |Out Rdy */
/**************************************************************************/
#define        ISA_SOS             7	/* Offset for this Register       */
#define        ISA_SOS_READY    0x01	/* If 1 : we can write Data       */
#define        ISA_SOS_INT      0x02	/* Enable Interrupt for WRITE     */

#define        ISA_REGION          8	/* Number of Registers            */


/* Macros for accessing ports */
#define ISA_PORT_COR (card->port+ISA_COR)
#define ISA_PORT_ISR (card->port+ISA_ISR)
#define ISA_PORT_EPR (card->port+ISA_EPR)
#define ISA_PORT_EER (card->port+ISA_EER)
#define ISA_PORT_SDI (card->port+ISA_SDI)
#define ISA_PORT_SDO (card->port+ISA_SDO)
#define ISA_PORT_SIS (card->port+ISA_SIS)
#define ISA_PORT_SOS (card->port+ISA_SOS)

/* Prototypes */

extern int act2000_isa_detect(unsigned short portbase);
extern int act2000_isa_config_irq(act2000_card * card, short irq);
extern int act2000_isa_config_port(act2000_card * card, unsigned short portbase);
extern int act2000_isa_download(act2000_card * card, act2000_ddef __user * cb);
extern void act2000_isa_release(act2000_card * card);
extern void act2000_isa_receive(act2000_card *card);
extern void act2000_isa_send(act2000_card *card);

#endif                          /* act2000_isa_h */
