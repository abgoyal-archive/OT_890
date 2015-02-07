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
 * linux/fs/xattr_acl.c
 *
 * Almost all from linux/fs/ext2/acl.c:
 * Copyright (C) 2001 by Andreas Gruenbacher, <a.gruenbacher@computer.org>
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/posix_acl_xattr.h>


/*
 * Convert from extended attribute to in-memory representation.
 */
struct posix_acl *
posix_acl_from_xattr(const void *value, size_t size)
{
	posix_acl_xattr_header *header = (posix_acl_xattr_header *)value;
	posix_acl_xattr_entry *entry = (posix_acl_xattr_entry *)(header+1), *end;
	int count;
	struct posix_acl *acl;
	struct posix_acl_entry *acl_e;

	if (!value)
		return NULL;
	if (size < sizeof(posix_acl_xattr_header))
		 return ERR_PTR(-EINVAL);
	if (header->a_version != cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		return ERR_PTR(-EOPNOTSUPP);

	count = posix_acl_xattr_count(size);
	if (count < 0)
		return ERR_PTR(-EINVAL);
	if (count == 0)
		return NULL;
	
	acl = posix_acl_alloc(count, GFP_KERNEL);
	if (!acl)
		return ERR_PTR(-ENOMEM);
	acl_e = acl->a_entries;
	
	for (end = entry + count; entry != end; acl_e++, entry++) {
		acl_e->e_tag  = le16_to_cpu(entry->e_tag);
		acl_e->e_perm = le16_to_cpu(entry->e_perm);

		switch(acl_e->e_tag) {
			case ACL_USER_OBJ:
			case ACL_GROUP_OBJ:
			case ACL_MASK:
			case ACL_OTHER:
				acl_e->e_id = ACL_UNDEFINED_ID;
				break;

			case ACL_USER:
			case ACL_GROUP:
				acl_e->e_id = le32_to_cpu(entry->e_id);
				break;

			default:
				goto fail;
		}
	}
	return acl;

fail:
	posix_acl_release(acl);
	return ERR_PTR(-EINVAL);
}
EXPORT_SYMBOL (posix_acl_from_xattr);

/*
 * Convert from in-memory to extended attribute representation.
 */
int
posix_acl_to_xattr(const struct posix_acl *acl, void *buffer, size_t size)
{
	posix_acl_xattr_header *ext_acl = (posix_acl_xattr_header *)buffer;
	posix_acl_xattr_entry *ext_entry = ext_acl->a_entries;
	int real_size, n;

	real_size = posix_acl_xattr_size(acl->a_count);
	if (!buffer)
		return real_size;
	if (real_size > size)
		return -ERANGE;
	
	ext_acl->a_version = cpu_to_le32(POSIX_ACL_XATTR_VERSION);

	for (n=0; n < acl->a_count; n++, ext_entry++) {
		ext_entry->e_tag  = cpu_to_le16(acl->a_entries[n].e_tag);
		ext_entry->e_perm = cpu_to_le16(acl->a_entries[n].e_perm);
		ext_entry->e_id   = cpu_to_le32(acl->a_entries[n].e_id);
	}
	return real_size;
}
EXPORT_SYMBOL (posix_acl_to_xattr);
