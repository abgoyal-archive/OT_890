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
 *	linux/arch/alpha/kernel/err_impl.h
 *
 *	Copyright (C) 2000 Jeff Wiedemeier (Compaq Computer Corporation)
 *
 *	Contains declarations and macros to support Alpha error handling
 * 	implementations.
 */

union el_timestamp;
struct el_subpacket;
struct ev7_lf_subpackets;

struct el_subpacket_annotation {
	struct el_subpacket_annotation *next;
	u16 class;
	u16 type;
	u16 revision;
	char *description;
	char **annotation;
};
#define SUBPACKET_ANNOTATION(c, t, r, d, a) {NULL, (c), (t), (r), (d), (a)}

struct el_subpacket_handler {
	struct el_subpacket_handler *next;
	u16 class;
	struct el_subpacket *(*handler)(struct el_subpacket *);
};
#define SUBPACKET_HANDLER_INIT(c, h) {NULL, (c), (h)}

/*
 * Manipulate a field from a register given it's name. defines
 * for the LSB (__S - shift count) and bitmask (__M) are required
 *
 * EXTRACT(u, f) - extracts the field and places it at bit position 0
 * GEN_MASK(f) - creates an in-position mask for the field
 */
#define EXTRACT(u, f) (((u) >> f##__S) & f##__M)
#define GEN_MASK(f) ((u64)f##__M << f##__S)

/*
 * err_common.c
 */
extern char *err_print_prefix;

extern void mchk_dump_mem(void *, size_t, char **);
extern void mchk_dump_logout_frame(struct el_common *);
extern void el_print_timestamp(union el_timestamp *);
extern void el_process_subpackets(struct el_subpacket *, int);
extern struct el_subpacket *el_process_subpacket(struct el_subpacket *);
extern void el_annotate_subpacket(struct el_subpacket *);
extern void cdl_check_console_data_log(void);
extern int cdl_register_subpacket_annotation(struct el_subpacket_annotation *);
extern int cdl_register_subpacket_handler(struct el_subpacket_handler *);

/*
 * err_ev7.c
 */
extern struct ev7_lf_subpackets *
ev7_collect_logout_frame_subpackets(struct el_subpacket *,
				    struct ev7_lf_subpackets *);
extern void ev7_register_error_handlers(void);
extern void ev7_machine_check(u64, u64);

/*
 * err_ev6.c
 */
extern void ev6_register_error_handlers(void);
extern int ev6_process_logout_frame(struct el_common *, int);
extern void ev6_machine_check(u64, u64);

/*
 * err_marvel.c
 */
extern void marvel_machine_check(u64, u64);
extern void marvel_register_error_handlers(void);

/*
 * err_titan.c
 */
extern int titan_process_logout_frame(struct el_common *, int);
extern void titan_machine_check(u64, u64);
extern void titan_register_error_handlers(void);
extern int privateer_process_logout_frame(struct el_common *, int);
extern void privateer_machine_check(u64, u64);
