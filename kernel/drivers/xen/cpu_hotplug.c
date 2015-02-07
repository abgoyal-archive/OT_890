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

#include <linux/notifier.h>

#include <xen/xenbus.h>

#include <asm/xen/hypervisor.h>
#include <asm/cpu.h>

static void enable_hotplug_cpu(int cpu)
{
	if (!cpu_present(cpu))
		arch_register_cpu(cpu);

	cpu_set(cpu, cpu_present_map);
}

static void disable_hotplug_cpu(int cpu)
{
	if (cpu_present(cpu))
		arch_unregister_cpu(cpu);

	cpu_clear(cpu, cpu_present_map);
}

static void vcpu_hotplug(unsigned int cpu)
{
	int err;
	char dir[32], state[32];

	if (!cpu_possible(cpu))
		return;

	sprintf(dir, "cpu/%u", cpu);
	err = xenbus_scanf(XBT_NIL, dir, "availability", "%s", state);
	if (err != 1) {
		printk(KERN_ERR "XENBUS: Unable to read cpu state\n");
		return;
	}

	if (strcmp(state, "online") == 0) {
		enable_hotplug_cpu(cpu);
	} else if (strcmp(state, "offline") == 0) {
		(void)cpu_down(cpu);
		disable_hotplug_cpu(cpu);
	} else {
		printk(KERN_ERR "XENBUS: unknown state(%s) on CPU%d\n",
		       state, cpu);
	}
}

static void handle_vcpu_hotplug_event(struct xenbus_watch *watch,
					const char **vec, unsigned int len)
{
	unsigned int cpu;
	char *cpustr;
	const char *node = vec[XS_WATCH_PATH];

	cpustr = strstr(node, "cpu/");
	if (cpustr != NULL) {
		sscanf(cpustr, "cpu/%u", &cpu);
		vcpu_hotplug(cpu);
	}
}

static int setup_cpu_watcher(struct notifier_block *notifier,
			      unsigned long event, void *data)
{
	static struct xenbus_watch cpu_watch = {
		.node = "cpu",
		.callback = handle_vcpu_hotplug_event};

	(void)register_xenbus_watch(&cpu_watch);

	return NOTIFY_DONE;
}

static int __init setup_vcpu_hotplug_event(void)
{
	static struct notifier_block xsn_cpu = {
		.notifier_call = setup_cpu_watcher };

	if (!xen_pv_domain())
		return -ENODEV;

	register_xenstore_notifier(&xsn_cpu);

	return 0;
}

arch_initcall(setup_vcpu_hotplug_event);

