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


/* confdata.c */
P(conf_parse,void,(const char *name));
P(conf_read,int,(const char *name));
P(conf_read_simple,int,(const char *name, int));
P(conf_write,int,(const char *name));
P(conf_write_autoconf,int,(void));
P(conf_get_changed,bool,(void));
P(conf_set_changed_callback, void,(void (*fn)(void)));

/* menu.c */
P(rootmenu,struct menu,);

P(menu_is_visible,bool,(struct menu *menu));
P(menu_get_prompt,const char *,(struct menu *menu));
P(menu_get_root_menu,struct menu *,(struct menu *menu));
P(menu_get_parent_menu,struct menu *,(struct menu *menu));
P(menu_has_help,bool,(struct menu *menu));
P(menu_get_help,const char *,(struct menu *menu));

/* symbol.c */
P(symbol_hash,struct symbol *,[SYMBOL_HASHSIZE]);

P(sym_lookup,struct symbol *,(const char *name, int flags));
P(sym_find,struct symbol *,(const char *name));
P(sym_re_search,struct symbol **,(const char *pattern));
P(sym_type_name,const char *,(enum symbol_type type));
P(sym_calc_value,void,(struct symbol *sym));
P(sym_get_type,enum symbol_type,(struct symbol *sym));
P(sym_tristate_within_range,bool,(struct symbol *sym,tristate tri));
P(sym_set_tristate_value,bool,(struct symbol *sym,tristate tri));
P(sym_toggle_tristate_value,tristate,(struct symbol *sym));
P(sym_string_valid,bool,(struct symbol *sym, const char *newval));
P(sym_string_within_range,bool,(struct symbol *sym, const char *str));
P(sym_set_string_value,bool,(struct symbol *sym, const char *newval));
P(sym_is_changable,bool,(struct symbol *sym));
P(sym_get_choice_prop,struct property *,(struct symbol *sym));
P(sym_get_default_prop,struct property *,(struct symbol *sym));
P(sym_get_string_value,const char *,(struct symbol *sym));

P(prop_get_type_name,const char *,(enum prop_type type));

/* expr.c */
P(expr_compare_type,int,(enum expr_type t1, enum expr_type t2));
P(expr_print,void,(struct expr *e, void (*fn)(void *, struct symbol *, const char *), void *data, int prevtoken));
