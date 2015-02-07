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

#include <string.h>
#include "slip_common.h"
#include "net_user.h"

int slip_proto_read(int fd, void *buf, int len, struct slip_proto *slip)
{
	int i, n, size, start;

	if(slip->more > 0){
		i = 0;
		while(i < slip->more){
			size = slip_unesc(slip->ibuf[i++], slip->ibuf,
					  &slip->pos, &slip->esc);
			if(size){
				memcpy(buf, slip->ibuf, size);
				memmove(slip->ibuf, &slip->ibuf[i],
					slip->more - i);
				slip->more = slip->more - i;
				return size;
			}
		}
		slip->more = 0;
	}

	n = net_read(fd, &slip->ibuf[slip->pos],
		     sizeof(slip->ibuf) - slip->pos);
	if(n <= 0)
		return n;

	start = slip->pos;
	for(i = 0; i < n; i++){
		size = slip_unesc(slip->ibuf[start + i], slip->ibuf,&slip->pos,
				  &slip->esc);
		if(size){
			memcpy(buf, slip->ibuf, size);
			memmove(slip->ibuf, &slip->ibuf[start+i+1],
				n - (i + 1));
			slip->more = n - (i + 1);
			return size;
		}
	}
	return 0;
}

int slip_proto_write(int fd, void *buf, int len, struct slip_proto *slip)
{
	int actual, n;

	actual = slip_esc(buf, slip->obuf, len);
	n = net_write(fd, slip->obuf, actual);
	if(n < 0)
		return n;
	else return len;
}
