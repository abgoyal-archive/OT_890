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

#ifndef _AV7110_AV_H_
#define _AV7110_AV_H_

struct av7110;

extern int av7110_set_vidmode(struct av7110 *av7110,
			      enum av7110_video_mode mode);

extern int av7110_record_cb(struct dvb_filter_pes2ts *p2t, u8 *buf, size_t len);
extern int av7110_pes_play(void *dest, struct dvb_ringbuffer *buf, int dlen);
extern int av7110_write_to_decoder(struct dvb_demux_feed *feed, const u8 *buf, size_t len);

extern int av7110_set_volume(struct av7110 *av7110, int volleft, int volright);
extern int av7110_av_stop(struct av7110 *av7110, int av);
extern int av7110_av_start_record(struct av7110 *av7110, int av,
			  struct dvb_demux_feed *dvbdmxfeed);
extern int av7110_av_start_play(struct av7110 *av7110, int av);

extern void dvb_video_add_event(struct av7110 *av7110, struct video_event *event);

extern void av7110_p2t_init(struct av7110_p2t *p, struct dvb_demux_feed *feed);
extern void av7110_p2t_write(u8 const *buf, long int length, u16 pid, struct av7110_p2t *p);

extern int av7110_av_register(struct av7110 *av7110);
extern void av7110_av_unregister(struct av7110 *av7110);
extern int av7110_av_init(struct av7110 *av7110);
extern void av7110_av_exit(struct av7110 *av7110);


#endif /* _AV7110_AV_H_ */
