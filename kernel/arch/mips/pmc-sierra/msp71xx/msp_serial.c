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
 * The setup file for serial related hardware on PMC-Sierra MSP processors.
 *
 * Copyright 2005 PMC-Sierra, Inc.
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/serial.h>
#include <linux/serial_core.h>
#include <linux/serial_reg.h>

#include <asm/bootinfo.h>
#include <asm/io.h>
#include <asm/processor.h>
#include <asm/serial.h>
#include <linux/serial_8250.h>

#include <msp_prom.h>
#include <msp_int.h>
#include <msp_regs.h>

void __init msp_serial_setup(void)
{
	char    *s;
	char    *endp;
	struct uart_port up;
	unsigned int uartclk;

	memset(&up, 0, sizeof(up));

	/* Check if clock was specified in environment */
	s = prom_getenv("uartfreqhz");
	if(!(s && *s && (uartclk = simple_strtoul(s, &endp, 10)) && *endp == 0))
		uartclk = MSP_BASE_BAUD;
	ppfinit("UART clock set to %d\n", uartclk);

	/* Initialize first serial port */
	up.mapbase      = MSP_UART0_BASE;
	up.membase      = ioremap_nocache(up.mapbase, MSP_UART_REG_LEN);
	up.irq          = MSP_INT_UART0;
	up.uartclk      = uartclk;
	up.regshift     = 2;
	up.iotype       = UPIO_DWAPB; /* UPIO_MEM like */
	up.flags        = ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST;
	up.type         = PORT_16550A;
	up.line         = 0;
	up.private_data		= (void*)UART0_STATUS_REG;
	if (early_serial_setup(&up))
		printk(KERN_ERR "Early serial init of port 0 failed\n");

	/* Initialize the second serial port, if one exists */
	switch (mips_machtype) {
		case MACH_MSP4200_EVAL:
		case MACH_MSP4200_GW:
		case MACH_MSP4200_FPGA:
		case MACH_MSP7120_EVAL:
		case MACH_MSP7120_GW:
		case MACH_MSP7120_FPGA:
			/* Enable UART1 on MSP4200 and MSP7120 */
			*GPIO_CFG2_REG = 0x00002299;
			break;

		default:
			return; /* No second serial port, good-bye. */
	}

	up.mapbase      = MSP_UART1_BASE;
	up.membase      = ioremap_nocache(up.mapbase, MSP_UART_REG_LEN);
	up.irq          = MSP_INT_UART1;
	up.line         = 1;
	up.private_data		= (void*)UART1_STATUS_REG;
	if (early_serial_setup(&up))
		printk(KERN_ERR "Early serial init of port 1 failed\n");
}
