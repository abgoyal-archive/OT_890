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
 * Randomness driver for virtio
 *  Copyright (C) 2007, 2008 Rusty Russell IBM Corporation
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */
#include <linux/err.h>
#include <linux/hw_random.h>
#include <linux/scatterlist.h>
#include <linux/spinlock.h>
#include <linux/virtio.h>
#include <linux/virtio_rng.h>

/* The host will fill any buffer we give it with sweet, sweet randomness.  We
 * give it 64 bytes at a time, and the hwrng framework takes it 4 bytes at a
 * time. */
#define RANDOM_DATA_SIZE 64

static struct virtqueue *vq;
static u32 *random_data;
static unsigned int data_left;
static DECLARE_COMPLETION(have_data);

static void random_recv_done(struct virtqueue *vq)
{
	int len;

	/* We never get spurious callbacks. */
	if (!vq->vq_ops->get_buf(vq, &len))
		BUG();

	data_left = len / sizeof(random_data[0]);
	complete(&have_data);
}

static void register_buffer(void)
{
	struct scatterlist sg;

	sg_init_one(&sg, random_data, RANDOM_DATA_SIZE);
	/* There should always be room for one buffer. */
	if (vq->vq_ops->add_buf(vq, &sg, 0, 1, random_data) != 0)
		BUG();
	vq->vq_ops->kick(vq);
}

/* At least we don't udelay() in a loop like some other drivers. */
static int virtio_data_present(struct hwrng *rng, int wait)
{
	if (data_left)
		return 1;

	if (!wait)
		return 0;

	wait_for_completion(&have_data);
	return 1;
}

/* virtio_data_present() must have succeeded before this is called. */
static int virtio_data_read(struct hwrng *rng, u32 *data)
{
	BUG_ON(!data_left);

	*data = random_data[--data_left];

	if (!data_left) {
		init_completion(&have_data);
		register_buffer();
	}
	return sizeof(*data);
}

static struct hwrng virtio_hwrng = {
	.name = "virtio",
	.data_present = virtio_data_present,
	.data_read = virtio_data_read,
};

static int virtrng_probe(struct virtio_device *vdev)
{
	int err;

	/* We expect a single virtqueue. */
	vq = vdev->config->find_vq(vdev, 0, random_recv_done);
	if (IS_ERR(vq))
		return PTR_ERR(vq);

	err = hwrng_register(&virtio_hwrng);
	if (err) {
		vdev->config->del_vq(vq);
		return err;
	}

	register_buffer();
	return 0;
}

static void virtrng_remove(struct virtio_device *vdev)
{
	vdev->config->reset(vdev);
	hwrng_unregister(&virtio_hwrng);
	vdev->config->del_vq(vq);
}

static struct virtio_device_id id_table[] = {
	{ VIRTIO_ID_RNG, VIRTIO_DEV_ANY_ID },
	{ 0 },
};

static struct virtio_driver virtio_rng = {
	.driver.name =	KBUILD_MODNAME,
	.driver.owner =	THIS_MODULE,
	.id_table =	id_table,
	.probe =	virtrng_probe,
	.remove =	__devexit_p(virtrng_remove),
};

static int __init init(void)
{
	int err;

	random_data = kmalloc(RANDOM_DATA_SIZE, GFP_KERNEL);
	if (!random_data)
		return -ENOMEM;

	err = register_virtio_driver(&virtio_rng);
	if (err)
		kfree(random_data);
	return err;
}

static void __exit fini(void)
{
	kfree(random_data);
	unregister_virtio_driver(&virtio_rng);
}
module_init(init);
module_exit(fini);

MODULE_DEVICE_TABLE(virtio, id_table);
MODULE_DESCRIPTION("Virtio random number driver");
MODULE_LICENSE("GPL");
