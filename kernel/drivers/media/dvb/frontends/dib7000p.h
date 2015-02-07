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

#ifndef DIB7000P_H
#define DIB7000P_H

#include "dibx000_common.h"

struct dib7000p_config {
	u8 output_mpeg2_in_188_bytes;
	u8 hostbus_diversity;
	u8 tuner_is_baseband;
	int (*update_lna) (struct dvb_frontend *, u16 agc_global);

	u8 agc_config_count;
	struct dibx000_agc_config *agc;
	struct dibx000_bandwidth_config *bw;

#define DIB7000P_GPIO_DEFAULT_DIRECTIONS 0xffff
	u16 gpio_dir;
#define DIB7000P_GPIO_DEFAULT_VALUES     0x0000
	u16 gpio_val;
#define DIB7000P_GPIO_PWM_POS0(v)        ((v & 0xf) << 12)
#define DIB7000P_GPIO_PWM_POS1(v)        ((v & 0xf) << 8 )
#define DIB7000P_GPIO_PWM_POS2(v)        ((v & 0xf) << 4 )
#define DIB7000P_GPIO_PWM_POS3(v)         (v & 0xf)
#define DIB7000P_GPIO_DEFAULT_PWM_POS    0xffff
	u16 gpio_pwm_pos;

	u16 pwm_freq_div;

	u8 quartz_direct;

	u8 spur_protect;

	int (*agc_control) (struct dvb_frontend *, u8 before);

	u8 output_mode;
};

#define DEFAULT_DIB7000P_I2C_ADDRESS 18

#if defined(CONFIG_DVB_DIB7000P) || (defined(CONFIG_DVB_DIB7000P_MODULE) && defined(MODULE))
extern struct dvb_frontend *dib7000p_attach(struct i2c_adapter *i2c_adap,
					    u8 i2c_addr,
					    struct dib7000p_config *cfg);
extern struct i2c_adapter *dib7000p_get_i2c_master(struct dvb_frontend *,
						   enum dibx000_i2c_interface,
						   int);
extern int dib7000p_i2c_enumeration(struct i2c_adapter *i2c,
				    int no_of_demods, u8 default_addr,
				    struct dib7000p_config cfg[]);
extern int dib7000p_set_gpio(struct dvb_frontend *, u8 num, u8 dir, u8 val);
extern int dib7000p_set_wbd_ref(struct dvb_frontend *, u16 value);
#else
static inline struct dvb_frontend *dib7000p_attach(struct i2c_adapter *i2c_adap,
						   u8 i2c_addr,
						   struct dib7000p_config *cfg)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}

static inline
struct i2c_adapter *dib7000p_get_i2c_master(struct dvb_frontend *fe,
					    enum dibx000_i2c_interface i, int x)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return NULL;
}

static inline
int dib7000p_i2c_enumeration(struct i2c_adapter *i2c,
				    int no_of_demods, u8 default_addr,
				    struct dib7000p_config cfg[])
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return -ENODEV;
}

static inline
int dib7000p_set_gpio(struct dvb_frontend *fe, u8 num, u8 dir, u8 val)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return -ENODEV;
}

static inline
int dib7000p_set_wbd_ref(struct dvb_frontend *fe, u16 value)
{
	printk(KERN_WARNING "%s: driver disabled by Kconfig\n", __func__);
	return -ENODEV;
}
#endif

extern int dib7000pc_detection(struct i2c_adapter *i2c_adap);

#endif
