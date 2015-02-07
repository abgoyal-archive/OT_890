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
 *  Copyright (C) 2007, OpenWrt.org, Florian Fainelli <florian@openwrt.org>
 *  	Architecture specific GPIO support
 *
 *  This program is free software; you can redistribute	 it and/or modify it
 *  under  the terms of	 the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the	License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED	  ``AS	IS'' AND   ANY	EXPRESS OR IMPLIED
 *  WARRANTIES,	  INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO	EVENT  SHALL   THE AUTHOR  BE	 LIABLE FOR ANY	  DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED	  TO, PROCUREMENT OF  SUBSTITUTE GOODS	OR SERVICES; LOSS OF
 *  USE, DATA,	OR PROFITS; OR	BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN	 CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  Notes :
 * 	au1000 SoC have only one GPIO line : GPIO1
 * 	others have a second one : GPIO2
 */

#include <linux/module.h>

#include <asm/mach-au1x00/au1000.h>
#include <asm/gpio.h>

#define gpio1 sys
#if !defined(CONFIG_SOC_AU1000)

static struct au1x00_gpio2 *const gpio2 = (struct au1x00_gpio2 *) GPIO2_BASE;
#define GPIO2_OUTPUT_ENABLE_MASK 	0x00010000

static int au1xxx_gpio2_read(unsigned gpio)
{
	gpio -= AU1XXX_GPIO_BASE;
	return ((gpio2->pinstate >> gpio) & 0x01);
}

static void au1xxx_gpio2_write(unsigned gpio, int value)
{
	gpio -= AU1XXX_GPIO_BASE;

	gpio2->output = (GPIO2_OUTPUT_ENABLE_MASK << gpio) | ((!!value) << gpio);
}

static int au1xxx_gpio2_direction_input(unsigned gpio)
{
	gpio -= AU1XXX_GPIO_BASE;
	gpio2->dir &= ~(0x01 << gpio);
	return 0;
}

static int au1xxx_gpio2_direction_output(unsigned gpio, int value)
{
	gpio -= AU1XXX_GPIO_BASE;
	gpio2->dir |= 0x01 << gpio;
	gpio2->output = (GPIO2_OUTPUT_ENABLE_MASK << gpio) | ((!!value) << gpio);
	return 0;
}

#endif /* !defined(CONFIG_SOC_AU1000) */

static int au1xxx_gpio1_read(unsigned gpio)
{
	return (gpio1->pinstaterd >> gpio) & 0x01;
}

static void au1xxx_gpio1_write(unsigned gpio, int value)
{
	if (value)
		gpio1->outputset = (0x01 << gpio);
	else
		/* Output a zero */
		gpio1->outputclr = (0x01 << gpio);
}

static int au1xxx_gpio1_direction_input(unsigned gpio)
{
	gpio1->pininputen = (0x01 << gpio);
	return 0;
}

static int au1xxx_gpio1_direction_output(unsigned gpio, int value)
{
	gpio1->trioutclr = (0x01 & gpio);
	au1xxx_gpio1_write(gpio, value);
	return 0;
}

int au1xxx_gpio_get_value(unsigned gpio)
{
	if (gpio >= AU1XXX_GPIO_BASE)
#if defined(CONFIG_SOC_AU1000)
		return 0;
#else
		return au1xxx_gpio2_read(gpio);
#endif
	else
		return au1xxx_gpio1_read(gpio);
}
EXPORT_SYMBOL(au1xxx_gpio_get_value);

void au1xxx_gpio_set_value(unsigned gpio, int value)
{
	if (gpio >= AU1XXX_GPIO_BASE)
#if defined(CONFIG_SOC_AU1000)
		;
#else
		au1xxx_gpio2_write(gpio, value);
#endif
	else
		au1xxx_gpio1_write(gpio, value);
}
EXPORT_SYMBOL(au1xxx_gpio_set_value);

int au1xxx_gpio_direction_input(unsigned gpio)
{
	if (gpio >= AU1XXX_GPIO_BASE)
#if defined(CONFIG_SOC_AU1000)
		return -ENODEV;
#else
		return au1xxx_gpio2_direction_input(gpio);
#endif

	return au1xxx_gpio1_direction_input(gpio);
}
EXPORT_SYMBOL(au1xxx_gpio_direction_input);

int au1xxx_gpio_direction_output(unsigned gpio, int value)
{
	if (gpio >= AU1XXX_GPIO_BASE)
#if defined(CONFIG_SOC_AU1000)
		return -ENODEV;
#else
		return au1xxx_gpio2_direction_output(gpio, value);
#endif

	return au1xxx_gpio1_direction_output(gpio, value);
}
EXPORT_SYMBOL(au1xxx_gpio_direction_output);
