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

/* Prom access routines for the sun3x */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/tty.h>
#include <linux/console.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/string.h>

#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/bootinfo.h>
#include <asm/setup.h>
#include <asm/traps.h>
#include <asm/sun3xprom.h>
#include <asm/idprom.h>
#include <asm/segment.h>
#include <asm/sun3ints.h>
#include <asm/openprom.h>
#include <asm/machines.h>

void (*sun3x_putchar)(int);
int (*sun3x_getchar)(void);
int (*sun3x_mayget)(void);
int (*sun3x_mayput)(int);
void (*sun3x_prom_reboot)(void);
e_vector sun3x_prom_abort;
struct linux_romvec *romvec;

/* prom vector table */
e_vector *sun3x_prom_vbr;

/* Handle returning to the prom */
void sun3x_halt(void)
{
	unsigned long flags;

	/* Disable interrupts while we mess with things */
	local_irq_save(flags);

	/* Restore prom vbr */
	asm volatile ("movec %0,%%vbr" : : "r" ((void*)sun3x_prom_vbr));

	/* Restore prom NMI clock */
//	sun3x_disable_intreg(5);
	sun3_enable_irq(7);

	/* Let 'er rip */
	asm volatile ("trap #14");

	/* Restore everything */
	sun3_disable_irq(7);
	sun3_enable_irq(5);

	asm volatile ("movec %0,%%vbr" : : "r" ((void*)vectors));
	local_irq_restore(flags);
}

void sun3x_reboot(void)
{
	/* This never returns, don't bother saving things */
	local_irq_disable();

	/* Restore prom vbr */
	asm volatile ("movec %0,%%vbr" : : "r" ((void*)sun3x_prom_vbr));

	/* Restore prom NMI clock */
	sun3_disable_irq(5);
	sun3_enable_irq(7);

	/* Let 'er rip */
	(*romvec->pv_reboot)("vmlinux");
}

static void sun3x_prom_write(struct console *co, const char *s,
                             unsigned int count)
{
	while (count--) {
		if (*s == '\n')
			sun3x_putchar('\r');
		sun3x_putchar(*s++);
	}
}

/* debug console - write-only */

static struct console sun3x_debug = {
	.name	= "debug",
	.write	= sun3x_prom_write,
	.flags	= CON_PRINTBUFFER,
	.index	= -1,
};

void __init sun3x_prom_init(void)
{
	/* Read the vector table */

	sun3x_putchar = *(void (**)(int)) (SUN3X_P_PUTCHAR);
	sun3x_getchar = *(int (**)(void)) (SUN3X_P_GETCHAR);
	sun3x_mayget = *(int (**)(void))  (SUN3X_P_MAYGET);
	sun3x_mayput = *(int (**)(int))   (SUN3X_P_MAYPUT);
	sun3x_prom_reboot = *(void (**)(void)) (SUN3X_P_REBOOT);
	sun3x_prom_abort = *(e_vector *)  (SUN3X_P_ABORT);
	romvec = (struct linux_romvec *)SUN3X_PROM_BASE;

	idprom_init();

	if (!((idprom->id_machtype & SM_ARCH_MASK) == SM_SUN3X)) {
		printk("Warning: machine reports strange type %02x\n",
			idprom->id_machtype);
		printk("Pretending it's a 3/80, but very afraid...\n");
		idprom->id_machtype = SM_SUN3X | SM_3_80;
	}

	/* point trap #14 at abort.
	 * XXX this is futile since we restore the vbr first - oops
	 */
	vectors[VEC_TRAP14] = sun3x_prom_abort;
}

static int __init sun3x_debug_setup(char *arg)
{
	/* If debug=prom was specified, start the debug console */
	if (MACH_IS_SUN3X && !strcmp(arg, "prom"))
		register_console(&sun3x_debug);
	return 0;
}

early_param("debug", sun3x_debug_setup);

/* some prom functions to export */
int prom_getintdefault(int node, char *property, int deflt)
{
	return deflt;
}

int prom_getbool (int node, char *prop)
{
	return 1;
}

void prom_printf(char *fmt, ...)
{
}

void prom_halt (void)
{
	sun3x_halt();
}

/* Get the idprom and stuff it into buffer 'idbuf'.  Returns the
 * format type.  'num_bytes' is the number of bytes that your idbuf
 * has space for.  Returns 0xff on error.
 */
unsigned char
prom_get_idprom(char *idbuf, int num_bytes)
{
        int i;

	/* make a copy of the idprom structure */
	for (i = 0; i < num_bytes; i++)
		idbuf[i] = ((char *)SUN3X_IDPROM)[i];

        return idbuf[0];
}
