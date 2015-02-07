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

/**
 * eCryptfs: Linux filesystem encryption layer
 * Functions only useful for debugging.
 *
 * Copyright (C) 2006 International Business Machines Corp.
 *   Author(s): Michael A. Halcrow <mahalcro@us.ibm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#include "ecryptfs_kernel.h"

/**
 * ecryptfs_dump_auth_tok - debug function to print auth toks
 *
 * This function will print the contents of an ecryptfs authentication
 * token.
 */
void ecryptfs_dump_auth_tok(struct ecryptfs_auth_tok *auth_tok)
{
	char salt[ECRYPTFS_SALT_SIZE * 2 + 1];
	char sig[ECRYPTFS_SIG_SIZE_HEX + 1];

	ecryptfs_printk(KERN_DEBUG, "Auth tok at mem loc [%p]:\n",
			auth_tok);
	if (auth_tok->flags & ECRYPTFS_PRIVATE_KEY) {
		ecryptfs_printk(KERN_DEBUG, " * private key type\n");
	} else {
		ecryptfs_printk(KERN_DEBUG, " * passphrase type\n");
		ecryptfs_to_hex(salt, auth_tok->token.password.salt,
				ECRYPTFS_SALT_SIZE);
		salt[ECRYPTFS_SALT_SIZE * 2] = '\0';
		ecryptfs_printk(KERN_DEBUG, " * salt = [%s]\n", salt);
		if (auth_tok->token.password.flags &
		    ECRYPTFS_PERSISTENT_PASSWORD) {
			ecryptfs_printk(KERN_DEBUG, " * persistent\n");
		}
		memcpy(sig, auth_tok->token.password.signature,
		       ECRYPTFS_SIG_SIZE_HEX);
		sig[ECRYPTFS_SIG_SIZE_HEX] = '\0';
		ecryptfs_printk(KERN_DEBUG, " * signature = [%s]\n", sig);
	}
	ecryptfs_printk(KERN_DEBUG, " * session_key.flags = [0x%x]\n",
			auth_tok->session_key.flags);
	if (auth_tok->session_key.flags
	    & ECRYPTFS_USERSPACE_SHOULD_TRY_TO_DECRYPT)
		ecryptfs_printk(KERN_DEBUG,
				" * Userspace decrypt request set\n");
	if (auth_tok->session_key.flags
	    & ECRYPTFS_USERSPACE_SHOULD_TRY_TO_ENCRYPT)
		ecryptfs_printk(KERN_DEBUG,
				" * Userspace encrypt request set\n");
	if (auth_tok->session_key.flags & ECRYPTFS_CONTAINS_DECRYPTED_KEY) {
		ecryptfs_printk(KERN_DEBUG, " * Contains decrypted key\n");
		ecryptfs_printk(KERN_DEBUG,
				" * session_key.decrypted_key_size = [0x%x]\n",
				auth_tok->session_key.decrypted_key_size);
		ecryptfs_printk(KERN_DEBUG, " * Decrypted session key "
				"dump:\n");
		if (ecryptfs_verbosity > 0)
			ecryptfs_dump_hex(auth_tok->session_key.decrypted_key,
					  ECRYPTFS_DEFAULT_KEY_BYTES);
	}
	if (auth_tok->session_key.flags & ECRYPTFS_CONTAINS_ENCRYPTED_KEY) {
		ecryptfs_printk(KERN_DEBUG, " * Contains encrypted key\n");
		ecryptfs_printk(KERN_DEBUG,
				" * session_key.encrypted_key_size = [0x%x]\n",
				auth_tok->session_key.encrypted_key_size);
		ecryptfs_printk(KERN_DEBUG, " * Encrypted session key "
				"dump:\n");
		if (ecryptfs_verbosity > 0)
			ecryptfs_dump_hex(auth_tok->session_key.encrypted_key,
					  auth_tok->session_key.
					  encrypted_key_size);
	}
}

/**
 * ecryptfs_dump_hex - debug hex printer
 * @data: string of bytes to be printed
 * @bytes: number of bytes to print
 *
 * Dump hexadecimal representation of char array
 */
void ecryptfs_dump_hex(char *data, int bytes)
{
	int i = 0;
	int add_newline = 1;

	if (ecryptfs_verbosity < 1)
		return;
	if (bytes != 0) {
		printk(KERN_DEBUG "0x%.2x.", (unsigned char)data[i]);
		i++;
	}
	while (i < bytes) {
		printk("0x%.2x.", (unsigned char)data[i]);
		i++;
		if (i % 16 == 0) {
			printk("\n");
			add_newline = 0;
		} else
			add_newline = 1;
	}
	if (add_newline)
		printk("\n");
}

