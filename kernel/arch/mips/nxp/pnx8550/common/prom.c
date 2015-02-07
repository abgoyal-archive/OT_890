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
 *
 * Per Hallsmark, per.hallsmark@mvista.com
 *
 * Based on jmr3927/common/prom.c
 *
 * 2004 (c) MontaVista Software, Inc. This file is licensed under the
 * terms of the GNU General Public License version 2. This program is
 * licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/serial_pnx8xxx.h>

#include <asm/bootinfo.h>
#include <uart.h>

/* #define DEBUG_CMDLINE */

extern int prom_argc;
extern char **prom_argv, **prom_envp;

typedef struct
{
    char *name;
/*    char *val; */
}t_env_var;


char * prom_getcmdline(void)
{
	return &(arcs_cmdline[0]);
}

void __init prom_init_cmdline(void)
{
	int i;

	arcs_cmdline[0] = '\0';
	for (i = 0; i < prom_argc; i++) {
		strcat(arcs_cmdline, prom_argv[i]);
		strcat(arcs_cmdline, " ");
	}
}

char *prom_getenv(char *envname)
{
	/*
	 * Return a pointer to the given environment variable.
	 * Environment variables are stored in the form of "memsize=64".
	 */

	t_env_var *env = (t_env_var *)prom_envp;
	int i;

	i = strlen(envname);

	while(env->name) {
		if(strncmp(envname, env->name, i) == 0) {
			return(env->name + strlen(envname) + 1);
		}
		env++;
	}
	return(NULL);
}

inline unsigned char str2hexnum(unsigned char c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if(c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return 0; /* foo */
}

inline void str2eaddr(unsigned char *ea, unsigned char *str)
{
	int i;

	for(i = 0; i < 6; i++) {
		unsigned char num;

		if((*str == '.') || (*str == ':'))
			str++;
		num = str2hexnum(*str++) << 4;
		num |= (str2hexnum(*str++));
		ea[i] = num;
	}
}

int get_ethernet_addr(char *ethernet_addr)
{
        char *ethaddr_str;

        ethaddr_str = prom_getenv("ethaddr");
	if (!ethaddr_str) {
	        printk("ethaddr not set in boot prom\n");
		return -1;
	}
	str2eaddr(ethernet_addr, ethaddr_str);
	return 0;
}

void __init prom_free_prom_memory(void)
{
}

extern int pnx8550_console_port;

/* used by early printk */
void prom_putchar(char c)
{
	if (pnx8550_console_port != -1) {
		/* Wait until FIFO not full */
		while( ((ip3106_fifo(UART_BASE, pnx8550_console_port) & PNX8XXX_UART_FIFO_TXFIFO) >> 16) >= 16)
			;
		/* Send one char */
		ip3106_fifo(UART_BASE, pnx8550_console_port) = c;
	}
}

EXPORT_SYMBOL(prom_getcmdline);
EXPORT_SYMBOL(get_ethernet_addr);
EXPORT_SYMBOL(str2eaddr);
