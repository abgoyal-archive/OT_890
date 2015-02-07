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
 * JFFS2 -- Journalling Flash File System, Version 2.
 *
 * Copyright © 2004   Ferenc Havasi <havasi@inf.u-szeged.hu>,
 *		      University of Szeged, Hungary
 *
 * For licensing information, see the file 'LICENCE' in this directory.
 *
 */

#ifndef __JFFS2_COMPR_H__
#define __JFFS2_COMPR_H__

#include <linux/kernel.h>
#include <linux/vmalloc.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/jffs2.h>
#include "jffs2_fs_i.h"
#include "jffs2_fs_sb.h"
#include "nodelist.h"

#define JFFS2_RUBINMIPS_PRIORITY 10
#define JFFS2_DYNRUBIN_PRIORITY  20
#define JFFS2_LZARI_PRIORITY     30
#define JFFS2_RTIME_PRIORITY     50
#define JFFS2_ZLIB_PRIORITY      60
#define JFFS2_LZO_PRIORITY       80


#define JFFS2_RUBINMIPS_DISABLED /* RUBINs will be used only */
#define JFFS2_DYNRUBIN_DISABLED  /*	   for decompression */

#define JFFS2_COMPR_MODE_NONE       0
#define JFFS2_COMPR_MODE_PRIORITY   1
#define JFFS2_COMPR_MODE_SIZE       2
#define JFFS2_COMPR_MODE_FAVOURLZO  3

#define FAVOUR_LZO_PERCENT 80

struct jffs2_compressor {
	struct list_head list;
	int priority;			/* used by prirority comr. mode */
	char *name;
	char compr;			/* JFFS2_COMPR_XXX */
	int (*compress)(unsigned char *data_in, unsigned char *cpage_out,
			uint32_t *srclen, uint32_t *destlen, void *model);
	int (*decompress)(unsigned char *cdata_in, unsigned char *data_out,
			  uint32_t cdatalen, uint32_t datalen, void *model);
	int usecount;
	int disabled;		/* if set the compressor won't compress */
	unsigned char *compr_buf;	/* used by size compr. mode */
	uint32_t compr_buf_size;	/* used by size compr. mode */
	uint32_t stat_compr_orig_size;
	uint32_t stat_compr_new_size;
	uint32_t stat_compr_blocks;
	uint32_t stat_decompr_blocks;
};

int jffs2_register_compressor(struct jffs2_compressor *comp);
int jffs2_unregister_compressor(struct jffs2_compressor *comp);

int jffs2_compressors_init(void);
int jffs2_compressors_exit(void);

uint16_t jffs2_compress(struct jffs2_sb_info *c, struct jffs2_inode_info *f,
			unsigned char *data_in, unsigned char **cpage_out,
			uint32_t *datalen, uint32_t *cdatalen);

int jffs2_decompress(struct jffs2_sb_info *c, struct jffs2_inode_info *f,
		     uint16_t comprtype, unsigned char *cdata_in,
		     unsigned char *data_out, uint32_t cdatalen, uint32_t datalen);

void jffs2_free_comprbuf(unsigned char *comprbuf, unsigned char *orig);

/* Compressor modules */
/* These functions will be called by jffs2_compressors_init/exit */

#ifdef CONFIG_JFFS2_RUBIN
int jffs2_rubinmips_init(void);
void jffs2_rubinmips_exit(void);
int jffs2_dynrubin_init(void);
void jffs2_dynrubin_exit(void);
#endif
#ifdef CONFIG_JFFS2_RTIME
int jffs2_rtime_init(void);
void jffs2_rtime_exit(void);
#endif
#ifdef CONFIG_JFFS2_ZLIB
int jffs2_zlib_init(void);
void jffs2_zlib_exit(void);
#endif
#ifdef CONFIG_JFFS2_LZO
int jffs2_lzo_init(void);
void jffs2_lzo_exit(void);
#endif

#endif /* __JFFS2_COMPR_H__ */
