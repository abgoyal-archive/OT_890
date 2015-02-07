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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/uaccess.h>

#include <asm/auxio.h>

#define LED_MAX_LENGTH 8 /* maximum chars written to proc file */

static inline void led_toggle(void)
{
	unsigned char val = get_auxio();
	unsigned char on, off;

	if (val & AUXIO_LED) {
		on = 0;
		off = AUXIO_LED;
	} else {
		on = AUXIO_LED;
		off = 0;
	}

	set_auxio(on, off);
}

static struct timer_list led_blink_timer;

static void led_blink(unsigned long timeout)
{
	led_toggle();

	/* reschedule */
	if (!timeout) { /* blink according to load */
		led_blink_timer.expires = jiffies +
			((1 + (avenrun[0] >> FSHIFT)) * HZ);
		led_blink_timer.data = 0;
	} else { /* blink at user specified interval */
		led_blink_timer.expires = jiffies + (timeout * HZ);
		led_blink_timer.data = timeout;
	}
	add_timer(&led_blink_timer);
}

static int led_proc_show(struct seq_file *m, void *v)
{
	if (get_auxio() & AUXIO_LED)
		seq_puts(m, "on\n");
	else
		seq_puts(m, "off\n");
	return 0;
}

static int led_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, led_proc_show, NULL);
}

static ssize_t led_proc_write(struct file *file, const char __user *buffer,
			      size_t count, loff_t *ppos)
{
	char *buf = NULL;

	if (count > LED_MAX_LENGTH)
		count = LED_MAX_LENGTH;

	buf = kmalloc(sizeof(char) * (count + 1), GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	if (copy_from_user(buf, buffer, count)) {
		kfree(buf);
		return -EFAULT;
	}

	buf[count] = '\0';

	/* work around \n when echo'ing into proc */
	if (buf[count - 1] == '\n')
		buf[count - 1] = '\0';

	/* before we change anything we want to stop any running timers,
	 * otherwise calls such as on will have no persistent effect
	 */
	del_timer_sync(&led_blink_timer);

	if (!strcmp(buf, "on")) {
		auxio_set_led(AUXIO_LED_ON);
	} else if (!strcmp(buf, "toggle")) {
		led_toggle();
	} else if ((*buf > '0') && (*buf <= '9')) {
		led_blink(simple_strtoul(buf, NULL, 10));
	} else if (!strcmp(buf, "load")) {
		led_blink(0);
	} else {
		auxio_set_led(AUXIO_LED_OFF);
	}

	kfree(buf);

	return count;
}

static const struct file_operations led_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= led_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= led_proc_write,
};

static struct proc_dir_entry *led;

#define LED_VERSION	"0.1"

static int __init led_init(void)
{
	init_timer(&led_blink_timer);
	led_blink_timer.function = led_blink;

	led = proc_create("led", 0, NULL, &led_proc_fops);
	if (!led)
		return -ENOMEM;
	led->owner = THIS_MODULE;

	printk(KERN_INFO
	       "led: version %s, Lars Kotthoff <metalhead@metalhead.ws>\n",
	       LED_VERSION);

	return 0;
}

static void __exit led_exit(void)
{
	remove_proc_entry("led", NULL);
	del_timer_sync(&led_blink_timer);
}

module_init(led_init);
module_exit(led_exit);

MODULE_AUTHOR("Lars Kotthoff <metalhead@metalhead.ws>");
MODULE_DESCRIPTION("Provides control of the front LED on SPARC systems.");
MODULE_LICENSE("GPL");
MODULE_VERSION(LED_VERSION);
