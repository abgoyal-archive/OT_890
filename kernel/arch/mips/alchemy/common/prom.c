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
 * BRIEF MODULE DESCRIPTION
 *    PROM library initialisation code, supports YAMON and U-Boot.
 *
 * Copyright 2000-2001, 2006, 2008 MontaVista Software Inc.
 * Author: MontaVista Software, Inc. <source@mvista.com>
 *
 * This file was derived from Carsten Langgaard's
 * arch/mips/mips-boards/xx files.
 *
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 1999,2000 MIPS Technologies, Inc.  All rights reserved.
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

#include <linux/module.h>
#include <linux/init.h>
#include <linux/string.h>

#include <asm/bootinfo.h>

int prom_argc;
char **prom_argv;
char **prom_envp;

char * __init_or_module prom_getcmdline(void)
{
	return &(arcs_cmdline[0]);
}

void prom_init_cmdline(void)
{
	char *cp;
	int actr;

	actr = 1; /* Always ignore argv[0] */

	cp = &(arcs_cmdline[0]);
	while (actr < prom_argc) {
		strcpy(cp, prom_argv[actr]);
		cp += strlen(prom_argv[actr]);
		*cp++ = ' ';
		actr++;
	}
	if (cp != &(arcs_cmdline[0])) /* get rid of trailing space */
		--cp;
	if (prom_argc > 1)
		*cp = '\0';
}

char *prom_getenv(char *envname)
{
	/*
	 * Return a pointer to the given environment variable.
	 * YAMON uses "name", "value" pairs, while U-Boot uses "name=value".
	 */

	char **env = prom_envp;
	int i = strlen(envname);
	int yamon = (*env && strchr(*env, '=') == NULL);

	while (*env) {
		if (yamon) {
			if (strcmp(envname, *env++) == 0)
				return *env;
		} else if (strncmp(envname, *env, i) == 0 && (*env)[i] == '=')
			return *env + i + 1;
		env++;
	}

	return NULL;
}

static inline unsigned char str2hexnum(unsigned char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;

	return 0; /* foo */
}

static inline void str2eaddr(unsigned char *ea, unsigned char *str)
{
	int i;

	for (i = 0; i < 6; i++) {
		unsigned char num;

		if ((*str == '.') || (*str == ':'))
			str++;
		num  = str2hexnum(*str++) << 4;
		num |= str2hexnum(*str++);
		ea[i] = num;
	}
}

int prom_get_ethernet_addr(char *ethernet_addr)
{
	char *ethaddr_str;
	char *argptr;

	/* Check the environment variables first */
	ethaddr_str = prom_getenv("ethaddr");
	if (!ethaddr_str) {
		/* Check command line */
		argptr = prom_getcmdline();
		ethaddr_str = strstr(argptr, "ethaddr=");
		if (!ethaddr_str)
			return -1;

		ethaddr_str += strlen("ethaddr=");
	}

	str2eaddr(ethernet_addr, ethaddr_str);

	return 0;
}
EXPORT_SYMBOL(prom_get_ethernet_addr);

void __init prom_free_prom_memory(void)
{
}
