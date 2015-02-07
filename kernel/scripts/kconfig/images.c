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
 * Copyright (C) 2002 Roman Zippel <zippel@linux-m68k.org>
 * Released under the terms of the GNU GPL v2.0.
 */

static const char *xpm_load[] = {
"22 22 5 1",
". c None",
"# c #000000",
"c c #838100",
"a c #ffff00",
"b c #ffffff",
"......................",
"......................",
"......................",
"............####....#.",
"...........#....##.##.",
"..................###.",
".................####.",
".####...........#####.",
"#abab##########.......",
"#babababababab#.......",
"#ababababababa#.......",
"#babababababab#.......",
"#ababab###############",
"#babab##cccccccccccc##",
"#abab##cccccccccccc##.",
"#bab##cccccccccccc##..",
"#ab##cccccccccccc##...",
"#b##cccccccccccc##....",
"###cccccccccccc##.....",
"##cccccccccccc##......",
"###############.......",
"......................"};

static const char *xpm_save[] = {
"22 22 5 1",
". c None",
"# c #000000",
"a c #838100",
"b c #c5c2c5",
"c c #cdb6d5",
"......................",
".####################.",
".#aa#bbbbbbbbbbbb#bb#.",
".#aa#bbbbbbbbbbbb#bb#.",
".#aa#bbbbbbbbbcbb####.",
".#aa#bbbccbbbbbbb#aa#.",
".#aa#bbbccbbbbbbb#aa#.",
".#aa#bbbbbbbbbbbb#aa#.",
".#aa#bbbbbbbbbbbb#aa#.",
".#aa#bbbbbbbbbbbb#aa#.",
".#aa#bbbbbbbbbbbb#aa#.",
".#aaa############aaa#.",
".#aaaaaaaaaaaaaaaaaa#.",
".#aaaaaaaaaaaaaaaaaa#.",
".#aaa#############aa#.",
".#aaa#########bbb#aa#.",
".#aaa#########bbb#aa#.",
".#aaa#########bbb#aa#.",
".#aaa#########bbb#aa#.",
".#aaa#########bbb#aa#.",
"..##################..",
"......................"};

static const char *xpm_back[] = {
"22 22 3 1",
". c None",
"# c #000083",
"a c #838183",
"......................",
"......................",
"......................",
"......................",
"......................",
"...........######a....",
"..#......##########...",
"..##...####......##a..",
"..###.###.........##..",
"..######..........##..",
"..#####...........##..",
"..######..........##..",
"..#######.........##..",
"..########.......##a..",
"...............a###...",
"...............###....",
"......................",
"......................",
"......................",
"......................",
"......................",
"......................"};

static const char *xpm_tree_view[] = {
"22 22 2 1",
". c None",
"# c #000000",
"......................",
"......................",
"......#...............",
"......#...............",
"......#...............",
"......#...............",
"......#...............",
"......########........",
"......#...............",
"......#...............",
"......#...............",
"......#...............",
"......#...............",
"......########........",
"......#...............",
"......#...............",
"......#...............",
"......#...............",
"......#...............",
"......########........",
"......................",
"......................"};

static const char *xpm_single_view[] = {
"22 22 2 1",
". c None",
"# c #000000",
"......................",
"......................",
"..........#...........",
"..........#...........",
"..........#...........",
"..........#...........",
"..........#...........",
"..........#...........",
"..........#...........",
"..........#...........",
"..........#...........",
"..........#...........",
"..........#...........",
"..........#...........",
"..........#...........",
"..........#...........",
"..........#...........",
"..........#...........",
"..........#...........",
"..........#...........",
"......................",
"......................"};

static const char *xpm_split_view[] = {
"22 22 2 1",
". c None",
"# c #000000",
"......................",
"......................",
"......#......#........",
"......#......#........",
"......#......#........",
"......#......#........",
"......#......#........",
"......#......#........",
"......#......#........",
"......#......#........",
"......#......#........",
"......#......#........",
"......#......#........",
"......#......#........",
"......#......#........",
"......#......#........",
"......#......#........",
"......#......#........",
"......#......#........",
"......#......#........",
"......................",
"......................"};

static const char *xpm_symbol_no[] = {
"12 12 2 1",
"  c white",
". c black",
"            ",
" .......... ",
" .        . ",
" .        . ",
" .        . ",
" .        . ",
" .        . ",
" .        . ",
" .        . ",
" .        . ",
" .......... ",
"            "};

static const char *xpm_symbol_mod[] = {
"12 12 2 1",
"  c white",
". c black",
"            ",
" .......... ",
" .        . ",
" .        . ",
" .   ..   . ",
" .  ....  . ",
" .  ....  . ",
" .   ..   . ",
" .        . ",
" .        . ",
" .......... ",
"            "};

static const char *xpm_symbol_yes[] = {
"12 12 2 1",
"  c white",
". c black",
"            ",
" .......... ",
" .        . ",
" .        . ",
" .      . . ",
" .     .. . ",
" . .  ..  . ",
" . ....   . ",
" .  ..    . ",
" .        . ",
" .......... ",
"            "};

static const char *xpm_choice_no[] = {
"12 12 2 1",
"  c white",
". c black",
"            ",
"    ....    ",
"  ..    ..  ",
"  .      .  ",
" .        . ",
" .        . ",
" .        . ",
" .        . ",
"  .      .  ",
"  ..    ..  ",
"    ....    ",
"            "};

static const char *xpm_choice_yes[] = {
"12 12 2 1",
"  c white",
". c black",
"            ",
"    ....    ",
"  ..    ..  ",
"  .      .  ",
" .   ..   . ",
" .  ....  . ",
" .  ....  . ",
" .   ..   . ",
"  .      .  ",
"  ..    ..  ",
"    ....    ",
"            "};

static const char *xpm_menu[] = {
"12 12 2 1",
"  c white",
". c black",
"            ",
" .......... ",
" .        . ",
" . ..     . ",
" . ....   . ",
" . ...... . ",
" . ...... . ",
" . ....   . ",
" . ..     . ",
" .        . ",
" .......... ",
"            "};

static const char *xpm_menu_inv[] = {
"12 12 2 1",
"  c white",
". c black",
"            ",
" .......... ",
" .......... ",
" ..  ...... ",
" ..    .... ",
" ..      .. ",
" ..      .. ",
" ..    .... ",
" ..  ...... ",
" .......... ",
" .......... ",
"            "};

static const char *xpm_menuback[] = {
"12 12 2 1",
"  c white",
". c black",
"            ",
" .......... ",
" .        . ",
" .     .. . ",
" .   .... . ",
" . ...... . ",
" . ...... . ",
" .   .... . ",
" .     .. . ",
" .        . ",
" .......... ",
"            "};

static const char *xpm_void[] = {
"12 12 2 1",
"  c white",
". c black",
"            ",
"            ",
"            ",
"            ",
"            ",
"            ",
"            ",
"            ",
"            ",
"            ",
"            ",
"            "};
