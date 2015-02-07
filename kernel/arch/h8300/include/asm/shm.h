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

#ifndef _H8300_SHM_H
#define _H8300_SHM_H


/* format of page table entries that correspond to shared memory pages
   currently out in swap space (see also mm/swap.c):
   bits 0-1 (PAGE_PRESENT) is  = 0
   bits 8..2 (SWP_TYPE) are = SHM_SWP_TYPE
   bits 31..9 are used like this:
   bits 15..9 (SHM_ID) the id of the shared memory segment
   bits 30..16 (SHM_IDX) the index of the page within the shared memory segment
                    (actually only bits 25..16 get used since SHMMAX is so low)
   bit 31 (SHM_READ_ONLY) flag whether the page belongs to a read-only attach
*/
/* on the m68k both bits 0 and 1 must be zero */
/* format on the sun3 is similar, but bits 30, 31 are set to zero and all
   others are reduced by 2. --m */

#ifndef CONFIG_SUN3
#define SHM_ID_SHIFT	9
#else
#define SHM_ID_SHIFT	7
#endif
#define _SHM_ID_BITS	7
#define SHM_ID_MASK	((1<<_SHM_ID_BITS)-1)

#define SHM_IDX_SHIFT	(SHM_ID_SHIFT+_SHM_ID_BITS)
#define _SHM_IDX_BITS	15
#define SHM_IDX_MASK	((1<<_SHM_IDX_BITS)-1)

#endif /* _H8300_SHM_H */
