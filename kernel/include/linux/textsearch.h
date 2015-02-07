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

#ifndef __LINUX_TEXTSEARCH_H
#define __LINUX_TEXTSEARCH_H

#include <linux/types.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/slab.h>

struct ts_config;

#define TS_AUTOLOAD	1 /* Automatically load textsearch modules when needed */
#define TS_IGNORECASE	2 /* Searches string case insensitively */

/**
 * struct ts_state - search state
 * @offset: offset for next match
 * @cb: control buffer, for persistent variables of get_next_block()
 */
struct ts_state
{
	unsigned int		offset;
	char			cb[40];
};

/**
 * struct ts_ops - search module operations
 * @name: name of search algorithm
 * @init: initialization function to prepare a search
 * @find: find the next occurrence of the pattern
 * @destroy: destroy algorithm specific parts of a search configuration
 * @get_pattern: return head of pattern
 * @get_pattern_len: return length of pattern
 * @owner: module reference to algorithm
 */
struct ts_ops
{
	const char		*name;
	struct ts_config *	(*init)(const void *, unsigned int, gfp_t, int);
	unsigned int		(*find)(struct ts_config *,
					struct ts_state *);
	void			(*destroy)(struct ts_config *);
	void *			(*get_pattern)(struct ts_config *);
	unsigned int		(*get_pattern_len)(struct ts_config *);
	struct module		*owner;
	struct list_head	list;
};

/**
 * struct ts_config - search configuration
 * @ops: operations of chosen algorithm
 * @flags: flags
 * @get_next_block: callback to fetch the next block to search in
 * @finish: callback to finalize a search
 */
struct ts_config
{
	struct ts_ops		*ops;
	int 			flags;

	/**
	 * get_next_block - fetch next block of data
	 * @consumed: number of bytes consumed by the caller
	 * @dst: destination buffer
	 * @conf: search configuration
	 * @state: search state
	 *
	 * Called repeatedly until 0 is returned. Must assign the
	 * head of the next block of data to &*dst and return the length
	 * of the block or 0 if at the end. consumed == 0 indicates
	 * a new search. May store/read persistent values in state->cb.
	 */
	unsigned int		(*get_next_block)(unsigned int consumed,
						  const u8 **dst,
						  struct ts_config *conf,
						  struct ts_state *state);

	/**
	 * finish - finalize/clean a series of get_next_block() calls
	 * @conf: search configuration
	 * @state: search state
	 *
	 * Called after the last use of get_next_block(), may be used
	 * to cleanup any leftovers.
	 */
	void			(*finish)(struct ts_config *conf,
					  struct ts_state *state);
};

/**
 * textsearch_next - continue searching for a pattern
 * @conf: search configuration
 * @state: search state
 *
 * Continues a search looking for more occurrences of the pattern.
 * textsearch_find() must be called to find the first occurrence
 * in order to reset the state.
 *
 * Returns the position of the next occurrence of the pattern or
 * UINT_MAX if not match was found.
 */ 
static inline unsigned int textsearch_next(struct ts_config *conf,
					   struct ts_state *state)
{
	unsigned int ret = conf->ops->find(conf, state);

	if (conf->finish)
		conf->finish(conf, state);

	return ret;
}

/**
 * textsearch_find - start searching for a pattern
 * @conf: search configuration
 * @state: search state
 *
 * Returns the position of first occurrence of the pattern or
 * UINT_MAX if no match was found.
 */ 
static inline unsigned int textsearch_find(struct ts_config *conf,
					   struct ts_state *state)
{
	state->offset = 0;
	return textsearch_next(conf, state);
}

/**
 * textsearch_get_pattern - return head of the pattern
 * @conf: search configuration
 */
static inline void *textsearch_get_pattern(struct ts_config *conf)
{
	return conf->ops->get_pattern(conf);
}

/**
 * textsearch_get_pattern_len - return length of the pattern
 * @conf: search configuration
 */
static inline unsigned int textsearch_get_pattern_len(struct ts_config *conf)
{
	return conf->ops->get_pattern_len(conf);
}

extern int textsearch_register(struct ts_ops *);
extern int textsearch_unregister(struct ts_ops *);
extern struct ts_config *textsearch_prepare(const char *, const void *,
					    unsigned int, gfp_t, int);
extern void textsearch_destroy(struct ts_config *conf);
extern unsigned int textsearch_find_continuous(struct ts_config *,
					       struct ts_state *,
					       const void *, unsigned int);


#define TS_PRIV_ALIGNTO	8
#define TS_PRIV_ALIGN(len) (((len) + TS_PRIV_ALIGNTO-1) & ~(TS_PRIV_ALIGNTO-1))

static inline struct ts_config *alloc_ts_config(size_t payload,
						gfp_t gfp_mask)
{
	struct ts_config *conf;

	conf = kzalloc(TS_PRIV_ALIGN(sizeof(*conf)) + payload, gfp_mask);
	if (conf == NULL)
		return ERR_PTR(-ENOMEM);

	return conf;
}

static inline void *ts_config_priv(struct ts_config *conf)
{
	return ((u8 *) conf + TS_PRIV_ALIGN(sizeof(struct ts_config)));
}

#endif
