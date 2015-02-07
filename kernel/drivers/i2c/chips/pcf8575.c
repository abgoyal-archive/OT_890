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
  pcf8575.c

  About the PCF8575 chip: the PCF8575 is a 16-bit I/O expander for the I2C bus
  produced by a.o. Philips Semiconductors.

  Copyright (C) 2006 Michael Hennerich, Analog Devices Inc.
  <hennerich@blackfin.uclinux.org>
  Based on pcf8574.c.

  Copyright (c) 2007 Bart Van Assche <bart.vanassche@gmail.com>.
  Ported this driver from ucLinux to the mainstream Linux kernel.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/slab.h>  /* kzalloc() */
#include <linux/sysfs.h> /* sysfs_create_group() */

/* Addresses to scan: none, device can't be detected */
static const unsigned short normal_i2c[] = { I2C_CLIENT_END };

/* Insmod parameters */
I2C_CLIENT_INSMOD;


/* Each client has this additional data */
struct pcf8575_data {
	int write;		/* last written value, or error code */
};

/* following are the sysfs callback functions */
static ssize_t show_read(struct device *dev, struct device_attribute *attr,
			 char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	u16 val;
	u8 iopin_state[2];

	i2c_master_recv(client, iopin_state, 2);

	val = iopin_state[0];
	val |= iopin_state[1] << 8;

	return sprintf(buf, "%u\n", val);
}

static DEVICE_ATTR(read, S_IRUGO, show_read, NULL);

static ssize_t show_write(struct device *dev, struct device_attribute *attr,
			  char *buf)
{
	struct pcf8575_data *data = dev_get_drvdata(dev);
	if (data->write < 0)
		return data->write;
	return sprintf(buf, "%d\n", data->write);
}

static ssize_t set_write(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct pcf8575_data *data = i2c_get_clientdata(client);
	unsigned long val = simple_strtoul(buf, NULL, 10);
	u8 iopin_state[2];

	if (val > 0xffff)
		return -EINVAL;

	data->write = val;

	iopin_state[0] = val & 0xFF;
	iopin_state[1] = val >> 8;

	i2c_master_send(client, iopin_state, 2);

	return count;
}

static DEVICE_ATTR(write, S_IWUSR | S_IRUGO, show_write, set_write);

static struct attribute *pcf8575_attributes[] = {
	&dev_attr_read.attr,
	&dev_attr_write.attr,
	NULL
};

static const struct attribute_group pcf8575_attr_group = {
	.attrs = pcf8575_attributes,
};

/*
 * Real code
 */

/* Return 0 if detection is successful, -ENODEV otherwise */
static int pcf8575_detect(struct i2c_client *client, int kind,
			  struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;

	if (!i2c_check_functionality(adapter, I2C_FUNC_I2C))
		return -ENODEV;

	/* This is the place to detect whether the chip at the specified
	   address really is a PCF8575 chip. However, there is no method known
	   to detect whether an I2C chip is a PCF8575 or any other I2C chip. */

	strlcpy(info->type, "pcf8575", I2C_NAME_SIZE);

	return 0;
}

static int pcf8575_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	struct pcf8575_data *data;
	int err;

	data = kzalloc(sizeof(struct pcf8575_data), GFP_KERNEL);
	if (!data) {
		err = -ENOMEM;
		goto exit;
	}

	i2c_set_clientdata(client, data);
	data->write = -EAGAIN;

	/* Register sysfs hooks */
	err = sysfs_create_group(&client->dev.kobj, &pcf8575_attr_group);
	if (err)
		goto exit_free;

	return 0;

exit_free:
	kfree(data);
exit:
	return err;
}

static int pcf8575_remove(struct i2c_client *client)
{
	sysfs_remove_group(&client->dev.kobj, &pcf8575_attr_group);
	kfree(i2c_get_clientdata(client));
	return 0;
}

static const struct i2c_device_id pcf8575_id[] = {
	{ "pcf8575", 0 },
	{ }
};

static struct i2c_driver pcf8575_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= "pcf8575",
	},
	.probe		= pcf8575_probe,
	.remove		= pcf8575_remove,
	.id_table	= pcf8575_id,

	.detect		= pcf8575_detect,
	.address_data	= &addr_data,
};

static int __init pcf8575_init(void)
{
	return i2c_add_driver(&pcf8575_driver);
}

static void __exit pcf8575_exit(void)
{
	i2c_del_driver(&pcf8575_driver);
}

MODULE_AUTHOR("Michael Hennerich <hennerich@blackfin.uclinux.org>, "
	      "Bart Van Assche <bart.vanassche@gmail.com>");
MODULE_DESCRIPTION("pcf8575 driver");
MODULE_LICENSE("GPL");

module_init(pcf8575_init);
module_exit(pcf8575_exit);
