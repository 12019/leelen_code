/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of TGT Limited. (C) 2016
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("TGT SOFTWARE")
*  RECEIVED FROM TGT AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. TGT EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES TGT PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE TGT SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. TGT SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY TGT SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND TGT'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE TGT SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT TGT'S OPTION, TO REVISE OR REPLACE THE TGT SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  TGT FOR SUCH TGT SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename: softsim_driver.h
 *
 * Description:
 *
 * Author: chuan.mei
 * 
 * Date: 2016-10-28
 ******************************************************************************/

#ifndef __SOFTSIM_DRIVER_H__
#define __SOFTSIM_DRIVER_H__
/*****************************************************************************
* Include                                                                     
*****************************************************************************/
#include "softsim_type.h"
/*****************************************************************************
* Define                                                                      
*****************************************************************************/
#define ASYNCB_HUP_NOTIFY 0
#define ASYNCB_FOURPORT 1/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ASYNCB_SAK 2
#define ASYNCB_SPLIT_TERMIOS 3
#define ASYNCB_SPD_HI 4
#define ASYNCB_SPD_VHI 5/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ASYNCB_SKIP_TEST 6
#define ASYNCB_AUTO_IRQ 7
#define ASYNCB_SESSION_LOCKOUT 8
#define ASYNCB_PGRP_LOCKOUT 9/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ASYNCB_CALLOUT_NOHUP 10
#define ASYNCB_HARDPPS_CD 11
#define ASYNCB_SPD_SHI 12
#define ASYNCB_LOW_LATENCY 13/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ASYNCB_BUGGY_UART 14
#define ASYNCB_AUTOPROBE 15
#define ASYNCB_LAST_USER 15
#define ASYNCB_INITIALIZED 31/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ASYNCB_SUSPENDED 30
#define ASYNCB_NORMAL_ACTIVE 29
#define ASYNCB_BOOT_AUTOCONF 28
#define ASYNCB_CLOSING 27/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ASYNCB_CTS_FLOW 26
#define ASYNCB_CHECK_CD 25
#define ASYNCB_SHARE_IRQ 24
#define ASYNCB_CONS_FLOW 23/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ASYNCB_FIRST_KERNEL 22
#define ASYNC_HUP_NOTIFY (1U << ASYNCB_HUP_NOTIFY)
#define ASYNC_SUSPENDED (1U << ASYNCB_SUSPENDED)
#define ASYNC_FOURPORT (1U << ASYNCB_FOURPORT)/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ASYNC_SAK (1U << ASYNCB_SAK)
#define ASYNC_SPLIT_TERMIOS (1U << ASYNCB_SPLIT_TERMIOS)
#define ASYNC_SPD_HI (1U << ASYNCB_SPD_HI)
#define ASYNC_SPD_VHI (1U << ASYNCB_SPD_VHI)/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ASYNC_SKIP_TEST (1U << ASYNCB_SKIP_TEST)
#define ASYNC_AUTO_IRQ (1U << ASYNCB_AUTO_IRQ)
#define ASYNC_SESSION_LOCKOUT (1U << ASYNCB_SESSION_LOCKOUT)
#define ASYNC_PGRP_LOCKOUT (1U << ASYNCB_PGRP_LOCKOUT)/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ASYNC_CALLOUT_NOHUP (1U << ASYNCB_CALLOUT_NOHUP)
#define ASYNC_HARDPPS_CD (1U << ASYNCB_HARDPPS_CD)
#define ASYNC_SPD_SHI (1U << ASYNCB_SPD_SHI)
#define ASYNC_LOW_LATENCY (1U << ASYNCB_LOW_LATENCY)/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ASYNC_BUGGY_UART (1U << ASYNCB_BUGGY_UART)
#define ASYNC_AUTOPROBE (1U << ASYNCB_AUTOPROBE)
#define ASYNC_FLAGS ((1U << (ASYNCB_LAST_USER + 1)) - 1)
#define ASYNC_USR_MASK (ASYNC_SPD_MASK|ASYNC_CALLOUT_NOHUP|   ASYNC_LOW_LATENCY)/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ASYNC_SPD_CUST (ASYNC_SPD_HI|ASYNC_SPD_VHI)
#define ASYNC_SPD_WARP (ASYNC_SPD_HI|ASYNC_SPD_SHI)
#define ASYNC_SPD_MASK (ASYNC_SPD_HI|ASYNC_SPD_VHI|ASYNC_SPD_SHI)
#define ASYNC_INITIALIZED (1U << ASYNCB_INITIALIZED)/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ASYNC_NORMAL_ACTIVE (1U << ASYNCB_NORMAL_ACTIVE)
#define ASYNC_BOOT_AUTOCONF (1U << ASYNCB_BOOT_AUTOCONF)
#define ASYNC_CLOSING (1U << ASYNCB_CLOSING)
#define ASYNC_CTS_FLOW (1U << ASYNCB_CTS_FLOW)/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ASYNC_CHECK_CD (1U << ASYNCB_CHECK_CD)
#define ASYNC_SHARE_IRQ (1U << ASYNCB_SHARE_IRQ)
#define ASYNC_CONS_FLOW (1U << ASYNCB_CONS_FLOW)
#define ASYNC_INTERNAL_FLAGS (~((1U << ASYNCB_FIRST_KERNEL) - 1))/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */

/*****************************************************************************
* Typedef                                                                     
*****************************************************************************/
struct serial_struct 
{	
	int type; 
	int line; 
	unsigned int port;/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */ 
	int irq; 
	int flags; 
	int xmit_fifo_size; 
	int custom_divisor;/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */ 
	int baud_base; 
	unsigned short close_delay; 
	char io_type; 
	char reserved_char[1];/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */ 
	int hub6; 
	unsigned short closing_wait; 
	unsigned short closing_wait2; 
	unsigned char *iomem_base;/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */ 
	unsigned short iomem_reg_shift; 
	unsigned int port_high; 
	unsigned long iomap_base;
};

/*****************************************************************************
* Static Local Variable                                                              
*****************************************************************************/

/*****************************************************************************
* Global Variable                                                       
*****************************************************************************/

/*****************************************************************************
* Extern Global Variable                                                           
*****************************************************************************/

/*****************************************************************************
* Static Local Function                                                              
*****************************************************************************/

/*****************************************************************************
* Global Function                                                      
*****************************************************************************/

/*****************************************************************************
* Extern Global Function                                                     
*****************************************************************************/
extern s32 softsim_baudrate_set(s32 fd, s32 baudrate);
extern s32 softsim_baudrate(s32 fd, s32 baudrate);
extern s32 softsim_cat_gpio_value(s32 gpio_number);
extern s32 softsim_echo_value(char *path, s32 value);
extern s32 softsim_export(char *path, s32 value);
/****************************************************************************
 * Function
 ****************************************************************************/

#endif /*__SOFTSIM_DRIVER_H__*/
