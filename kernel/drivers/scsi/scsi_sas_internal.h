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

#ifndef _SCSI_SAS_INTERNAL_H
#define _SCSI_SAS_INTERNAL_H

#define SAS_HOST_ATTRS		0
#define SAS_PHY_ATTRS		17
#define SAS_PORT_ATTRS		1
#define SAS_RPORT_ATTRS		7
#define SAS_END_DEV_ATTRS	3
#define SAS_EXPANDER_ATTRS	7

struct sas_internal {
	struct scsi_transport_template t;
	struct sas_function_template *f;
	struct sas_domain_function_template *dft;

	struct device_attribute private_host_attrs[SAS_HOST_ATTRS];
	struct device_attribute private_phy_attrs[SAS_PHY_ATTRS];
	struct device_attribute private_port_attrs[SAS_PORT_ATTRS];
	struct device_attribute private_rphy_attrs[SAS_RPORT_ATTRS];
	struct device_attribute private_end_dev_attrs[SAS_END_DEV_ATTRS];
	struct device_attribute private_expander_attrs[SAS_EXPANDER_ATTRS];

	struct transport_container phy_attr_cont;
	struct transport_container port_attr_cont;
	struct transport_container rphy_attr_cont;
	struct transport_container end_dev_attr_cont;
	struct transport_container expander_attr_cont;

	/*
	 * The array of null terminated pointers to attributes
	 * needed by scsi_sysfs.c
	 */
	struct device_attribute *host_attrs[SAS_HOST_ATTRS + 1];
	struct device_attribute *phy_attrs[SAS_PHY_ATTRS + 1];
	struct device_attribute *port_attrs[SAS_PORT_ATTRS + 1];
	struct device_attribute *rphy_attrs[SAS_RPORT_ATTRS + 1];
	struct device_attribute *end_dev_attrs[SAS_END_DEV_ATTRS + 1];
	struct device_attribute *expander_attrs[SAS_EXPANDER_ATTRS + 1];
};
#define to_sas_internal(tmpl)	container_of(tmpl, struct sas_internal, t)

#endif
