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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/kfifo.h>

#include <linux/firmware.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/proc_fs.h>

#include <mach/mt6516_pll.h>
#include <mach/mt6516_boot.h>

#define MOD "BOOT"

/* this vairable will be set by mt6516_fixup */
BOOTMODE g_boot_mode = UNKNOWN_BOOT;

static ssize_t (*md_show)(char*) = NULL;
static ssize_t (*md_store)(const char*,size_t) = NULL;

void boot_register_md_func(ssize_t (*show)(char*), ssize_t (*store)(const char*,size_t))
{
	md_show = show;
	md_store = store;
}

static ssize_t boot_show(struct kobject *kobj, struct attribute *a, char *buf)
{
	if (!strncmp(a->name, MD_SYSFS_ATTR, strlen(MD_SYSFS_ATTR)) && md_show) {
		return md_show(buf);
	}
	else
            return sprintf(buf, "%d\n", g_boot_mode);
}

static ssize_t boot_store(struct kobject *kobj, struct attribute *a, const char *buf, size_t count)
{
    if (!strncmp(a->name, MD_SYSFS_ATTR, strlen(MD_SYSFS_ATTR)) && md_store) {
	    return md_store(buf, count);
    }
    return count;
}


/* boot object */
static struct kobject boot_kobj;
static struct sysfs_ops boot_sysfs_ops = {
    .show = boot_show,
    .store = boot_store
};

/* boot attribute */
struct attribute boot_attr = {BOOT_SYSFS_ATTR, THIS_MODULE, 0644};
struct attribute md_attr = {MD_SYSFS_ATTR, THIS_MODULE, 0644};
static struct attribute *boot_attrs[] = {
    &boot_attr,
    &md_attr,
    NULL
};

/* boot type */
static struct kobj_type boot_ktype = {
    .sysfs_ops = &boot_sysfs_ops,
    .default_attrs = boot_attrs
};

/* boot device node */
static dev_t boot_dev_num;
static struct cdev boot_cdev;
static struct file_operations boot_fops = {
    .owner = THIS_MODULE,
    .open = NULL,
    .release = NULL,
    .write = NULL,
    .read = NULL,
    .ioctl = NULL
};

/* boot device class */
static struct class *boot_class;
static struct device *boot_device;


/* return boot mode */
BOOTMODE get_boot_mode(void)
{	return g_boot_mode;
}

/* for convenience, simply check is meta mode or not */
bool is_meta_mode(void)
{	if(g_boot_mode == META_BOOT)
	{	return true;
	}
	else
	{	return false;
	}
}

static int boot_mode_proc(char *page, char **start, off_t off,int count, int *eof, void *data)
{
  char *p = page;
  int len = 0; 

  p += sprintf(p, "\n\rMT6516 BOOT MODE : " );
  switch(g_boot_mode)
  {
  	case NORMAL_BOOT :
		  p += sprintf(p, "NORMAL BOOT\n");  		
		  break;
  	case META_BOOT :
		  p += sprintf(p, "META BOOT\n");  		
		  break;
	default :	  	
		  p += sprintf(p, "UNKNOWN BOOT\n");  		
		  break;
  }  
  *start = page + off;
  len = p - page;
  if (len > off)
		len -= off;
  else
		len = 0;

  return len < count ? len  : count;     
}

static int __init boot_mod_init(void)
{
    int ret;

    /* allocate device major number */
    if (alloc_chrdev_region(&boot_dev_num, 0, 1, BOOT_DEV_NAME) < 0) {
        printk("[%s] fail to register chrdev\n",MOD);
        return -1;
    }

    /* add character driver */
    cdev_init(&boot_cdev, &boot_fops);
    ret = cdev_add(&boot_cdev, boot_dev_num, 1);
    if (ret < 0) {
        printk("[%s] fail to add cdev\n",MOD);
        return ret;
    }

	/* create class (device model) */
    boot_class = class_create(THIS_MODULE, BOOT_DEV_NAME);
    if (IS_ERR(boot_class)) {
        printk("[%s] fail to create class\n",MOD);
        return (int)boot_class;
    }

    boot_device = device_create(boot_class, NULL, boot_dev_num, NULL, BOOT_DEV_NAME);
    if (IS_ERR(boot_device)) {
        printk("[%s] fail to create device\n",MOD);
        return (int)boot_device;
    }

    /* add kobject */
    ret = kobject_init_and_add(&boot_kobj, &boot_ktype, &(boot_device->kobj), BOOT_SYSFS);
    if (ret < 0) {
        printk("[%s] fail to add kobject\n",MOD);
        return ret;
    }

    /* create proc entry at /proc/boot_mode */
    create_proc_read_entry("boot_mode", S_IRUGO, NULL, boot_mode_proc, NULL);

    return 0;
}

static void __exit boot_mod_exit(void)
{
    cdev_del(&boot_cdev);
}

module_init(boot_mod_init);
module_exit(boot_mod_exit);
MODULE_DESCRIPTION("MT6516 Boot Information Querying Driver");
MODULE_LICENSE("Proprietary");
EXPORT_SYMBOL(is_meta_mode);
EXPORT_SYMBOL(get_boot_mode);
EXPORT_SYMBOL(boot_register_md_func);

